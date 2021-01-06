/*************************************************************************
*                              arith.c
* 
* 
*      Authors: Jae Hyun Cheigh (jcheig01), Suyu Lui (sliu21)
*
*      Fall 2020 - COMP40
*      HW 6
* 
* 
*      Summary: Implementation file for arith module. Deals with
*               conditional move, addition, multiplication, division,
*               bitwise NAND operations of UM
*     
**************************************************************************/

#include <stdint.h>
#include <assert.h>
#include "arith.h"


/****************************************************************
 * conditional_move
 * Inputs: 1) Uarray of registers
 *         2) Register A index
 *         3) Register B index
 *         4) Register C index
 * Output: Void
 * Description: Conditional move operation of UM.
 *              If register C value is not 0, assign
 *              register B value to A value.
 *****************************************************************/
void conditional_move(UArray_T registers, unsigned a,
                      unsigned b, unsigned c)
{
    assert(registers != NULL && a < 8 && b < 8 && c < 8);

    uint32_t *a_value = (uint32_t *) UArray_at(registers, a);
    uint32_t *b_value = (uint32_t *) UArray_at(registers, b);
    uint32_t *c_value = (uint32_t *) UArray_at(registers, c);

    if (*c_value != 0)
    {
        *a_value = *b_value;
    }
}

/****************************************************************
 * addition
 * Inputs: 1) Uarray of registers
 *         2) Register A index
 *         3) Register B index
 *         4) Register C index
 * Output: Void
 * Description: Addition operation of UM.
 *              Add register B value and register C value
 *              and assign to register A.
 *****************************************************************/
void addition(UArray_T registers, unsigned a,
              unsigned b, unsigned c)
{
    assert(registers != NULL && a < 8 && b < 8 && c < 8);

    uint32_t *a_value = (uint32_t *) UArray_at(registers, a);
    uint32_t *b_value = (uint32_t *) UArray_at(registers, b);
    uint32_t *c_value = (uint32_t *) UArray_at(registers, c);

    *a_value = ((*b_value) + (*c_value)) % 4294967296;
}

void multiplication(UArray_T registers, unsigned a,
                    unsigned b, unsigned c)
{
    assert(registers != NULL && a < 8 && b < 8 && c < 8);

    uint32_t *a_value = (uint32_t *) UArray_at(registers, a);
    uint32_t *b_value = (uint32_t *) UArray_at(registers, b);
    uint32_t *c_value = (uint32_t *) UArray_at(registers, c);

    *a_value = ((*b_value) * (*c_value)) % 4294967296;
}

void division(UArray_T registers, unsigned a,
              unsigned b, unsigned c)
{
    assert(registers != NULL && a < 8 && b < 8 && c < 8);

    uint32_t *a_value = (uint32_t *) UArray_at(registers, a);
    uint32_t *b_value = (uint32_t *) UArray_at(registers, b);
    uint32_t *c_value = (uint32_t *) UArray_at(registers, c);

    *a_value = (*b_value) / (*c_value);
}

void bitwise_NAND(UArray_T registers, unsigned a,
                  unsigned b, unsigned c)
{
    assert(registers != NULL && a < 8 && b < 8 && c < 8);

    uint32_t *a_value = (uint32_t *) UArray_at(registers, a);
    uint32_t *b_value = (uint32_t *) UArray_at(registers, b);
    uint32_t *c_value = (uint32_t *) UArray_at(registers, c);

    *a_value = ~((*b_value) & (*c_value));
}