/*
	Simple application allowing for
	motor control via serial comms
	A. Caravello 10/8/2016
*/

int trainSpeed[4];
int targetSpeed[4];
String trainDirection[4];
String targetDirection[4];
int trainAccel = 1;
int trainSelected = 0;
char actionSelected = '_';
int nextReport = 0;
int trains = 2;
int train = 0;

void setup()
{
	Serial.begin(9600);
	while (!Serial) {
		;
	}

	for (train = 1; train <= trains; train ++) {
		trainSpeed[train] = 0;
		targetSpeed[train] = 0;
		trainDirection[train] = "FORWARD";
		targetDirection[train] = "FORWARD";
	}
}

void loop()
{
	if (Serial.available() > 0) {
		if (trainSelected > 0) {
			byte incomingByte = Serial.read();
			actionSelected = (char) incomingByte;
			
			Serial.print("Selected action ");
			Serial.print(actionSelected);
			Serial.println("");

			if (actionSelected == 'a') {
				targetSpeed[trainSelected] += 5;
				Serial.print("Accelerate train ");
				Serial.print(trainSelected);
				Serial.print(" to ");
				Serial.print(targetSpeed[trainSelected],DEC);
				Serial.println("");
			}
			else if (actionSelected == 'd') {
				targetSpeed[trainSelected] -= 5;
				Serial.print("Decelerate");
				Serial.print(trainSelected);
				Serial.println("");
			}
			else if (actionSelected == 's') {
				targetSpeed[trainSelected] = 0;
				Serial.println("Stop");
				Serial.print(trainSelected);
				Serial.println("");
			}
			else if (actionSelected == '<') {
				targetDirection[trainSelected] = "FORWARD";
				Serial.println("Forward");
			}
			else if (actionSelected == '>') {
				targetDirection[trainSelected] = "REVERSE";
				Serial.println("Reverse");
			}
			trainSelected = 0;
		}
		else {
			trainSelected = Serial.parseInt();
			if (trainSelected > trains) {
				trainSelected = 0;
			}
			Serial.print("Selected train ");
			Serial.print(trainSelected);
			Serial.println("");
		}
	}

	for (train = 1; train <= trains; train ++) {
		if (targetDirection[train] != trainDirection[train]) {
			if (trainSpeed[train] > 0) {
				trainSpeed[train] -= trainAccel;
			}
			else {
				trainDirection[train] = targetDirection[train];
			}
		}
		else if (trainSpeed[train] < targetSpeed[train]) {
			trainSpeed[train] += trainAccel;
		}
		else if (trainSpeed[train] > targetSpeed[train]) {
			trainSpeed[train] -= trainAccel;
		}
	}

	if (nextReport < 1) {
		showReport();
		nextReport = 60;
	}
	else {
		nextReport --;
	}

	delay(50);
}

void showReport() {
	for (train = 1; train <= trains; train ++) {
		Serial.print("Train ");
		Serial.print(train);

		if (trainSpeed[train] > 0) {
			Serial.print(" is moving ");
			Serial.print(trainDirection[train]);
			Serial.print(" at ");
			Serial.print(trainSpeed[train],DEC);
		}
		else {
			Serial.print(" is stopped");
		}

		if (trainSpeed[train] < targetSpeed[train]) {
			Serial.print(". It is ");
			Serial.println("accelerating. ");
		}
		else if (trainSpeed[train] > targetSpeed[train]) {
			Serial.print(". It is ");
			Serial.println("decelerating. ");
		}
		else {
			Serial.println(".");
		}
	}
	if (trainSelected > 0) {
		Serial.print("You have selected train ");
		Serial.println(trainSelected,DEC);
	}
}
