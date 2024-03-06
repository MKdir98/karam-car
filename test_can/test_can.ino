#include <SPI.h>
#include <mcp_can.h>
#include <WiFi.h>
#include <HTTPClient.h>

const int SPI_CS_PIN = 22;  // پین SS/CS
const char *server = "http://192.168.43.219:5000";
char* ssid = "Redmi 9A";
char* password = "qazwsxedc";

MCP_CAN can(SPI_CS_PIN);

void setup() {
  Serial.begin(115200);

  // اتصال به WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("اتصال به WiFi...");
  }
  Serial.println("اتصال موفق به WiFi");

  if (can.begin(CAN_500KBPS, CAN_250KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("MCP2515 Initialized Successfully!");
  } else {
    Serial.println("Error Initializing MCP2515...");
    while (1);
  }
}

void loop() {
  // دریافت داده از CAN
  unsigned char len = 0;
  unsigned char buf[8];
  unsigned long id;
  unsigned char ext;

  if (can.checkReceive() == CAN_MSGAVAIL) {
    can.readMsgBuf(&id, &ext, &len, buf);
    
    // ارسال داده به سرور HTTP
    sendCANDataToServer(buf, len);
  }
}

void sendCANDataToServer(unsigned char *data, unsigned char length) {
  HTTPClient http;
  
  // آدرس سرور و پارامترهای POST را تعیین کنید
  String url = server;
  url += "/receive_can_data";  
  // اتصال به سرور
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  // تبدیل داده‌های CAN به یک رشته JSON
  String jsonPayload = "{";
  for (int i = 0; i < length; i++) {
    jsonPayload += "\"byte" + String(i) + "\":" + data[i];
    if (i < length - 1) {
      jsonPayload += ",";
    }
  }
  jsonPayload += "}";

  // ارسال داده به سرور
  int httpResponseCode = http.POST(jsonPayload);
  
  // نمایش وضعیت ارسال در Serial Monitor
  Serial.print("HTTP Response Code: ");
  Serial.println(httpResponseCode);
  
  // قطع اتصال
  http.end();
  
  // تاخیر برای اجتناب از ارسال دوباره همان داده
  delay(1000);
}
