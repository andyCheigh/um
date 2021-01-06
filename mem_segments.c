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
*      Summary: Header file for mem_segments module. Deals with
*               segmented load, segmented store, map segment,
*               unmap segment operations of UM
*     
**************************************************************************/

#include <stdint.h>
#include <assert.h>
#include "mem_segments.h"

void segmented_load(UArray_T registers, Seq_T segments, 
                    unsigned a, unsigned b, unsigned c)
{
    assert(registers != NULL && segments != NULL);
    assert(a < 8 && b < 8 && c < 8);

    uint32_t *b_value = (uint32_t *) UArray_at(registers, b);
    UArray_T segment_b = (UArray_T) Seq_get(segments, *b_value);
    uint32_t *c_value = (uint32_t *) UArray_at(registers, c);
    uint32_t *seg_b_value = (uint32_t *) UArray_at(segment_b, *c_value);

    uint32_t *a_value = (uint32_t *) UArray_at(registers, a);
    *a_value = *seg_b_value;

}

void segmented_store(UArray_T registers, Seq_T segments, 
                     unsigned a, unsigned b, unsigned c)
{
    assert(registers != NULL && segments != NULL);
    assert(a < 8 && b < 8 && c < 8);

    uint32_t *a_value = (uint32_t *) UArray_at(registers, a);
    UArray_T segment_a = (UArray_T) Seq_get(segments, *a_value);
    uint32_t *b_value = (uint32_t *) UArray_at(registers, b);
    uint32_t *seg_a_value = (uint32_t *) UArray_at(segment_a, *b_value);

    uint32_t *c_value = UArray_at(registers, c);
    *seg_a_value = *c_value;
}

void map_segment(UArray_T registers, Seq_T segments, 
                 Seq_T unmapped, unsigned b, unsigned c)
{
    assert(registers != NULL && segments != NULL && unmapped != NULL);
    assert(b < 8 && c < 8);

    uint32_t *c_value = (uint32_t *) UArray_at(registers, c);
    UArray_T new_segment = UArray_new(*c_value, sizeof(uint32_t));
    assert(new_segment != NULL);

    uint32_t segments_len = (uint32_t) Seq_length(segments);
    uint32_t new_index;

    if (Seq_length(unmapped) != 0)
    {
        new_index = (uint32_t)(uintptr_t) Seq_remhi(unmapped);
    }

    else
    {
        new_index = segments_len;
    }

    if (new_index < segments_len)
    {
        Seq_put(segments, new_index, new_segment);
    }

    else
    {
        Seq_addhi(segments, new_segment);
    }

    for (int i = 0; i < UArray_length(new_segment); i++)
    {
        uint32_t *temp = (uint32_t *) UArray_at(new_segment, i);
        *temp = 0;
    }

    uint32_t *b_value = (uint32_t *) UArray_at(registers, b);
    *b_value = new_index;
}

void unmap_segment(UArray_T registers, Seq_T segments,
                   Seq_T unmapped, unsigned c)
{
    assert(registers != NULL && segments != NULL && unmapped != NULL);
    assert(c < 8);

    uint32_t *c_value = (uint32_t *) UArray_at(registers, c);

    UArray_T c_segment = (UArray_T) Seq_get(segments, *c_value);

    if (c_segment != NULL)
    {
        UArray_free(&c_segment);
    }

    Seq_put(segments, *c_value, NULL);
    Seq_addhi(unmapped, (void *)(uintptr_t) *c_value);

}