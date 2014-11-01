var Led = require('./pi-led').PiLed;
var sleep = require('sleep');
var led = new Led();

var message = [126, 32, 16, 8, 126, 0, 28, 34, 34, 34, 28, 0, 32, 32, 124, 34, 34, 0, 0, 0, 0, 0, 28, 34, 34, 34, 0, 0, 28, 34, 34, 34, 28, 0, 62, 16, 32, 32, 30, 0, 62, 16, 32, 32, 30, 0, 28, 42, 42, 42, 24, 0, 28, 34, 34, 34, 0, 0, 32, 32, 124, 34, 34, 0, 28, 42, 42, 42, 24, 0, 12, 18, 18, 126];
while (true) {
    for (var i = 0; i < message.length; i++) {
        led.WriteBytes(message.slice(i), function(err, result) {
            // This Callback happens when a message has finished scrolling
        });
        sleep.usleep(10000);
    }
}