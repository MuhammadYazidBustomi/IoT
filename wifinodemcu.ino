#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial
#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "Tomi1503" 
#define MQTT_PASS "aio_HkZN08OxEZ4o8sFPVK77TlX1Yww7"
#define relay D2

const char *ssid = "Redmi";
const char *pass = "yazid1503";
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);
Adafruit_MQTT_Subscribe Lock = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/Lock");


void setup() {
Serial.begin(9600);
delay(10);
mqtt.subscribe(&Lock);
pinMode(relay, OUTPUT);
digitalWrite(relay, LOW);
Serial.println("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, pass);
while (WiFi.status() != WL_CONNECTED){
  delay(500);
  Serial.print(".");
 }
Serial.println("");
Serial.println("WiFi connected");
}

void loop(){
  MQTT_connect();  
Adafruit_MQTT_Subscribe * subscription;
while ((subscription = mqtt.readSubscription(5000)))
     {   
   if (subscription == &Lock)
     {
      //Print the new value to the serial monitor
      Serial.println((char*) Lock.lastread);     
   if (!strcmp((char*) Lock.lastread, "Close"))
      {
        digitalWrite(relay, LOW);
        Serial.print("Door Unlocked");
    }
    if (!strcmp((char*) Lock.lastread, "Open"))
      {
        digitalWrite(relay, HIGH);
        Serial.print("Door Closed");
    }
 }  
     }
}

void MQTT_connect() 
{
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) 
  {
    return;
  }
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  {       
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) 
       {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
}
