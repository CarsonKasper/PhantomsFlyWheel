#include "main.h"
//"griffin is stupid"
pros::Motor Arm(10);
pros::Motor FlyWheel(2);
pros::ADIDigitalOut PTOPiston('A');

Drive chassis ({-14, -11, -15}, {16/*was 5*/, 13/*was 5*/, 8}, 1, 4.125, 600, 0.57142857142);

int endgame = 0;

void Arm_Control() {
  while (true) {
    pros::delay(10);
    if (endgame == 0) {
    while (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      Arm.move(127);
    }
    while (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      Arm.move(-80);
    }
    } 
    else {
      while (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      Arm.move(127);
    }
    while (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      Arm.move(-127);
    }
    }
    Arm.brake();
  }
}

void ActuatePTO () {
  int check = 1;
  while (true) {
    pros::delay(200);
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      if (check == 1) {
        PTOPiston.set_value(true);
        check = 0;
        endgame = 1;
      } else {
        PTOPiston.set_value(false);
        check = 1;
        endgame = 0;
      }
    }      
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
 // pros::Task BangBangTask(BangBangLoop);

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
  pros::Task PTO_Actuate(ActuatePTO);
  while (true) {
    chassis.arcade_standard(ez::SPLIT);

    pros::delay(ez::util::DELAY_TIME);

  }
}