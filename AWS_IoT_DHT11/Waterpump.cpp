#include "Waterpump.h"
// 물펌프 관련 설정
Waterpump::Waterpump(int pin1,int pin2) {
  // Use 'this->' to make the difference between the
  // 'pin' attribute of the class and the 
  // local variable 'pin' created from the parameter.
  this->pin1 = pin1;
  this->pin2 = pin2;
  init();
}
void Waterpump::init() {
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  // Always try to avoid duplicate code.
  // Instead of writing digitalWrite(pin, LOW) here,
  // call the function off() which already does that
  off();
  state = WATER_OFF;
}
void Waterpump::on() {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  state = WATER_ON;
}
void Waterpump::off() {
  digitalWrite(pin1,LOW);
  digitalWrite(pin2,LOW);
  state = WATER_OFF;
}
byte Waterpump::getState() {
  return state;
}
