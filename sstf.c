#include "dispatcher.h"

void sstf_clock_tick() {}
void sstf_primary_request(int direction, int station) {}
void sstf_secondary_request(int target) {}
void tar(int a)
{
    int b = 0;
    for(b = 0; b < MAX_LEN; b++) {
        if(a == target[b])
            state.target[b] == 0;
    }
}
void cw(int a)
{
    int b = 0;
    for(b = 0; b < MAX_LEN; b++) {
        if(a == clockwise_request[b])
            state.clockwise_request[b] == 0;
    }
}
void ccw(int a)
{
    int b = 0;
    for(b = 0; b < MAX_LEN; b++) {
        if(a == counterclockwise_request[b])
            state.counterclockwise_request[b] == 0;
    }
}
void sstf_clock_tick()
{
    state.time++;
    if (state.state == 4)
        start(1);
    else if(state.state == 2)
        start(-1);
    else if(state.state == 3)
        park();
    else if(state.state == 1)
        stop();
    if (state.position % config.distance == 0 && state.state == 3) {

        void tar(state.position / config.distance + 1);
        if(state.last_state == 4)
            void cw(state.position / config.distance + 1);
        else if(state.last_state == 2)
            void ccw(state.position / config.distance + 1);
    }
}
void sstf_primary_request(int direction, int station)
{
    state.requests->next = (ListNode*)malloc(sizeof(ListNode));
    state.requests->type = direction;
    state.requests->val = station;
}
void sstf_secondary_request(int target)
{
    state.requests->next = (ListNode*)malloc(sizeof(ListNode));
    state.requests->type = direction;
    state.requests->val = station;
}
void sstf_work_end()
{
    state.last_state = -1;
    state.position = 0;
    state.state = 0; //?
}
