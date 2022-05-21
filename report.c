#include "definition.h"
#include "report.h"

extern Config config;
extern State state;

void report()
{
    printf("BUS:\nposition:%d\ntarget:", state.position);
    for (int i = 0; i < config.total_station; i++)
        printf("%d", state.target[i]);
    printf("\n");

    printf("STATION:\nclockwise:");
    for (int i = 0; i < config.total_station; i++)
        printf("%d", state.clockwise_request[i]);
    printf("\n");
    
    printf("counterclockwise: ");
    for (int i = 0; i < config.total_station; i++)
        printf("%d", state.counterclockwise_request[i]);
    printf("\n");
}
