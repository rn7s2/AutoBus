#include <stdio.h>
#include "main.h"
#include "list.h"

Config config;     // 全局配置
State state;       // 全局状态

int main(int argc, char *argv[])
{
    printf("Hello, world!\n");
    return 0;
}

void init_state()
{
    state.primary_requests = new_node(-1);
    state.secondary_requests = new_node(-1);
}

void dispatch(Instruction t)
{
}

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
