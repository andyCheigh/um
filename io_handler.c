/*************************************************************************
*                              io_handler.c
* 
* 
*      Authors: Jae Hyun Cheigh (jcheig01), Suyu Lui (sliu21)
*
*      Fall 2020 - COMP40
*      HW 6
* 
* 
*      Summary: Header file for io_handler module. Deals with
*               input, output operations of UM
*     
**************************************************************************/

#include <stdint.h>
#include <assert.h>
#include "io_handler.h"

void input(UArray_T registers, unsigned c)
{
    assert(registers != NULL && c < 8);

    uint32_t *c_value = (uint32_t *) UArray_at(registers, c);
    int value = fgetc(stdin);
    if (value == EOF)
    {
        *c_value = ~0;
        return;
    }

    assert(value >=0 && value <= 255);

    *c_value = (uint32_t) value;
}

void output(UArray_T registers, unsigned c)
{
    assert(registers != NULL && c < 8);

    uint32_t *c_value = (uint32_t *) UArray_at(registers, c);

    assert((*c_value) <= 255);

    char value = (char) *c_value;
    fputc(value, stdout);
}