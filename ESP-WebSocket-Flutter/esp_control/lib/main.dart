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
  double stepPerUnit = 200;
  double speed = 2;
  double pos = 0;

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
    StepMotor step = StepMotor(stepPerUnit, speed, pos);
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
            floatingActionButton: FloatingActionButton.extended(
              heroTag: 'run',
              backgroundColor: Colors.green,
              // foregroundColor: Colors.red,
              onPressed: () {
                sendcmd();
              },
              label: Text(
                'Run',
                style: TextStyle(fontSize: 18),
              ),
              icon: Icon(
                Icons.play_arrow,
                size: 30,
              ),
            ),
            body: SingleChildScrollView(
                child: Column(children: [
              Container(
                height: 20,
              ),
              Card(
                margin: EdgeInsets.all(5),
                child: Row(
                  children: [
                    Text("Pul/unit"),
                    Expanded(
                        child: Slider(
                      value: stepPerUnit,
                      min: 0,
                      max: 1000,
                      divisions: 100,
                      activeColor: Colors.orange,
                      inactiveColor: Colors.grey,
                      label: stepPerUnit.round().toString(),
                      onChanged: connected
                          ? (double value) {
                              setState(() {
                                stepPerUnit = value;
                              });
                            }
                          : null,
                    ))
                  ],
                ),
              ),
              Card(
                margin: EdgeInsets.all(5),
                child: Row(
                  children: [
                    Text("Speed"),
                    Expanded(
                        child: Slider(
                      value: speed,
                      min: 0,
                      max: 10,
                      divisions: 100,
                      activeColor: Colors.orange,
                      inactiveColor: Colors.grey,
                      label: speed.toStringAsFixed(1).toString(),
                      onChanged: connected
                          ? (double value) {
                              setState(() {
                                speed = value;
                              });
                            }
                          : null,
                    ))
                  ],
                ),
              ),
              Card(
                margin: EdgeInsets.all(5),
                child: Row(
                  children: [
                    Text("Position"),
                    Expanded(
                        child: Slider(
                      value: pos,
                      min: -180,
                      max: 180,
                      divisions: 100,
                      activeColor: Colors.orange,
                      inactiveColor: Colors.grey,
                      label: pos.toStringAsFixed(1).toString(),
                      onChanged: connected
                          ? (double value) {
                              setState(() {
                                pos = value;
                              });
                            }
                          : null,
                    ))
                  ],
                ),
              )
            ]))));
  }
}
