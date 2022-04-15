import 'package:json_annotation/json_annotation.dart';
import './dht.dart';
part 'sensor.g.dart';

@JsonSerializable()
class Sensor {
  Dht dht;

  Sensor(this.dht);

  factory Sensor.fromJson(Map<String, dynamic> json) => _$SensorFromJson(json);
  Map<String, dynamic> toJson() => _$SensorToJson(this);
}
