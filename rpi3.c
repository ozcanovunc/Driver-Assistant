#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define VIBR_MTR_01 8
#define VIBR_MTR_02 9

void hardwareSetup(){
	if(wiringPiSetup() == -1){
		exit(-1);
	} 

	// Set which gpio pins are output for vibration motor
	pinMode(VIBR_MTR_01,OUTPUT);
	pinMode(VIBR_MTR_02,OUTPUT);
}

// Drive motor via gpio output signal
int vibrMtrController(int motor_state){
	if(motor_state == 0){
		digitalWrite(VIBR_MTR_01,0);
		digitalWrite(VIBR_MTR_02,0);		
	}
	else if(motor_state == 1)
		digitalWrite(VIBR_MTR_01,1);
	else if(motor_state == 2){
		digitalWrite(VIBR_MTR_01,1);
		digitalWrite(VIBR_MTR_02,1);
	}
	else{
		fprintf(stderr,"wrong motor_state parameter is reached. Allowed limit is between [0,2]\n Current parameter value is %d\n",motor_state);
		return -1;	
	}

	return 0;
}

void reset(){
	vibrationStateChange(0);
}

void delayMS(int ms){
	delay(ms);
}
