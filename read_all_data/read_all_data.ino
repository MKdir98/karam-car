#define INPUT_PIN 23

void setup() {
  Serial.begin(115200);
  pinMode(INPUT_PIN, INPUT);
}

void loop() {
  int digitalData = digitalRead(INPUT_PIN);
  Serial.println(digitalData);
  delay(1000);
}
