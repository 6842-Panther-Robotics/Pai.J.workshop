#include "main.h"
//Sensors
pros::ADIPotentiometer pot('B'); //4095 ticks in 265 degrees
pros::ADILineSensor line ('C'); //4095 shades of color
//Motors
pros::Motor liftMotor (5, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);

//Variables

//PIDController
PIDController liftP(0.46,5);

//Helper Functions
void lift(int speed)
{
  liftMotor.move(speed);
}
void lift()
{
  liftMotor.move(0);
}
void brakeLift()
{
  liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}
void coastLift()
{
  liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

//Functions
void moveLiftT(int speed, int time)
{
  lift(speed);
  wait(time);
  all();
  brakeDrive();
}
void moveLift(int position)
{
  int time = 0;

  while(time<1){

    int speed = liftP.getMotorSpeed(position, ((pot.get_value()/4095)*256));

    lift(speed);

    double error = liftP.getError();
    if(fabs(error) < 1){
      time++;
      wait(5);
    }
  }
  lift();
}

//Driver Control
void liftOP(){
  brakeLift();

  if(master.get_digital(DIGITAL_L1)) {
    liftMotor.move_velocity(100);
    }
  else if(master.get_digital(DIGITAL_L2)){
    liftMotor.move_velocity(-100);
  }
  else{
    lift();
  }
}
