
#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h> // change to #include <WiFi101.h> for MKR1000
#include <ArduinoJson.h>
#include "Led.h"
#include "Waterpump.h"
#include "arduino_secrets.h"
#include "DHT.h"
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);
#define A0Pin 0 //토양센서인식 핀
#define LED_1_PIN 5 //LED핀 정의
#define AA 10 //물펌프 AA출력핀 정의
#define AB 6 //물펌프 AB 출력핀 정의



/////// Enter your sensitive data in arduino_secrets.h
const char ssid[]        = SECRET_SSID;
const char pass[]        = SECRET_PASS;
const char broker[]      = SECRET_BROKER;
const char* certificate  = SECRET_CERTIFICATE;
int sensorVal = 0;
int pumpTime=0;
int pumpTime2=0;
WiFiClient    wifiClient;            // Used for the TCP socket connection
BearSSLClient sslClient(wifiClient); // Used for SSL/TLS connection, integrates with ECC508
MqttClient    mqttClient(sslClient);
int UserMode = 20; //유저모드
#define SelfMode 20
#define AutoMode 21
unsigned long lastMillis = 0;

Led led1(LED_1_PIN);
Waterpump waterpump1(AA,AB);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  
  dht.begin();

  if (!ECCX08.begin()) {
    Serial.println("No ECCX08 present!");
    while (1);
  }
  waterpump1.off();
  // Set a callback to get the current time
  // used to validate the servers certificate
  ArduinoBearSSL.onGetTime(getTime);

  // Set the ECCX08 slot to use for the private key
  // and the accompanying public certificate for it
  sslClient.setEccSlot(0, certificate);

  // Optional, set the client id used for MQTT,
  // each device that is connected to the broker
  // must have a unique client id. The MQTTClient will generate
  // a client id for you based on the millis() value if not set
  //
  // mqttClient.setId("clientId");

  // Set the message callback, this function is
  // called when the MQTTClient receives a message
  mqttClient.onMessage(onMessageReceived);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!mqttClient.connected()) {
    // MQTT client is disconnected, connect
    connectMQTT();
  }

  // poll for new MQTT messages and send keep alives
  mqttClient.poll();

  // publish a message roughly every 5 seconds.
  if (millis() - lastMillis > 5000) {
    lastMillis = millis();
    char payload[512];
    getDeviceStatus(payload);
    if (UserMode ==SelfMode){
      waterpump1.on();
      delay(pumpTime2*1000);
      waterpump1.off();
    }
    sendMessage(payload);
  }
}

unsigned long getTime() {
  // get the current time from the WiFi module  
  return WiFi.getTime();
}

void connectWiFi() {
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(ssid);
  Serial.print(" ");

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

  Serial.println("You're connected to the network");
  Serial.println();
}

void connectMQTT() {
  Serial.print("Attempting to MQTT broker: ");
  Serial.print(broker);
  Serial.println(" ");

  while (!mqttClient.connect(broker, 8883)) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

  Serial.println("You're connected to the MQTT broker");
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe("$aws/things/MyMKRWiFi1010/shadow/update/delta");
}

void getDeviceStatus(char* payload) {
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  int soil = analogRead(A0Pin);

  //self모드가 아닐때를 Automode로 가정 , 각 조건에 따른 물펌프의 가동을 위한 변수값 설정
  const char* mode = (UserMode == SelfMode)? "Self": "Auto";
  const char* led = (led1.getState() == LED_ON)? "ON" : "OFF";
  const char* water = (waterpump1.getState() ==WATER_ON)? "ON" :"OFF";
  // make payload for the device update topic ($aws/things/MyMKRWiFi1010/shadow/update)
  sprintf(payload,"{\"state\":{\"reported\":{\"temperature\":\"%0.2f\",\"LED\":\"%s\",\"Soilsensor\" :\"%d\",\"Waterpump\":\"%s\",\"UserMode\":\"%s\"}}}",t,led,soil,water,mode);
}

void sendMessage(char* payload) {
  char TOPIC_NAME[]= "$aws/things/MyMKRWiFi1010/shadow/update";
  
  Serial.print("Publishing send message:");
  Serial.println(payload);
  mqttClient.beginMessage(TOPIC_NAME);
  mqttClient.print(payload);
  mqttClient.endMessage();
}

//값을 json형식으로 받아 필요한 정보를 분류하는 작업 및 동작
void onMessageReceived(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // store the message received to the buffer
  char buffer[512] ;
  int count=0;
  while (mqttClient.available()) {
     buffer[count++] = (char)mqttClient.read();
  }
  buffer[count]='\0'; // 버퍼의 마지막에 null 캐릭터 삽입
  Serial.println(buffer);
  Serial.println();

  // JSon 형식의 문자열인 buffer를 파싱하여 필요한 값을 얻어옴.
  // 디바이스가 구독한 토픽이 $aws/things/MyMKRWiFi1010/shadow/update/delta 이므로,
  // JSon 문자열 형식은 다음과 같다.
  // {
  //    "version":391,
  //    "timestamp":1572784097,
  //    "state":{
  //        "LED":"ON"
  //        "Soilsensor:ON"
  //        "MOTER:ON"
  //        "user_mode:auto"
  //    },
  //    "metadata":{
  //        "LED":{
  //          "timestamp":15727840
  //         }
  //    }
  // }
  //
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, buffer);
  JsonObject root = doc.as<JsonObject>();
  JsonObject state = root["state"];
  const char* led = state["LED"];
  const char* waterpump =state["Waterpump"];
  const char* usermode = state["UserMode"]; //유저모드를 위한 값
  const char* Pump = state["PumP"];//Self모드에서 들여오는 제어값
  Serial.println(led);
  Serial.println(waterpump);
  Serial.println(usermode);
  Serial.println(led);  
  char payload[512];
  //strcmp ==문자열 비교 led와 on이 같을때 작동 아닐때 정지

  if (strcmp(led,"ON")==0) {
    led1.on();
    sprintf(payload,"{\"state\":{\"reported\":{\"LED\":\"%s\"}}}","ON");
    sendMessage(payload);
    
  } else if (strcmp(led,"OFF")==0) {
    led1.off();
    sprintf(payload,"{\"state\":{\"reported\":{\"LED\":\"%s\"}}}","OFF");
    sendMessage(payload);
  }
  //strcmp ==문자열 비교 moter와 on이 같을때 작동 아닐때 정지
  if(strcmp(waterpump,"ON")==0){
    waterpump1.on();
    sprintf(payload,"{\"state\":{\"reported\":{\"Waterpump\":\"%s\"}}}","ON"); 
    sendMessage(payload);
    delay(1000);
    //물넘침 방지용
    waterpump1.off();
    sprintf(payload,"{\"state\":{\"reported\":{\"Waterpump\":\"%s\"}}}","ON"); 
    sendMessage(payload);
  } else if (strcmp(waterpump,"OFF")==0){
    waterpump1.off();
    sprintf(payload,"{\"state\":{\"reported\":{\"Waterpump\":\"%s\"}}}","OFF"); 
    sendMessage(payload);
  }
  //automode를 통해 값을 전달받을시 물펌프를 해당 값만큼 작동
  if(strcmp(usermode,"Auto")!=0 && strcmp(usermode,"Self")!=0){
    pumpTime = atoi(usermode);
    waterpump1.on();
    delay(pumpTime*1000);
    waterpump1.off();
    sprintf(payload,"{\"state\":{\"reported\":{\"UserMode\":\"%s\"}}}","Auto");
  }
   else if(strcmp(usermode,"Self")==0){
      UserMode = SelfMode;
      pumpTime2= atoi(Pump);
      sprintf(payload,"{\"state\":{\"reported\":{\"UserMode\":\"%s\"}}}","Self"); 
      sendMessage(payload);
   }
   delay(1000);
}
