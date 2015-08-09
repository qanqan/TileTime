var configSettings = null;

Pebble.addEventListener("ready", function(e) {
	console.log('PebbleKit JS ready');
  //Pebble.showSimpleNotificationOnPebble("TileTime", "Watchface has loaded");
});

Pebble.addEventListener("showConfiguration", function (e) {
	var url = "http://tiletime.lankamp.org:8111/index.html?";

	console.log('Showing configuration page: ' + url);

  Pebble.openURL(url);
});

Pebble.addEventListener("webviewclosed", function(e) {
  if ((typeof e.response === 'string') && (e.response.length > 0)) {
	  var configData = JSON.parse(decodeURIComponent(e.response));
	  console.log('Configuration page returned: ' + JSON.stringify(configData));
  
    if (configData.tijd) {
      Pebble.sendAppMessage({
        language: parseInt(configData.language),
        tijd: configData.tijd,
        box1: parseInt(configData.box1),
        big1: configData.big1,
        colbox1: parseInt(configData.colbox1, 16),
        txtbox1: parseInt(configData.txtbox1, 16),
        box2: parseInt(configData.box2),
        big2: configData.big2,
        colbox2: parseInt(configData.colbox2, 16),
        txtbox2: parseInt(configData.txtbox2, 16),
        box3: parseInt(configData.box3),
        big3: configData.big3,
        colbox3: parseInt(configData.colbox3, 16),
        txtbox3: parseInt(configData.txtbox3, 16),
        box4: parseInt(configData.box4),
        big4: configData.big4,
        colbox4: parseInt(configData.colbox4, 16),
        txtbox4: parseInt(configData.txtbox4, 16)
      }, function() {
          console.log('Send succesful!');
      }, function() {
          console.log('Send failed!');
          var lang = parseInt(configData.language);
          if (lang == 0) {
            Pebble.showSimpleNotificationOnPebble("TileTime", "Het ontvangen van instellingen van de telefoon is mislukt!");
          } else {
            Pebble.showSimpleNotificationOnPebble("TileTime", "Receiving setting from phone failed!");
            Pebble.show
          }
      });
    }
  } else {
    if (e.response.length > 0) console.log('Configuration page did not return any value!');
    if (typeof e.response === 'string') console.log('Configuration page returned no string!'); 
  }
});
