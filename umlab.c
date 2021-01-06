/*
 * umlab.c
 *
 * Functions to generate UM unit tests. Once complete, this module
 * should be augmented and then linked against umlabwrite.c to produce
 * a unit test writing program.
 *  
 * A unit test is a stream of UM instructions, represented as a Hanson
 * Seq_T of 32-bit words adhering to the UM's instruction format.  
 * 
 * Any additional functions and unit tests written for the lab go
 * here. 
 *  
 */


#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>


typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        Um_instruction inst = 0;

        inst = Bitpack_newu(inst, 4, 28, op);
        inst = Bitpack_newu(inst, 3, 6, ra);
        inst = Bitpack_newu(inst, 3, 3 , rb);
        inst = Bitpack_newu(inst, 3, 0, rc);

        return inst;
}

Um_instruction loadval(unsigned ra, unsigned val)
{
        Um_instruction inst = 0;

        inst = Bitpack_newu(inst, 4, 28, LV);
        inst = Bitpack_newu(inst, 3, 25, ra);
        inst = Bitpack_newu(inst, 25, 0 , val);

        return inst;
}


/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;


static inline Um_instruction cond_mov(Um_register a, Um_register b, 
                                      Um_register c) 
{
        return three_register(CMOV, a, b, c);
}

static inline Um_instruction segmented_load(Um_register a, Um_register b, 
                                            Um_register c)
{
        return three_register(SLOAD, a, b, c);  
}

static inline Um_instruction segmented_store(Um_register a, Um_register b,
                                             Um_register c)
{
        return three_register(SSTORE, a, b, c);  
}

static inline Um_instruction add(Um_register a, Um_register b, Um_register c)
{
        return three_register(ADD, a, b, c);
}

static inline Um_instruction mult(Um_register a, Um_register b, Um_register c)
{
        return three_register(MUL, a, b, c);
}

static inline Um_instruction div(Um_register a, Um_register b, Um_register c)
{
        return three_register(DIV, a, b, c);
}

static inline Um_instruction bit_NAND(Um_register a, Um_register b,
                                      Um_register c) 
{
        return three_register(NAND, a, b, c);
}

static inline Um_instruction map_segment(Um_register b, Um_register c)
{
        return three_register(ACTIVATE, 0, b, c);  
}

static inline Um_instruction unmap_segment(Um_register c)
{
        return three_register(INACTIVATE, 0, 0, c);  
}

static inline Um_instruction output(Um_register c)
{
        return three_register(OUT, 0, 0, c);
}

static inline Um_instruction input(Um_register c)
{
        return three_register(IN, 0, 0, c);
}

static inline Um_instruction load_program(Um_register b, Um_register c)
{
        return three_register(LOADP, 0, b, c);  
}

/* Functions for working with streams */

static inline void append(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

const uint32_t Um_word_width = 32;

void Um_write_sequence(FILE *output, Seq_T stream)
{
        assert(output != NULL && stream != NULL);
        int stream_length = Seq_length(stream);
        for (int i = 0; i < stream_length; i++) {
                Um_instruction inst = (uintptr_t)Seq_remlo(stream);
                for (int lsb = Um_word_width - 8; lsb >= 0; lsb -= 8) {
                        fputc(Bitpack_getu(inst, 8, lsb), output);
                }
        }
      
}


/* Unit tests for the UM */

/* checks if halt runs correctly */
void build_halt_test(Seq_T stream)
{
        append(stream, halt());
}

/* checks if halt runs before other operations */
void build_verbose_halt_test(Seq_T stream)
{
        append(stream, halt());
        append(stream, loadval(r1, 'B'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'a'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'd'));
        append(stream, output(r1));
        append(stream, loadval(r1, '!'));
        append(stream, output(r1));
        append(stream, loadval(r1, '\n'));
        append(stream, output(r1));
}

/* checks if add runs with no error */
void build_add_test(Seq_T stream)
{
        append(stream, add(r1, r2, r3));
        append(stream, halt());
}

/* prints loaded value in register */
/* prints '4' */
void build_output_test(Seq_T stream)
{
        append(stream, loadval(r1, 52));
        append(stream, output(r1));
        append(stream, halt());
}

/* add 48 with 6 */
/* print '6' */
void build_add_unit_test(Seq_T stream)
{
        append(stream, loadval(r2, 48));
        append(stream, loadval(r3, 6));
        append(stream, add(r1, r2, r3));
        append(stream, output(r1));
        append(stream, halt());
}

/* multiply 48 with 6 */
/* tests when output value > 255 */
/*
void build_mult_unit_test(Seq_T stream)
{
        append(stream, loadval(r2, 48));
        append(stream, loadval(r3, 6));
        append(stream, mult(r1, r2, r3));
        append(stream, output(r1));
        append(stream, halt());
}
*/

/* multiply 20 with 4 */
/* prints 'P' */
void build_mult_unit_test2(Seq_T stream)
{
        append(stream, loadval(r2, 20));
        append(stream, loadval(r3, 4));
        append(stream, mult(r1, r2, r3));
        append(stream, output(r1));
        append(stream, halt());
}

/* divide 130 with 2 */
/* prints 'A' */
void build_div_unit_test(Seq_T stream)
{
        append(stream, loadval(r2, 130));
        append(stream, loadval(r3, 2));
        append(stream, div(r1, r2, r3));
        append(stream, output(r1));
        append(stream, halt());
}

/* conditional move when a=20 b=48 c=6 */
/* prints '0' */
void build_cond_mov_unit_test(Seq_T stream)
{
        append(stream, loadval(r1, 20));
        append(stream, loadval(r2, 48));
        append(stream, loadval(r3, 6));
        append(stream, cond_mov(r1, r2, r3));
        append(stream, output(r1));
        append(stream, halt());
}

/* conditional move when a=40 b=48 c=0 */
/* prints '(' */
void build_cond_mov_unit_test2(Seq_T stream)
{
        append(stream, loadval(r1, 40));
        append(stream, loadval(r2, 48));
        append(stream, loadval(r3, 0));
        append(stream, cond_mov(r1, r2, r3));
        append(stream, output(r1));
        append(stream, halt());
}

/* bitwise NAND to get the original loaded value */
/* prints '%' */
void build_bit_NAND_unit_test(Seq_T stream)
{
        append(stream, loadval(r2, 37));
        append(stream, loadval(r3, 37));
        append(stream, bit_NAND(r1, r2, r3));
        append(stream, bit_NAND(r4, r2, r3));
        append(stream, bit_NAND(r5, r1, r4));
        append(stream, output(r5));
        append(stream, halt());
}

/* prints first three characters */
/* prints 'hel' */
void build_input_test(Seq_T stream)
{
        append(stream, input(r1));
        append(stream, input(r2));
        append(stream, input(r3));

        append(stream, output(r1));
        append(stream, output(r2));
        append(stream, output(r3));
        append(stream, halt());
}

/* checks if stored value is loaded correctly */
/* prints '2' */
void build_segmented_unit_test(Seq_T stream)
{
        append(stream, loadval(r3, 50));
        append(stream, segmented_store(r1, r2, r3));
        append(stream, segmented_load(r4, r1, r2)); 
        append(stream, output(r4)); 
        append(stream, halt());
}

/* checks for map/unmap segment leaks or error */
void build_map_unit_test(Seq_T stream)
{
        append(stream, loadval(r1, 48));
        append(stream, map_segment(r2, r1));
        append(stream, unmap_segment(r2));
        append(stream, halt());
}

/* checks if load program loads correctly */
/* skips printing 'h' and 'E', and only prints 'y' */
void build_load_program_unit_test(Seq_T stream)
{
        append(stream, loadval(r0, 0));
        append(stream, loadval(r1, 8));
        append(stream, loadval(r2, 104));
        append(stream, loadval(r3, 69));
        append(stream, loadval(r4, 121));

        append(stream, load_program(r5, r1));

        append(stream, output(r2));
        append(stream, output(r3));
        append(stream, output(r4));

        append(stream, halt());
}


