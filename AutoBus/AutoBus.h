#pragma once

#include "List.h"
#include <string>

#define MAX_LEN 10 + 1         // �������Ԫ����
#define MAX_BUF 100 + 1        // ����ַ�������������

enum Strategy {                // ����
    FCFS,                      // �����ȷ������
    SSTF,                      // ���Ѱ��ʱ�������㷨
    SCAN                       // ˳��������
};

typedef struct {               // ȫ������
    int distance, total_station;
    enum Strategy strategy;
} Config;

typedef struct {               // ȫ��״̬
    int last_state, state;
    int time, position, current_target;
    struct ListNode* requests;
    int target[MAX_LEN];
    int clockwise_request[MAX_LEN];
    int counterclockwise_request[MAX_LEN];
} State;

typedef struct {               // ָ��
    int type;        	       // ָ�����ͣ�0Ϊend��1Ϊclock���Դ�����
    int station;     	       // ���typeΪ2, 3, 4����Ϊվ�㣬����������
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
