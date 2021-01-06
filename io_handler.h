/*************************************************************************
*                              io_handler.h
* 
* 
*      Authors: Jae Hyun Cheigh (jcheig01), Suyu Lui (sliu21)
*
*      Fall 2020 - COMP40
*      HW 6
* 
* 
*      Summary: Header file for io_handler module. Functions consist of
*               I/O operations of UM
*     
**************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include "uarray.h"

#ifndef IO_HANDLER_INCLUDED
#define IO_HANDLER_INCLUDED

void input(UArray_T registers, unsigned c);
void output(UArray_T registers, unsigned c);

#endif