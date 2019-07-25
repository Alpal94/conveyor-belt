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

#define relay12VPinMaster 13
#define on 0
#define off 1

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper frontStepper(HALFSTEP, frontMotorPin1, frontMotorPin3, frontMotorPin2, frontMotorPin4);
AccelStepper rearStepper(HALFSTEP, rearMotorPin1, rearMotorPin3, rearMotorPin2, rearMotorPin4);

bool beltActive = false;
bool firstRun = false;
bool finalRun = false;
bool _test = false;

void setup() {
	Serial.begin(9600);
	if(_test) test();
	pinMode(relay12VPinMaster, OUTPUT);
	digitalWrite(relay12VPinMaster, off);
}

void loop() {
	float distance = 5000;
	stop();
	run();
	
	if(_test) run();
	else {
		if (Serial.available()) {
			char data = Serial.read();
			Serial.write(data);
			if(data == 's' && !beltActive && frontStepper.distanceToGo() == 0 && rearStepper.distanceToGo() == 0) {
				digitalWrite(relay12VPinMaster, on);
				beltActive = true;
				firstRun = true;
				towardsCupboard(distance);
				run();

				Serial.print("AA");
			}
		} else {
			if(!firstRun) Serial.write('N');
		}
		if(firstRun) {
			if (rearStepper.distanceToGo() == 0) { //Only interested in the rear stepper reaching its target
				towardsDesk(1);
				run();
				firstRun = false;
				finalRun = true;
				Serial.print("D");
			} else {
				Serial.print("M");
			}
		}
		if(finalRun) {
			if (frontStepper.distanceToGo() == 0) { //Only interested in the front stepper reaches its target
				firstRun = false;
				finalRun = false;
				beltActive = false;

				//Deactivate belt
				stop();
				run();
				digitalWrite(relay12VPinMaster, off);
			}
		}
		run();
	}
}

void test() {
	//towardsDesk(4200);
	//towardsCupboard(4200);
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

void towardsCupboard(int _dist) {
	//Driver
	float _fSpeed = 80.0;
	float _fAccel = 80.0;
	//Back
	float _rSpeed = 80.0;
	float _rAccel = 80.0;

        frontStepper.setMaxSpeed(_fSpeed);
        frontStepper.setAcceleration(_fAccel);
        frontStepper.setSpeed(_fSpeed);

        rearStepper.setMaxSpeed(_rSpeed);
        rearStepper.setAcceleration(_rAccel);
        rearStepper.setSpeed(_rSpeed);

        frontStepper.moveTo(_dist);
        rearStepper.moveTo(-_dist);
}

void towardsDesk(int dist) {
	//Back
	float fSpeed = 80.0;
	float fAccel = 80.0;
	//Driver
	float rSpeed = 80.0;
	float rAccel = 80.0;

        frontStepper.setMaxSpeed(fSpeed);
        frontStepper.setAcceleration(fAccel);
        frontStepper.setSpeed(fSpeed);

        rearStepper.setMaxSpeed(rSpeed);
        rearStepper.setAcceleration(rAccel);
        rearStepper.setSpeed(rSpeed);

        frontStepper.moveTo(-dist);
        rearStepper.moveTo(dist);
}

void stop() {
        frontStepper.setMaxSpeed(1);
        frontStepper.setAcceleration(1);
        frontStepper.setSpeed(1);

        rearStepper.setMaxSpeed(1);
        rearStepper.setAcceleration(1);
        rearStepper.setSpeed(1);

        frontStepper.moveTo(1);
        rearStepper.moveTo(1);
}
