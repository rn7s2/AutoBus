#include "list.h"                                                               //2022.5.27 SunZhengjun
#include "definition.h"
#include "dispatcher.h"

extern Config config;
extern State state;

fcfs_clock_tick()
{
    state.time++;                                                               // Add time 
    switch (state.state) {
        
        case 1: {                                                               // Go counterclockwisely 
        state.position--;                                                       // decrease position
        if (state.position < 0)
                state.position = config.total_station * config.distance - 1;    
            if (state.position % config.distance == 0) {                        // at a station
                int station = state.position / config.distance + 1;             //Calculate position
                int current_request_val =  list_first_node_get_val(state.requests);
                int need_to_stop = ((station == state.current_target)           //If this station is either current_target 
                                     + (station == current_request_val) >= 1 ); //or current_request , we stop .            
                if (need_to_stop == 1) {                        
                    state.last_state = 1;                                       //Change the state
                    state.state = 2;
                }
            }
        break;
    }
        
        case 2: {                                                               // Stop at a station during this second
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
                if (list_length(state.requests) == 0) {                         // no requests currently
                    state.last_state = 2;
                    state.state = 0;
                } else {
                    state.current_target = list_first_node_get_val(state.requests);
                    state.last_state = 2;
                    int current_direction = list_first_node_get_type(state.requests);
                     if(current_direction == 0)   
                     state.state = 1 ;
                     if(current_direction == 1)
                     state.state = 3 ;  
                        
                }
            } else {
                state.state = state.last_state;
                state.last_state = 2;
            }
            break;
    } 
        
         case 3: {                                                              // Go clockwisely 
        state.position++;
        if (state.position >= config.total_station * config.distance)
                state.position = 0;
            if (state.position % config.distance == 0) { // at a station
                int station = state.position / config.distance + 1;
                int need_to_stop = ((station == state.current_target) 
                                     + (station == current_request_val) >= 1 ); 
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
        if (direction < 0) {
            state.counterclockwise_request[station] = 1;
            list_node_new_append(state.requests, station, 0);
        } else {
            state.clockwise_request[station] = 1;
            list_node_new_append(state.requests, station, 1);
        }

        if (state.state == 0) {
            state.last_state = 0;
            state.current_target = station;
        }
}

void scan_secondary_request(int target)
{
        state.target[target] = 1;
        list_node_new_append(state.requests, target, 2);

        if (state.state == 0) {
            state.last_state = 0;
            state.current_target = target;
        }
}
