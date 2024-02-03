#include "main.h"

pros::Motor Arm(10);
pros::Motor FlyWheel(2);
pros::ADIDigitalOut PTOPiston('A');

Drive chassis ({-14, -11, -15}, {16, 13, 8}, 1, 4.125, 600, 0.57142857142);

void Arm_Control() {
  while (true) {
    pros::delay(10);
    while (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
    Arm.move(127);
    }
    while (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      Arm.move(-127);
    }
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
      chassis.set_drive_brake(MOTOR_BRAKE_HOLD);
      Arm.move(-127);
      pros::delay(1000);
      PTOPiston.set_value(true);
      chassis.set_tank(127, 127);
      delay(1000);
      chassis.set_tank(0,0);
      Arm.brake();
    }
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
      chassis.set_drive_brake(MOTOR_BRAKE_COAST);
      PTOPiston.set_value(false);
    }
    Arm.brake();
  }
}

void BangBangLoop() {
  int BangInputRPM = 550;
  if (FlyWheel.get_actual_velocity() > BangInputRPM)
    {
      FlyWheel.move_voltage(5000);
    }
  else
    {
      FlyWheel.move_voltage(12000);
    }
  pros::delay(5);
}

void FlyWheel_Control() {
  int i = 1;
  while (true) {
    pros::delay(200);
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      if (i == 1) {
        i = 0;
      } else {
        i = 1;
      }
    }
    if (i == 1) {
      BangBangLoop();
    } else {
      FlyWheel.brake();
    }  
  }
}

void initialize() {
  pros::delay(500);

  chassis.toggle_modify_curve_with_controller(true); // Enables modifying the controller curve with buttons on the joysticks
  chassis.set_active_brake(0); // Sets the active brake kP. We recommend 0.1.
  chassis.set_curve_default(0, 0); // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)  
  default_constants(); // Set the drive to your own constants from autons.cpp!

  ez::as::auton_selector.add_autons({
    Auton("Near Auton", Near_Auton),
    Auton("Far Auton", Far_Auton),
    Auton("Skills Auton", Skills_Auton),
  });

  chassis.initialize();
  ez::as::initialize();
}

void disabled() {

}

void competition_initialize() {

}

void autonomous() {
  chassis.reset_pid_targets(); 
  chassis.reset_gyro(); 
  chassis.reset_drive_sensor(); 
  chassis.set_drive_brake(MOTOR_BRAKE_HOLD); 
  ez::as::auton_selector.call_selected_auton(); 
}

void opcontrol() {
  autonomous();
  chassis.set_drive_brake(MOTOR_BRAKE_COAST);
  Arm.set_brake_mode(MOTOR_BRAKE_HOLD); 
  FlyWheel.set_brake_mode(MOTOR_BRAKE_COAST);
  pros::Task Control_Arm(Arm_Control);
  pros::Task Control_FlyWheel(FlyWheel_Control);
  while (true) {
    chassis.arcade_standard(ez::SPLIT);
    pros::delay(ez::util::DELAY_TIME);
  }
}