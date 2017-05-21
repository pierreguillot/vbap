/*
// Copyright (c) 2016-2017 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include <vbap.h>

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <assert.h>

void test_stereo(t_vbapf* vbap)
{
    float angles[2] = {45.f, -45.f};
    float result[2] = {0.f, 0.f};
    printf("stereo version... ");
    if(!vbapf_2d_prepare(vbap, 2, angles))
    {
        assert(vbapf_nls(vbap) == 2);
        
        vbapf_2d_perform(vbap, 0.f, result);
        assert(fabsf(result[0] - 0.707107f) < 0.00001f);
        assert(fabsf(result[1] - 0.707107f) < 0.00001f);
        
        vbapf_2d_perform(vbap, -45.f, result);
        assert(fabsf(result[0]) < 0.00001f);
        assert(fabsf(result[1] - 1.f) < 0.00001f);
        
        vbapf_2d_perform(vbap, 45.f, result);
        assert(fabsf(result[1]) < 0.00001f);
        assert(fabsf(result[0] - 1.f) < 0.00001f);
        printf("done\n");
    }
    else
    {
        printf("preparation failed!\n");
    }
}

int main(int argc, const char * argv[])
{
    t_vbapf* vbap   = vbapf_new();
    printf("running 2d tests :\n");
    assert(vbap);
    test_stereo(vbap);
    printf("end\n");
    return 0;
}