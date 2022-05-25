#include "list.h"
#include "definition.h"
#include "dispatcher.h"

extern Config config;
extern State state;

int less_than_half way(int from, int to, int direction)
{
    int h = config.total_station / 2;
    int t = config.total_station;
    if (direction < 0) {
        if (from < h) {
            return (h + from < to && to <= t) || (1 <= to && to <= from);
        } else {
            return (from - h < to && to <= from);
        }
    } else {
        if (from <= h) {
            return from <= to && to <= from + h;
        } else {
            return (1 <= to && to <= from - h) || (from <= to && to <= t);
        }
    }
}

void scan_clock_tick()
{
    state.time++;
    switch (state.state) {
        case 1: { // go counterclockwisely during this second
            state.position--;
            if (state.position < 0)
                state.position = config.total_station * config.distance - 1;
            if (state.position % config.distance == 0) { // at a station
                int station = state.position / config.distance + 1;
                int need_to_stop = (state.counterclockwise_request[station]
                                    + state.clockwise_request[station]
                                    + state.target[station] >= 1);
                if (need_to_stop == 1) {
                    state.last_state = 1;
                    state.state = 2;
                }
            }
            break;
        }
        case 2: { // stop at a station during this second, ready to start now
            int station = state.position / config.distance + 1;
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

            // target reached, set next one
            if (state.current_target == station) {
                state.current_target = -1;
                if (list_length(state.requests) == 0) { // no requests currently
                    state.last_state = 2;
                    state.state = 0;
                } else {
                    state.current_target
                        = list_first_node_get_val(state.requests);
                        
                    if(station > state.current_target )
                    state = 1 ;
                    if(station < state.current_target)
                    state = 3 ;
                }
            } else {
                state.state = state.last_state;
                state.last_state = 2;
            }
            break;
        }
        case 3: { // go clockwisely during this second
            state.position++;
            if (state.position >= config.total_station * config.distance)
                state.position = 0;
            if (state.position % config.distance == 0) { // at a station
                int station = state.position / config.distance + 1;
                int need_to_stop = (state.counterclockwise_request[station]
                                    + state.clockwise_request[station]
                                    + state.target[station] >= 1);
                if (need_to_stop == 1) {
                    state.last_state = 3;
                    state.state = 2;
                }
            }
            break;
        }
        default:
            break;
    }
}

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
