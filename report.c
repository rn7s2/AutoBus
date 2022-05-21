#include "definition.h"                                                         //2022.5.21 
                                                                                //SunZhengjun
extern State state;

void report()
{
       
printf("BUS:\nposition:%d\ntarget: ");
    State * p = State->target ;
    if (!p)
	{
		return;
	}
	State * a = p;
	do
	{
		if (a == p->next)
		{
			printf("%d", p->data);
			return;
		}
		printf("%d", p->data);
		p = p->next;
	} while (1);

printf("STATION:\nclockwise:");
State * p = State->clockwise_request ;
    if (!p)
	{
		return;
	}
	State * a = p;
	do
	{
		if (a == p->next)
		{
			printf("%d", p->data);
			return;
		}
		printf("%d", p->data);
		p = p->next;
	} while (1);
printf("counterclockwise: ");
  State * p = State->counterclockwise_request ;
    if (!p)
	{
		return;
	}
	State * a = p;
	do
	{
		if (a == p->next)
		{
			printf("%d", p->data);
			return;
		}
		printf("%d", p->data);
		p = p->next;
	} while (1);

}
