#include <stdio.h>
#include "list.h"
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
            break;
        }
        case 1: { // go counterclockwisely during this second
            scan_counterclockwise_go();
            break;
        }
        case 2: { // stop at a station during this second, ready to start now
            int station = state.position / config.distance + 1;
            scan_request_complete(station); // complete requests
            if (state.current_target == station) { // target reached, set next
                state.current_target = 0;
                scan_find_target(station);
            } else { // restore to last_state
                state.state = state.last_state;
                state.last_state = 2;
            }
            break;
        }
        case 3: { // go clockwisely during this second
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
    int flag = (state.position % config.total_station == 0);
    int now_station = state.position / config.total_station + 1;
    flag = flag && state.state == 2 && station == now_station;
    if (!flag) {
        if (direction < 0) {
            if (state.counterclockwise_request[station] == 1)
                return;
            state.counterclockwise_request[station] = 1;
            list_node_new_append(state.requests, station, 0);
        } else {
            if (state.clockwise_request[station] == 1)
                return;
            state.clockwise_request[station] = 1;
            list_node_new_append(state.requests, station, 1);
        }

        if (state.state == 0) {
            state.last_state = 0;
            state.current_target = station;
            if (less_than_halfway(now_station, state.current_target, 1)) {
                state.state = 3;
            } else {
                state.state = 1;
            }
        }
    }
}

// target request handler
void scan_secondary_request(int target)
{
    int flag = (state.position % config.total_station == 0);
    int now_station = state.position / config.total_station + 1;
    flag = flag && state.state == 2 && target == now_station;
    if (!flag) {
        if (state.target[target] == 1)
            return;
        state.target[target] = 1;
        list_node_new_append(state.requests, target, 2);

        if (state.state == 0) {
            state.last_state = 0;
            state.current_target = target;
            if (less_than_halfway(now_station, state.current_target, 1)) {
                state.state = 3;
            } else {
                state.state = 1;
            }
        }
    }
}

// complete requests at a station
void scan_request_complete(int station)
{
    if (state.counterclockwise_request[station] == 1) {
        state.counterclockwise_request[station] = 0;
        list_node_remove(state.requests, station, 0);
    }
    if (state.clockwise_request[station] == 1) {
        state.clockwise_request[station] = 0;
        list_node_remove(state.requests, station, 1);
    }
    if (state.target[station] == 1) {
        state.target[station] = 0;
        list_node_remove(state.requests, station, 2);
    }
}

// find and set next target to reach
void scan_find_target(int station)
{
    if (list_length(state.requests) == 0) { // no requests currently
        state.last_state = 2;
        state.state = 0;
    } else {
        state.current_target = list_first_node_get_val(state.requests);
        if (state.last_state == 1) { // counterclockwise
            state.last_state = 2;
            state.state = less_than_halfway(station,
                                            state.current_target,
                                            -1) ? 1 : 3;
        } else if (state.last_state == 3) { // clockwise
            state.last_state = 2;
            state.state = less_than_halfway(station,
                                            state.current_target,
                                            1) ? 3 : 1;
        }
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
        int need_to_stop = (state.counterclockwise_request[station]
                            + state.clockwise_request[station]
                            + state.target[station] >= 1);
        if (need_to_stop == 1) {
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
        int need_to_stop = (state.counterclockwise_request[station]
                            + state.clockwise_request[station]
                            + state.target[station] >= 1);
        if (need_to_stop == 1) {
            state.last_state = 3;
            state.state = 2;
        }
    }
}
