// 중지를 위해 ID 보관
var intervalId = null;

// API 시작
function device_state(deviceId) {
    invokeAPI(deviceId);
    document.getElementById("result").innerHTML=deviceId +"값을 조회중입니다..."
    intervalId = setInterval(invokeAPI, 1000);
    
}
// API 중지

function Stop() {
    if(intervalId != null) {
        clearInterval(intervalId);
    }
}

var invokeAPI = function(deviceId) {

    // 디바이스 조회 URI
    // prod 스테이지 편집기의 맨 위에 있는 "호출 URL/devices"로 대체해야 함
    var API_URI = 'https://wingediuak.execute-api.ap-northeast-2.amazonaws.com/prod/devices/MyMKRWiFi1010';                 
    $.ajax(API_URI, {
        method: 'GET',
        contentType: "application/json",

        success: function (data, status, xhr) {
                var result = JSON.parse(data);
                printData(result,deviceId);  // 성공시, 데이터 출력을 위한 함수 호출
                
                stop();
        }
    });

};

// 데이터 출력을 위한 함수

var printData = function(result,deviceId){
    if(deviceId=='temperture'){
        document.getElementById("result").innerHTML=deviceId +" : "+ result.state.reported.temperature;
        document.getElementById("temperture").innerText=result.state.reported.temperature;

    }
        else if(deviceId=='Soilsensor'){
        document.getElementById("result").innerHTML=deviceId +" : "+ result.state.reported.Soilsensor;
        document.getElementById("Soil").innerText=result.state.reported.Soilsensor;
    }
        else if(deviceId=='LED'){
        document.getElementById("result").innerHTML=deviceId +" : "+ result.state.reported.LED;
    }
        else if(deviceId=='UserMode'){
        document.getElementById("result").innerHTML="UserMode : "+ result.state.reported.UserMode;
    }
        else if(deviceId=='Waterpump'){
        document.getElementById("result").innerHTML=deviceId +" : "+ result.state.reported.Waterpump;
    }

        else if(deviceId=='Total'){
        document.getElementById("result").innerHTML = "temp: "+ result.state.reported.temperature +", LED: " + result.state.reported.LED+", Waterpump: " + result.state.reported.Waterpump+ " , SoilSensor: " + result.state.reported.Soilsensor + ", UserMode = "+ result.state.reported.UserMode;
    }
  
}  
 