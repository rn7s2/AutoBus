#pragma once

#include "List.h"
#include <string>

#define MAX_LEN 10 + 1         // 最大数组元素数
#define MAX_BUF 100 + 1        // 最大字符串缓冲区长度

enum Strategy {                // 策略
    FCFS,                      // 先来先服务策略
    SSTF,                      // 最短寻找时间优先算法
    SCAN                       // 顺便服务策略
};

typedef struct {               // 全局配置
    int distance, total_station;
    enum Strategy strategy;
} Config;

typedef struct {               // 全局状态
    int last_state, state;
    int time, position, current_target;
    struct ListNode* requests;
    int target[MAX_LEN];
    int clockwise_request[MAX_LEN];
    int counterclockwise_request[MAX_LEN];
} State;

typedef struct {               // 指令
    int type;        	       // 指令类型，0为end，1为clock，以此类推
    int station;     	       // 如果type为2, 3, 4，则为站点，否则无意义
} Instruction;

class AutoBus
{
public:
    AutoBus(std::string configPath);
    ~AutoBus();
    void Dispatch(const Instruction& t);

private:
    void Init();
    void ReadConfig(const char* configPath);
    void DispatchFCFS(const Instruction& t);
    void DispatchSSTF(const Instruction& t);
    void DispatchSCAN(const Instruction& t);
    void fcfs_clock_tick();
    void fcfs_primary_request(int direction, int station);
    void fcfs_secondary_request(int target);
    void fcfs_clockwise_go();
    void fcfs_counterclockwise_go();
    void fcfs_request_complete(int station);
    void fcfs_set_next_target(int station);
    void sstf_clock_tick();
    void sstf_primary_request(int direction, int station);
    void sstf_secondary_request(int target);
    int sstf_station();
    int sstf_find_flag();
    void sstf_state_change();
    void sstf_state_change_target();
    void sstf_state_change_target_new();
    bool less_than_halfway(int from, int to, int direction);
    void scan_clock_tick();
    void scan_primary_request(int direction, int station);
    void scan_secondary_request(int target);
    void scan_request_complete(int station);
    bool scan_has_request(int station);
    bool scan_has_requests();
    void scan_find_init_target(int station);
    void scan_find_target(int station);
    void scan_counterclockwise_go();
    void scan_clockwise_go();

public:
    Config config;
    State state;
};
