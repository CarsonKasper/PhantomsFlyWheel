#include "main.h"

pros::Motor Arm(4);
pros::Motor FlyWheel(20);
pros::ADIDigitalOut PTOPiston('C');
pros::ADIDigitalOut LeftWing('A');
pros::ADIDigitalOut RightWing('B');

Drive chassis ({-5, -19, -9}, {6, 7, 8}, 11, 4.125, 600, /*0.57142857142 */1.75);

void Arm_Control() {
  while (true) {
    pros::delay(10);
    while (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
    Arm.move(127);
    }
    while (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      Arm.move(-127);
    }
    /*
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
      chassis.drive_brake_set(MOTOR_BRAKE_HOLD);
      chassis.drive_set(30, 30);
      pros::delay(200);
      PTOPiston.set_value(true);
      Arm.move(-127);
      pros::delay(100);
      chassis.drive_set(-127, -127);
      pros::delay(500);
      chassis.drive_set(0, 0);
      Arm.brake();
    }
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
      chassis.drive_brake_set(MOTOR_BRAKE_COAST);
      PTOPiston.set_value(false);
    }
    */
    Arm.brake();
  }
}

void BangBangLoop() {
  int BangInputRPM = 150;
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
  int i = 0;
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

void Wing_Control() {
  int i = 1;
  while (true)
  {
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      if (i == 1) {
        LeftWing.set_value(false);
        RightWing.set_value(false);
        i = 0;
      } else {
        LeftWing.set_value(true);
        RightWing.set_value(true);
        i = 1;
      }
    }
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
      if (i == 1) {
        LeftWing.set_value(false);
        i = 0;
      } else {
        LeftWing.set_value(true);
        i = 1;
      }
    }
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
      if (i == 0) {
        RightWing.set_value(false);
        i = 0;
      } else {
        RightWing.set_value(true);
        i = 1;
      }
    }
    pros::delay(300);
  }
}

void initialize() {
  pros::delay(500);
  chassis.drive_imu_reset(); 
  chassis.drive_sensor_reset();
  chassis.opcontrol_curve_buttons_toggle(true); // Enables modifying the controller curve with buttons on the joysticks
  chassis.opcontrol_drive_activebrake_set(0.1); // Sets the active brake kP. We recommend 0.1.
  chassis.opcontrol_curve_default_set(0, 0); // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)  
  default_constants(); // Set the drive to your own constants from autons.cpp!

  ez::as::auton_selector.autons_add({
    //Auton("Skills Auton", Skills_Auton),
    Auton("Near Auton", Near_Auton),
    Auton("Far Auton", Far_Auton),
    Auton("Skills Auton", Skills_Auton),
     
  });

  chassis.initialize();
  ez::as::initialize();

  pros::Task Control_Arm(Arm_Control);
  pros::Task Control_FlyWheel(FlyWheel_Control);
  pros::Task Control_Wings(Wing_Control);
  PTOPiston.set_value(false);
}

void disabled() {

}

void competition_initialize() {

}

void autonomous() {
  chassis.pid_targets_reset(); 
  chassis.drive_imu_reset(); 
  chassis.drive_sensor_reset(); 
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD); 
  ez::as::auton_selector.selected_auton_call(); 
}

void opcontrol() {
  LeftWing.set_value(false);
  RightWing.set_value(false);
  chassis.drive_brake_set(MOTOR_BRAKE_COAST);
  Arm.set_brake_mode(MOTOR_BRAKE_HOLD); 
  FlyWheel.set_brake_mode(MOTOR_BRAKE_COAST);
  pros::Task Control_Arm(Arm_Control);
  pros::Task Control_FlyWheel(FlyWheel_Control);
  while (true) {

    /*
    // PID Tuner
    // After you find values that you're happy with, you'll have to set them in auton.cpp
    if (!pros::competition::is_connected()) { 
      // Enable / Disable PID Tuner
      if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_Y)) 
        chassis.pid_tuner_toggle();
        
      // Trigger the selected autonomous routine 
      if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) 
        Far_Auton();

      chassis.pid_tuner_iterate(); // Allow PID Tuner to iterate
    } 
    */

    chassis.opcontrol_arcade_standard(ez::SPLIT);
    pros::delay(ez::util::DELAY_TIME);
  }
}