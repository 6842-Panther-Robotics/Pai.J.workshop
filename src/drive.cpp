#include "main.h"
//Sensors
pros::Imu inertial(20);
//Motors
pros::Motor leftFront (1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor leftBack (2, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rightFront(3, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rightBack (4, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);


//Variables
double wheelDiameter = 4;
int ticsPerRotation = 360;//345 RIP
double widthOfRobot = 12.7;
double pi = 3.1415926535897932384;
int THRESHOLD = -5;
double DOWNSLOWER = 0.8;

//PIDControllers
PIDController moveP(0.5,30);
PIDController turnP(2.5,30);

//Helper Functions
void brakeDrive()
{
  rightBack.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  rightFront.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  leftBack.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  leftFront.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}
void all()
{
  left(0);
  right(0);
}
void all(int speed)
{
  left(speed);
  right(speed);
}
void coastDrive()
{
  rightBack.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  rightFront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  leftBack.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  leftFront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}
void wait(int time)
{
  pros::delay(time);
}
void left(int speed)
{
  leftFront.move(speed);
  leftBack.move(speed);
}
void right(int speed)
{
  rightFront.move(speed);
  rightBack.move (speed);
}

//Functions
void moveDriveT(int speed, int time)
{
  left(speed);
  right(speed);
  wait(time);
  all();
  brakeDrive();
}
void moveDrive(int speed, int inches)
{
  rightFront.tare_position();
  int distanceInTics = inches * (1/((wheelDiameter * pi)/ticsPerRotation));
  while (rightFront.get_position() < distanceInTics){
    left(speed);
    right(speed);
    wait(2);//5
  }
  all();
  brakeDrive();
}
void moveForwardP(int inches)
{
  int time = 0;
  rightFront.tare_position();
  leftFront.tare_position();
  int distanceInTics = inches * (1/((wheelDiameter * pi)/ticsPerRotation));

  while(time<1){

    int speed = moveP.getMotorSpeed(distanceInTics, rightFront.get_position());



    if(rightFront.get_position() - leftFront.get_position() <= THRESHOLD){
      left(speed * DOWNSLOWER);
      right(speed);
    }
    else if(leftFront.get_position() - rightFront.get_position() <= THRESHOLD){
      left(speed);
      right(speed * DOWNSLOWER);
    }
    else{
      left(speed);
      right(speed);
    }


    double error = moveP.getError();
    if(fabs(error) < 1){
      time++;
      wait(2);
    }
  }
  all();
}
void moveBackwardP(int inches)
{
  int time = 0;
  rightFront.tare_position();
  leftFront.tare_position();
  int distanceInTics = inches * (1/((wheelDiameter * pi)/ticsPerRotation));
  //ticsPerRotation * (inches/(wheelDiameter * pi))

  while(time<1){

    int speed = moveP.getMotorSpeed(distanceInTics, rightFront.get_position());


    if(rightFront.get_position() - leftFront.get_position() <= -THRESHOLD){
      left(-(speed * DOWNSLOWER));
      right(-speed);
    }
    else if(leftFront.get_position() - rightFront.get_position() <= -THRESHOLD){
      left(-speed);
      right(-(speed * DOWNSLOWER));
    }
    else{
      left(-speed);
      right(-speed);
    }


    double error = moveP.getError();
    if(fabs(error) < 1){
      time++;
      wait(2);
    }
  }
  all();
}
void moveLeft(int speed, int degree)
{
  rightFront.tare_position();
  while(inertial.get_rotation() < degree){
    left(-speed);
    right(speed);
  }
  all();
  brakeDrive();
}
void moveRight(int speed, int degree)
{
  while(inertial.get_rotation() < degree){
    left(speed);
    right(-speed);
  }
  all();
  brakeDrive();
}
void turnDriveP(int degree)
{
  int time = 0;

  while(time<1){

    int speed = turnP.getMotorSpeed(degree, inertial.get_rotation());
    left(speed);
    right(-speed);

    double error = turnP.getError();
    if(fabs(error) < 1){
      time++;
      wait(2);
    }
  }
  all();
}

//Driver Control
void driveOP()
{
  leftFront.move(master.get_analog(ANALOG_LEFT_Y));
  leftBack.move(master.get_analog(ANALOG_LEFT_Y));
  rightFront.move(master.get_analog(ANALOG_RIGHT_Y));
  rightBack.move(master.get_analog(ANALOG_RIGHT_Y));
}
