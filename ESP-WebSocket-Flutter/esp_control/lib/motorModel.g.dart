// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'motorModel.dart';

// **************************************************************************
// JsonSerializableGenerator
// **************************************************************************

StepMotor _$StepMotorFromJson(Map<String, dynamic> json) => StepMotor(
      json['stepPerUnit'],
      json['speed'],
      json['pos'],
    );

Map<String, dynamic> _$StepMotorToJson(StepMotor instance) => <String, dynamic>{
      'stepPerUnit': instance.stepPerUnit,
      'speed': instance.speed,
      'pos': instance.pos,
    };
