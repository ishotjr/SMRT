// we want to grab as many data points as possible during flight, 
// so we read a bunch into an array and send them together vs. 
// continually sending one at a time:

const int measurements = 5; // TODO: refactor String::format to allow adjustment 
// TODO: reduce for actual flight:
const int interval = 10000; // milliseconds
int altitude[measurements]; // total time between publishing = measurements * interval (ms)
int i = 0;

// time in milliseconds of last measurement
long lastMeasurement = 0;

void setup() {
    // none needed!
}

void loop() {

    // measure every (interval) ms
    if (millis() - lastMeasurement > interval) {

        // TODO: replace floating pin mock with actual altitude from DPS310
        altitude[i] = analogRead(A0);

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
