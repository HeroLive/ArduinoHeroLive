import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:web_socket_channel/io.dart';
import './motorModel.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      home: HomePage(),
    );
  }
}

class HomePage extends StatefulWidget {
  const HomePage({Key? key}) : super(key: key);

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  late IOWebSocketChannel channel;
  late bool connected; //boolean value to track if WebSocket is connected
  int count = 0;
  @override
  void initState() {
    connected = false; //initially connection status is "NO" so its FALSE
    // _deleteMotors();
    loadLocalData();
    Future.delayed(Duration.zero, () async {
      channelconnect(); //connect to WebSocket wth NodeMCU
    });

    super.initState();
  }

  void loadLocalData() async {
    return;
  }

  channelconnect() {
    //function to connect
    try {
      channel = IOWebSocketChannel.connect(
          Uri.parse('ws://192.168.99.99:81')); //channel IP : Port
      channel.stream.listen(
        (message) {
          print('Received from MCU $message');
          setState(() {
            if (message == "connected") {
              setState(() {
                connected = true;
              });
            } else if (message != '') {
              parseData(message);
            }
          });
        },
        onDone: () {
          //if WebSocket is disconnected
          print("Web socket is closed");
          setState(() {
            connected = false;
          });
        },
        onError: (error) {
          print(error.toString());
        },
      );
    } catch (_) {
      print("error on connecting to websocket.");
    }
  }

  parseData(String message) {
    print(message);
  }

  void sendcmd() async {
    StepMotor step = StepMotor(200, 2, 10);
    print('Send to MCU ${step.toJson()}');
    channel.sink.add(jsonEncode(step)); //sending Command to NodeMCU
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
        home: Scaffold(
      appBar: AppBar(
        title: Text("Motors control"),
        backgroundColor: Colors.orange,
        actions: <Widget>[
          IconButton(
            onPressed: () {},
            icon: Icon(Icons.refresh),
          ),
          IconButton(
              onPressed: () {
                connected ? null : channelconnect();
              },
              icon: Icon(
                Icons.cast_connected,
                color: connected ? Colors.white : Colors.white54,
              )),
        ],
      ),
      body: Container(
        padding: const EdgeInsets.all(8.0),
        child: Column(
          children: [
            ElevatedButton(
              onPressed: () {
                sendcmd();
              },
              child: Icon(
                Icons.arrow_back_ios,
                color: Colors.white,
                size: 40,
              ),
              style: ElevatedButton.styleFrom(
                fixedSize: Size(100, 100),
                primary: Colors.orange,
                onPrimary: Colors.black,
                shadowColor: Colors.brown,
                elevation: 5,
              ),
            ),
          ],
        ),
      ),
    ));
  }
}
