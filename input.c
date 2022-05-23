#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

Instruction read_event()
{
    Instruction read;
    read.type = 5;
    read.station = -1;

    char str[30] = {};
    char* num = NULL;

    gets(str);

    if (str[0] == 'e') {
        read.type = 0;
    } else if (str[0] == 'c' && str[5] == '\0') {
        read.type = 1;
    } else if (str[2] == 'u') {
        read.type = 2;
        num = strrchr(str, ' '), num++;
        read.station = atoi(num);
    } else if (str[0] == 'c' && str[5] == 'w') {
        read.type = 3;
        num = strrchr(str, ' '), num++;
        read.station = atoi(num);
    } else if (str[0] == 't') {
        read.type = 4;
        num = strrchr(str, ' '), num++;
        read.station = atoi(num);
    }

    return read;
}
