#include <SPI.h>
#include <mcp_can.h>

// MCP2515 Module
#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(10);                               // Set CS to pin 10

// MAX9814 Module
const int sampleWindow = 1000; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

void setup() {
  Serial.begin(115200);
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");
  
  CAN0.setMode(MCP_NORMAL);                     // Change to normal mode to allow messages to be transmitted
}

void loop() {
  // Read data from MCP2515
  unsigned char len = 0;
  unsigned char buf[8];
  unsigned long id = 0;
  unsigned char ext = 0;
  if (CAN0.readMsgBuf(&id, &ext, &len, buf) == CAN_OK) {
    // You can now do something with 'buf', which contains the CAN message bytes.
    Serial.write(buf, len);
  }

  // Read data from MAX9814
  unsigned long startMillis= millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
     sample = analogRead(0);
     if (sample < 1024)  // toss out spurious readings
     {
        if (sample > signalMax)
        {
           signalMax = sample;  // save just the max levels
        }
        else if (sample < signalMin)
        {
           signalMin = sample;  // save just the min levels
        }
     }
  }
  peakToPeak = signalM
  ax - signalMin;  // max - min = peak-peak amplitude
  double volts = (peakToPeak * 3.3) / 1024;  // convert to volts

  // Send data via ESP01
  Serial.println(volts);
}
