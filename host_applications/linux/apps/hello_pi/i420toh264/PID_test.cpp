#include <stdio.h>
#include <unistd.h>

#include "Arduino.h"
#include "Arduino-PID-Library/PID_v1.h"

double Setpoint, Input, Output;

double Kp=2, Ki=5, Kd=1;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

int main()
{
  printf("%ld\n",millis());
  usleep(100000);
  printf("%ld\n",millis());

  Input = 42;
  Setpoint = 100;

  myPID.SetMode(AUTOMATIC);

  Input = 48;
  myPID.Compute();
  printf("%lf\n",Output);

  return 0;
}
