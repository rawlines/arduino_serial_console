#include <Arduino.h>

#include "console/console.hpp"

void setup()
{
    Serial.begin(SERIAL_BAUD_RATE);
}

void loop()
{
    bool setup = true;
    while (setup) {
        char* command = readCommand();
        Serial.println();
        Serial.print("Command readed: ");
        Serial.println(command);
        Serial.print("Len: ");
        Serial.println(strlen(command));

        //Handle command interpretation here

        free(command);
    }

    Serial.end();
}
