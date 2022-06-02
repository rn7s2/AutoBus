#include "list.h"
#include "definition.h"
#include "dispatcher.h"

extern Config config;
extern State state;

void fcfs_clock_tick()
{
    state.time++; // Add time
    switch (state.state) {
        case 1: { // Go counterclockwisely
            state.position--;// Decrease position
            if (state.position < 0)
                state.position = config.total_station * config.distance - 1;
            if (state.position % config.distance == 0) { // at a station
                // Calculate position
                int station = state.position / config.distance + 1;
                if (station == state.current_target) {
                    state.last_state = 1;
                    state.state = 2;  //Change the state
                }
            }
            state.last_state = 1;
            break;
        }
        case 2: { // stop at a station during this second, ready to start now
            int station = state.position / config.distance + 1;
            int type = list_first_node_get_type(state.requests);
            if (type == 0) {
                state.counterclockwise_request[station] = 0;
            } else if (type == 1) {
                state.clockwise_request[station] = 0;
            } else if (type == 2) {
                state.target[station] = 0;
            }
            list_first_node_remove(state.requests);

            struct ListNode* head = state.requests->next;
            while (head) {
                if (head->val == station) {
                    int request_type = head->type;
                    head = state.requests->next;
                    if (request_type == 0) {
                        state.counterclockwise_request[station] = 0;
                    } else if (request_type == 1) {
                        state.clockwise_request[station] = 0;
                    } else if (request_type == 2) {
                        state.target[station] = 0;
                    }
                    list_node_remove(state.requests, station, request_type);
                } else {
                    break;
                }
            }

            // target reached, set next one
            state.current_target = 0;
            if (list_length(state.requests) == 0) { // no requests currently
                state.last_state = 2;
                state.state = 0;
            } else {
                state.current_target
                    = list_first_node_get_val(state.requests);
                state.last_state = 2;
                if (less_than_halfway(station,
                                      state.current_target, -1)) {
                    state.state = 1;
                } else {
                    state.state = 3;
                }
            }
            break;
        }
        case 3: { // go clockwisely during this second
            state.position++;
            if (state.position >= config.total_station * config.distance)
                state.position = 0;
            if (state.position % config.distance == 0) { // at a station
                int station = state.position / config.distance + 1;
                if (station == state.current_target) {
                    state.last_state = 3;
                    state.state = 2;
                }
            }
            state.last_state = 3;
            break;
        }
        default:
            break;
    }
}

void fcfs_primary_request(int direction, int station)
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

    if (state.state == 0) {
        state.last_state = 0;
        state.current_target = station;
        int now_station = state.position / config.distance + 1;
        if (less_than_halfway(now_station, state.current_target, 1)) {
            state.state = 3;
        } else {
            state.state = 1;
        }
    }
}

void fcfs_secondary_request(int target)
{
    if (state.target[target] == 1)
        return;
    state.target[target] = 1;
    list_node_new_append(state.requests, target, 2);

    if (state.state == 0) {
        state.last_state = 0;
        state.current_target = target;
        int now_station = state.position / config.distance + 1;
        if (less_than_halfway(now_station, state.current_target, 1)) {
            state.state = 3;
        } else {
            state.state = 1;
        }
    }
}
