const int STEP_PIN = 2;
const int DIR_PIN  = 3;
const int ENA_PIN  = 4;

long targetSteps = 0;
long currentStep = 0;
unsigned long stepDelayMicros = 0;
bool moving = false;

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);

  digitalWrite(ENA_PIN, LOW);   // Enable DM556
  digitalWrite(STEP_PIN, LOW);

  Serial.begin(115200);
  Serial.println("READY");
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd.startsWith("MOVE")) parseMove(cmd);
  }

  if (moving && currentStep < targetSteps) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(stepDelayMicros / 2);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(stepDelayMicros / 2);
    currentStep++;
  }

  if (moving && currentStep >= targetSteps) {
    moving = false;
    Serial.println("DONE");
  }
}

void parseMove(String cmd) {
  int s1 = cmd.indexOf(' ');
  int s2 = cmd.indexOf(' ', s1 + 1);

  long steps = cmd.substring(s1 + 1, s2).toInt();
  long speed = cmd.substring(s2 + 1).toInt();

  if (steps == 0 || speed <= 0) return;

  digitalWrite(DIR_PIN, steps > 0 ? HIGH : LOW);
  targetSteps = abs(steps);
  stepDelayMicros = 1000000UL / speed;

  currentStep = 0;
  moving = true;
}
