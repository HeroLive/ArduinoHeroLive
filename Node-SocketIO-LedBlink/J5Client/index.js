const five = require("johnny-five");
const board = new five.Board();

var buzz = null;
var D9 = null;
var D10 = null;
var D11 = null;
var ledInit = { led1: null, led2: null, led3: null };
var tempNull = { tempC: "", tempF: "", tempK: "" };
var led = ledInit;
var temp = tempNull;
var tempSetting = { freq: 1 };
var freqSender = 1;

var url = "http://192.168.1.10:3484/station01";
// var  url = "https://j5led.herokuapp.com/station01";
var io = require("socket.io-client")(url);

board.on("exit", function (event) {
  console.log("exit", event);
  D9.off();
  D10.off();
  D11.off();
});

board.on("close", function (event) {
  console.log("close");
  process.exit();
});

board.on("connect", function (event) {
  console.log("connect", event);
});

board.on("ready", function () {
  buzz = new five.Led(8);
  D9 = new five.Led(9);
  D10 = new five.Led(10);
  D11 = new five.Led(11);

  var thermometer = new five.Thermometer({
    controller: "LM35",
    pin: "A0",
    freq: 1000,
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
    

    if (freqSender >= tempSetting.freq) {
      io.emit("temp", temp);
      freqSender = 1;
    } else freqSender = freqSender + 1;
  });
});

io.on("connect", () => {
  io.emit("j5name", "ST001");
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

io.on("getTemp", (msg) => {
  console.log("Listen getTempInit from server");
  io.emit("temp", temp);
});

// io.on("tempSetting", (msg) => {
//   console.log(msg);
//   tempSetting = msg;
// });
