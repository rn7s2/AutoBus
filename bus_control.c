#include "definition.h"
#include "bus_control.h"
#include <stdio.h>

extern State state;
extern Config config;

void start(int direction)
{
    if (direction == 1) {
        state.last_state = state.state;
        state.state = 4;
        if(state.position == config.distance * config.total_station - 1)
            state.position = 0;
        else
            state.position++;
    } else if (direction == -1) {
        state.last_state = state.state;
        state.state = 2;
        if (state.position == 0)
            state.position == config.distance * config.total_station - 1;
        else
            state.position--;
    }
}

void stop()
{
    state.last_state = state.state;
    state.state = 1;
}

void park()
{
    state.last_state = state.state;
    state.state = 3;
    state.target[state.position / config.distance] = 0;
}
