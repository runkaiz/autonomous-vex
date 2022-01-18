#include "main.h"

/**
 * A callback function for LLEMU's center button.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "Winston is always late");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
#define LEFT_FRONT_WHEEL_PORT 1
#define LEFT_BACK_WHEEL_PORT 2
#define RIGHT_FRONT_WHEEL_PORT 3
#define RIGHT_BACK_WHEEL_PORT 4
#define BELT_PORT 5
#define ARM_LEFT_PORT 6
#define ARM_RIGHT_PORT 7
// #define BASKET_PORT 8
#define MOTOR_MAX_SPEED 100 // The motor has the 36 Gearset

void autonomous() {
  pros::Motor left_front_wheel (LEFT_FRONT_WHEEL_PORT);
  pros::Motor left_back_wheel (LEFT_BACK_WHEEL_PORT);
  pros::Motor right_front_wheel (RIGHT_FRONT_WHEEL_PORT, true); // This reverses the motor
  pros::Motor right_back_wheel (RIGHT_BACK_WHEEL_PORT, true); // This reverses the motor

  pros::Motor belt_motor (BELT_PORT);
  pros::Motor arm_left_motor (ARM_LEFT_PORT);
  pros::Motor arm_right_motor (ARM_RIGHT_PORT);
  // pros::Motor basket_motor(BASKET_PORT);

  right_front_wheel.move_relative(1000, MOTOR_MAX_SPEED);
  right_back_wheel.move_relative(1000, MOTOR_MAX_SPEED);
  left_front_wheel.move_relative(1000, MOTOR_MAX_SPEED);
  left_back_wheel.move_relative(1000, MOTOR_MAX_SPEED);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor left_mtr(1);
	pros::Motor right_mtr(2);

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);

		left_mtr = left;
		right_mtr = right;
		pros::delay(20);
	}
}
