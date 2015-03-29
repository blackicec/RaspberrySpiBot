
#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

/* Voltage levels */
#define VLT_LOW 0
#define VLT_HIGH 1

/* Directional controls for the robot */
#define LEFT_CTRL 4
#define FORWARD_CTRL 8
#define RIGHT_CTRL 6
#define REVERSE_CTRL 2
#define STOP_CTRL 5

/*
* The following two pins are used to move the RC car forward.
* To make this work, simply set these two outputs to HIGH and set
* the rev outputs to LOW
*/
extern const int output_pin_left_for; /* pin 11 (left side) */
extern const int output_pin_right_for; /* pin 16 (right side) */

/*
* Setting these two pins to HIGH while the forward pins are
* set to LOW, will make the RC car move in the reverse direction.
*/
extern const int output_pin_left_rev; /* pin 13 (left side) */
extern const int output_pin_right_rev; /* pin 18 (right side) */

int motor_pin_setup(const char* config_file_location);
void forward_movement();
void reverse_movement();
void left_turn_movement();
void right_turn_movement();
void stop();
void instruction_handler(int direction);

#endif
