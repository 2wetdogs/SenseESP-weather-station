#include <Adafruit_BME280.h>
#include "sensesp/signalk/signalk_output.h"
#include "sensesp_app_builder.h"

using namespace sensesp;

reactesp::ReactESP app;

Adafruit_BME280 bme;

unsigned long delayTime;

SKOutput<float>* pressure_output;
SKOutput<float>* temperature_output;
SKOutput<float>* humidity_output;
SKOutput<float>* altitude_output;

void printValues(){

  float temperature = bme.readTemperature() * 9 / 5 + 32;
  float pressure_Pa = bme.readPressure() *  0.000296133971008484;
  float humidity_ratio = bme.readHumidity();
  float altitude_m = bme.readAltitude(1013.25);

  temperature_output->set_input(temperature);
  pressure_output->set_input(pressure_Pa);
  humidity_output->set_input(humidity_ratio);
  altitude_output->set_input(altitude_m);


  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println("°C");
  
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature() * 9 / 5 + 32); 
  Serial.println("°F");

  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() * .000296133971008484);
  Serial.println(" Inches of Mercury");

  Serial.print("Relative Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println("%");

  Serial.print("Altitude = ");
  Serial.print(bme.readAltitude(1013.25));
  Serial.println(" meters???");

  Serial.println();
  Serial.println();
};

void setup() 
{
  Serial.begin(115200);
  delayTime = 1000;


  SensESPAppBuilder builder;
  sensesp_app = (&builder)
       ->set_hostname("2wetdogs-weather-station-01")
       ->set_wifi("HomeAP", "6122824629")
       ->get_app();

  
  if (!bme.begin(0x76)) 
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
    
  pressure_output = new SKOutput<float>(
    "2wetdogs.01.environment.pressure",
    "/2wetdogs/01/sensors/bme280/pressure",
    new SKMetadata("Inches", "Barometric Pressure")
  );

  temperature_output = new SKOutput<float>(
    "2wetdogs.01.environment.temperature",
    "/2wetdogs/01/sensors/bme280/temperature",
    new SKMetadata("F", "Tempature in Fahrenheit.")
  );

  humidity_output = new SKOutput<float>(
    "2wetdogs.01.environment.humidity",
    "/2wetdogs/01/sensors/bme280/humidity",
    new SKMetadata("%", "Relative humidity.")
  );

  altitude_output = new SKOutput<float>(
    "2wetdogs.01.environment.altitude",
    "/2wetdogs/01/sensors/bme280/altitude",
    new SKMetadata("meters???", "Altitude above sea level.")
  );

    sensesp_app->start();
}

void loop()
{
  static unsigned long last_run = millis();

  if (millis() - last_run >= delayTime) {
    printValues();
    last_run = millis();
  }

  app.tick();
}