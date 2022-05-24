#ifndef _DISPATCHER_H
#define _DISPATCHER_H

typedef struct {
    int target;
} SCAN_State;

void fcfs_init();
void fcfs_clock_tick();
void fcfs_primary_request(int direction, int station);
void fcfs_secondary_request(int target);

void sstf_init();
void sstf_clock_tick();
void sstf_primary_request(int direction, int station);
void sstf_secondary_request(int target);

void scan_init();
void scan_clock_tick();
void scan_primary_request(int direction, int station);
void scan_secondary_request(int target);

#endif
