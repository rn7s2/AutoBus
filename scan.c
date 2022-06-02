#include <stdio.h>
#include "definition.h"
#include "dispatcher.h"

extern Config config;
extern State state;

// judge if the distance longer than halfway
int less_than_halfway(int from, int to, int direction)
{
    int t = config.total_station;
    if (direction < 0) {
        int h = (config.total_station - 1) / 2;
        int end_point = ((from - 1) + t - h) % t + 1;
        if (end_point < from) {
            return end_point <= to && to <= from;
        } else {
            return (end_point <= to && to <= t) || (1 <= to && to <= from);
        }
    } else {
        int h = config.total_station / 2;
        int end_point = ((from - 1) + h) % t + 1;
        if (end_point > from) {
            return from <= to && to <= end_point;
        } else {
            return (from <= to && to <= t) || (1 <= to && to <= end_point);
        }
    }
}

// clock tick handler
void scan_clock_tick()
{
    state.time++;
    switch (state.state) {
        case 0: { // no request currently.
            int station = state.position / config.distance + 1;
            scan_find_target(station);
            state.last_state = 0;
            if (state.state == 1) {
                state.last_state = 1;
                scan_counterclockwise_go();
            } else if (state.state == 3) {
                state.last_state = 3;
                scan_clockwise_go();
            }
            break;
        }
        case 1: { // go counterclockwisely during this second
            state.last_state = 1;
            scan_counterclockwise_go();
            break;
        }
        case 2: { // stop at a station during this second, ready to start now
            int station = state.position / config.distance + 1;
            scan_request_complete(station);        // complete requests
            if (station == state.current_target) { // target reached, set next
                state.current_target = 0;
                scan_find_target(station);
            } else { // restore to last_state
                state.state = state.last_state;
            }
            state.last_state = 2;
            break;
        }
        case 3: { // go clockwisely during this second
            state.last_state = 3;
            scan_clockwise_go();
            break;
        }
        default: {
            fprintf(stderr, "Unknown state.\n");
            break;
        }
    }
}

// (counter)clockwise request handler
void scan_primary_request(int direction, int station)
{
    int now_station = state.position / config.distance + 1;
    int flag = (state.position % config.distance == 0)
               && (state.state == 2 || state.state == 0)
               && (station == now_station);
    if (!flag) {
        if (direction == -1) {
            state.counterclockwise_request[station] = 1;
        } else if (direction == 1) {
            state.clockwise_request[station] = 1;
        }
    }
}

// target request handler
void scan_secondary_request(int target)
{
    int now_station = state.position / config.distance + 1;
    int flag = (state.position % config.distance == 0)
               && (state.state == 2 || state.state == 0)
               && (target == now_station);
    if (!flag) {
        state.target[target] = 1;
    }
}

// complete requests at a station
void scan_request_complete(int station)
{
    state.counterclockwise_request[station] = 0;
    state.clockwise_request[station] = 0;
    state.target[station] = 0;
}

int has_request(int station)
{
    if (state.counterclockwise_request[station]
            || state.clockwise_request[station]
            || state.target[station]) {
        return 1;
    }
    return 0;
}

int has_requests()
{
    int flag = 0;
    for (int i = 1; i <= config.total_station; i++) {
        if (has_request(i)) {
            flag = 1;
            break;
        }
    }
    return flag;
}

void find_init_target(int station)
{
    int distance = 1;
    while (distance <= config.total_station / 2) {
        int target = (station - 1 + distance) % config.total_station + 1;
        int counter_target = (station - 1 + config.total_station
                              - distance) % config.total_station + 1;
        if (has_request(target)) {
            state.current_target = target;
            break;
        }
        if (has_request(counter_target)) {
            state.current_target = counter_target;
            break;
        }
        ++distance;
    }
}

// find and set next target to reach
void scan_find_target(int station)
{
    state.current_target = 0;
    if (has_requests() == 0) { // no requests currently
        state.state = 0;
    } else {
        int target = station;
        while (1) {
            if (state.last_state == 1) {
                target--;
                if (target == 0) {
                    target = config.total_station;
                }
            } else if (state.last_state == 3) {
                target++;
                if (target == config.total_station + 1) {
                    target = 1;
                }
            } else {
                break;
            }
            if (has_request(target)) {
                state.current_target = target;
                break;
            }
        }

        if (state.current_target == 0) {
            find_init_target(station);
        }

        state.state = less_than_halfway(station,
                                        state.current_target,
                                        1) ? 3 : 1;
    }
}

// go counterclockwisely during last second
void scan_counterclockwise_go()
{
    state.position--;
    if (state.position < 0)
        state.position = config.total_station * config.distance - 1;

    int at_station = (state.position % config.distance == 0);
    int station = state.position / config.distance + 1;

    if (at_station) {
        if (has_request(station)) {
            state.last_state = 1;
            state.state = 2;
        }
    }
}

// go clockwisely during last second
void scan_clockwise_go()
{
    state.position++;
    if (state.position >= config.total_station * config.distance)
        state.position = 0;

    int at_station = (state.position % config.distance == 0);
    int station = state.position / config.distance + 1;

    if (at_station) {
        if (has_request(station)) {
            state.last_state = 3;
            state.state = 2;
        }
    }
}
