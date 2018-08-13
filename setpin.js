//var led = require('./pi-led');
var led = require(__dirname + '/build/Release/piLedAddon.node');

led.Init(false, 500000);
var pin = parseInt(process.argv[2], 10);
var value = parseInt(process.argv[3], 10);
console.log("pin", pin, "value", value);
led.SetPin(pin, value);
