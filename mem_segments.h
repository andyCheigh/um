/*************************************************************************
*                              mem_segments.h
* 
* 
*      Authors: Jae Hyun Cheigh (jcheig01), Suyu Lui (sliu21)
*
*      Fall 2020 - COMP40
*      HW 6
* 
* 
*      Summary: Header file for mem_segments module. Functions consist of
*               memory segment operations of UM
*     
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "uarray.h"
#include "seq.h"

#ifndef MEM_SEGMENTS_INCLUDED
#define MEM_SEGMENTS_INCLUDED

void segmented_load(UArray_T registers, Seq_T segments, 
                    unsigned a, unsigned b, unsigned c);
void segmented_store(UArray_T registers, Seq_T segments, 
                     unsigned a, unsigned b, unsigned c);
void map_segment(UArray_T registers, Seq_T segments, 
                 Seq_T unmapped, unsigned b, unsigned c);
void unmap_segment(UArray_T registers, Seq_T segments,
                   Seq_T unmapped, unsigned c);

#endif