var configSettings = null;

function toconfig(key, value) {
  if (key.substring(0,3) == "col") {
    return "0x" + value;
  }
  if (key.substring(0,3) == "txt") {
    return "0x" + value;
  }
  return value;
}

function fromconfig(key, value) {
  if (key === '') { 
    return value; 
  }
  if (key.substring(0,3) == "col") {
    return value.substring(2);
  }
  if (key.substring(0,3) == "txt") {
    return value.substring(2);
  }
  return value;
}

Pebble.addEventListener("ready", function(e) {
  
  try {
    configSettings = JSON.parse(localStorage['TileTime']);
    MessageQueue.sendAppMessage(configSettings);
  }
  catch(e) {
    // the dicitonary was never stored, the watchapp uses its default values
    configSettings = {};
    localStorage['TileTime'] = JSON.stringify(configSettings);
  }
  
});

Pebble.addEventListener("showConfiguration", function (e) {
  var url = "http://tiletime.lankamp.org:8111/template.html?";

  for (var key in configSettings) {
    if (configSettings.hasOwnProperty(key)) {
      url += encodeURIComponent(key) + "=" + encodeURIComponent(toconfig(key,configSettings[key])) + "&";
    }
  }

  console.log("Hosted URL: " + url);

  Pebble.openURL(url);
});

Pebble.addEventListener("webviewclosed", function(e) {
  
    if ((typeof e.response === 'string') && (e.response.length > 0)) {
        var newSettings = decodeURIComponent(e.response);
        configSettings = JSON.parse(newSettings,fromconfig);

        localStorage['TileTime'] = newSettings;
        MessageQueue.sendAppMessage(configSettings);
    }
});

var MessageQueue=function(){
  var RETRY_MAX=5;
  var queue=[];
  var sending=false;
  var timer=null;
  return{reset:reset,sendAppMessage:sendAppMessage,size:size};
  function reset(){queue=[];sending=false;}
  function sendAppMessage(message,ack,nack){
    if(!isValidMessage(message)){
      return false;
    }
    queue.push({message:message,ack:ack||null,nack:nack||null,attempts:0});
    setTimeout(function(){sendNextMessage();},1);
    return true;
  }
  function size(){
    return queue.length;
  }
  function isValidMessage(message){
    if(message!==Object(message)){
      return false;
    }
    var keys=Object.keys(message);
    if(!keys.length){
      return false;
    }
    for(var k=0;k<keys.length;k+=1){
      var validKey=/^[0-9a-zA-Z-_]*$/.test(keys[k]);
      if(!validKey){return false;}
      var value=message[keys[k]];
      if(!validValue(value)){return false;}
    }
    return true;
    function validValue(value){
      switch(typeof value){
        case"string":return true;
        case"number":return true;
        case"object":if(toString.call(value)=="[object Array]"){return true;}
      }
      return false;
    }
  }
  function sendNextMessage(){
    if(sending){return;}
    var message=queue.shift();
    if(!message){return;}
    message.attempts+=1;
    sending=true;
    Pebble.sendAppMessage(message.message,ack,nack);
    timer=setTimeout(function(){timeout();},1e3);
    function ack(){
      clearTimeout(timer);
      setTimeout(function(){sending=false;sendNextMessage();},200);
      if(message.ack){message.ack.apply(null,arguments);}
    }
    function nack(){
      clearTimeout(timer);
      if(message.attempts<RETRY_MAX){
        queue.unshift(message);
        setTimeout(function(){sending=false;sendNextMessage()},200*message.attempts);
      } else {
        if(message.nack){message.nack.apply(null,arguments);}
      }
    }
    function timeout(){
      setTimeout(function(){sending=false;sendNextMessage();},1e3);
      if(message.ack){message.ack.apply(null,arguments);}
    }
  }
}();