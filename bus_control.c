#include "bus_control.h"
#include<stdio.h>
void start(int direction)
{
    if(direction == 1) {
        State.last_state = State.state;
        State.state = 4;
        if(State.position == Config.distance * total_station - 1)
            State.position = 0;
        else
            State.position++;
    } else if(direction == -1) {
        State.last_state = State.state;
        State.state = 2;
        if(State.position == 0)
            State.position == Config.distance*total_station - 1;
        else
            State.position--;
    }
}
void stop()
{
    State.last_state = State.state;
    State.state = 1;
}
void park()
{
    State.last_state = State.state;
    State.state = 3;
    State.target[State.position/Config.distance]=0;
}