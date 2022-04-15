import 'dart:ffi';

import 'package:json_annotation/json_annotation.dart';
part 'dht.g.dart';

@JsonSerializable()
class Dht {
  dynamic tempC;
  dynamic humi;
  dynamic count;
  dynamic date;

  Dht(this.tempC, this.humi, this.count, this.date);

  factory Dht.fromJson(Map<String, dynamic> json) => _$DhtFromJson(json);
  Map<String, dynamic> toJson() => _$DhtToJson(this);
}
