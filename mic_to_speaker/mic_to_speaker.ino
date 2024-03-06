/**
 * @file streams-analog-serial.ino
 * @author Phil Schatzmann
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-stream/streams-adc-serial/README.md
 * @copyright GPLv3
 * #TODO retest is outstanding
 */

#include "Arduino.h"
#include "AudioTools.h"
#include <WiFi.h>
#include <WebSocketsClient.h>



class WebSocketOutput : public AudioOutput {

  WebSocketsClient webSocket;

public:
  char* ssid;
  char* password;
  char* webSocketServer;
  int webSocketPort;
  String webSocketPath;
  WebSocketOutput(char* ssid, char* password, char* webSocketServer, int webSocketPort, String webSocketPath) {
    this->ssid = ssid;
    this->password = password;
    this->webSocketServer = webSocketServer;
    this->webSocketPort = webSocketPort;
    this->webSocketPath = webSocketPath;
  }

  /// Starts the processing with 2 channels
  bool begin() {
    TRACED();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    webSocket.begin(webSocketServer, webSocketPort, webSocketPath);
    // webSocket.onEvent(webSocketEvent);
    return true;
  }

  // AudioInfo defaultConfig(){
  //     AudioInfo info;
  //     info.channels = 2;
  //     info.sample_rate = 44100;
  //     info.bits_per_sample = sizeof(T)*8;
  //     return info;
  // }

  /// Sets the CsvOutput as inactive
  void end() {
    TRACED();
  }

  virtual size_t write(const uint8_t* data, size_t len) override {
    if (webSocket.isConnected()) {
        webSocket.sendBIN(data, len);
        data++;
    } else {
      Serial.println("websocket is disconnected...");
    }
    return len;
  }

  void loopWebSocket() {
    webSocket.loop();
  }
};

AnalogAudioStream in;
AudioInfo info(44100, 1, 16);
char* ssid = "Redmi 9A";
char* password = "qazwsxedc";
char* webSocketServer = "192.168.43.219";
// char* ssid = "iToll";
// char* password = "jooj00@@";
// char* webSocketServer = "192.168.101.152";
int webSocketPort = 8666;
String webSocketPath = "/";
WebSocketOutput out(ssid, password, webSocketServer, webSocketPort, webSocketPath);
StreamCopy copier(out, in);

// Arduino Setup
void setup(void) {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Warning);
  auto cfgRx = in.defaultConfig(RX_MODE);
  cfgRx.copyFrom(info);
  in.begin(cfgRx);
  out.begin();
}

// Arduino loop - copy data
void loop() {
  copier.copy();  //
  out.loopWebSocket();
}
