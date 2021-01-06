/*************************************************************************
*                               um.c
* 
* 
*      Authors: Jae Hyun Cheigh (jcheig01), Suyu Lui (sliu21)
*
*      Fall 2020 - COMP40
*      HW 7
* 
* 
*      Summary: Driver file that runs the main function.
*     
**************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "um_loader.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Needs 1 input file\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(argv[1], "rb");
    assert(fp != NULL);

    run_um(fp);

    exit(EXIT_FAILURE);
}