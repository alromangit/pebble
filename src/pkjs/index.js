var tussamApi = require('./tussamapi');

Pebble.addEventListener('ready', 
  function(e) {
    console.log('PebbleKit JS ready!');
    console.log('Call to getNodosCercanos');
    tussamApi.getNodosCercanos();
  }
);                  

