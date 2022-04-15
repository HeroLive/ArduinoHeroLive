import 'package:flutter/material.dart';
import 'package:socket_io_client/socket_io_client.dart' as IO;
import './sensor.dart';
import 'dht.dart';

void main() {
  runApp(const MyApp());
}

IO.Socket socket = IO.io(
    'http://192.168.1.7:3484', //'https://dht-led.glitch.me',
    IO.OptionBuilder().setTransports(['websocket']).build());

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Monitor app',
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
  Sensor _sensorData = Sensor(Dht(null, null, null, null));

  @override
  void initState() {
    super.initState();
    connectAndListen();
  }

  void connectAndListen() {
    print('Call func connectAndListen');

    socket.onConnect((_) {
      print('connect');
      socket.emit('from-user', 'test from user');
    });

    socket.on('server2user', (data) {
      print(data);
      var sensor = Sensor.fromJson(data);
      print(
          '-Nhiet do: ${sensor.dht.tempC} -Do am: ${sensor.dht.humi} -Count: ${sensor.dht.count}');
      setState(() {
        _sensorData = sensor;
      });
    });

    //When an event recieved from server, data is added to the stream
    socket.onDisconnect((_) => print('disconnect'));
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text('DHT sensor')),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.start,
          children: <Widget>[
            Card(
              elevation: 3,
              child: Padding(
                padding: const EdgeInsets.all(8.0),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    const Text(
                      'DHT11',
                      style: TextStyle(
                          fontSize: 18,
                          fontWeight: FontWeight.bold,
                          color: Colors.redAccent),
                    ),
                    Padding(padding: EdgeInsets.all(4)),
                    Row(
                      children: [
                        Expanded(
                          child: Card(
                            margin: EdgeInsets.zero,
                            color: Colors.blue,
                            child: Padding(
                              padding: const EdgeInsets.all(8.0),
                              child: Column(
                                children: [
                                  Text(
                                    'Temperature',
                                    style: TextStyle(
                                        fontSize: 18, color: Colors.white70),
                                  ),
                                  Padding(padding: EdgeInsets.all(8)),
                                  Text(
                                    _sensorData.dht.tempC == null
                                        ? 'N/A'
                                        : '${_sensorData.dht.tempC}°',
                                    style: TextStyle(
                                        fontSize: 40, color: Colors.white),
                                  ),
                                  Padding(padding: EdgeInsets.all(8)),
                                ],
                              ),
                            ),
                          ),
                        ),
                        Expanded(
                          child: Card(
                            color: Colors.blue,
                            child: Padding(
                              padding: const EdgeInsets.all(8.0),
                              child: Column(
                                children: [
                                  Text(
                                    'Humidity',
                                    style: TextStyle(
                                        fontSize: 18, color: Colors.white70),
                                  ),
                                  Padding(padding: EdgeInsets.all(8)),
                                  Text(
                                    _sensorData.dht.humi == null
                                        ? 'N/A'
                                        : '${_sensorData.dht.humi}%',
                                    style: TextStyle(
                                        fontSize: 40, color: Colors.white),
                                  ),
                                  Padding(padding: EdgeInsets.all(8)),
                                ],
                              ),
                            ),
                          ),
                        )
                      ],
                    ),
                  ],
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
