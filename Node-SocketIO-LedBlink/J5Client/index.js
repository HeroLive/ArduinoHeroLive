var five = require('johnny-five');
var board = new five.Board();
var led=null;
// var url = "http://192.168.1.4:3484";
url = "https://j5led.herokuapp.com";
var io = require('socket.io-client')(url);

var temp = 0;

board.on('ready', function () {
    led= new five.Led(13);

    var sensor = new five.Sensor("A0");

    sensor.on("change", () => {
      temp = sensor.scaleTo([0, 1023]);
      console.log(temp);
      io.emit("temp",temp);
    });
});

io.on("led on", msg => {
    console.log(msg);
    led.on();
    io.emit('ledStatus',"ON");
});
io.on("led off", msg => {
  console.log(msg);
  led.off();
  io.emit('ledStatus',"OFF");
});


