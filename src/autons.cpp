#include "main.h"

const int DRIVE_SPEED = 110;
const int TURN_SPEED  = 90;
const int SWING_SPEED = 90;

void default_constants() {
  // Drive Constants
  chassis.pid_drive_constants_set(10, 0, 100); // Sets forward and backward
  chassis.pid_drive_constants_forward_set(3, 0, 20);
  chassis.pid_drive_constants_backward_set(3, 0, 20);
  chassis.pid_heading_constants_set(5, 0, 30);

  // Turn Constants
  chassis.pid_turn_constants_set(3, 0, 20);

  // Swing Constants
  chassis.pid_swing_constants_set(5, 0, 30); // Sets forward and backward
  chassis.pid_swing_constants_forward_set(5, 0, 30);
  chassis.pid_swing_constants_backward_set(5, 0, 30);
}

void tug (int attempts) {
  for (int i=0; i<attempts-1; i++) {
    printf("i - %i", i);
    chassis.pid_drive_set(-12, 110, false);
    chassis.pid_wait();

    if (chassis.interfered) {
      chassis.drive_sensor_reset();
      chassis.pid_drive_set(-2, 20, false);
      pros::delay(1000);
    }
    else {
      return;
    }
  }
}

void stupid_break() {
  chassis.drive_set(-127, -127);
  delay(25);
  chassis.drive_set(0, 0);
}

void drive_example() {
  // The first parameter is target inches
  // The second parameter is max speed the robot will drive at
  // The third parameter is a boolean (true or false) for enabling/disabling a slew at the start of drive motions
  // for slew, only enable it when the drive distance is greater then the slew distance + a few inches

  chassis.pid_drive_set(24, DRIVE_SPEED, false);
  chassis.pid_wait();

  chassis.pid_drive_set(-12, DRIVE_SPEED, false);
  chassis.pid_wait();

  chassis.pid_drive_set(-12, DRIVE_SPEED, false);
  chassis.pid_wait();
}

void Near_Auton(){
  //chassis.drive_brake_set(MOTOR_BRAKE_HOLD);
  LeftWing.set_value(true);
  chassis.drive_set(70, 70); // forward
  pros::delay(300);
  stupid_break();
  pros::delay(200);
  chassis.drive_set(-100, 100); // left
  pros::delay(150);
  LeftWing.set_value(false);
  stupid_break();
  pros::delay(500);
  chassis.drive_set(100, 100); // forward
  pros::delay(600);
  stupid_break();
  chassis.drive_set(-100, -100); // backward
  pros::delay(80);
  stupid_break();
  chassis.drive_set(-100, 100); // left
  pros::delay(285);
  stupid_break();
  chassis.drive_set(100, 100); // forward
  pros::delay(600);
  stupid_break();
  chassis.drive_set(0, 0);
  chassis.drive_set(100, 100); // forward
  pros::delay(600);
  chassis.drive_set(0, 0);
    chassis.drive_set(100, -100); // left
  pros::delay(150);
  chassis.drive_set(0, 0);


  // chassis.drive_imu_reset(); 
  // chassis.drive_sensor_reset();
  // chassis.pid_drive_set(100, 110, false, true);
  // pros::delay(5000);
  // chassis.pid_wait();
}
 
void Far_Auton(){
  chassis.pid_turn_set(90, 110, false);
  //chassis.set_drive_pid();
  chassis.pid_wait();
}

void Skills_Auton(){



  // pros::delay(35000);
  // chassis.pid_drive_set(-170, DRIVE_SPEED, false);
  // chassis.pid_wait();
  // chassis.pid_drive_set(-135, DRIVE_SPEED, false);
  // chassis.pid_wait();
  // chassis.pid_drive_set(-50, DRIVE_SPEED, false);
  // chassis.pid_wait();
  // //chassis.set_drive_pid(300, 127);
  // chassis.drive_set(127, 127);
  // pros::delay(7000);
  // chassis.drive_set(0, 0);
  // chassis.pid_drive_set(-75, 127, false);
  // chassis.pid_wait();
  // chassis.pid_drive_set(359, 127, false);
}