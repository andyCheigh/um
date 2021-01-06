/*************************************************************************
*                             umlabwrite.c
* 
* 
*      Authors: Jae Hyun Cheigh (jcheig01), Suyu Lui (sliu21)
*
*      Fall 2020 - COMP40
*      HW 6
* 
* 
*      Summary: Generate um test files
*     
**************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "fmt.h"
#include "seq.h"

extern void Um_write_sequence(FILE *output, Seq_T instructions);

extern void build_halt_test(Seq_T instructions);
extern void build_verbose_halt_test(Seq_T instructions);
extern void build_add_test(Seq_T instructions);
extern void build_add_unit_test(Seq_T instructions);
extern void build_output_test(Seq_T stream);
// extern void build_mult_unit_test(Seq_T stream);
extern void build_mult_unit_test2(Seq_T stream);
extern void build_div_unit_test(Seq_T stream);
extern void build_cond_mov_unit_test(Seq_T stream);
extern void build_cond_mov_unit_test2(Seq_T stream);
extern void build_bit_NAND_unit_test(Seq_T stream);
extern void build_input_test(Seq_T stream);
extern void build_segmented_unit_test(Seq_T stream);
extern void build_map_unit_test(Seq_T stream);
extern void build_load_program_unit_test(Seq_T stream);

/* The array `tests` contains all unit tests for the lab. */

static struct test_info {
        const char *name;
        const char *test_input;          /* NULL means no input needed */
        const char *expected_output;
        /* writes instructions into sequence */
        void (*build_test)(Seq_T stream);
} tests[] = {
        { "halt",         NULL, "", build_halt_test },
        { "halt-verbose", NULL, "", build_verbose_halt_test },
        { "add", NULL, "", build_add_test },
        { "output", NULL, "4", build_output_test },
        { "print-six", NULL, "6", build_add_unit_test },
        //{ "mult", NULL, "", build_mult_unit_test },
        { "mult2", NULL, "P", build_mult_unit_test2 },
        { "div", NULL, "A", build_div_unit_test },
        { "cond_mov", NULL, "0", build_cond_mov_unit_test },
        { "cond_mov2", NULL, "(", build_cond_mov_unit_test2 },
        { "bit_NAND", NULL, "%", build_bit_NAND_unit_test },
        { "input", "hello", "hel", build_input_test },
        { "segmented", NULL, "2", build_segmented_unit_test },
        { "map", NULL, "", build_map_unit_test },
        { "load_program", NULL, "y", build_load_program_unit_test }
};

  
#define NTESTS (sizeof(tests)/sizeof(tests[0]))

/*
 * open file 'path' for writing, then free the pathname;
 * if anything fails, checked runtime error
 */
static FILE *open_and_free_pathname(char *path);

/*
 * if contents is NULL or empty, remove the given 'path', 
 * otherwise write 'contents' into 'path'.  Either way, free 'path'.
 */
static void write_or_remove_file(char *path, const char *contents);

static void write_test_files(struct test_info *test);


int main (int argc, char *argv[])
{
        bool failed = false;
        if (argc == 1)
                for (unsigned i = 0; i < NTESTS; i++) {
                        printf("***** Writing test '%s'.\n", tests[i].name);
                        write_test_files(&tests[i]);
                }
        else
                for (int j = 1; j < argc; j++) {
                        bool tested = false;
                        for (unsigned i = 0; i < NTESTS; i++)
                                if (!strcmp(tests[i].name, argv[j])) {
                                        tested = true;
                                        write_test_files(&tests[i]);
                                }
                        if (!tested) {
                                failed = true;
                                fprintf(stderr,
                                        "***** No test named %s *****\n",
                                        argv[j]);
                        }
                }
        return failed; /* failed nonzero == exit nonzero == failure */
}


static void write_test_files(struct test_info *test)
{
        FILE *binary = open_and_free_pathname(Fmt_string("%s.um", test->name));
        Seq_T instructions = Seq_new(0);
        test->build_test(instructions);
        Um_write_sequence(binary, instructions);
        Seq_free(&instructions);
        fclose(binary);

        write_or_remove_file(Fmt_string("%s.0", test->name),
                             test->test_input);
        write_or_remove_file(Fmt_string("%s.1", test->name),
                             test->expected_output);
}


static void write_or_remove_file(char *path, const char *contents)
{
        if (contents == NULL || *contents == '\0') {
                remove(path);
        } else {
                FILE *input = fopen(path, "wb");
                assert(input != NULL);

                fputs(contents, input);
                fclose(input);
        }
        free(path);
}


static FILE *open_and_free_pathname(char *path)
{
        FILE *fp = fopen(path, "wb");
        assert(fp != NULL);

        free(path);
        return fp;
}
