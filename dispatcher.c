#include "list.h"
#include "definition.h"
#include "dispatcher.h"

extern Config config;
extern State state;

void fcfs_clock_tick() {}

void fcfs_primary_request(int direction, int station) {}
void fcfs_secondary_request(int target) {}
void fcfs_work_end() {}

void sstf_clock_tick() {}
void sstf_primary_request(int direction, int station) {}
void sstf_secondary_request(int target) {}
void sstf_work_end() {}

void scan_clock_tick()
{
    state.time++;
    switch (state.state) {
        case 1: // go counterclockwisely during this second
            state.position--;
            if (state.position < 0)
                state.position = config.total_station - 1;
            break;
        case 2: // stop at a station during this second, ready to start now

            break;
        case 3: // go clockwisely during this second
            state.position++;
            if (state.position >= config.total_station)
                state.position = 0;
            break;
        default:
            break;
    }
}

void scan_primary_request(int direction, int station)
{
    if (direction == -1) {
        if (state.counterclockwise_request[station] == 1)
            return;
        state.counterclockwise_request[station] = 1;
        list_node_new_append(state.requests, station, 0);
    } else if (direction == 1) {
        if (state.clockwise_request[station] == 1)
            return;
        state.clockwise_request[station] = 1;
        list_node_new_append(state.requests, station, 1);
    }

}

void scan_secondary_request(int target)
{
    if (state.target[target] == 1)
        return;
    state.target[target] = 1;
    list_node_new_append(state.requests, target, 2);
}

void scan_work_end()
{
}
