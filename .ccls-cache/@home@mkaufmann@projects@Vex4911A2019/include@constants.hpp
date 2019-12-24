#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
//robot constant values (e.g. ports and measurements)
class Constants{
public:
  //MOTOR PORTS
  static int LEFT_FRONT_MOTOR_PORT;
  static int LEFT_REAR_MOTOR_PORT;
  static int RIGHT_FRONT_MOTOR_PORT;
  static int RIGHT_REAR_MOTOR_PORT;
  static int STACKER_TREAD_1_MOTOR_PORT;
  static int STACKER_TREAD_2_MOTOR_PORT;
  static int TILTER_MOTOR_PORT;

  //SENSOR PORTS
  static int LEFT_TRACKING_ENCODER_TOP;
  static int LEFT_TRACKING_ENCODER_BOTTOM;
  static int RIGHT_TRACKING_ENCODER_TOP;
  static int RIGHT_TRACKING_ENCODER_BOTTOM;
  static int REAR_TRACKING_ENCODER_TOP;
  static int REAR_TRACKING_ENCODER_BOTTOM;

  //ROBOT MEASUREMENTS (IN INCHES)
  static double LEFT_WHEEL_TO_TRACKING_CENTER;
  static double RIGHT_WHEEL_TO_TRACKING_CENTER;
  static double REAR_WHEEL_TO_TRACKING_CENTER;

  //CONVERSION FACTORS
  static int ENCODER_TICKS_PER_INCH;
};
#endif
