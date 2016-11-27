var parser = require('xml2json');

function getNodosCercanos() {
  function success(pos) {
    console.log("Position found");
    var soapBody = '<soap:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/" targetNamespace="http://impl.services.infotusws.tussam.com/" xmlns:ns1="http://services.infotusws.tussam.com/" xmlns:ns2="http://schemas.xmlsoap.org/soap/http"><soap:Body><getNodosCercanos xmlns="http://services.infotusws.tussam.com/"><latitud xmlns="">'+pos.coords.latitude+'</latitud><longitud xmlns="">'+pos.coords.longitude+'</longitud><radio xmlns="">100</radio></getNodosCercanos></soap:Body></soap:Envelope>';
    
    var method = 'POST';
    var url = 'http://www.infobustussam.com:9005/InfoTusWS/services/InfoTus?WSDL';
    
    // Create the request
    var req = new XMLHttpRequest();
    req.open(method, url,true);
    req.setRequestHeader("Authorization", "Basic aW5mb3R1cy11c2VybW9iaWxlOjJpbmZvdHVzMHVzZXIxbW9iaWxlMg==");
	  req.setRequestHeader("Content-Type", "text/xml; charset=utf-8");
    req.setRequestHeader("SOAPAction", "");
    req.setRequestHeader("deviceid", Pebble.getAccountToken());
    
    // Send the request
    req.send(soapBody);
    
    // Specify the callback for when the request is completed
    req.onload = function() {
      // The request was successfully completed!
      var json = parser.toJson(this.responseText);
      console.log('Got response: ' + this.responseText);
      console.log("################");
      console.log(json);
    };
    
    /*
    var dictionary = {
      'LATITUDE' : pos.coords.latitude,
      'LONGITUDE': pos.coords.longitude
    };
    Pebble.sendAppMessage(dictionary,
    function(e) {
      console.log('You are latitude -> ' + dictionary.LATITUDE + " longitude ->" + dictionary.LONGITUDE);
    },
    function(e) {
      console.log('Error sending info to Pebble!');
    }); */
  }
  
  function error(err) {
    //Si no funciona la geolocalización devolvemos el centro de sevilla
    console.log("Position not found. Using default position");
    var pos = {
      coords : {
        latitude: 37.3881,
        longitude: -5.9823
      }
    };
    
    var dictionary = {
      'LATITUDE' : pos.coords.latitude,
      'LONGITUDE': pos.coords.longitude
    };
    
    Pebble.sendAppMessage(dictionary,
    function(e) {
      console.log('You are latitude -> ' + dictionary.LATITUDE + " longitude ->" + dictionary.LONGITUDE);
    },
    function(e) {
      console.log('Error sending info to Pebble!');
    }); 
  }
  
  var options = {
    enableHighAccuracy: true,
    maximumAge: 10000,
    timeout: 10000
  };
  
  navigator.geolocation.getCurrentPosition(success, error, options);
}

module.exports = {
  getNodosCercanos : getNodosCercanos
};