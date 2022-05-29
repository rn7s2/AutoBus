#include "definition.h"
#include "dispatcher.h"
extern State state;
extern Config config;

int fflag() // 找出最短路径
{
    int a = 1;
    int b = 1;
    int c = 1, d = -1;
    int tipc = 0;
    int tipcc = 0;
    while(1) {
        if(state.position / config.distance + a >= config.total_station)
            a = -(state.position / config.distance);
        if(state.clockwise_request[state.position / config.distance + a] == 1
                || state.counterclockwise_request[state.position / config.distance + a] == 1
                || state.target[state.position / config.distance + a] == 1) {
            tipc = 1;
            break;
        } else if(a != 0) {
            a++;
            b++;
        } else if(a == 0)
            break;
    }

    while(1) {
        if(state.position / config.distance + d <= -1)
            d = -(state.position / config.distance - config.total_station + 1);
        if(state.clockwise_request[state.position / config.distance + d] == 1
                || state.counterclockwise_request[state.position / config.distance + d] == 1
                || state.target[state.position / config.distance + d] == 1) {
            tipcc = 1;
            break;
        } else if(d != 0) {
            d--;
            c++;
        } else if(d == 0)
            break;
    }
    if(tipc == 1 && tipcc == 1 && c < b)
        return (-c);
    if(tipc == 1 && tipcc == 1 && c >= b)
        return b;
    if(tipc == 0)
        return 0;
}

void sstf_clock_tick()
{
    state.time++;
    if(state.state == 0 && state.current_target != 0) {
        if(state.current_target > 0) {

            state.state = 3;
        } else if(state.current_target < 0) {

            state.state = 1 ;
        }
    }
    state.last_state = state.state ;
    if(state.state == 3) {
        if(state.position != (config.distance * config.total_station) - 1)
            state.position++;
        else
            state.position = 0;
    }
     else if(state.state == 1) {
        if(state.position == 0)
            state.position = config.distance * config.total_station - 1;
        else
            state.position--;
    }
    else if (state.state == 2) {
        if(state.current_target == 0)
            state.current_target = fflag();
        if (state.current_target > 0)
            state.state = 3;
        else if(state.current_target < 0)
            state.state = 1;
        else if(state.current_target == 0)
            state.state = 0;


    }
    if (state.position % config.distance == 0 && state.last_state != 2
            && state.last_state != 0) {
        if(state.state == 1) {

            state.current_target++;
        } else if(state.state == 3) {

            state.current_target--;
        }
        if (state.state == 1
                &&             (state.target[state.position / config.distance] == 1 ||
                                state.counterclockwise_request[state.position / config.distance] == 1)) {
            state.state = 2;
            state.target[state.position / config.distance] = 0;
            state.counterclockwise_request[state.position / config.distance] = 0;
            if (state.current_target==0)
            {
                state.clockwise_request[state.position / config.distance] = 0;
            }
        }
        else if (state.state == 3
                &&             (state.target[state.position / config.distance] == 1 ||
                                state.clockwise_request[state.position / config.distance] == 1)) {
            state.state = 2;
            state.target[state.position / config.distance] = 0;
            state.clockwise_request[state.position / config.distance] = 0;
            if (state.current_target==0)
            {
                state.counterclockwise_request[state.position / config.distance] = 0;
            }
        }

    }

}

void sstf_primary_request(int direction, int station)
{
    if(direction == 1)
        state.clockwise_request[station - 1] = 1;
    else if(direction == -1)
        state.counterclockwise_request[station - 1] = 1;
    if(state.state == 0 )
        state.current_target = fflag();
}

void sstf_secondary_request(int target)
{
    state.target[target - 1] = 1;
    if(state.state == 0)
        state.current_target = fflag();
}
