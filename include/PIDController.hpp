
class PIDController{

private:
  double kP;
  int minspeed;
  double error;

public:
  PIDController(double inKP, int inMinSpeed);
  int getMotorSpeed(int target, int current);
  double getError();
  // double sideError(int target, int current);
};
