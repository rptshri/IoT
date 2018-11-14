#include <ThingSpeak.h>

#include <ESP8266WiFi.h>
String apiKey = "WVZRLOLAVI4333Q5";
const char server[] = "api.thingspeak.com";
WiFiClient client;
void setup() {
  WiFi.begin("0x32", "00110010");
}

void loop() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    delay(500 );
  }

  if (client.connect(server, 80)) { // "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += millis();
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
  }

}
