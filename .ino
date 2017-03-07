#include <dht11.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
dht11 DHT11;

#define DHT11PIN 16
const char* ssid = "Connectify-muh me lo";
const char* password = "pawankumar123";
WiFiClient client;
unsigned long myChannelNumber = 237692;
const char * myWriteAPIKey = "ZYQ5562BCKQYE1EI";
uint8_t temperature, humidity;
void setup()
{
  Serial.begin(115200);
  //dht11.begin();
  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
}

void loop()
{
  float sum_humid = 0.0;
  float sum_temp = 0.0;
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
  delay(1000);
  Serial.print("Humidity (%): ");
  for (int i = 0; i < 100; i++) {

    float humid = DHT11.humidity;
    sum_humid = sum_humid + humid;
    Serial.println(humid, 2);
    //delay(1000);
  }
  float avg_humid = sum_humid / 100;
  for (int i = 0; i < 100; i++) {
    float temp = DHT11.temperature;
    sum_temp = sum_temp + temp;
    Serial.println(temp, 2);
    //delay(1000);
  }
  float avg_temp = sum_temp / 100;
  Serial.print("Temperature (°C): ");
  Serial.println(avg_temp, 2);
  ThingSpeak.writeField(myChannelNumber, 1, avg_temp, myWriteAPIKey);
  delay(15000);
  ThingSpeak.writeField(myChannelNumber, 2, avg_humid, myWriteAPIKey);
  delay(15000);

}
//
// END OF FILE
//
