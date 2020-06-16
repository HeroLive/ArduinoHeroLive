const { disconnect } = require("process");

var app = require("express")();
var http = require("http").createServer(app);
var io = require("socket.io")(http);

var five = require('johnny-five');
var board = new five.Board();
var led=null;

app.get("/", (req, res) => {
  res.sendFile(__dirname + "/home.html");
});

board.on('ready', function () {
    led= new five.Led(13);
});

io.on("connection", (socket) => {
  console.log("a user connected, id: ", socket.id);
  socket.on("disconnect", () => {
    console.log("user ", socket.id, " disconnected");
  });
  socket.on("led on", (msg) => {
    console.log("Led status: ", msg);
    led.on();
    io.emit('led status', msg);
  });
  socket.on("led off", (msg) => {
    console.log("Led status: ", msg);
    led.off();
    io.emit('led status', msg);
  });
});

http.listen(3000, () => {
  console.log("listening on *:3000");
});
