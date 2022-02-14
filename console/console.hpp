#ifndef __CONSOLE_H
#define __CONSOLE_H 1

#define SERIAL_BAUD_RATE      9600
#define READING_BUFF_LENGTH   8
#define COMMAND_MAX_LENGTH    32
#define COMMAND_PLACEHOLDER   "> "
#define LINE_MAX_LENGTH       COMMAND_MAX_LENGTH + 2
#define NEXT_PAGE_LINES       20

char* readCommand();

#endif
