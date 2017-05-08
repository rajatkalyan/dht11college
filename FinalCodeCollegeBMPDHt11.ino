#include <dht11.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
#include <SFE_BMP180.h>
#include <Wire.h>
SFE_BMP180 pressure;
double baseline;
dht11 DHT11;

#define DHT11PIN 16
const char* ssid = "Connectify-pk";
const char* password = "pawankumar123";
WiFiClient client;
unsigned long myChannelNumber = 237692;
const char * myWriteAPIKey = "ZYQ5562BCKQYE1EI";
uint8_t temperature, humidity;
void setup()
{
  Serial.begin(115200);
  ///DHT11.begin();
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
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while (1); // Pause forever.
  }
  baseline = getPressure();

  Serial.print("baseline pressure: ");
  Serial.print(baseline);
  Serial.println(" mb");
}

void loop()
{
  float sum_humid = 0.0;
  float sum_temp = 0.0;
  Serial.println("\n");
  double a, P;
  P = getPressure();
  a = pressure.altitude(P, baseline);
  Serial.println(P);
  Serial.print("relative altitude: ");
  if (a >= 0.0) Serial.print(" "); // add a space for positive numbers
  Serial.print(a, 1);
  Serial.print(" meters, ");
  if (a >= 0.0) Serial.print(" "); // add a space for positive numbers
  Serial.print(a * 3.28084, 0);
  Serial.println(" feet");

  delay(500);
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
    //Serial.println(temp, 2);
    //delay(1000);
  }
  float avg_temp = sum_temp / 100;
  Serial.print("Temperature (°C): ");
  Serial.println(avg_temp, 2);
  ThingSpeak.writeField(myChannelNumber, 1, avg_temp, myWriteAPIKey);
  delay(15000);
  ThingSpeak.writeField(myChannelNumber, 2, avg_humid, myWriteAPIKey);
  delay(15000);
  ThingSpeak.writeField(myChannelNumber, 3, (int)P, myWriteAPIKey);
  //delay(15000);
}
double getPressure()
{
  char status;
  double T, P, p0, a;

  // You must first get a temperature measurement to perform a pressure reading.

  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:

    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Use '&P' to provide the address of P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          return (P);
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
}
//
// END OF FILE
//
