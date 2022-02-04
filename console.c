#include <string.h>
#include <Arduino.h>

#define SERIAL_BAUD_RATE      9600
#define READING_BUFF_LENGTH   8
#define COMMAND_MAX_LENGTH    32
#define COMMAND_PLACEHOLDER   "> "
#define LINE_MAX_LENGTH       COMMAND_MAX_LENGTH + 2
#define NEXT_PAGE_LINES       20


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

void __clearLine()
{
    Serial.print('\r');
    for(int i = 0; i < LINE_MAX_LENGTH; i++)
        Serial.print(' ');
    Serial.print('\r');
}

char* __readCommand()
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

            } else if (bRead == 0x08) { //^H
                __clearLine();

                if (commandLength != 0) {
                    command[commandLength - 1] = 0;
                    commandLength -= 1;
                }

                __newLine(command);

            } else if (bRead == 0x0c) { //^L
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

bool setupConsole()
{
    Serial.begin(SERIAL_BAUD_RATE);

    bool setup = true;
    while (setup) {
        char* command = __readCommand();
        Serial.println();
        Serial.print("Command readed: ");
        Serial.println(command);
        Serial.print("Len: ");
        Serial.println(strlen(command));

        //Handle command interpretation here

        free(command);
    }

    Serial.end();

    return false;
}
