var Led = require('pi-led').PiLed;
var led = new Led();

led.WriteMessage("Hello World!", function(err, result) {
    // This Callback happens when a message has finished scrolling
});