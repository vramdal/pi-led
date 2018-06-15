var Led = require('./pi-led').PiLed;
//var sleep = require('sleep');
var led = new Led();

var message = [126, 32, 16, 8, 126, 0, 28, 34, 34, 34, 28, 0, 32, 32, 124, 34, 34, 0, 0, 0, 0, 0, 28, 34, 34, 34, 0, 0, 28, 34, 34, 34, 28, 0, 62, 16, 32, 32, 30, 0, 62, 16, 32, 32, 30, 0, 28, 42, 42, 42, 24, 0, 28, 34, 34, 34, 0, 0, 32, 32, 124, 34, 34, 0, 28, 42, 42, 42, 24, 0, 12, 18, 18, 126];
var empty = [256];
for (var i = 0; i < 255; i++) {
    empty[i] = 0;
}

var bytes = empty.slice(0);
//for (var i = 0; i < message.length; i++) {
//    bytes[i] = message[i];
//    bytes[128+i] = message[i];
//}
try {
    led.ClearMatrix();
//    led.WriteBytes(bytes, 0);
    for (var i = 0; i < 128; i++) {
        led.WriteBytes([message[i]], 128 + i);
        //sleep.usleep(50000);
    }
} catch (err) {
    console.error(err);
}

