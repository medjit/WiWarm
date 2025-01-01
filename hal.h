#ifndef HAL_H
#define HAL_H

#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define DHT_READ_INTERVAL 60000
#define DS18B20_READ_INTERVAL 60000

//TODO:
//define pinout
#define DHTPIN 4     // Digital pin connected to the DHT sensor
// GPIO where the DS18B20 is connected to
const int oneWireBus = 4; 

//set fan speed (pwm)
//set buzzer
//read buttons

static float ambientTemperature = 0.0;
static float ambientHumidity = 0.0;
static float radiatorTemperature = 0.0;

DHT dht(DHTPIN, DHTTYPE);

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature ds18b20(&oneWire);

void hal_init();
void hal_loop();
void dht_read();
void ds18b20_read();

void hal_init()
{
    dht.begin();
    // Start the DS18B20 sensor
    ds18b20.begin();
    // Set DS18B20 resolution to 9 bits for faster read
    ds18b20.setResolution(9);
}

void hal_loop()
{
    dht_read();
    ds18b20_read();
}


//+++++++++++++++++++ DHT22 +++++++++++++++++++++++++++++++
void dht_read()
{
    unsigned long currentTime = millis();
    static unsigned long lastDHTReadTime = 0;
    if (currentTime - lastDHTReadTime >= DHT_READ_INTERVAL)
    {
        float h = dht.readHumidity();
        float t = dht.readTemperature();

        if (isnan(h) || isnan(t))
        {
            ambientHumidity = 100.0;  //error - max value
            ambientTemperature = 125.0;  //error - max value
        }
        else
        {
            ambientTemperature = t;
            ambientHumidity = h;
        }
    }
}

float getAmbientTemperature()
{
    return ambientTemperature;
}

float getAmbientHumidity()
{
    return ambientHumidity;
}


//+++++++++++++++++++ DS18B20 +++++++++++++++++++++++++++++++
void ds18b20_read()
{
    unsigned long currentTime = millis();
    static unsigned long lastDS18B20ReadTime = 0;
    if (currentTime - lastDS18B20ReadTime >= DS18B20_READ_INTERVAL)
    {
        ds18b20.requestTemperatures(); // Send the command to get temperatures
        radiatorTemperature = ds18b20.getTempCByIndex(0);
    }
}

float getRadiatorTemperature()
{
    return radiatorTemperature;
}

#endif // HAL_H
