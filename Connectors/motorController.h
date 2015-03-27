
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

long 

void forward_movement();

void controlHandler(int direction);

#endif



int new_direction (local) = STOP_CTRL;
int currentDirection (global) = -1;
// stop() stop the vehicle
###thread_1 - will read the data 
while(1) {
	direction = read(. . .)
	
	if( read[failed] ) {
		direction = currentDirection;
		continue;
	}
	
	if(direction != currentDirection) {
		currentDirection = direction;
	}
}

##thread_2
function() {
	
	while(1) {
		if(new_direction != currentDirection) {
			direction_change(new_direction);
		}
	}
}

void direction_change(const int new_direction) {
	switch(direction) {
		case LEFT_CTRL:
			break;
		case RIGHT_CTRL:
			break;
		case FORWARD_CTRL:
			break;
		case REVERSE_CTRL:
			break;
		default: // We can either ignore the instruction all together or force the vehicle to come to a full stop (maybe put this in config file as option)
			break;
	}
}