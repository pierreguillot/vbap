/*
// Copyright (c) 2016-2017 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "vbap.h"

#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <stdint.h>

#define VBAP_DEG_TO_RAD_F 0.01745329238474369049072266f

struct _vbapf
{
    size_t  v_nls;
    size_t  v_n;
    float*  v_matrices;
    size_t* v_indices;
};

static size_t vbap_factorial(size_t value)
{
    size_t result = value;
    if(value == 0)
        return 1;
    while(--value > 0)
        result *= value;
    return result;
}





t_vbapf* vbapf_new(void)
{
    t_vbapf* vbap = (t_vbapf *)malloc(sizeof(t_vbapf));
    if(vbap)
    {
        vbap->v_nls         = 0;
        vbap->v_n           = 0;
        vbap->v_matrices    = NULL;
        vbap->v_indices     = NULL;
        return vbap;
    }
    return NULL;
}

void vbapf_free(t_vbapf* vbap)
{
    if(vbap)
    {
        if(vbap->v_n && vbap->v_matrices)
        {
            free(vbap->v_matrices);
        }
        if(vbap->v_n && vbap->v_indices)
        {
            free(vbap->v_indices);
        }
        vbap->v_n           = 0;
        vbap->v_matrices    = NULL;
        vbap->v_indices     = NULL;
        free(vbap);
    }
}

size_t vbapf_nls(t_vbapf const* vbap)
{
    return vbap->v_nls;
}

char vbapf_2d_prepare(t_vbapf* vbap, size_t const nangles, float const * angles)
{
    char valid, err = 0;
    float x1, y1, x2, y2, xc, yc, xr, yr, dc, deta, ra;
    size_t i = 0, j = 0, k = 0;
    size_t const max = vbap_factorial(nangles);
    
    if(vbap->v_n && vbap->v_matrices)
    {
        free(vbap->v_matrices);
    }
    if(vbap->v_n && vbap->v_indices)
    {
        free(vbap->v_indices);
    }
    
    vbap->v_nls      = nangles;
    vbap->v_n        = 0;
    vbap->v_matrices = (float *)malloc(max * 4 * sizeof(float));
    vbap->v_indices  = (size_t *)malloc(max * 2 * sizeof(size_t));
    
    if(!vbap->v_matrices || !vbap->v_indices)
    {
        if(vbap->v_indices)
        {
            free(vbap->v_indices);
            vbap->v_indices = NULL;
        }
        if(vbap->v_matrices)
        {
            free(vbap->v_matrices);
            vbap->v_matrices = NULL;
        }
        return -1;
    }
    
    for(i = 0; i < nangles; ++i)
    {
        ra = angles[i] * VBAP_DEG_TO_RAD_F;
        x1 = -sinf(ra);
        y1 = cosf(ra);
        for(j = i+1; j < nangles; ++j)
        {
            ra = angles[j] * VBAP_DEG_TO_RAD_F;
            x2 = -sinf(ra);
            y2 = cosf(ra);
            xc = (x1 + x2) / 2.f;
            yc = (y1 + y2) / 2.f;
            
            xr = xc - x2;
            yr = yc - y2;
            dc = sqrtf(xr * xr + yr * yr);
            valid = (char)(fabsf(xc) >= FLT_EPSILON || fabsf(yc) >= FLT_EPSILON);
            for(k = 0; k < nangles && valid; ++k)
            {
                if(k != i && k != j)
                {
                    ra = angles[k] * VBAP_DEG_TO_RAD_F;
                    xr = xc + sinf(ra);
                    yr = yc - cosf(ra);
                    valid = (char)(sqrtf(xr * xr + yr * yr) > dc);
                }
            }
            if(valid)
            {
                /* Transposed of the inverse of the matrix */
                deta = (x1 * y2 - x2 * y1);
                if(fabsf(deta) < FLT_EPSILON)
                {
                    deta = 1.f;
                    err++;
                }
                vbap->v_indices[vbap->v_n*2]   = i;
                vbap->v_indices[vbap->v_n*2+1] = j;
                
                vbap->v_matrices[vbap->v_n*4]   = y2 / deta;
                vbap->v_matrices[vbap->v_n*4+1] = -x2 / deta;
                vbap->v_matrices[vbap->v_n*4+2] = -y1 / deta;
                vbap->v_matrices[vbap->v_n*4+3] = x1 / deta;
                vbap->v_n++;
            }
        }
    }
    
    return err;
}

void vbapf_2d_perform(t_vbapf const* vbap, float const angle, float * coefficients)
{
    size_t i = 0, index = SIZE_MAX;
    float r1, r2, s1 = 0.f, s2 = 0.f, powr, ref = 0.f;
    float const ra = angle * VBAP_DEG_TO_RAD_F;
    float const x = -sinf(ra);
    float const y = cosf(ra);
    
    for(i = 0; i < vbap->v_n; ++i)
    {
        r1 = vbap->v_matrices[i*4] * x + vbap->v_matrices[i*4+1] * y;
        r2 = vbap->v_matrices[i*4+2] * x + vbap->v_matrices[i*4+3] * y;
        
        if(r1 >= 0.f && r2 >= 0.f)
        {
            powr = sqrtf(r1 * r1 + r2 * r2);
            if(powr > ref)
            {
                ref = powr;
                index = i;
                s1 = r1;
                s2 = r2;
            }
        }
    }
    for(i = 0; i < vbap->v_nls; ++i)
    {
        coefficients[i] = 0.f;
    }
    
    if(ref != 0.f)
    {
        coefficients[vbap->v_indices[index*2]]   = s1 / ref;
        coefficients[vbap->v_indices[index*2+1]] = s2 / ref;
    }
}



