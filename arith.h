/*************************************************************************
*                              arith.h
* 
* 
*      Authors: Jae Hyun Cheigh (jcheig01), Suyu Lui (sliu21)
*
*      Fall 2020 - COMP40
*      HW 6
* 
* 
*      Summary: Header file for arith module. Functions consist of
*               arithmetic operations of UM
*     
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "uarray.h"

#ifndef ARITH_INCLUDED
#define ARITH_INCLUDED

void conditional_move(UArray_T registers, unsigned a,
                      unsigned b, unsigned c);
void addition(UArray_T registers, unsigned a,
              unsigned b, unsigned c);
void multiplication(UArray_T registers, unsigned a,
                    unsigned b, unsigned c);
void division(UArray_T registers, unsigned a,
              unsigned b, unsigned c);
void bitwise_NAND(UArray_T registers, unsigned a,
                  unsigned b, unsigned c);

#endif