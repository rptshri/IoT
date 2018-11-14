#include <OneWire.h>

#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

#include <ESP8266WiFi.h>

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

String apiKey = "APIKEY";
const char* ssid = "SSID";
const char* password = "PASSWORD";

const char* server = "api.thingspeak.com";

WiFiClient client;


void setup(void)
{
  // start serial port
  Serial.begin(115200);
  delay(10);
  Serial.println("Dallas Temperature IC Control Library");

  // Start up the library
  sensors.begin();

  WiFi.begin(ssid, password);
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  float h = (sensors.getTempCByIndex(0));  
  float t = (DallasTemperature::toFahrenheit(h));
  
  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempCByIndex(0));  

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from sensor!");
    return;
  }
 
  if (client.connect(server,80)) {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(t);
           
           postStr += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(postStr.length());
     client.print("\n\n");
     client.print(postStr);
 
     Serial.print("Temperature: ");
     Serial.print(t);
     Serial.print(" degrees F or: ");
     Serial.print(h);
     Serial.println(" Degrees C send to Thingspeak");
  }
  client.stop();
 
  Serial.println("Waiting...");
  // thingspeak needs minimum 15 sec delay between updates
  // I use a 1 inute delay
  delay(60000);
}
// END


