/******************************************************************************
 * Filename:    main.h
 * Author:      雷瑞祺
 * Purpose:     Prototypes of module main.
 * Last update: 2022-05-15
 ******************************************************************************/

#ifndef _MAIN_H
#define _MAIN_H

#include "definition.h"

void init_state();
void dispatch(Instruction t);
void read_config();

#endif
