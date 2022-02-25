#include <string.h>
#include <Arduino.h>

#include "console.hpp"

bool __ignoreChar(char chr)
{
    return chr == '\r';
}

void __nextPage()
{
    for(int i = 0; i < NEXT_PAGE_LINES; i++)
        Serial.println();
}

void __newLine(char* str = NULL)
{
    Serial.print(COMMAND_PLACEHOLDER);
    if (str != NULL)
        Serial.print(str);
}

void __backSpace(char* command, size_t& commandLength)
{
    if (commandLength != 0) {
        Serial.print((char)0x08);
        Serial.print(' ');
        Serial.print((char)0x08);

        command[commandLength - 1] = 0;
        commandLength -= 1;
    }
}

char* readCommand()
{
    char* command = (char*)calloc(COMMAND_MAX_LENGTH + 1, sizeof(char)); //+1 For NULL terminate char

    size_t commandLength = 0;
    char buffer[READING_BUFF_LENGTH];
    memset(buffer, 0, READING_BUFF_LENGTH * sizeof(char));

    __newLine();
    bool reading = true;
    while (reading) {
        int available = Serial.available();
        if (available == 0)
            continue;

        if (available > READING_BUFF_LENGTH)
            available = READING_BUFF_LENGTH;

        int i = 0;
        int copied = 0;
        while (i++ < available && reading) {
            char bRead = Serial.read();
            if (bRead == '\n') {
                reading = false;

            } else if (bRead == 0x08) { //^H backspace
                __backSpace(command, commandLength);

            } else if (bRead == 0x0c) { //^L Next page
                __nextPage();
                __newLine(command);

            } else if (!__ignoreChar(bRead)) {
                buffer[copied++] = bRead;

            }
        }

        if (commandLength == COMMAND_MAX_LENGTH || copied == 0)
            continue;

        size_t charsToAppend = copied;
        if (charsToAppend + commandLength > COMMAND_MAX_LENGTH)
            charsToAppend = COMMAND_MAX_LENGTH - commandLength;

        Serial.print(buffer);
        memmove(command + commandLength, buffer, charsToAppend);
        commandLength += charsToAppend;
    }

    return command;
}
