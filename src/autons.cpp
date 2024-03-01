#include "main.h"

const int DRIVE_SPEED = 110;
const int TURN_SPEED  = 90;
const int SWING_SPEED = 90;

void default_constants() {
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void profile_one() {
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void profile_two() {
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void modified_exit_condition() {
  chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}

void tug (int attempts) {
  for (int i=0; i<attempts-1; i++) {
    printf("i - %i", i);
    chassis.set_drive_pid(-12, 127);
    chassis.wait_drive();

    if (chassis.interfered) {
      chassis.reset_drive_sensor();
      chassis.set_drive_pid(-2, 20);
      pros::delay(1000);
    }
    else {
      return;
    }
  }
}

void Near_Auton(){
  chassis.set_drive_pid(105, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-50, DRIVE_SPEED);
  //pros::delay(2000);
}

void Far_Auton(){
  //chassis.set_drive_pid();
  pros::delay(4000);
}

void Skills_Auton(){
  pros::delay(35000);
  chassis.set_drive_pid(-170, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(-135, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-50, DRIVE_SPEED);
  chassis.wait_drive();
  //chassis.set_drive_pid(300, 127);
  chassis.set_tank(127, 127);
  pros::delay(7000);
  chassis.set_tank(0, 0);
  chassis.set_drive_pid(-75, 127);
  chassis.wait_drive();
  chassis.set_turn_pid(359, 127);
}