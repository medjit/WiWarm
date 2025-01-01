#include "hal.h"

void setup() {
    hal_init();
}

void loop() {

    static unsigned long lastMillis = 0;
    if (millis() - lastMillis >= 1) {
        lastMillis = millis();
        hal_loop();
    }
}
