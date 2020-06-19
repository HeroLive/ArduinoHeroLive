var app = require("express")();
var http = require("http").createServer(app);
var io = require("socket.io")(http);

const PORT = 3484;
http.listen(process.env.PORT || PORT, console.log("server running ", PORT));

app.get("/", (req, res) => {
  res.sendFile(__dirname + "/index.html");
});

io.on("connection", (socket) => {
  console.log("a user connected, id: ", socket.id);
  socket.on("disconnect", () => {
    console.log("user ", socket.id, " disconnected");
  });
  socket.on("led on", (msg) => {
    console.log("Led status to j5: ", msg);
    io.emit("led on", msg);
  });
  socket.on("led off", (msg) => {
    console.log("Led status to j5: ", msg);
    io.emit("led off", msg);
  });
  socket.on("ledStatus", (msg) => {
    console.log("Led status from j5: ", msg);
    io.emit("ledStatus2web", msg);
  });

  socket.on("temp", (msg) => {
    console.log("Temp from j5: ", msg);
    io.emit("temp2web", msg);
  });

});
