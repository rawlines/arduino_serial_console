#include <Arduino.h>
#include <cstring>

#include "console/console.hpp"

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    bool setup = false;
    while(setup) {
        char* command = readCommand(); //Will wait until a command is received (user press enter)
        Serial.println();
        Serial.print("Command read: ");
        Serial.println(command);
        Serial.print("Len: ");
        Serial.println(strlen(command));

        //Handle command interpretation here
        if (strcmp(command, "exit") == 0) {
            setup = false; //Exit setup
        }

        free(command); //Do not forget to free pointer, it is allocated using malloc
    }

    Serial.end();
}
