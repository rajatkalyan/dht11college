#include <dht11.h>
#include <ESP8266WiFi.h>  
#include <WiFiClient.h>  
#include <ThingSpeak.h> 
dht11 DHT11;

#define DHT11PIN 2
const char* ssid = "WiFiName";  
const char* password = "Password";  
WiFiClient client;  
unsigned long myChannelNumber = 174314;  
const char * myWriteAPIKey = "JQPT38EM56K0MJM1";  
uint8_t temperature, humidity;  
void setup()
{
  Serial.begin(115200);
  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
}

void loop()
{
  Serial.println("\n");

  int chk = DHT11.read(DHT11PIN);

  Serial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK: 
    Serial.println("OK"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.println("Checksum error"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.println("Time out error"); 
    break;
    default: 
    Serial.println("Unknown error"); 
    break;
  }

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature (°C): ");
  Serial.println((float)DHT11.temperature, 2);

  
}
//
// END OF FILE
//
