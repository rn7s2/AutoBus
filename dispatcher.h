/******************************************************************************
 * Filename:    dispatcher.h
 * Author:      雷瑞祺
 * Purpose:     Provides prototypes of dispatching functions.
 * Last update: 2022-06-02
 ******************************************************************************/

#ifndef _DISPATCHER_H
#define _DISPATCHER_H

int less_than_halfway(int from, int to, int direction);

void fcfs_clock_tick();
void fcfs_primary_request(int direction, int station);
void fcfs_secondary_request(int target);

void sstf_clock_tick();
void sstf_primary_request(int direction, int station);
void sstf_secondary_request(int target);

void scan_clock_tick();
void scan_primary_request(int direction, int station);
void scan_secondary_request(int target);

#endif
