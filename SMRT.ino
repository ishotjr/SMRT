#include "ifx_dps310.h"
// forked https://github.com/Infineon/DPS310-Pressure-Sensor as
// https://github.com/ishotjr/DPS310-Pressure-Sensor and uploaded
// using `particle library upload` (after extensive tweaking to work
// with Particle!) then added as project dependency via project.properties


// DPS310 I2C is 0x76 based on scan
const uint8_t address = 0x76;


// we want to grab as many data points as possible during flight,
// so we read a bunch into an array and send them together vs.
// continually sending one at a time:

const int measurements = 5; // TODO: refactor String::format to allow adjustment (then increase!)
// TODO: reduce interval for actual flight!
const int interval = 10000; // milliseconds
int altitude[measurements]; // total time between publishing = measurements * interval (ms)
int i = 0;

// time in milliseconds of last measurement
long lastMeasurement = 0;

void setup() {
    // initialize DPS310 and serial logging

    Serial.begin(9600);
    ifxDps310.begin(Wire, address);

    Serial.println("\nbegin");
}

void loop() {


    long int temperature = 0;
    long int pressure = 0;
    int oversampling = 7;
    int ret;


    // measure every (interval) ms
    if (millis() - lastMeasurement > interval) {


        // TODO: remove! (I2C check)

        /*
        Wire.beginTransmission(address);
        byte error = Wire.endTransmission();
        if (error == 0)
        {
            Serial.print("I2C device found at address 0x");
            Serial.println(address,HEX);
        }
        */


        Serial.println();


        // temp is currently unused, but retained for refernce!

        /*
        ret = ifxDps310.measureTempOnce(temperature, oversampling);

        if (ret != 0) {
            // Something went wrong.
            / /Look at the library code for more information about return codes
            Serial.print("FAIL! ret = ");
            Serial.println(ret);
        }
        else {
            Serial.print("Temperature: ");
            Serial.print(temperature);
            Serial.println(" degrees of Celsius");
        }
        */

        // pressure measurement
        ret = ifxDps310.measurePressureOnce(pressure, oversampling);
        if (ret != 0) {
            // Something went wrong.
            // Look at the library code for more information about return codes
            Serial.print("FAIL! ret = ");
            Serial.println(ret);
        }
        else {
            Serial.print("Pressure: ");
            Serial.print(pressure);
            Serial.println(" Pascal");
        }



        // TODO: add error checking: shouldn't really record measurement if ret != 0



        // calculate altitude per SES2G User Guide
        float seaLevelPressureMbar = 1013.25; // should be calibrated really!
        float measuredPressureMbar = pressure / 100.0;

        altitude[i] = 44330.0 * (1.0 - pow((measuredPressureMbar / seaLevelPressureMbar), (1.0 / 5.255)));

        lastMeasurement = millis(); // reset to now

        // publish data once the array is full
        if(i == (measurements - 1)){
            // TODO: need to enable arbitrary number of %d or otherwise construct string
            Particle.publish("A", String::format("%d,%d,%d,%d,%d",
                altitude[0],altitude[1],altitude[2],altitude[3],altitude[4]));
            i = 0;
        }
        else {
            i++;
        }
    }
}
