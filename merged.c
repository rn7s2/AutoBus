#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * Filename:    list.h
 * Author:      雷瑞祺
 * Purpose:     Definition of a single-linked list.
 * Last update: 2022-05-15
 ******************************************************************************/

// type: 0 for counterclockwise, 1 for clockwise, 2 for target
struct ListNode {
    int val, type;
    struct ListNode* next;
};

struct ListNode* list_node_new(int val, int type);
int list_first_node_get_val(struct ListNode* head);
int list_first_node_get_type(struct ListNode* head);
void list_first_node_remove(struct ListNode* head);
void list_node_remove(struct ListNode* head, int val, int type);
void list_node_new_append(struct ListNode* head, int val, int type);
int list_length(struct ListNode* head);
void list_free(struct ListNode* head);

/******************************************************************************
 * Filename:    definition.h
 * Author:      孙正君，雷瑞祺，王盛宇
 * Purpose:     Provides definitions of data structures.
 * Last update: 2022-05-20
 ******************************************************************************/

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

/******************************************************************************
 * Filename:    main.h
 * Author:      雷瑞祺
 * Purpose:     Prototypes of module main.
 * Last update: 2022-05-15
 ******************************************************************************/

void init_state();
void dispatch(Instruction t);
void read_config();

/******************************************************************************
 * Filename:    report.h
 * Author:      雷瑞祺
 * Purpose:     Prototype of function report.
 * Last update: 2022-05-15
 ******************************************************************************/

void report();

/******************************************************************************
 * Filename:    input.h
 * Author:      雷瑞祺
 * Purpose:     Provide prototype of function read_event.
 * Last update: 2022-05-16
 ******************************************************************************/

Instruction read_event(FILE* fin);

/******************************************************************************
 * Filename:    dispatcher.h
 * Author:      雷瑞祺
 * Purpose:     Provides prototypes of dispatching functions.
 * Last update: 2022-06-02
 ******************************************************************************/

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

/******************************************************************************
 * Filename:    main.c
 * Author:      雷瑞祺，孙正君
 * Purpose:     Maintaining the main loop and the config.
 * Last update: 2022-06-05
 ******************************************************************************/

Config config;     // 全局配置
State state;       // 全局状态

// Function: main
//   Author: 雷瑞祺
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
        if (t.type > 0 && t.type <= 4) {
            dispatch(t);
        }
        if (t.type == 0) {
            puts("end");
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

// Function: init_state
//   Author: 雷瑞祺
//  Purpose: 初始化状态
void init_state()
{
    state.last_state = -1;
    state.state = 0;
    state.current_target = 0;
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

// Function: fcfs_dispatch
//   Author: 雷瑞祺
//  Purpose: FCFS策略分派
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

// Function: sstf_dispatch
//   Author: 雷瑞祺
//  Purpose: SSTF策略分派
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

// Function: scan_dispatch
//   Author: 雷瑞祺
//  Purpose: SCAN策略分派
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

// Function: dispatch
//   Author: 雷瑞祺
//  Purpose: 按策略分派执行
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

// Function: read_config
//   Author: 孙正君，雷瑞祺
//  Purpose: 读取配置文件
void read_config()
{
    FILE* fin = fopen("dict.dic", "r");
    char buf[MAX_BUF] = {};

    if(fin == NULL)
        return;

    while (fgets(buf, MAX_BUF, fin)) {
        char* right = NULL;

        int tmp = strlen(buf) - 1;
        while (buf[tmp] == ' ' || buf[tmp] == '\n') {
            buf[tmp] = '\0', --tmp;
        }

        switch (buf[0]) {
            case 'T':
                right = strrchr(buf, ' '), right++;
                config.total_station = atoi(right);
                break;
            case 'S':
                right = strrchr(buf, ' '), right++;
                if (!strcmp(right, "FCFS")) {
                    config.strategy = FCFS;
                } else if (!strcmp(right, "SSTF")) {
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

/******************************************************************************
 * Filename:    fcfs.c
 * Author:      孙正君
 * Purpose:     Implement strategy FCFS.
 * Last update: 2022-06-05
 ******************************************************************************/

void fcfs_clock_tick()
{
    state.time++; // Add time

    switch (state.state) { // dispatch according to current state
    
        case 1: { // go counterclockwisely during this second
            fcfs_clockwise_go();
            break;
        }
        case 2: { // stop at a station during this second, ready to start now
            int station = state.position / config.distance + 1;

            fcfs_request_complete(station);
            //When in special circumstances
            struct ListNode* head = state.requests->next;
            while (head) { // Iterate through the linked list
                if (head->val == station) {
                    int request_type = head->type;
                    head = head->next;
                    if (request_type == 0) {
                        state.counterclockwise_request[station] = 0;
                    } else if (request_type == 1) {
                        state.clockwise_request[station] = 0;
                    } else if (request_type == 2) {
                        state.target[station] = 0;
                    }
                    list_node_remove(state.requests, station, request_type);
                } else {
                    break;
                }
            }
            // target reached, set next one
            set_next_target(station);
            break;
        }
        case 3: { // go clockwisely during this second
            fcfs_counterclockwise_go();
            break;
        }
        default:
            break;
    }
}

// (counter)clockwise request handler
void fcfs_primary_request(int direction, int station)
{
    if (direction == -1) {
        if (state.counterclockwise_request[station] == 1)
            return;
        state.counterclockwise_request[station] = 1;// set station request
        list_node_new_append(state.requests, station, 0);
    } else if (direction == 1) {
        if (state.clockwise_request[station] == 1)
            return;
        state.clockwise_request[station] = 1;// set station request
        list_node_new_append(state.requests, station, 1);
    }

    if (state.state == 0) { //If at rest (state = 0 )
        state.last_state = 0;
        state.current_target = station;
        int now_station = state.position / config.distance + 1;
        // find direction
        if (less_than_halfway(now_station, state.current_target, 1)) {
            state.state = 3;
        } else {
            state.state = 1;
        }
    }
}

// target request handler
void fcfs_secondary_request(int target)
{
    if (state.target[target] == 1)
        return;
    state.target[target] = 1; // set station request
    list_node_new_append(state.requests, target, 2);

    if (state.state == 0) { //If at rest (state = 0 )
        state.last_state = 0;
        state.current_target = target;
        int now_station = state.position / config.distance + 1;
        // find direction
        if (less_than_halfway(now_station, state.current_target, 1)) {
            state.state = 3;
        } else {
            state.state = 1;
        }
    }
}

// go clockwisely during last second
void fcfs_clockwise_go()
{
    state.position--; // move position
    if (state.position < 0)
        state.position = config.total_station * config.distance - 1;
    if (state.position % config.distance == 0) { // at a station
        // Calculate position
        int station = state.position / config.distance + 1;
        if (station == state.current_target) {
            state.last_state = 1;
            state.state = 2;  //Change the state
        }
    }
    state.last_state = 1;
}

// go counterclockwisely during last second
void fcfs_counterclockwise_go()
{
    state.position++; // move position
    if (state.position >= config.total_station * config.distance)
        state.position = 0;
    if (state.position % config.distance == 0) { // at a station
        int station = state.position / config.distance + 1;
        if (station == state.current_target) {
            state.last_state = 3;
            state.state = 2;
        }
    }
    state.last_state = 3;
}

// complete requests at a station
void fcfs_request_complete(int station)
{
    int type = list_first_node_get_type(state.requests);
    if (type == 0) { //counterclockwise
        state.counterclockwise_request[station] = 0;
    } else if (type == 1) { //clockwise
        state.clockwise_request[station] = 0;
    } else if (type == 2) { //target
        state.target[station] = 0;
    }
    list_first_node_remove(state.requests);
}

// target reached, set next one
void set_next_target(int station)
{
    state.current_target = 0;
    if (list_length(state.requests) == 0) { // no requests currently
        state.last_state = 2;
        state.state = 0;
    } else {
        state.current_target
            = list_first_node_get_val(state.requests);
        state.last_state = 2;
        // find direction
        if (less_than_halfway(station,
                              state.current_target, -1)) {
            state.state = 1;
        } else {
            state.state = 3;
        }
    }
}

/******************************************************************************
 * Filename:    input.c
 * Author:      王盛宇，雷瑞祺
 * Purpose:     Implement read_event.
 * Last update: 2022-05-16
 ******************************************************************************/

Instruction read_event(FILE* fin)
{
    Instruction read;
    read.type = 5;
    read.station = -1;

    char str[MAX_BUF] = {};
    char* num = NULL;

    fgets(str, MAX_BUF, fin);

    int tmp = strlen(str) - 1;
    while (str[tmp] == ' ' || str[tmp] == '\n') {
        str[tmp] = '\0', --tmp;
    }

    if (str[0] == 'e') {
        read.type = 0;
    } else if (str[0] == 'c' && str[5] == '\0') {
        read.type = 1;
    } else if (str[2] == 'u') {
        read.type = 2;
        num = strrchr(str, ' '), num++;
        read.station = atoi(num);
    } else if (str[0] == 'c' && str[5] == 'w') {
        read.type = 3;
        num = strrchr(str, ' '), num++;
        read.station = atoi(num);
    } else if (str[0] == 't') {
        read.type = 4;
        num = strrchr(str, ' '), num++;
        read.station = atoi(num);
    }

    return read;
}

/******************************************************************************
 * Filename:    list.c
 * Author:      雷瑞祺
 * Purpose:     Implement a single-linked list.
 * Last update: 2022-05-15
 ******************************************************************************/

struct ListNode* list_node_new(int val, int type)
{
    struct ListNode* tmp = malloc(sizeof(struct ListNode));
    tmp->val = val, tmp->type = type;
    tmp->next = NULL;
    return tmp;
}

int list_first_node_get_val(struct ListNode* head)
{
    return head->next->val;
}

int list_first_node_get_type(struct ListNode* head)
{
    return head->next->type;
}

void list_first_node_remove(struct ListNode* head)
{
    struct ListNode* first_node = head->next;
    head->next = first_node->next;
    free(first_node), first_node = NULL;
}

void list_node_remove(struct ListNode* head, int val, int type)
{
    struct ListNode* cur = head->next;
    struct ListNode* lst = head;
    while (cur) {
        if (cur->val == val && cur->type == type) {
            lst->next = cur->next;
            free(cur), cur = NULL;
            break;
        }
        lst = cur, cur = cur->next;
    }
}

void list_node_new_append(struct ListNode* head, int val, int type)
{
    struct ListNode* cur = head;
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = list_node_new(val, type);
}

int list_length(struct ListNode* head)
{
    int ans = 0;
    while (head->next) {
        head = head->next;
        ++ans;
    }
    return ans;
}

void list_free(struct ListNode* head)
{
    while (head != NULL) {
        struct ListNode* tmp = head;
        head = head->next;
        free(tmp), tmp = NULL;
    }
}

/******************************************************************************
 * Filename:    report.c
 * Author:      孙正君
 * Purpose:     Implement function report.
 * Last update: 2022-05-16
 ******************************************************************************/

void report()
{
    printf("TIME:%d\n", state.time);
    printf("BUS:\nposition:%d\ntarget:", state.position);
    for (int i = 1; i <= config.total_station; i++)
        printf("%d", state.target[i]);
    printf("\n");

    printf("STATION:\nclockwise:");
    for (int i = 1; i <= config.total_station; i++)
        printf("%d", state.clockwise_request[i]);
    printf("\n");
    
    printf("counterclockwise:");
    for (int i = 1; i <= config.total_station; i++)
        printf("%d", state.counterclockwise_request[i]);
    printf("\n");
}

/******************************************************************************
 * Filename:    scan.c
 * Author:      雷瑞祺
 * Purpose:     Implement strategy SCAN.
 * Last update: 2022-06-05
 ******************************************************************************/

// judge if the distance longer than halfway
int less_than_halfway(int from, int to, int direction)
{
    int t = config.total_station;
    if (direction < 0) { // go counterclockwisely from 'from' to 'to'
        int h = (config.total_station - 1) / 2;
        int end_point = ((from - 1) + t - h) % t + 1; // halfway station id
        // judge if 'to' falls in valid intervals
        if (end_point < from) {
            return end_point <= to && to <= from;
        } else {
            return (end_point <= to && to <= t) || (1 <= to && to <= from);
        }
    } else { // go clockwisely from 'from' to 'to'
        int h = config.total_station / 2;
        int end_point = ((from - 1) + h) % t + 1; // halfway station id
        // judge if 'to' falls in valid intervals
        if (end_point > from) {
            return from <= to && to <= end_point;
        } else {
            return (from <= to && to <= t) || (1 <= to && to <= end_point);
        }
    }
}

// complete requests at a station
void scan_request_complete(int station)
{
    state.counterclockwise_request[station] = 0;
    state.clockwise_request[station] = 0;
    state.target[station] = 0;
}

// judge whether a station has requests
int has_request(int station)
{
    return state.counterclockwise_request[station]
           || state.clockwise_request[station]
           || state.target[station];
}

// judge whether all stations has requests currently
int has_requests()
{
    int flag = 0;
    for (int i = 1; i <= config.total_station; i++) {
        if (has_request(i)) {
            flag = 1;
            break;
        }
    }
    return flag;
}

// find the target when the bus is not moving
void find_init_target(int station)
{
    int distance = 1;
    while (distance <= config.total_station / 2) {
        int target = (station - 1 + distance) % config.total_station + 1;
        int counter_target = (station - 1 + config.total_station
                              - distance) % config.total_station + 1;
        if (has_request(target)) { // found clockwisely
            state.current_target = target;
            break;
        }
        if (has_request(counter_target)) { // found counterclockwisely
            state.current_target = counter_target;
            break;
        }
        ++distance;
    }
}

// find and set next target to reach
void scan_find_target(int station)
{
    state.current_target = 0;
    if (has_requests() == 0) { // no requests currently
        state.state = 0;
    } else { // has requests now
        int target = station;
        while (1) { // find target following the current running direction
            if (state.last_state == 1) { // currently going counterclockwisely
                target--;
                if (target == 0) {
                    target = config.total_station;
                }
            } else if (state.last_state == 3) { // currently going clockwisely
                target++;
                if (target == config.total_station + 1) {
                    target = 1;
                }
            } else { // not moving, should find init target later
                break;
            }
            if (has_request(target)) { // found target station
                state.current_target = target;
                break;
            }
        }

        if (state.current_target == 0) { // find target when the bus not moving
            find_init_target(station);
        }

        state.state = less_than_halfway(station,
                                        state.current_target,
                                        1) ? 3 : 1; // determine the direction
    }
}

// go counterclockwisely during last second
void scan_counterclockwise_go()
{
    // move position
    state.position--;
    if (state.position < 0)
        state.position = config.total_station * config.distance - 1;

    int at_station = (state.position % config.distance == 0);
    int station = state.position / config.distance + 1;

    // if at a station and has requests to complete, then park here
    if (at_station && has_request(station)) {
        state.last_state = 1;
        state.state = 2;
    }
}

// go clockwisely during last second
void scan_clockwise_go()
{
    // move position
    state.position++;
    if (state.position >= config.total_station * config.distance)
        state.position = 0;

    int at_station = (state.position % config.distance == 0);
    int station = state.position / config.distance + 1;

    // if at a station and has requests to complete, then park here
    if (at_station && has_request(station)) {
        state.last_state = 3;
        state.state = 2;
    }
}

// clock tick handler
void scan_clock_tick()
{
    state.time++;
    switch (state.state) { // dispatch according to current state
        case 0: { // no request currently.
            int station = state.position / config.distance + 1;
            scan_find_target(station);
            state.last_state = 0;
            if (state.state == 1) { // start counterclockwisely immediately
                state.last_state = 1;
                scan_counterclockwise_go();
            } else if (state.state == 3) { // start clockwisely immediately
                state.last_state = 3;
                scan_clockwise_go();
            }
            break;
        }
        case 1: { // go counterclockwisely during this second
            state.last_state = 1;
            scan_counterclockwise_go();
            break;
        }
        case 2: { // stop at a station during this second, ready to start now
            int station = state.position / config.distance + 1;
            scan_request_complete(station);        // complete requests
            if (station == state.current_target) { // target reached, set next
                state.current_target = 0;
                scan_find_target(station);
            } else { // restore to last_state, continue going
                state.state = state.last_state;
            }
            state.last_state = 2;
            break;
        }
        case 3: { // go clockwisely during this second
            state.last_state = 3;
            scan_clockwise_go();
            break;
        }
        default: {
            fprintf(stderr, "Unknown state.\n");
            break;
        }
    }
}

// (counter)clockwise request handler
void scan_primary_request(int direction, int station)
{
    int now_station = state.position / config.distance + 1;
    int flag = (state.position % config.distance == 0)
               && (state.state == 2 || state.state == 0)
               && (station == now_station); // whether we could ignore it
    if (!flag) {
        if (direction == -1) { // set station request
            state.counterclockwise_request[station] = 1;
        } else if (direction == 1) {
            state.clockwise_request[station] = 1;
        }
    }
}

// target request handler
void scan_secondary_request(int target)
{
    int now_station = state.position / config.distance + 1;
    int flag = (state.position % config.distance == 0)
               && (state.state == 2 || state.state == 0)
               && (target == now_station); // whether we could ignore it
    if (!flag) {
        state.target[target] = 1; // set target request
    }
}

/******************************************************************************
 * Filename:    sstf.c
 * Author:      王盛宇
 * Purpose:     Implement strategy SSTF.
 * Last update: 2022-06-05
 ******************************************************************************/

int flag_passrequest = 0, flag_target = 0;

int station()
{
    return state.position / config.distance;
}

int find_flag() // 找出最短路径
{
    int distance = 1, request = 1, count_request = 1, count_distance = -1;
    int tip = 0, tip_count = 0;
    // 找出顺时针运行下到达最近目标的距离
    while(1) {
        if(station() + distance >= config.total_station) {
            distance = -(station());
        }
        if(distance == 0) {
            break;
        }
        if(state.clockwise_request[station() + distance + 1]
                || state.counterclockwise_request[station() + distance + 1]
                || state.target[station() + distance + 1]) {
            tip = 1;
            break;
        } else if(distance != 0) {
            distance++;
            request++;
        }
    }
    // 找出逆时针运行下到达最近目标站点的距离
    while(1) {
        if(station() + count_distance <= -1) {
            count_distance = -(station() - config.total_station + 1);
        }
        if(count_distance == 0) {
            break;
        }
        if(state.clockwise_request[station() + count_distance + 1]
           || state.counterclockwise_request[station() + count_distance + 1]
           || state.target[station() + count_distance + 1]) {
            tip_count = 1;
            break;
        } else if(count_distance != 0) {
            count_distance--;
            count_request++;
        }
    }

    if(tip == 1 && tip_count == 1 && count_request < request) {
        return (-count_request);
    }
    if(tip == 1 && tip_count == 1 && count_request >= request) {
        return request;
    }
    if(tip == 0) {
        return 0;
    }
}

void state_change() // 改变position（顺时针情况下）
{
    if(state.position == 0) {
        state.position = config.distance * config.total_station - 1;
    } else {
        state.position--;
    }
}

void state_change_target() // 确定新目标并依据state.current_target确定运行方向
{
    if(state.current_target == 0) {
        state.current_target = find_flag();
    }
    if (state.current_target > 0) {
        state.state = 3;
    }      else if(state.current_target < 0) {
        state.state = 1;
    } else if(state.current_target == 0) {
        state.state = 0;
    }
}

void state_change_target_new() // 当位于站点时，改变target并更改相关状态
{
    if(state.state == 1) {
        state.current_target++;
    } else if(state.state == 3) {
        state.current_target--;
    }
    if ((state.state == 1
            && (state.target[station() + 1] ||
                state.counterclockwise_request[station() + 1]))
            || state.current_target == 0) {
        state.state = 2;
        flag_passrequest = 1;

        if (state.current_target == 0) {
            flag_target = 1;
        }
    } else if ((state.state == 3
                &&             (state.target[station() + 1] ||
                                state.clockwise_request[station() + 1]))
               || state.current_target == 0) {
        state.state = 2;
        flag_passrequest = 3;

        if (state.current_target == 0) {
            flag_target = 1;
        }
    }

}

void sstf_clock_tick()
{
    state.time++;
    // 当上上一秒为park且上上一秒之前再无请求时判断上一秒运行状态
    if(state.state == 0 && state.current_target != 0) {
        if(state.current_target > 0) {
            state.state = 3;
        } else if(state.current_target < 0) {
            state.state = 1 ;
        }
    }
    state.last_state = state.state ;
    if(state.state == 3) {
        if(state.position != (config.distance * config.total_station) - 1) {
            state.position++;
        } else {
            state.position = 0;
        }
    } else if(state.state == 1) {
        state_change();
    } else if (state.state == 2) {
        state_change_target();
    }
    if (state.position % config.distance == 0 && state.last_state != 2
            && state.last_state != 0) {
        state_change_target_new();
    }
    // 更新状态
    if(state.position % config.distance == 0 && state.last_state == 2) {
        if (flag_target == 1) {
            state.counterclockwise_request[station() + 1] = 0;
            state.target[station() + 1] = 0;
            state.clockwise_request[station() + 1] = 0;
        } else {
            if (flag_passrequest == 1) {
                state.target[station() + 1] = 0;
                state.counterclockwise_request[station() + 1] = 0;
            } else if(flag_passrequest == 3) {
                state.target[station() + 1] = 0;
                state.clockwise_request[station() + 1] = 0;
            }
        }
        flag_target = 0;
        flag_passrequest = 0;
    }
}

void sstf_primary_request(int direction, int station)
{
    if(direction == 1) {
        state.clockwise_request[station ] = 1;
    } else if(direction == -1) {
        state.counterclockwise_request[station ] = 1;
    }
    if(state.state == 0 ) {
        state.current_target = find_flag();
    }
}

void sstf_secondary_request(int target)
{
    state.target[target ] = 1;
    if(state.state == 0) {
        state.current_target = find_flag();
    }
}
