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

static void test_f_stereo(t_vbapf* vbap)
{
    float angles[2] = {45.f, -45.f};
    float result[2];
    printf("    stereo version... ");
    if(!vbapf_2d_prepare(vbap, 2, angles))
    {
        assert(vbapf_nls(vbap) == 2);
        assert(vbapf_dimension(vbap) == 2);
        
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

static void test_f_square(t_vbapf* vbap)
{
    float angles[4] = {0.f, 90.f, 180.f, 270.f};
    float result[4];
    printf("    quadri version... ");
    if(!vbapf_2d_prepare(vbap, 4, angles))
    {
        assert(vbapf_nls(vbap) == 4);
        assert(vbapf_dimension(vbap) == 2);
        
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

static void test_f_triangle(t_vbapf* vbap)
{
    float angles[6] = {45.f, 0.f, 0.f, 35.3f, -45.f, 0.f};
    float result[3];
    printf("    triangle version... ");
    if(!vbapf_3d_prepare(vbap, 3, angles))
    {
        assert(vbapf_nls(vbap) == 3);
        assert(vbapf_dimension(vbap) == 3);
        
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

static void test_f_cube(t_vbapf* vbap)
{
    float angles[16] = {45.f, 35.3f, 135.f, 35.3f, 225.f, 35.3f, 315.f, 35.3f, 45.f, -35.3f, 135.f, -35.3f, 225.f, -35.3f, 315.f, -35.3f};
    float result[8];
    printf("    cube version... ");
    if(!vbapf_3d_prepare(vbap, 8, angles))
    {
        assert(vbapf_nls(vbap) == 8);
        assert(vbapf_dimension(vbap) == 3);
        
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

static void test_f_icosahedron(t_vbapf* vbap)
{
    float angles[] = {180, 69.1, 159.1, 0, 90, 20.9, 135, 35.3, 135, -35.3, 180, -69.1, 20.9, 0, 90, -20.9, 45, 35.3, 45, -35.3, 0, 69.1, 200.9, 0, 270, 20.9, 225, 35.3, 225, -35.3, 0, -69.1, 339.1, 0, 270, -20.9, 315, 35.3, 315, -35.3};
    printf("    icosahedron version... ");
    if(!vbapf_3d_prepare(vbap, 20, angles))
    {
        printf("done\n");
    }
    else
    {
        printf("preparation failed!\n");
    };
}

static void test_f_invalid(t_vbapf* vbap)
{
    float angles_2d[4] = {0.f, 0.f, 90.f, 90.f};
    float angles_3d[6] = {0.f, 0.f, 2.f, 0.f, 4.f, 0.f};
    printf("    invalid version... ");
    assert(vbapf_2d_prepare(vbap, 2, angles_2d));
    assert(vbapf_3d_prepare(vbap, 3, angles_3d));
}

static void test_f(void)
{
    t_vbapf* vbapf = vbapf_new();
    printf("single precision...\n");
    assert(vbapf);
    test_f_stereo(vbapf);
    test_f_square(vbapf);
    test_f_triangle(vbapf);
    test_f_cube(vbapf);
    test_f_icosahedron(vbapf);
    test_f_invalid(vbapf);
    vbapf_free(vbapf);
    printf("done\n");
}


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

static void test_d_stereo(t_vbap* vbap)
{
    double angles[2] = {45., -45.};
    double result[2];
    printf("    stereo version... ");
    if(!vbap_2d_prepare(vbap, 2, angles))
    {
        assert(vbap_nls(vbap) == 2);
        assert(vbap_dimension(vbap) == 2);
        
        vbap_2d_perform(vbap, 0., result);
        assert(fabs(result[0] - 0.707107) < 0.00001);
        assert(fabs(result[1] - 0.707107) < 0.00001);
        
        vbap_2d_perform(vbap, -45., result);
        assert(fabs(result[0]) < 0.00001);
        assert(fabs(result[1] - 1.) < 0.00001);
        
        vbap_2d_perform(vbap, 45., result);
        assert(fabs(result[1]) < 0.00001);
        assert(fabs(result[0] - 1.) < 0.00001);
        printf("done\n");
    }
    else
    {
        printf("preparation failed!\n");
    }
}

static void test_d_square(t_vbap* vbap)
{
    double angles[4] = {0., 90., 180., 270.};
    double result[4];
    printf("    quadri version... ");
    if(!vbap_2d_prepare(vbap, 4, angles))
    {
        assert(vbap_nls(vbap) == 4);
        assert(vbap_dimension(vbap) == 2);
        
        vbap_2d_perform(vbap, 0., result);
        assert(fabs(result[0] - 1) < 0.00001);
        assert(fabs(result[1] - 0) < 0.00001);
        assert(fabs(result[2] - 0) < 0.00001);
        assert(fabs(result[3] - 0) < 0.00001);
        
        vbap_2d_perform(vbap, 45., result);
        assert(fabs(result[0] - 0.707107) < 0.00001);
        assert(fabs(result[1] - 0.707107) < 0.00001);
        assert(fabs(result[2] - 0) < 0.00001);
        assert(fabs(result[3] - 0) < 0.00001);
        
        vbap_2d_perform(vbap, 90., result);
        assert(fabs(result[0] - 0) < 0.00001);
        assert(fabs(result[1] - 1.) < 0.00001);
        assert(fabs(result[2] - 0) < 0.00001);
        assert(fabs(result[3] - 0) < 0.00001);
        
        vbap_2d_perform(vbap, 135., result);
        assert(fabs(result[0] - 0) < 0.00001);
        assert(fabs(result[1] - 0.707107) < 0.00001);
        assert(fabs(result[2] - 0.707107) < 0.00001);
        assert(fabs(result[3] - 0) < 0.00001);
        
        vbap_2d_perform(vbap, 180., result);
        assert(fabs(result[0] - 0) < 0.00001);
        assert(fabs(result[1] - 0.) < 0.00001);
        assert(fabs(result[2] - 1.) < 0.00001);
        assert(fabs(result[3] - 0) < 0.00001);
        
        vbap_2d_perform(vbap, 225, result);
        assert(fabs(result[0] - 0) < 0.00001);
        assert(fabs(result[1] - 0) < 0.00001);
        assert(fabs(result[2] - 0.707107) < 0.00001);
        assert(fabs(result[3] - 0.707107) < 0.00001);
        
        vbap_2d_perform(vbap, 270, result);
        assert(fabs(result[0] - 0) < 0.00001);
        assert(fabs(result[1] - 0) < 0.00001);
        assert(fabs(result[2] - 0) < 0.00001);
        assert(fabs(result[3] - 1) < 0.00001);
        
        vbap_2d_perform(vbap, 315., result);
        assert(fabs(result[0] - 0.707107) < 0.00001);
        assert(fabs(result[1] - 0) < 0.00001);
        assert(fabs(result[2] - 0) < 0.00001);
        assert(fabs(result[3] - 0.707107) < 0.00001);
        printf("done\n");
    }
    else
    {
        printf("preparation failed!\n");
    }
}

static void test_d_triangle(t_vbap* vbap)
{
    double angles[6] = {45., 0., 0., 35.3, -45., 0.};
    double result[3];
    printf("    triangle version... ");
    if(!vbap_3d_prepare(vbap, 3, angles))
    {
        assert(vbap_nls(vbap) == 3);
        assert(vbap_dimension(vbap) == 3);
        
        vbap_3d_perform(vbap, 0., 0., result);
        assert(fabs(result[0] - 0.707107) < 0.00001);
        assert(fabs(result[1] - 0) < 0.00001);
        assert(fabs(result[2] - 0.707107) < 0.00001);
        
        vbap_3d_perform(vbap, 45., 0., result);
        assert(fabs(result[0] - 1.) < 0.00001);
        assert(fabs(result[1] - 0) < 0.00001);
        assert(fabs(result[2] - 0) < 0.00001);
        
        vbap_3d_perform(vbap, -45., 0., result);
        assert(fabs(result[0] - 0) < 0.00001);
        assert(fabs(result[1] - 0) < 0.00001);
        assert(fabs(result[2] - 1.) < 0.00001);
        
        vbap_3d_perform(vbap, 0., 35.3, result);
        assert(fabs(result[0] - 0) < 0.00001);
        assert(fabs(result[1] - 1.) < 0.00001);
        assert(fabs(result[2] - 0) < 0.00001);
        
        vbap_3d_perform(vbap, 12., 20., result);
        assert(fabs(result[0] - 0.586984) < 0.00001);
        assert(fabs(result[1] - 0.77806) < 0.00001);
        assert(fabs(result[2] - 0.22377) < 0.00001);
        
        vbap_3d_perform(vbap, -17., 25., result);
        assert(fabs(result[0] - 0.00415882) < 0.00001);
        assert(fabs(result[1] - 0.888272) < 0.00001);
        assert(fabs(result[2] - 0.459299) < 0.00001);
        
        vbap_3d_perform(vbap, -30., 4., result);
        assert(fabs(result[0] - 0.204581) < 0.00001);
        assert(fabs(result[1] - 0.130997) < 0.00001);
        assert(fabs(result[2] - 0.970045) < 0.00001);
        
        printf("done\n");
    }
    else
    {
        printf("preparation failed!\n");
    }
}

void test_d_cube(t_vbap* vbap)
{
    double angles[16] = {45., 35.3, 135., 35.3, 225., 35.3, 315., 35.3, 45., -35.3, 135., -35.3, 225., -35.3, 315., -35.3};
    double result[8];
    printf("    cube version... ");
    if(!vbap_3d_prepare(vbap, 8, angles))
    {
        assert(vbap_nls(vbap) == 8);
        
        vbap_3d_perform(vbap, 0., 0., result);
        assert(fabs(result[0] - 0) < 0.00001);
        assert(fabs(result[1] - 0) < 0.00001);
        assert(fabs(result[2] - 0) < 0.00001);
        assert(fabs(result[3] - 0.707107) < 0.00001);
        assert(fabs(result[4] - 0.707107) < 0.00001);
        assert(fabs(result[5] - 0) < 0.00001);
        assert(fabs(result[6] - 0) < 0.00001);
        assert(fabs(result[7] - 0) < 0.00001);
        
        vbap_3d_perform(vbap, 26., 39., result);
        assert(fabs(result[0] - 0.961695) < 0.00001);
        assert(fabs(result[1] - 0) < 0.00001);
        assert(fabs(result[2] - 0) < 0.00001);
        assert(fabs(result[3] - 0.266355) < 0.00001);
        assert(fabs(result[4] - 0) < 0.00001);
        assert(fabs(result[5] - 0) < 0.00001);
        assert(fabs(result[6] - 0) < 0.00001);
        assert(fabs(result[7] - 0.0647837) < 0.00001);
        
        vbap_3d_perform(vbap, -37., -23., result);
        assert(fabs(result[0] - 0.157329) < 0.00001);
        assert(fabs(result[1] - 0) < 0.00001);
        assert(fabs(result[2] - 0) < 0.00001);
        assert(fabs(result[3] - 0.142205) < 0.00001);
        assert(fabs(result[4] - 0) < 0.00001);
        assert(fabs(result[5] - 0) < 0.00001);
        assert(fabs(result[6] - 0) < 0.00001);
        assert(fabs(result[7] - 0.977254) < 0.00001);
        
        vbap_3d_perform(vbap, 156., 12., result);
        assert(fabs(result[0] - 0) < 0.00001);
        assert(fabs(result[1] - 0.90076) < 0.00001);
        assert(fabs(result[2] - 0) < 0.00001);
        assert(fabs(result[3] - 0) < 0.00001);
        assert(fabs(result[4] - 0) < 0.00001);
        assert(fabs(result[5] - 0.155586) < 0.00001);
        assert(fabs(result[6] - 0.405493) < 0.00001);
        assert(fabs(result[7] - 0) < 0.00001);
        
        vbap_3d_perform(vbap, 45., -35., result);
        assert(fabs(result[0] - 0.00556134) < 0.00001);
        assert(fabs(result[1] - 0) < 0.00001);
        assert(fabs(result[2] - 0) < 0.00001);
        assert(fabs(result[3] - 0) < 0.00001);
        assert(fabs(result[4] - 0.999985) < 0.00001);
        assert(fabs(result[5] - 0) < 0.00001);
        assert(fabs(result[6] - 0) < 0.00001);
        assert(fabs(result[7] - 0) < 0.00001);
        
        printf("done\n");
    }
    else
    {
        printf("preparation failed!\n");
    }
}

static void test_d_invalid(t_vbap* vbap)
{
    double angles_2d[4] = {0., 0., 90., 90.};
    printf("    invalid version... ");
    assert(vbap_2d_prepare(vbap, 2, angles_2d));
}

static void test_d(void)
{
    t_vbap* vbap = vbap_new();
    printf("double precision...\n");
    assert(vbap);
    test_d_stereo(vbap);
    test_d_square(vbap);
    test_d_triangle(vbap);
    test_d_cube(vbap);
    test_d_invalid(vbap);
    vbap_free(vbap);
    printf("done\n");
}

int main(int argc, const char * argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    printf("vbap running test:\n");
    test_f();
    test_d();
    printf("end\n");
    return 0;
}
