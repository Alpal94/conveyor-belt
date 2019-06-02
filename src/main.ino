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

void setup() {
	Serial.begin(9600);
}

void loop() {
	if (Serial.available()) {
		char data = Serial.read();
		Serial.write(data);
		if(data == 's') {
			beltActive = true;
			Serial.print("AA");
		}
	} else {
		if(!beltActive) Serial.write('N');
	}
	if(beltActive) {
		towardsDesk(2800);
		if (frontStepper.distanceToGo() == 0 && rearStepper.distanceToGo() == 0) {
			towardsCupboard(2600);
			beltActive = false;
			Serial.print("D");
		} else {
			Serial.print("M");
		}
		run();
	}
}

void run() {
	frontStepper.run();
	rearStepper.run();
}

void towardsCupboard(int dist) {
        frontStepper.setMaxSpeed(100.0);
        frontStepper.setAcceleration(40.0);
        frontStepper.setSpeed(100);

        rearStepper.setMaxSpeed(80.0);
        rearStepper.setAcceleration(40.0);
        rearStepper.setSpeed(80);

        frontStepper.moveTo(-dist - 800);
        rearStepper.moveTo(-dist);
}

void towardsDesk(int dist) {
        frontStepper.setMaxSpeed(100.0);
        frontStepper.setAcceleration(40.0);
        frontStepper.setSpeed(100);

        rearStepper.setMaxSpeed(150.0);
        rearStepper.setAcceleration(50.0);
        rearStepper.setSpeed(150);

        frontStepper.moveTo(dist);
        rearStepper.moveTo(dist + 1400);
}
