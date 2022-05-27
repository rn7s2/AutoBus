#include "dispatcher.h"

void sstf_clock_tick() {}
void sstf_primary_request(int direction, int station) {}
void sstf_secondary_request(int target) {}
int state.current_target = 0;
int fflag()//找出最短路径
{
    int a = 1;
    int b = 1;
    int c = 1;
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
    a = -1;
    while(1) {
        if(state.position / config.distance + a <= -1)
            a = -(state.position / config.distance - config.total_station+1);
        if(state.clockwise_request[state.position / config.distance + a] == 1
                || state.counterclockwise_request[state.position / config.distance + a] == 1
                || state.target[state.position / config.distance + a] == 1) {
            tipcc = 1;
            break;
        } else if(a != 0) {
            a--;
            c++;
        } else if(a == 0)
            break;

    }
    if(tipc == 1 && tipcc == 1 && c < b)
        return (-c);
    if(tipc == 1 && tipcc == 1)
        return b;
    if(tipc == 0)
        return 0;
}
void sstf_clock_tick()
{
    state.time++;
    state.last_state = state.state ;

    if(state.state == 4) {
        if(state.position != config.distance * config.total_station - 2)
            state.position++;
        else
            state.position == 0;
    }
    if(state.state == 2) {
        if(state.position == 0)
            state.position == config.distance*config.total_station - 2;
        else
            state.position--;
    }
    if (state.state == 3 || state.state == 1) {
        if(state.current_target == 0) {
            state.current_target = fflag();
            if (state.current_target > 0)
                state.state = 4;
            else if(state.current_target < 0)
                state.state = 2;
            else if(state.current_target == 0)
                state.state = 1;
        }//逻辑是如果clock（为停留的）后有更优解，那执行？？？？？？？
    }
    if (state.position % config.distance == 0 && state.state != 3) {
        if(state.state == 2) {

            state.current_target++;
        } else if(state.state == 4) {

            state.current_target--;
        }
        if (state.state == 2
                &&             (state.target[state.position / config.distance] == 1 ||
                                state.counterclockwise_request[state.position / config.distance] == 1)) {
            state.state = 3;
            state.target[state.position / config.distance] = 0;
            state.counterclockwise_request[state.position / config.distance] = 0;
        }
        if (state.state == 4
                &&             (state.target[state.position / config.distance] == 1 ||
                                state.clockwise_request[state.position / config.distance] == 1)) {
            state.state = 3;
            state.target[state.position / config.distance] = 0;
            state.clockwise_request[state.position / config.distance] = 0;
        }

    }

}
void sstf_primary_request(int direction, int station)
{
    if(direction == 1)
        state.clockwise_request[station - 1] = 1;
    else if(direction == -1)
        state.counterclockwise_request[station - 1] = 1;
    if(state.current_target == 0) {
        state.current_target = fflag();
        if (state.current_target > 0)
            state.state = 4;
        else if(state.current_target < 0)
            state.state = 2;
        else if(state.current_target == 0)
            state.state = 1;
    }//逻辑是如果clock（为停留的）后有更优解，那执行？？？？？？？

}
void sstf_secondary_request(int target)
{
    state.target[target - 1] = 1;
    if(state.current_target == 0) {
        state.current_target = fflag();
        if (flag > 0)
            state.state = 4;
        else if(state.current_target < 0)
            state.state = 2;
        else if(state.current_target == 0)
            state.state = 1;
    }//逻辑是如果clock（为停留的）后有更优解，那执行？？？？？？？
}
