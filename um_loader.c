/*************************************************************************
*                             um_loader.c
* 
* 
*      Authors: Jae Hyun Cheigh (jcheig01), Suyu Lui (sliu21)
*
*      Fall 2020 - COMP40
*      HW 6
* 
* 
*      Summary: Implementation file of um_loader module. This file serves
*               as an intermediary file that handles initialization and
*               execution of the program.
*     
**************************************************************************/

#include <assert.h>
#include "bitpack.h"
#include "um_loader.h"
#include "arith.h"
#include "io_handler.h"
#include "mem_segments.h"

static const int REGISTER_NUM = 8;
static const int SEQ_NUM = 100;
static const int BYTE_SIZE = 8;

/****************************************************************
 * run_um
 * Inputs: 1) File pointer
 * Output: Void
 * Description: Run the UM program.
 *              Initialize registers and segments. Then, call
 *              execute_um function to execute the program.
 *****************************************************************/

void run_um(FILE *fp)
{
    /* initialize program */
    UArray_T registers = reg_init();
    Seq_T segments = Seq_new(SEQ_NUM);
    Seq_T unmapped = Seq_new(SEQ_NUM);
    assert(segments != NULL && unmapped != NULL);
    init_segment_0(fp, segments);
    fclose(fp);

    /* execute program */
    execute_um(registers, segments, unmapped);
}

/****************************************************************
 * init_segment_0
 * Inputs: 1) File pointer
 *         2) Main memory segments
 * Output: Void
 * Description: Initialize segment 0.
 *              Get the instruction word count and
 *              store words into the segment 0 using Bitpack.
 *****************************************************************/
void init_segment_0(FILE *fp, Seq_T segments)
{
    int word_count = word_counter(fp);
    rewind(fp);

    UArray_T segment_0 = UArray_new(word_count, sizeof(uint32_t));
    assert(segment_0 != NULL);

    for (int i = 0; i < word_count; i++)
    {
        uint32_t *word = (uint32_t *) UArray_at(segment_0, i);
        *word = Bitpack_newu(*word, BYTE_SIZE, 24, getc(fp));
        *word = Bitpack_newu(*word, BYTE_SIZE, 16, getc(fp));
        *word = Bitpack_newu(*word, BYTE_SIZE, 8, getc(fp));
        *word = Bitpack_newu(*word, BYTE_SIZE, 0, getc(fp));
    }

    Seq_addhi(segments, segment_0);
}

/****************************************************************
 * execute_um
 * Inputs: 1) Uarray of registers
 *         2) Sequence of main memory segments
 *         3) Sequence of unmapped segment id
 * Output: Void
 * Description: Execute the program.
 *              Loop until the end of insturction
 *              word and make switch cases for
 *              appropriate opcode for each instruction.
 *****************************************************************/
void execute_um(UArray_T registers, Seq_T segments, Seq_T unmapped)
{
    uint32_t counter = 0;
    int condition = 1;

    while (condition == 1)
    {
        UArray_T segment_0 = (UArray_T) Seq_get(segments, 0);
        uint32_t *word = (uint32_t *) UArray_at(segment_0, counter);

        uint32_t opcode;
        unsigned a, b, c, value;
        unpack_word(*word, &opcode, &a, &b, &c, &value);

        switch(opcode)
        {
            case CMOV:
                conditional_move(registers, a, b, c);
                break;
            case SLOAD:
                segmented_load(registers, segments, a, b, c);
                break;
            case SSTORE:
                segmented_store(registers, segments, a, b, c);
                break;
            case ADD:
                addition(registers, a, b, c);
                break;
            case MUL:
                multiplication(registers, a, b, c);
                break;
            case DIV:
                division(registers, a, b, c);
                break;
            case NAND:
                bitwise_NAND(registers, a, b, c);
                break;
            case HALT:
                halt(registers, segments, unmapped);
                break;
            case ACTIVATE:
                map_segment(registers, segments, unmapped, b, c);
                break;
            case INACTIVATE:
                unmap_segment(registers, segments, unmapped, c);
                break;
            case OUT:
                output(registers, c);
                break;
            case IN:
                input(registers, c);
                break;
            case LOADP:
                load_program(registers, segments, &counter, b, c);
                break;
            case LV:
                a = Bitpack_getu(*word, 3, 25);
                load_value(registers, a, value);
                break;
            default:
                break;
        }

        if (opcode != LOADP)
        {
            counter++;
        }
        else
        {
            /* if load program, segment 0 is freed
             * so new initialization is needed */
            segment_0 = (UArray_T) Seq_get(segments, 0);
        }

        if (counter == (uint32_t) UArray_length(segment_0))
        {
            condition = 0;
        }
    }
}


/****************************************************************
 * reg_init
 * Inputs: None
 * Output: Void
 * Description: Initialize registers.
 *              Make unboxed array size of 8 and initialize
 *              each element with 0.
 *****************************************************************/
UArray_T reg_init()
{
    UArray_T registers = UArray_new(REGISTER_NUM, sizeof(uint32_t));
    assert(registers != NULL);

    for (int i = 0; i < REGISTER_NUM; i++)
    {
        uint32_t *reg = (uint32_t *) UArray_at(registers, i);
        *reg = 0;
    }

    return registers;
}

/****************************************************************
 * word_counter
 * Inputs: 1) File pointer
 * Output: Int
 * Description: Count instruction words.
 *              Get each character of the file and
 *              divide by 4.
 *****************************************************************/
int word_counter(FILE *fp)
{
    int count = 0;

    while (fgetc(fp) != EOF)
    {
        count++;
    }

    return count / 4;
}

/****************************************************************
 * unpack_word
 * Inputs: 1) Insturction word
 *         2) Pointer to opcode
 *         3) Pointer to register A
 *         4) Pointer to register B
 *         5) Pointer to register C
 *         6) Pointer to load value
 * Output: Void
 * Description: Unpack instruction words.
 *              Use bitpack to assign back input values.
 *****************************************************************/
void unpack_word(uint32_t word, uint32_t *opcode, unsigned *a,
                 unsigned *b, unsigned *c, unsigned *value)
{
    *opcode = (uint32_t) Bitpack_getu(word, 4, 28);
    *a = (unsigned) Bitpack_getu(word, 3, 6);
    *b = (unsigned) Bitpack_getu(word, 3, 3);
    *c = (unsigned) Bitpack_getu(word, 3, 0);
    *value = (unsigned) Bitpack_getu(word, 25, 0);
}

/****************************************************************
 * halt
 * Inputs: 1) Uarray of registers
 *         2) Sequence of main memory segments
 *         3) Sequence of unmapped segment id
 * Output: Void
 * Description: Halt operation of UM.
 *              Free segments and registers and exit program.
 *****************************************************************/
void halt(UArray_T registers, Seq_T segments, Seq_T unmapped)
{
    assert(registers != NULL && segments != NULL && unmapped != NULL);

    for (int i = 0; i < Seq_length(segments); i++)
    {
        UArray_T segment = (UArray_T) Seq_get(segments, i);
        if(segment != NULL)
        {
            UArray_free(&segment);
        }
    }

    UArray_free(&registers);
    Seq_free(&segments);
    Seq_free(&unmapped);

    exit(EXIT_SUCCESS);
}

/****************************************************************
 * load_value
 * Inputs: 1) Uarray of registers
 *         2) Register A index
 *         3) Load value
 * Output: Void
 * Description: Load Value operation of UM.
 *              Get value from register A index and assign
 *              input load value.
 *****************************************************************/
void load_value(UArray_T registers, unsigned a, unsigned value)
{
    assert(registers != NULL && a < 8);

    uint32_t *a_value = (uint32_t *) UArray_at(registers, a);
    *a_value = value;
}

/****************************************************************
 * load_program
 * Inputs: 1) Uarray of registers
 *         2) Sequence of main memory segments
 *         3) Pointer to program counter
 *         4) Register B index
 *         4) Register C index
 * Output: Void
 * Description: Load Program operation of UM.
 *              Duplicate segment at register B index. Free
 *              segment 0. Put duplicate segment at segment 0.
 *              Program counter point to register C value.
 *****************************************************************/
void load_program(UArray_T registers, Seq_T segments, 
                  uint32_t *counter, unsigned b, unsigned c)
{
    assert(registers != NULL && segments != NULL);
    assert(b < 8 && c < 8);

    uint32_t *b_value = (uint32_t *) UArray_at(registers, b);

    if (*b_value == 0)
    {
        uint32_t *c_value = (uint32_t *) UArray_at(registers, c);
        *counter = *c_value;
        return;
    }

    UArray_T segment_b = (UArray_T) Seq_get(segments, *b_value);
    int length = UArray_length(segment_b);
    UArray_T duplicate = UArray_new(length, sizeof(uint32_t));
    assert(duplicate != NULL);

    for (int i = 0; i < length; i++)
    {
        uint32_t *seg_b_value = (uint32_t *) UArray_at(segment_b, i);
        uint32_t *dup_value = (uint32_t *) UArray_at(duplicate, i);
        *dup_value = *seg_b_value;
    }

    UArray_T segment_0 = Seq_get(segments, 0);
    UArray_free(&segment_0);

    Seq_put(segments, 0, (void *) duplicate);
    uint32_t *c_value = (uint32_t *) UArray_at(registers, c);
    *counter = *c_value;
}