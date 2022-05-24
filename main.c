#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dispatcher.h"
#include "input.h"
#include "main.h"
#include "list.h"
#include "report.h"

Config config;     // 全局配置
State state;       // 全局状态

int main(int argc, char *argv[])
{
    // 初始化状态与读取配置
    init_state();
    read_config();

#ifdef __DEBUG__
    FILE* fin = fopen("test.in", "r");
    if (fin == NULL) {
        return 1;
    }
#endif

    report();

    // 主循环，读取-分派（执行策略）-报告
    while (1) {
#ifdef __DEBUG__
        Instruction t = read_event(fin);
#else
        Instruction t = read_event(stdin);
#endif
        if (t.type > 0 && t.type <= 4)
            dispatch(t);
        if (t.type == 0) {
            break;
        }
        if (t.type == 1) {
            report();
        }
    }

#ifdef __DEBUG__
    fclose(fin);
#endif
    list_free(state.requests);

    return 0;
}

// 初始化状态
void init_state()
{
    state.last_state = -1;
    state.state = 0;
    state.time = state.position = 0;
    state.requests = list_node_new(-1, -1);
    memset(state.target, 0, sizeof(state.target));
    memset(state.clockwise_request, 0, sizeof(state.clockwise_request));
    memset(state.counterclockwise_request, 0,
           sizeof(state.counterclockwise_request));

    config.total_station = 5;
    config.strategy = FCFS;
    config.distance = 2;
}

// FCFS策略分派
void fcfs_dispatch(Instruction t)
{
    switch (t.type) {
        case 1:
            fcfs_clock_tick();
            break;
        case 2:
            fcfs_primary_request(-1, t.station);
            break;
        case 3:
            fcfs_primary_request(1, t.station);
            break;
        case 4:
            fcfs_secondary_request(t.station);
            break;
        default:
            fprintf(stderr, "Unknown command!\n");
            exit(-1);
    }
}

// SSTF策略分派
void sstf_dispatch(Instruction t)
{
    switch (t.type) {
        case 1:
            sstf_clock_tick();
            break;
        case 2:
            sstf_primary_request(-1, t.station);
            break;
        case 3:
            sstf_primary_request(1, t.station);
            break;
        case 4:
            sstf_secondary_request(t.station);
            break;
        default:
            fprintf(stderr, "Unknown command!\n");
            exit(-1);
    }
}

// SCAN策略分派
void scan_dispatch(Instruction t)
{
    switch (t.type) {
        case 1:
            scan_clock_tick();
            break;
        case 2:
            scan_primary_request(-1, t.station);
            break;
        case 3:
            scan_primary_request(1, t.station);
            break;
        case 4:
            scan_secondary_request(t.station);
            break;
        default:
            fprintf(stderr, "Unknown command!\n");
            exit(-1);
    }
}

// 按策略分派执行
void dispatch(Instruction t)
{
    switch (config.strategy) {
        case FCFS:
            fcfs_dispatch(t);
            break;
        case SSTF:
            sstf_dispatch(t);
            break;
        case SCAN:
            scan_dispatch(t);
            break;
        default:
            fprintf(stderr, "Unknown strategy!\n");
            exit(-1);
    }
}

// 读取配置文件
void read_config()
{
    FILE* fin = fopen("dict.dic", "r");
    char buf[MAX_BUF] = {};

    if(fin == NULL)
        return;

    while (fgets(buf, MAX_BUF, fin)) {
        char* right = NULL;
        switch (buf[0]) {
            case 'T':
                right = strrchr(buf, ' '), right++;
                config.total_station = atoi(right);
                break;
            case 'S':
                right = strrchr(buf, ' '), right++;
                if (!strcmp(right, "FCFS")) {
                    config.strategy = FCFS;
                } else if (!strcmp(buf, "SSTF")) {
                    config.strategy = SSTF;
                } else {
                    config.strategy = SCAN;
                }
                break;
            case 'D':
                right = strrchr(buf, ' '), right++;
                config.distance = atoi(right);
                break;
            default:
                continue;
        }
    }

    fclose(fin);
}
