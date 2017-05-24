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
#define UNUSED(x) (void)(x)

//void test_compare(const size_t n, float const* results, float const* values);
void test_stereo(t_vbapf* vbap);
void test_square(t_vbapf* vbap);

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

void test_square(t_vbapf* vbap)
{
    float angles[4] = {0.f, 90.f, 180.f, 270.f};
    float result[4] = {0.f, 0.f, 0.f, 0.f};
    printf("quadri version... ");
    if(!vbapf_2d_prepare(vbap, 4, angles))
    {
        assert(vbapf_nls(vbap) == 4);
        
        vbapf_2d_perform(vbap, 0.f, result);
        assert(fabsf(result[0] - 1) < 0.00001f);
        assert(fabsf(result[1] - 0) < 0.00001f);
        assert(fabsf(result[2] - 0) < 0.00001f);
        assert(fabsf(result[3] - 0) < 0.00001f);
        
        vbapf_2d_perform(vbap, 45.f, result);
        assert(fabsf(result[0] - 0.707107f) < 0.00001f);
        assert(fabsf(result[1] - 0.707107f) < 0.00001f);
        assert(fabsf(result[2] - 0) < 0.00001f);
        assert(fabsf(result[3] - 0) < 0.00001f);

        vbapf_2d_perform(vbap, 90.f, result);
        assert(fabsf(result[0] - 0) < 0.00001f);
        assert(fabsf(result[1] - 1.f) < 0.00001f);
        assert(fabsf(result[2] - 0) < 0.00001f);
        assert(fabsf(result[3] - 0) < 0.00001f);
        
        vbapf_2d_perform(vbap, 135.f, result);
        assert(fabsf(result[0] - 0) < 0.00001f);
        assert(fabsf(result[1] - 0.707107f) < 0.00001f);
        assert(fabsf(result[2] - 0.707107f) < 0.00001f);
        assert(fabsf(result[3] - 0) < 0.00001f);
        
        vbapf_2d_perform(vbap, 180.f, result);
        assert(fabsf(result[0] - 0) < 0.00001f);
        assert(fabsf(result[1] - 0.f) < 0.00001f);
        assert(fabsf(result[2] - 1.f) < 0.00001f);
        assert(fabsf(result[3] - 0) < 0.00001f);
        
        vbapf_2d_perform(vbap, 225.f, result);
        assert(fabsf(result[0] - 0) < 0.00001f);
        assert(fabsf(result[1] - 0) < 0.00001f);
        assert(fabsf(result[2] - 0.707107f) < 0.00001f);
        assert(fabsf(result[3] - 0.707107f) < 0.00001f);
        
        vbapf_2d_perform(vbap, 270.f, result);
        assert(fabsf(result[0] - 0) < 0.00001f);
        assert(fabsf(result[1] - 0) < 0.00001f);
        assert(fabsf(result[2] - 0) < 0.00001f);
        assert(fabsf(result[3] - 1) < 0.00001f);
        
        vbapf_2d_perform(vbap, 315.f, result);
        assert(fabsf(result[0] - 0.707107f) < 0.00001f);
        assert(fabsf(result[1] - 0) < 0.00001f);
        assert(fabsf(result[2] - 0) < 0.00001f);
        assert(fabsf(result[3] - 0.707107f) < 0.00001f);
        printf("done\n");
    }
    else
    {
        printf("preparation failed!\n");
    }
}

int main(int argc, const char * argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    
    t_vbapf* vbap   = vbapf_new();
    printf("running 2d tests :\n");
    assert(vbap);
    test_stereo(vbap);
    test_square(vbap);    
    vbapf_free(vbap);
    printf("end\n");
    return 0;
}
