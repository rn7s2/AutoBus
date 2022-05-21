// author: Ruiqi Lei, SunZhengjun
//   date: 2022/5/15, 2022/5/16

#ifndef _DEFINITION_H
#define _DEFINITION_H

#define MAX_LEN 10             // 最大数组元素数
#define MAX_BUF 100 + 1

enum Strategy {                // 策略
    FCFS,	                   // 先来先服务策略
    SSTF,                      // 最短寻找时间优先算法
    SCAN                       // 顺便服务策略
};

typedef struct {               // 全局配置
    int distance, total_station;
    enum Strategy strategy;
} Config;

typedef struct {               // 全局状态
    int last_state, state;
    int time, position;
    struct ListNode* requests;
    int target[MAX_LEN];
    int clockwise_request[MAX_LEN];
    int counterclockwise_request[MAX_LEN];
} State;

typedef struct {               // 指令
    int type;        	       // 指令类型，0为end，1为clock，以此类推
    int station;     	       // 如果type为2, 3, 4，则为站点，否则无意义
} Instruction;

#endif
