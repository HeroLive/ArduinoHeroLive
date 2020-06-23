var app = require("express")();
var http = require("http").createServer(app);
var io = require("socket.io")(http);
const ST01 = io.of("/station01");
var idName = '';
var ledInit = { led1: null, led2: null, led3: null }
var tempNull = { tempC: "", tempF: "", tempK: "" };

const PORT = 3484;
http.listen(process.env.PORT || PORT, console.log("server running ", PORT));

app.get("/", (req, res) => {
  res.sendFile(__dirname + "/index.html");
});

// io.on("connection", (socket) => {
//   console.log("a user connected, id: ", socket.id);

//   io.emit("getLedStatus", "");
//   io.emit("getTemp", "");

//   socket.on("disconnect", () => {
//     console.log("user ", socket.id, " disconnected");
//   });

//   socket.on("led", (msg) => {
//     console.log("Led status from web: ", msg);
//     io.emit("led", msg);
//   });

//   socket.on("ledStatus", (msg) => {
//     console.log("Led status from J5: ", msg);
//     io.emit("ledStatus", msg);
//   });

//   socket.on("temp", (msg) => {
//     console.log("Temp from j5: ", msg);
//     io.emit("temp2web", msg);
//   });
// });

ST01.on("connection", (socket) => {
  console.log("a user connected station01, id: ", socket.id);

  ST01.emit("getLedStatus", "");
  ST01.emit("getTemp", "");

  socket.on('j5name', msg => {    
    idName = msg+'_'+socket.id;
    console.log(idName);
  })

  socket.on("disconnect", () => {
    console.log("user ", socket.id, " disconnected");
    if (idName.includes(socket.id)){
      console.log("user ", idName, " disconnected");
      ST01.emit('ledStatus',ledInit);
      ST01.emit("temp2web", tempNull);
    }
  });

  socket.on("led", (msg) => {
    console.log("Led status from web: ", msg);
    ST01.emit("led", msg);
  });

  socket.on("ledStatus", (msg) => {
    console.log("Led status from J5: ", msg);
    ST01.emit("ledStatus", msg);
  });

  socket.on("temp", (msg) => {
    console.log("Temp from j5: ", msg);
    ST01.emit("temp2web", msg);
  });

  // socket.on("tempSetting", (msg) => {
  //   console.log("tempSetting from web: ", msg);
  //   ST01.emit("tempSetting", msg);
  // });
});
