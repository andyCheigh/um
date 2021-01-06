/*************************************************************************
*                             um_loader.h
* 
* 
*      Authors: Jae Hyun Cheigh (jcheig01), Suyu Lui (sliu21)
*
*      Fall 2020 - COMP40
*      HW 7
* 
* 
*      Summary: Header file for um_loader module.
*     
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef UM_LOADER_INCLUDED
#define UM_LOADER_INCLUDED

typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

typedef struct Seg_T {
	unsigned length;
	uint32_t word[];
} Seg_T;

void run_um(FILE *fp);


#endif