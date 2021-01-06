/*************************************************************************
*                             um_loader.h
* 
* 
*      Authors: Jae Hyun Cheigh (jcheig01), Suyu Lui (sliu21)
*
*      Fall 2020 - COMP40
*      HW 6
* 
* 
*      Summary: Header file for um_loader module. Functions consist of
*               initialization and execution of programs and 3 functions
*               for um operations.
*     
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "uarray.h"
#include "seq.h"

#ifndef UM_LOADER_INCLUDED
#define UM_LOADER_INCLUDED

typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

void run_um(FILE *fp);
void init_segment_0(FILE *fp, Seq_T segments);
void execute_um(UArray_T registers, Seq_T segments, Seq_T unmapped);
UArray_T reg_init();
int word_counter(FILE *fp);
void unpack_word(uint32_t word, uint32_t *opcode, unsigned *a,
                 unsigned *b, unsigned *c, unsigned *value);
void halt(UArray_T registers, Seq_T segments, Seq_T unmapped);
void load_program(UArray_T registers, Seq_T segments, 
                  uint32_t *counter, unsigned b, unsigned c);
void load_value(UArray_T registers, unsigned a, unsigned value);

#endif