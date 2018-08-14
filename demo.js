//var led = require('./pi-led');
var led = require(__dirname + '/build/Release/piLedAddon.node');
//var sleep = require('sleep');

//var led = Led();
led.Init(true, 32000000);
//led.ClearMatrix();

//var message = [126, 32, 16, 8, 126, 0, 28, 34, 34, 34, 28, 0, 32, 32, 124, 34, 34, 0, 0, 0, 0, 0, 28, 34, 34, 34, 0, 0, 28, 34, 34, 34, 28, 0, 62, 16, 32, 32, 30, 0, 62, 16, 32, 32, 30, 0, 28, 42, 42, 42, 24, 0, 28, 34, 34, 34, 0, 0, 32, 32, 124, 34, 34, 0, 28, 42, 42, 42, 24, 0, 12, 18, 18, 126];
// var message = [];
// for (var module = 0; module < 8; module++) {
//     for (var x = 0; x < 32; x++) {
//         message.push(2^module/* | 128*/);
//     }

// }
// var empty = [256];
// for (var i = 0; i < 255; i++) {
//   empty[i] = i;
// }

// var full = [];
/*
for (var module = 0; module < 8; module++) {
    for (var x = 0; x < 32; x++) {
        full.push(x);
    }

}
*/

// var bytes = empty.slice(0);
//for (var i = 0; i < message.length; i++) {
//    bytes[i] = message[i];
//    bytes[128+i] = message[i];
//}
try {
    // led.ClearMatrix();
//    led.WriteBytes(bytes, 0);
    //led.WriteBytes(message, 0);
    led._DrawByte(224, 255);
    led._WriteMatrix();
   // setTimeout(function () {led.WriteBytes(empty, 0)}, 500);

    //for (var i = 0; i < 256; i++) {
        //sleep.usleep(50000);
    //}
} catch (err) {
    console.error(err);
}

/*led.ClearMatrix();
for (var i = 252; i >= 0; i--) {
    console.log("i", i);
    led.WriteBytes([1,2,3], i);
    sleep.usleep(50000);
} */

/*led.DigitalWrite(40, 0);

while (true) {
    console.log("Begge HØY");
    led.DigitalWrite(2, 1);
    led.DigitalWrite(3, 1);
    sleep.sleep(2);
    console.log("2 HØY, 3 LAV")
    led.DigitalWrite(2, 1);
    led.DigitalWrite(3, 0);
    sleep.sleep(2);
    console.log("2 LAV, 3 HØY")
    led.DigitalWrite(2, 0);
    led.DigitalWrite(3, 1);
    sleep.sleep(2);
    console.log("Begge LAV");
    led.DigitalWrite(2, 0);
    led.DigitalWrite(3, 0);
    sleep.sleep(2);
}*/


/*while (true) {
    for (var i = 0; i < message.length; i++) {
        led.WriteBytes(message.slice(i).concat(empty).slice(0, 256), 0);
//        led.WriteBytes([1,2,3], 128);
    }
    sleep.usleep(100000);
} */

