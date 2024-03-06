#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

// WiFi credentials
const char* ssid     = "Redmi 9A";
const char* password = "qazwsxedc";

// WebSockets
WebSocketsClient webSocket;

void setup() {
  Serial.begin(115200);
  Serial.println("esp01 started");

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Setup WebSocket connection
  webSocket.begin("192.168.43.219", 8666);

  // Use this to verify server connection, remove if not needed
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
  Serial.println("test");
  if (Serial.available() > 0) {
    // Read data from Arduino
    String data = Serial.readStringUntil('\n');
    
    // Send data to WebSocket server
    webSocket.sendTXT(data);
    Serial.println(data);
  }
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      Serial.printf("[WSc] Connected to url: %s\n", payload);
      break;
    case WStype_TEXT:
      Serial.printf("[WSc] Received text: %s\n", payload);

      // Send data to Arduino
      Serial.println((char*)payload);
      break;
  }
}
