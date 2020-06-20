const five = require("johnny-five");
const board = new five.Board();

var buzz = null;
var D9 = null;
var D10 = null;
var D11 = null;
var led = { led1: 0, led2: 0, led3: 0 };
var temp = { tempC:'', tempF: '', tempK: '' }

var url = "http://192.168.1.4:3484";
// url = "https://j5led.herokuapp.com";
var io = require("socket.io-client")(url);

board.on("ready", function () {
  buzz = new five.Led(8);
  D9 = new five.Led(9);
  D10 = new five.Led(10);
  D11 = new five.Led(11);

  var thermometer = new five.Thermometer({
    controller: "LM35",
    pin: "A0",
    freq: 1000
  });

  thermometer.on("data", () => {
    const { celsius, fahrenheit, kelvin } = thermometer;
    temp.tempC = celsius;
    temp.tempF = fahrenheit.toFixed(0);
    temp.tempK = kelvin.toFixed(1);
    // console.log("Thermometer");
    // console.log("celsius: %d", celsius);
    // console.log("fahrenheit: %d", fahrenheit);
    // console.log("kelvin: %d", kelvin);
    // console.log("--------------------------------------");
    io.emit("temp", temp);
  });
});
io.on("led", (msg) => {
  led = msg;
  console.log("led from server:", led);

  if (led.led1 === true) {
    D9.on();
  } else D9.off();
  if (led.led2 === true) {
    D10.on();
  } else D10.off();
  if (led.led3 === true) {
    D11.on();
  } else D11.off();

  io.emit("ledStatus", led);
});

io.on("getLedStatus", (msg) => {
  console.log("Listen getLedStatus from server");
  io.emit("ledStatus", led);
});

io.on("getTempInit", (msg) => {
  console.log("Listen getTempInit from server");
  io.emit("temp", temp);
});
