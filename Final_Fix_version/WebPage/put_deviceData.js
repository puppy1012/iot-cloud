var intervalId = null;
function Stop() {
    if(intervalId != null) {
        clearInterval(intervalId);
    }
}
//디바이스 제어를 위한 js
function put_deviceData(tagName,tagValue){
	putAPI(tagName,tagValue);
	document.getElementById("put_deviceDataResult").innerHTML=tagName +" : " + tagValue +"값을 입력중입니다..."
}

var putAPI = function(tagName,tagValue) {
	var JsonData = {"tags" : [{"tagName": tagName , "tagValue" : tagValue}]};
    // 디바이스 조회 URI
    // prod 스테이지 편집기의 맨 위에 있는 "호출 URL/devices"로 대체해야 함
    var API_URI = 'https://wingediuak.execute-api.ap-northeast-2.amazonaws.com/prod/devices/MyMKRWiFi1010';  
      
    $.ajax(API_URI, {
        method: 'PUT',
        contentType: "application/json;charset=UTF-8",
        data:JSON.stringify(JsonData),
        success: function(){
        	$("#put_deviceDataResult").empty()

        	document.getElementById("put_deviceDataResult").innerHTML=tagName +" : " + tagValue +"값을 성공적으로 입력하였습니다"
			}


    });
};
function put_deviceDataAuto(tagName,tagValue){
	var soil = document.getElementById("Soil");
	soil= soil.textContent;
	var temperture= document.getElementById("temperture");
	temperture = temperture.textContent;
	putAPIAuto(tagName,tagValue,temperture,soil);
	document.getElementById("put_deviceDataResult").innerHTML=tagName +" : " + tagValue +"값을 입력중입니다..."

}
var putAPIAuto = function(tagName,tagValue,temperture,soil) {

	var JsonData = {"tags" : [{"tagName": tagName , "tagValue" : tagValue}]}
	if(tagName == "UserMode"){
		if(tagValue =="Auto"){
			var JsonData = {"tags" : [{"tagName":"temperture" , "tagValue":temperture},{"tagName": "Soilsensor" , "tagValue" : soil},{"tagName": tagName , "tagValue" : tagValue}]}
		}
	}
    // 디바이스 조회 URI
    // prod 스테이지 편집기의 맨 위에 있는 "호출 URL/devices"로 대체해야 함
    var API_URI = 'https://wingediuak.execute-api.ap-northeast-2.amazonaws.com/prod/devices/MyMKRWiFi1010';  
    $.ajax(API_URI, {
        method: 'PUT',
        contentType: "application/json;charset=UTF-8",
        data:JSON.stringify(JsonData),
        success: function(){
        	$("#put_deviceDataResult").empty()
        	document.getElementById("put_deviceDataResult").innerHTML=tagName +" : " + tagValue +"값을 성공적으로 입력하였습니다"
			}
    });
};
//디바이스 제어를 위한 js
function put_deviceDataSelf(tagName,tagValue,PumpData){
	putAPISelf(tagName,tagValue,PumpData);
	document.getElementById("put_deviceDataResult").innerHTML=tagName +" : " + tagValue +"값을 입력중입니다..."
}

var putAPISelf = function(tagName,tagValue,PumpData) {

	if(tagName == "UserMode"){
		if(tagValue =="Self"){
				var JsonData = {"tags" : [{"tagName": tagName , "tagValue" : tagValue},{"tagName": "PumP" , "tagValue" : PumpData}]}
		}
	}
    // 디바이스 조회 URI
    // prod 스테이지 편집기의 맨 위에 있는 "호출 URL/devices"로 대체해야 함
    var API_URI = 'https://wingediuak.execute-api.ap-northeast-2.amazonaws.com/prod/devices/MyMKRWiFi1010';  
      
    $.ajax(API_URI, {
        method: 'PUT',
        contentType: "application/json;charset=UTF-8",
        data:JSON.stringify(JsonData),
        success: function(){
        	$("#put_deviceDataResult").empty()
        	document.getElementById("put_deviceDataResult").innerHTML=tagName +" : " + tagValue +"값을 성공적으로 입력하였습니다"
			}


    });
};