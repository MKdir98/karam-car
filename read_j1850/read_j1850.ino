#define J1850_VPW_PIN 23

void setup() {
  Serial.begin(115200);

  pinMode(J1850_VPW_PIN, INPUT);
}

void loop() {
  // Read the J1850 VPW data
  byte data[8];
  int count = readJ1850VPW(data);

  // Print the data
  if (count > 0) {
    Serial.print("Data: ");
    for (int i = 0; i < count; i++) {
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }

    // Check the type of data
    switch (data[0]) {
      case 0x00:
        Serial.println("Standard data frame");
        break;
      case 0x01:
        Serial.println("Request data frame");
        break;
      case 0x02:
        Serial.println("Response data frame");
        break;
      case 0x03:
        Serial.println("Error data frame");
        break;
      default:
        Serial.println("Unknown data frame type");
        break;
    }

    // Check for errors
    if (data[1] & 0x80) {
      Serial.println("Error detected");
      Serial.print("Error code: ");
      Serial.println(data[1] & 0x7F);
    }
  }

  delay(100);
}

int readJ1850VPW(byte* data) {
  int count = 0;
  while (true) {
    // Wait for the start bit
    while (digitalRead(J1850_VPW_PIN) == LOW);

    // Read the data bits
    for (int i = 0; i < 8; i++) {
      data[count] = 0;
      for (int j = 0; j < 8; j++) {
        data[count] |= digitalRead(J1850_VPW_PIN) << j;
        delayMicroseconds(125);
      }
      count++;
    }

    // Check for the stop bit
    if (digitalRead(J1850_VPW_PIN) == HIGH) {
      break;
    }
  }

  return count;
}
