#include "motorController.h"
#include <stdio.h>
#include <wiringPi.h>

/*
* The following two pins are used to move the RC car forward.
* To make this work, simply set these two outputs to HIGH and set
* the rev outputs to LOW
*/
int output_pin_left_for = 0; /* pin 11 (left side) */
int output_pin_right_for = 4; /* pin 16 (right side) */

/*
* Setting these two pins to HIGH while the forward pins are
* set to LOW, will make the RC car move in the reverse direction.
*/
int output_pin_left_rev = 2; /* pin 13 (left side) */
int output_pin_right_rev = 5; /* pin 18 (right side) */

int motor_pin_setup(const char* config_file_location) {
	/* TODO: if config file exists, then use the specified . . . */

    if(wiringPiSetup() == -1) {
        printf("An error occured during the wiringPi setup.\n");
        printf("Please be sure to have the WiringPi library installed.\n");
        return -1;
    }

    /* setup all pins defined above to be in output mode */
	pinMode(output_pin_left_for, OUTPUT);
	pinMode(output_pin_right_for, OUTPUT);
	pinMode(output_pin_left_rev, OUTPUT);
	pinMode(output_pin_right_rev, OUTPUT);

    return 0;
}

void forward_movement() {
	digitalWrite(output_pin_left_for, VLT_HIGH);
	digitalWrite(output_pin_right_for, VLT_HIGH);

	/* Make sure that no voltage is going out of the reverse pins */
	digitalWrite(output_pin_left_rev, VLT_LOW);
	digitalWrite(output_pin_right_rev, VLT_LOW);
}

void reverse_movement() {
	digitalWrite(output_pin_left_rev, VLT_HIGH);
	digitalWrite(output_pin_right_rev, VLT_HIGH);

	/* Make sure that no voltage is going out of the forward pins */
	digitalWrite(output_pin_left_for, VLT_LOW);
	digitalWrite(output_pin_right_for, VLT_LOW);
}

/*
* Making the left motors reverse while the right motor drive forward
* motors press forward will cause the vehicle to twist left
*/
void left_turn_movement() {
	digitalWrite(output_pin_left_for, VLT_LOW);
	digitalWrite(output_pin_left_rev, VLT_HIGH);

	digitalWrite(output_pin_right_for, VLT_HIGH);
	digitalWrite(output_pin_right_rev, VLT_LOW);
}

/*
* Turning right can be accomplished by mirroring the left_turn_movement().
* If the right motors push backwards while the left motors push forward,
* then the vehicle will twist rightward.
*/
void right_turn_movement() {
	digitalWrite(output_pin_left_for, VLT_HIGH);
	digitalWrite(output_pin_left_rev, VLT_LOW);

	digitalWrite(output_pin_right_for, VLT_LOW);
	digitalWrite(output_pin_right_rev, VLT_HIGH);
}

/*
* Set all outputs to low voltage, meaning the RC car will come to a complete
* stop.
*/
void stop() {
	digitalWrite(output_pin_left_for, VLT_LOW);
	digitalWrite(output_pin_right_for, VLT_LOW);
	digitalWrite(output_pin_left_rev, VLT_LOW);
	digitalWrite(output_pin_right_rev, VLT_LOW);
}

void instruction_handler(int direction) {
	switch(direction) {
		case LEFT_CTRL:
			stop();
			left_turn_movement();
			break;
		case RIGHT_CTRL:
			stop();
			right_turn_movement();
			break;
		case FORWARD_CTRL:
			stop();
			forward_movement();
			break;
		case REVERSE_CTRL:
			stop();
			reverse_movement();
			break;
		default: /* We could either stop the vehicle or ignore the command. Refer to config file */
			stop();
			break;
	}
}
