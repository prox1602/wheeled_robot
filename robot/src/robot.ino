#include "RobotController.h"

RobotController Robot;

void setup() {
  delay(2000);
  Serial.begin(115200);
  Robot.setup();

}

void loop() {
  Robot.run();
}
