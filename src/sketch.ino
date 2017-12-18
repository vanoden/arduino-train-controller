#include <Wire.h>
#include <Adafruit_MotorShield.h>

int targetSpeed[4];
int currentSpeed[4];
int accelaration[4];
int controlSpeed[4];
boolean reverse[4];
boolean wasReverse[4];

int track = 1;

String request = "";
boolean stringComplete = false;
boolean echo = true;

String device_label = "traincontrol01";
String device_version = "0.0.1";

Adafruit_MotorShield controller = Adafruit_MotorShield();

Adafruit_DCMotor *track1 = controller.getMotor(1);
Adafruit_DCMotor *track2 = controller.getMotor(2);

void setup()
{
	Serial.begin(19200);
	request.reserve(255);
	Serial.println("Train Layout v0.1");

	controller.begin();
	track1->setSpeed(0);
	track1->run(FORWARD);
	reverse[0] = false;
	wasReverse[0] = false;

	track2->setSpeed(0);
	track2->run(FORWARD);
	reverse[1] = false;
	wasReverse[1] = false;
}

void loop()
{
	// Process Requests
	if (stringComplete) {
		if (request.startsWith("identify")) {
			Serial.println(device_label);
		}
		else if (request.startsWith("version") {
			Serial.println(device_version);
		}
		else if (request.startsWith("f") {
			reverse[track] = false;
		}
		else if (request.startsWith("r") {
			reverse[track] = true;
		}
		else if (request.startsWith("stop")) {
			targetSpeed[track] = 0;
			Serial.print("Bringing track ");
			Serial.print(track);
			Serial.println(" to a stop");
		}
		else if (request.startsWith("emer")) {
			targetSpeed[track] = 0;
			currentSpeed[track] = 0;
			Serial.print("Immediately stopped track ");
			Serial.println(track);
		}
		else if (request.startsWith("track")) {
			int delim = request.indexOf(" ");
			if (delim > 0) {
				track = request.substring(delim).toInt();
				if (track > 4) {
					track = 4;
				}
				Serial.print("Track is now ");
				Serial.println(track);
			}
			else {
				Serial.print("Track is ");
				Serial.println(track);
			}
		}
		else if (request.startsWith("speed")) {
			int delim = request.indexOf(" ");
			if (delim > 0) {
				targetSpeed[track] = request.substring(delim).toInt();
				Serial.print("Track ");
				Serial.print(track);
				Serial.print("target speed is ");
				Serial.println(targetSpeed[track]);
			}
			else {
				Serial.print("Track ");
				Serial.print(track);
				Serial.print(" current speed is ");
				Serial.println(controlSpeed[track]);
			}

			// Keep Within Bounds
			if (currentSpeed[track] > 255) {
				currentSpeed[track] = 255;
			}
			if (currentSpeed[track] < 0) {
				currentSpeed[track] = 0;
			}
		}
		else {
			Serial.println("Unknown command");
		}

		request = "";
		stringComplete = false;
	}
	else {
		delay(100);
	}

	// Loop through 4 tracks
	uint8_t i;
	for (i = 1; i <= 4; i++) {
		// Acceleration
		if (targetSpeed[i] > currentSpeed[i]) {
			currentSpeed[i] += 1;
			if (currentSpeed[i] > targetSpeed[i]) {
				currentSpeed[i] = targetSpeed[i];
			}
		}
		else if (targetSpeed[i] < currentSpeed[i]) {
			currentSpeed[i] -= 1;
			if (currentSpeed[i] < targetSpeed[i]) {
				currentSpeed[i] = targetSpeed[i];
			}
		}

		// Change if different
		if (controlSpeed[i] != currentSpeed[i]) {
			controlSpeed[i] = currentSpeed[i];
			if (controlSpeed[i] < 15) {
				controlSpeed[i] = 0;
			}
			if (i == 1) {
				track1->setSpeed(controlSpeed[i]);
			}
			else if (i == 2) {
				track2->setSpeed(controlSpeed[i]);
			}
		}
	}

	while (Serial.available()) {
		char inChar = (char)Serial.read();
		if (inChar == 13) {
			stringComplete = true;
			if (echo) {
				Serial.println();
			}
		}
		else {
			request += inChar;
			if (echo) {
				Serial.print(inChar);
			}
		}
	}
}
