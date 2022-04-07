import 'package:json_annotation/json_annotation.dart';
part 'motorModel.g.dart';

@JsonSerializable()
class StepMotor {
  dynamic stepPerUnit;
  dynamic speed;
  dynamic pos;

  StepMotor(this.stepPerUnit, this.speed, this.pos);

  factory StepMotor.fromJson(Map<String, dynamic> json) =>
      _$StepMotorFromJson(json);
  Map<String, dynamic> toJson() => _$StepMotorToJson(this);
}
