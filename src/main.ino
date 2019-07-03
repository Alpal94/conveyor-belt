#include "AccelStepper.h"
#define HALFSTEP 8

// Front motor pin definitions
#define frontMotorPin1  3     // IN1 on the ULN2003 driver 1
#define frontMotorPin2  4     // IN2 on the ULN2003 driver 1
#define frontMotorPin3  5     // IN3 on the ULN2003 driver 1
#define frontMotorPin4  6     // IN4 on the ULN2003 driver 1
// Rear motor pin definitions
#define rearMotorPin1  7     // IN1 on the ULN2003 driver 1
#define rearMotorPin2  8     // IN2 on the ULN2003 driver 1
#define rearMotorPin3  9     // IN3 on the ULN2003 driver 1
#define rearMotorPin4  10    // IN4 on the ULN2003 driver 1

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper frontStepper(HALFSTEP, frontMotorPin1, frontMotorPin3, frontMotorPin2, frontMotorPin4);
AccelStepper rearStepper(HALFSTEP, rearMotorPin1, rearMotorPin3, rearMotorPin2, rearMotorPin4);

bool beltActive = false;
bool _test = true;

void setup() {
	Serial.begin(9600);
	if(_test) test();
}

void loop() {
	float distance = 600;
	if(_test) run();
	else {
		if (Serial.available()) {
			char data = Serial.read();
			Serial.write(data);
			if(data == 's' && !beltActive && frontStepper.distanceToGo() == 0 && rearStepper.distanceToGo() == 0) {
				beltActive = true;
				towardsDesk(distance);
				run();

				Serial.print("AA");
			}
		} else {
			if(!beltActive) Serial.write('N');
		}
		if(beltActive) {
			if (frontStepper.distanceToGo() == 0 && rearStepper.distanceToGo() == 0) {
				towardsCupboard(distance - 100);
				beltActive = false;
				Serial.print("D");
			} else {
				Serial.print("M");
			}
		}
		run();
	}
}

void test() {
	towardsDesk(1000);
}

void frontToCupboard() {
	float speed = 75.0;
	float acceleration = 75.0;
        frontStepper.setMaxSpeed(speed);
        frontStepper.setAcceleration(acceleration);
        frontStepper.setSpeed(speed);

        frontStepper.moveTo(1200);
}

void run() {
	frontStepper.run();
	rearStepper.run();
}
void lock() {
        frontStepper.setMaxSpeed(150.0);
        frontStepper.setAcceleration(60.0);
        frontStepper.setSpeed(150);

        rearStepper.setMaxSpeed(150.0);
        rearStepper.setAcceleration(60.0);
        rearStepper.setSpeed(150);

        rearStepper.moveTo(500);
}
float speed = 75.0;
float acceleration = 75.0;
void towardsCupboard(int dist) {
        frontStepper.setMaxSpeed(speed);
        frontStepper.setAcceleration(acceleration);
        frontStepper.setSpeed(speed);

        rearStepper.setMaxSpeed(speed);
        rearStepper.setAcceleration(acceleration);
        rearStepper.setSpeed(speed);

        frontStepper.moveTo(dist + 100);
        rearStepper.moveTo(-dist);
}

void towardsDesk(int dist) {
        frontStepper.setMaxSpeed(speed);
        frontStepper.setAcceleration(acceleration);
        frontStepper.setSpeed(speed);

        rearStepper.setMaxSpeed(speed);
        rearStepper.setAcceleration(acceleration);
        rearStepper.setSpeed(speed);

        frontStepper.moveTo(-dist);
        rearStepper.moveTo(dist + 99);
}
