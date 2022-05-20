#include <stdio.h>
#include "input.h"

Instruction read_event()
{
    Instruction read;
    int type1 = 5, b = 3;
    int station1 = 0;
    char str[30];
    gets(str);
    if (str[0] == 'e')
        type1 = 0;
    else if( str[0] == 't')
        type1 = 4;
    else {

        if (str[1] == 'o')
            type1 = 2;
        else if (str[5] == '\0')
            type1 = 1;
        else if(str[5] == 'w')
            type1 = 3;
    }
    for(b = 6; type1 != 0 && type1 != 1 && str[b] != '\0'; b++) {
        if (str[b] >= '2' && str[b] <= '9')
            station1 = (int) str[b];
        else if(str[b] == '1') {
            if(str[b + 1] == '0') {

                station1 = 10;
                break;
            } else
                station1 = 1;
        }
    }
    read.station = station1;
    read.type = type1;
    return read;
}

