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

    // 主循环，读取-分派（执行策略）-报告
    while (state.last_state != -1 && state.state != 0) {
        Instruction t = read_event();
        dispatch(t);
        if (t.type == 0) {
            report();
        }
    }

    return 0;
}

// 初始化状态
void init_state()
{
    state.last_state = -1;
    state.state = 0;
    state.time = state.position = 0;
    state.requests = new_node(-1);
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
        case 0:
            fcfs_work_end();
            break;
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
        case 0:
            sstf_work_end();
            break;
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
        case 0:
            scan_work_end();
            break;
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
    // 不合格，请重写
    /*
    Config *ptr = NULL;
    ptr = malloc(sizeof(Config)); // 为Config的三个量开辟空间
    char Identify_1, Identify_2, Identify_3,
         buf; // 这四个量都没用，只是走个过场，用来剔除注释与没用的部分

    FILE* fptr;                                                                 //打开文件，若失败，输出Open dict.dic error!
    if ((fptr = fopen("dict.dic", "r")) == NULL) {
        printf("Open dict.dic error!\n");
    } else {
        while(r != EOF) {
            if(r != '#') {
                if(fscanf( r, "%s", Identify_1 ) ==
                        "TOTAL_STATION = ") {           //识别TOTAL_STATION = 后的数
                    fscanf(r, "%d\n", ptr ->TOTAL_STATION );
                }
                if(fscanf( r, "%s", Identify_2 ) ==
                        "distance = ") {                //识别distance = 后的数
                    fscanf(r, "%d\n", ptr ->distance );
                }
                if(fscanf( r, "%s", Identify_3 ) ==
                        "Strategy strategy = ") {       //识别Strategy strategy = 后的字符
                    fscanf(r, "%s\n", ptr ->Strategy strategy );
                }
            } else {
                fsanf(fp, "%[^\n]\n",
                      buf);                                         //fscanf跳行,去除注释
            }

        }
    }

    fclose(fptr);                                                               //关闭文件
    return 0;
    */
}
