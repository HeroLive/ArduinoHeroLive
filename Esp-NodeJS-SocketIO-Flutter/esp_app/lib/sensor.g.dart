// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'sensor.dart';

// **************************************************************************
// JsonSerializableGenerator
// **************************************************************************

Sensor _$SensorFromJson(Map<String, dynamic> json) => Sensor(
      Dht.fromJson(json['dht'] as Map<String, dynamic>),
    );

Map<String, dynamic> _$SensorToJson(Sensor instance) => <String, dynamic>{
      'dht': instance.dht,
    };
