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

void test_stereo(t_vbapf* vbap);
void test_square(t_vbapf* vbap);
void test_triangle(t_vbapf* vbap);
void test_cube(t_vbapf* vbap);

void test_stereo(t_vbapf* vbap)
{
    float angles[2] = {45.f, -45.f};
    float result[2];
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
    float result[4];
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

void test_triangle(t_vbapf* vbap)
{
    float angles[6] = {45.f, 0.f, 0.f, 35.3f, -45.f, 0.f};
    float result[3];
    printf("triangle version... ");
    if(!vbapf_3d_prepare(vbap, 3, angles))
    {
        assert(vbapf_nls(vbap) == 3);
        
        vbapf_3d_perform(vbap, 0.f, 0.f, result);
        assert(fabsf(result[0] - 0.707107f) < 0.00001f);
        assert(fabsf(result[1] - 0) < 0.00001f);
        assert(fabsf(result[2] - 0.707107f) < 0.00001f);
        
        vbapf_3d_perform(vbap, 45.f, 0.f, result);
        assert(fabsf(result[0] - 1.f) < 0.00001f);
        assert(fabsf(result[1] - 0) < 0.00001f);
        assert(fabsf(result[2] - 0) < 0.00001f);
        
        vbapf_3d_perform(vbap, -45.f, 0.f, result);
        assert(fabsf(result[0] - 0) < 0.00001f);
        assert(fabsf(result[1] - 0) < 0.00001f);
        assert(fabsf(result[2] - 1.f) < 0.00001f);
        
        vbapf_3d_perform(vbap, 0.f, 35.3f, result);
        assert(fabsf(result[0] - 0) < 0.00001f);
        assert(fabsf(result[1] - 1.f) < 0.00001f);
        assert(fabsf(result[2] - 0) < 0.00001f);
        
        vbapf_3d_perform(vbap, 12.f, 20.f, result);
        assert(fabsf(result[0] - 0.586984f) < 0.00001f);
        assert(fabsf(result[1] - 0.77806f) < 0.00001f);
        assert(fabsf(result[2] - 0.22377f) < 0.00001f);
        
        vbapf_3d_perform(vbap, -17.f, 25.f, result);
        assert(fabsf(result[0] - 0.00415882f) < 0.00001f);
        assert(fabsf(result[1] - 0.888272f) < 0.00001f);
        assert(fabsf(result[2] - 0.459299f) < 0.00001f);
        
        vbapf_3d_perform(vbap, -30.f, 4.f, result);
        assert(fabsf(result[0] - 0.204581f) < 0.00001f);
        assert(fabsf(result[1] - 0.130997f) < 0.00001f);
        assert(fabsf(result[2] - 0.970045f) < 0.00001f);
        
        printf("done\n");
    }
    else
    {
        printf("preparation failed!\n");
    }
}

void test_cube(t_vbapf* vbap)
{
    float angles[16] = {45.f, 35.3f, 135.f, 35.3f, 225.f, 35.3f, 315.f, 35.3f, 45.f, -35.3f, 135.f, -35.3f, 225.f, -35.3f, 315.f, -35.3};
    float result[8];
    printf("cube version... ");
    if(!vbapf_3d_prepare(vbap, 8, angles))
    {
        assert(vbapf_nls(vbap) == 8);
        
        vbapf_3d_perform(vbap, 0.f, 0.f, result);
        assert(fabsf(result[0] - 0) < 0.00001f);
        assert(fabsf(result[1] - 0) < 0.00001f);
        assert(fabsf(result[2] - 0) < 0.00001f);
        assert(fabsf(result[3] - 0.707107f) < 0.00001f);
        assert(fabsf(result[4] - 0.707107f) < 0.00001f);
        assert(fabsf(result[5] - 0) < 0.00001f);
        assert(fabsf(result[6] - 0) < 0.00001f);
        assert(fabsf(result[7] - 0) < 0.00001f);
        
        vbapf_3d_perform(vbap, 26.f, 39.f, result);
        assert(fabsf(result[0] - 0.961695f) < 0.00001f);
        assert(fabsf(result[1] - 0) < 0.00001f);
        assert(fabsf(result[2] - 0) < 0.00001f);
        assert(fabsf(result[3] - 0.266355f) < 0.00001f);
        assert(fabsf(result[4] - 0) < 0.00001f);
        assert(fabsf(result[5] - 0) < 0.00001f);
        assert(fabsf(result[6] - 0) < 0.00001f);
        assert(fabsf(result[7] - 0.0647837f) < 0.00001f);
        
        vbapf_3d_perform(vbap, -37.f, -23.f, result);
        assert(fabsf(result[0] - 0.157329f) < 0.00001f);
        assert(fabsf(result[1] - 0) < 0.00001f);
        assert(fabsf(result[2] - 0) < 0.00001f);
        assert(fabsf(result[3] - 0.142205f) < 0.00001f);
        assert(fabsf(result[4] - 0) < 0.00001f);
        assert(fabsf(result[5] - 0) < 0.00001f);
        assert(fabsf(result[6] - 0) < 0.00001f);
        assert(fabsf(result[7] - 0.977254f) < 0.00001f);
        
        vbapf_3d_perform(vbap, 156.f, 12.f, result);
        assert(fabsf(result[0] - 0) < 0.00001f);
        assert(fabsf(result[1] - 0.90076f) < 0.00001f);
        assert(fabsf(result[2] - 0) < 0.00001f);
        assert(fabsf(result[3] - 0) < 0.00001f);
        assert(fabsf(result[4] - 0) < 0.00001f);
        assert(fabsf(result[5] - 0.155586f) < 0.00001f);
        assert(fabsf(result[6] - 0.405493f) < 0.00001f);
        assert(fabsf(result[7] - 0) < 0.00001f);
        
        vbapf_3d_perform(vbap, 45.f, -35.f, result);
        assert(fabsf(result[0] - 0.00556134f) < 0.00001f);
        assert(fabsf(result[1] - 0) < 0.00001f);
        assert(fabsf(result[2] - 0) < 0.00001f);
        assert(fabsf(result[3] - 0) < 0.00001f);
        assert(fabsf(result[4] - 0.999985f) < 0.00001f);
        assert(fabsf(result[5] - 0) < 0.00001f);
        assert(fabsf(result[6] - 0) < 0.00001f);
        assert(fabsf(result[7] - 0) < 0.00001f);
        
        printf("done\n");
    }
    else
    {
        printf("preparation failed!\n");
    }
}

int main(int argc, const char * argv[])
{
    t_vbapf* vbap;
    UNUSED(argc);
    UNUSED(argv);
    
    vbap = vbapf_new();
    printf("running tests :\n");
    assert(vbap);
    test_stereo(vbap);
    test_square(vbap);
    
    test_triangle(vbap);
    test_cube(vbap);
    
    vbapf_free(vbap);
    printf("end\n");
    return 0;
}
