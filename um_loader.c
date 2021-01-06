/*************************************************************************
*                             um_loader.c
* 
* 
*      Authors: Jae Hyun Cheigh (jcheig01), Suyu Lui (sliu21)
*
*      Fall 2020 - COMP40
*      HW 7
* 
* 
*      Summary: Implementation file of um_loader module. This file serves
*               as an intermediary file that handles initialization and
*               execution of the program.
*     
**************************************************************************/

#include <assert.h>
#include "um_loader.h"

#define REGISTER_NUM 8

/****************************************************************
 * run_um
 * Inputs: 1) File pointer
 * Output: Void
 * Description: Run the UM program.
 *              Initialize registers and segments. Then, perform
 *              UM operations.
 *****************************************************************/

void run_um(FILE *fp)
{
    /* initialize program */
    uint32_t registers[REGISTER_NUM] = {0, 0, 0, 0, 0, 0, 0, 0};
    int seg_cap = 50000;
    int unmap_cap = 50000;
    Seg_T **segments = malloc(sizeof(Seg_T*) * seg_cap);
    uint32_t *unmapped = malloc(sizeof(uint32_t) * unmap_cap);
    int segments_len = 0;
    int unmapped_len = 0;
    int word_count = 0;

    while (fgetc(fp) != EOF)
    {
        word_count++;
    }

    word_count = word_count / 4;
    rewind(fp);

    Seg_T *segment_0 = malloc(sizeof(*segment_0) +
                       sizeof(uint32_t) * word_count);
    segment_0->length = word_count;

    uint32_t unpack;

    for (int i = 0; i < word_count; i++)
    {
        unpack = 0;
        unpack = unpack + (getc(fp) << 24);
        unpack = unpack + (getc(fp) << 16);
        unpack = unpack + (getc(fp) << 8);
        unpack = unpack + (getc(fp) << 0);
        segment_0->word[i] = unpack;
    }

    segments[0] = segment_0;
    segments_len++;

    fclose(fp);

    /* execute program */
    uint32_t counter = 0;
    int condition = 1;

    while (condition == 1)
    {
        unpack = segment_0->word[counter];

        uint32_t opcode = (unpack & (~0U >> (32 - 4) << 28)) >> 28;
        uint32_t a = (unpack & (~0U >> (32 - 3) << 6)) >> 6;
        uint32_t b = (unpack & (~0U >> (32 - 3) << 3)) >> 3;
        uint32_t c = (unpack & (~0U >> (32 - 3) << 0)) >> 0;
        uint32_t value;


        /* ADDED INITIALIZATIONS */

        /* from arith module */
        uint32_t b_value;
        uint32_t c_value;

        /* from io_handler module */
        int value_i;
        char value_o;

        /* from mem_segments module */
        Seg_T *segment;
        Seg_T *segment_a;
        Seg_T *segment_b;
        Seg_T *segment_c;
        Seg_T** new_segments;
        uint32_t *new_unmapped;
        int segment_b_len;
        int new_index;
        int length_new;

        counter++;

        switch(opcode)
        {
            case CMOV:
                b_value = registers[b];
                c_value = registers[c];

                if (c_value != 0)
                {
                    registers[a] = b_value;
                }
                break;
            case SLOAD:
                segment_b = segments[registers[b]];
                registers[a] = segment_b->word[registers[c]];
                break;
            case SSTORE:
                segment_a = segments[registers[a]];
                segment_a->word[registers[b]] = registers[c];
                break;
            case ADD:
                b_value = registers[b];
                c_value = registers[c];

                registers[a] = ((b_value) + (c_value)) % 4294967296;
                break;
            case MUL:
                b_value = registers[b];
                c_value = registers[c];

                registers[a] = ((b_value) * (c_value)) % 4294967296;
                break;
            case DIV:
                b_value = registers[b];
                c_value = registers[c];

                registers[a] = (b_value) / (c_value);
                break;
            case NAND:
                b_value = registers[b];
                c_value = registers[c];

                registers[a] = ~((b_value) & (c_value));
                break;
            case HALT:
                for (int i = 0; i < segments_len; i++)
                {
                    segment = segments[i];
                    if(segment != NULL)
                    {
                        free(segment);
                    }
                }

                free(segments);
                free(unmapped);

                exit(EXIT_SUCCESS);
                break;
            case ACTIVATE:
                segment = malloc(sizeof(*segment) + 
                          sizeof(uint32_t) * registers[c]);
                segment->length = registers[c];

                length_new = registers[c];
                for (int i = 0; i < length_new; i++)
                {
                    segment->word[i] = 0;
                }

                if (unmapped_len != 0)
                {
                    new_index = unmapped[unmapped_len-1];
                    segments[new_index] = segment;
                    unmapped_len--;
                }
                else
                {
                    new_index = segments_len;
                    if (new_index == seg_cap)
                    {
                        new_segments = malloc(sizeof(Seg_T*) * seg_cap * 2);
                        for (int i = 0; i < seg_cap; i++)
                        {
                            new_segments[i] = segments[i];
                        }
                        free(segments);

                        segments = new_segments;
                        seg_cap = seg_cap * 2;
                    }

                    segments[new_index] = segment;
                    segments_len++;
                }

                registers[b] = new_index;
                break;
            case INACTIVATE:
                segment_c = segments[registers[c]];
                free(segment_c);
                segments[registers[c]] = NULL;

                if (unmapped_len == unmap_cap)
                {
                    new_unmapped = malloc(sizeof(uint32_t) * unmap_cap * 2);
                    for (int i = 0; i < unmap_cap; i++)
                    {
                        new_unmapped[i] = unmapped[i];
                    }
                    free(unmapped);
                    unmapped = new_unmapped;
                    unmap_cap = unmap_cap * 2;
                }

                unmapped[unmapped_len] = registers[c];
                unmapped_len++;
                break;
            case OUT:
                c_value = registers[c];

                assert((c_value) <= 255);

                value_o = (char) c_value;
                fputc(value_o, stdout);
                break;
            case IN:
                value_i = fgetc(stdin);

                if (value_i == EOF)
                {
                    registers[c] = ~0;
                    break;
                }

                assert(value_i >=0 && value_i <= 255);

                registers[c] = (uint32_t) value_i;
                break;
            case LOADP:
                if (registers[b] == 0)
                {
                    counter = registers[c];
                    break;
                }
                
                
                segment_b = segments[registers[b]];
                segment_b_len = segment_b->length;

                free(segment_0);
                segment_0 = malloc(sizeof(*segment_0) + 
                            sizeof(uint32_t) * segment_b_len);

                for (int i = 0; i < segment_b_len; i++)
                {
                    segment_0->word[i] = segment_b->word[i];
                }

                segments[0] = segment_0;
                counter = registers[c];
                break;
            case LV:
                value = unpack & (~0U >> (32 - 25));
                a = (unpack & (~0U >> (32 - 3) << 25)) >> 25;
                registers[a] = value;
                break;
            default:
                return;
        }
    }
}

