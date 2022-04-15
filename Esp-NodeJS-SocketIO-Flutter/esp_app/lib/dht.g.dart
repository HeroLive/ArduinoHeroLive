// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'dht.dart';

// **************************************************************************
// JsonSerializableGenerator
// **************************************************************************

Dht _$DhtFromJson(Map<String, dynamic> json) => Dht(
      json['tempC'],
      json['humi'],
      json['count'],
      json['date'],
    );

Map<String, dynamic> _$DhtToJson(Dht instance) => <String, dynamic>{
      'tempC': instance.tempC,
      'humi': instance.humi,
      'count': instance.count,
      'date': instance.date,
    };
