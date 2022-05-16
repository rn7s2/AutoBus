// author: Ruiqi Lei, SunZhengjun
//   date: 2022/5/15, 2022/5/16

#ifndef _DEFINITION_H
#define _DEFINITION_H

#define MAX_LEN 10            //最大数组元素数

enum Strategy {                // 策略
    FCFS,	                   //先来先服务策略
    SSTF,                      //最短寻找时间优先算法
    SCAN                       //顺便服务策略
};

typedef struct {               // 全局配置
    int distance, total_station;
    enum Strategy strategy;
} Config;

typedef struct {               // 全局状态
    int position;
    int target[MAX_LEN];
    int clockwise_request[MAX_LEN];
    int counterclockwise_request[MAX_LEN];
} State;

typedef struct {               // 指令
    int type;        	       // 指令类型，0为end，1为clock，以此类推
    int station;     	       // 如果type为2, 3, 4，则为站点，否则无意义
} Instruction;

void dispatch(Instruction t);
void read_config();
void report();
void fcfs_clock_tick();
void fcfs_primary_request(int direction, int station);
void fcfs_secondary_request(int target);
void fcfs_work_end();
void sstf_clock_tick();
void sstf_primary_request(int direction, int station);
void sstf_secondary_request(int target);
void sstf_work_end();
void scan_clock_tick();
void scan_primary_request(int direction, int station);
void scan_secondary_request(int target);
void scan_work_end();
Instruction read_event();
void start(int direction);
void park();
void stop();

#endif
