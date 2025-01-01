#ifndef HAL_H
#define HAL_H

#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>



//TODO:
//define pinout
//set fan speed (pwm)
//set buzzer
//read buttons


void hal_init();
void hal_loop();
void dht_init();
void dht_read();
void ds18b20_init();
void ds18b20_read();

void hal_init()
{
    dht_init();
    ds18b20_init();
}

void hal_loop()
{
    dht_read();
    ds18b20_read();
}


//+++++++++++++++++++ DHT22 +++++++++++++++++++++++++++++++
#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define DHT_READ_INTERVAL 60000

static float ambientTemperature = 0.0;
static float ambientHumidity = 0.0;

/**
 * @brief Initializes a DHT sensor object.
 * 
 * This line creates an instance of the DHT sensor class, specifying the pin 
 * number and the type of DHT sensor being used. The DHT sensor is used for 
 * measuring temperature and humidity.
 * 
 * @param DHTPIN The pin number to which the DHT sensor is connected.
 * @param DHTTYPE The type of DHT sensor (e.g., DHT11, DHT22).
 */
DHT dht(DHTPIN, DHTTYPE);

/**
 * @brief Initializes the DHT sensor.
 *
 * This function initializes the DHT sensor by calling the begin() method
 * on the dht object. It should be called before any other operations
 * are performed on the DHT sensor.
 */
void dht_init()
{
    dht.begin();
}

/**
 * @brief Reads temperature and humidity from the DHT sensor.
 *
 * This function reads the temperature and humidity values from the DHT sensor
 * at a specified interval defined by DHT_READ_INTERVAL. If the sensor returns
 * NaN values, it sets the ambient temperature and humidity to their maximum
 * error values. Otherwise, it updates the ambient temperature and humidity
 * with the values read from the sensor.
 *
 * @note This function should be called periodically to ensure the ambient
 * temperature and humidity values are updated.
 */
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

/**
 * @brief Retrieves the current ambient temperature.
 *
 * This function returns the value of the ambient temperature
 * measured by the system.
 *
 * @return float The current ambient temperature.
 */
float getAmbientTemperature()
{
    return ambientTemperature;
}

/**
 * @brief Retrieves the current ambient humidity.
 *
 * This function returns the value of the ambient humidity, which is
 * stored in the variable `ambientHumidity`.
 *
 * @return float The current ambient humidity.
 */
float getAmbientHumidity()
{
    return ambientHumidity;
}


//+++++++++++++++++++ DS18B20 +++++++++++++++++++++++++++++++
#define DS18B20_READ_INTERVAL 60000
#define ONE_WIRE_BUS 4

static float radiatorTemperature = 0.0;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature ds18b20(&oneWire);

/**
 * @brief Initializes the DS18B20 temperature sensor.
 *
 * This function initializes the DS18B20 temperature sensor by calling the
 * begin() method and sets the resolution of the sensor to 9 bits.
 */
void ds18b20_init()
{
    ds18b20.begin();
    ds18b20.setResolution(9);
}

/**
 * @brief Reads the temperature from the DS18B20 sensor at specified intervals.
 *
 * This function checks the elapsed time since the last temperature read. If the
 * elapsed time is greater than or equal to the defined DS18B20_READ_INTERVAL,
 * it sends a command to the DS18B20 sensor to request the temperature and then
 * retrieves the temperature in Celsius, storing it in the radiatorTemperature variable.
 *
 * @note This function relies on the millis() function to get the current time and
 *       a static variable to store the last read time.
 */
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

/**
 * @brief Retrieves the current temperature of the radiator.
 *
 * This function returns the value of the radiator temperature.
 *
 * @return float The current temperature of the radiator.
 */
float getRadiatorTemperature()
{
    return radiatorTemperature;
}

#endif // HAL_H
