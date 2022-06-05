/******************************************************************************
 * Filename:    input.c
 * Author:      王盛宇，雷瑞祺
 * Purpose:     Implement read_event.
 * Last update: 2022-05-16
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

Instruction read_event(FILE* fin)
{
    Instruction read;
    read.type = 5;
    read.station = -1;

    char str[MAX_BUF] = {};
    char* num = NULL;

    fgets(str, MAX_BUF, fin);

    int tmp = strlen(str) - 1;
    while (str[tmp] == ' ' || str[tmp] == '\n') {
        str[tmp] = '\0', --tmp;
    }

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
