
function put_deviceData(tagName,tagValue){
	putAPI(tagName,tagValue);
	document.getElementById("put_deviceDataResult").innerHTML=tagName +" : " + tagValue +"값을 입력중입니다..."
}
var putAPI = function(tagName,tagValue) {

	var JsonData = {"tags" : [{"tagName": tagName , "tagValue" : tagValue}]}
	if(tagName == "UserMode"){
		if(tagValue =="Auto"){

			var JsonData = {"tags" : [{"tagName":"temperture" , "tagValue":"0"},{"tagName": "Soilsensor" , "tagValue" : "0"},{"tagName": tagName , "tagValue" : tagValue}]}
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