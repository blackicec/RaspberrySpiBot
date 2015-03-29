
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

int motor_pin_setup(const char* config_file_location);
void forward_movement();
void reverse_movement();
void left_turn_movement();
void right_turn_movement();
void stop();
void instruction_handler(int direction);

#endif
