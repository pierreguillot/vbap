/*
// Copyright (c) 2016-2017 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "vbap.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <stdint.h>

static size_t vbap_factorial(size_t value)
{
    size_t result = value;
    if(value == 0)
        return 1;
    while(--value > 0)
        result *= value;
    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

#define VBAP_DEG_TO_RAD_F 0.01745329238474369049072266f

struct _vbapf
{
    size_t  v_nls;
    size_t  v_n;
    float*  v_matrices;
    size_t* v_indices;
    size_t  v_dim;
};


t_vbapf* vbapf_new(void)
{
    t_vbapf* vbap = (t_vbapf *)malloc(sizeof(t_vbapf));
    if(vbap)
    {
        vbap->v_nls         = 0;
        vbap->v_n           = 0;
        vbap->v_matrices    = NULL;
        vbap->v_indices     = NULL;
        vbap->v_dim        = 0;
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

unsigned char vbapf_dimension(t_vbapf const* vbap)
{
    return (unsigned char)(vbap->v_dim);
}

char vbapf_2d_prepare(t_vbapf* vbap, size_t const nls, float const * angles)
{
    char valid, err = 0;
    float x1, y1, x2, y2, xc, yc, xr, yr, dc, deta, ra;
    size_t i = 0, j = 0, k = 0;
    size_t const max = vbap_factorial(nls);
    
    vbap->v_dim = 2;
    if(vbap->v_n && vbap->v_matrices)
    {
        free(vbap->v_matrices);
    }
    if(vbap->v_n && vbap->v_indices)
    {
        free(vbap->v_indices);
    }
    
    vbap->v_nls      = nls;
    vbap->v_n        = 0;
    vbap->v_matrices = (float *)malloc(max * 4 * sizeof(float));
    vbap->v_indices  = (size_t *)malloc(max * 2 * sizeof(size_t));
    
    if(!vbap->v_matrices || !vbap->v_indices)
    {
// LCOV_EXCL_START
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
// LCOV_EXCL_STOP
    }
    
    for(i = 0; i < nls - 1; ++i)
    {
        for(j = i+1; j < nls; ++j)
        {
            ra = angles[i] * VBAP_DEG_TO_RAD_F;
            x1 = -sinf(ra);
            y1 = cosf(ra);
            ra = angles[j] * VBAP_DEG_TO_RAD_F;
            x2 = -sinf(ra);
            y2 = cosf(ra);
            xc = (x1 + x2) / 2.f;
            yc = (y1 + y2) / 2.f;
            
            xr = xc - x2;
            yr = yc - y2;
            dc = sqrtf(xr * xr + yr * yr);
            valid = (char)(fabsf(xc) >= FLT_EPSILON || fabsf(yc) >= FLT_EPSILON);
            for(k = 0; k < nls && valid; ++k)
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

void vbapf_2d_perform(t_vbapf const* vbap, float const azimuth, float * gains)
{
    size_t i = 0, index = SIZE_MAX;
    float r1, r2, s1 = 0.f, s2 = 0.f, powr, ref = 0.f;
    float const ra = azimuth * VBAP_DEG_TO_RAD_F;
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
        gains[i] = 0.f;
    }
    
    if(ref != 0.f)
    {
        gains[vbap->v_indices[index*2]]   = s1 / ref;
        gains[vbap->v_indices[index*2+1]] = s2 / ref;
    }
}

static void vbapf_circumcircle(float const x1, float const y1, float const z1,
                               float const x2, float const y2, float const z2,
                               float const x3, float const y3, float const z3,
                               float* xr, float* yr, float* zr, float* l)
{
    const double mx1 = (double)x2 - (double)x1, my1 = (double)y2 - (double)y1, mz1 = (double)z2 - (double)z1;
    const double mx2 = (double)x3 - (double)x1, my2 = (double)y3 - (double)y1, mz2 = (double)z3 - (double)z1;
    const double crx1 = my1 * mz2 - mz1 * my2, cry1 = mz1 * mx2 - mx1 * mz2, crz1 = mx1 * my2 - my1 * mx2;
    
    const double l1 = pow(sqrt(crx1*crx1+cry1*cry1+crz1*crz1), 2.) * 2.;
    const double l2 = pow(sqrt(mx1*mx1+my1*my1+mz1*mz1), 2.);
    const double l3 = pow(sqrt(mx2*mx2+my2*my2+mz2*mz2), 2.);
    
    const double crx2 = (cry1 * mz1 - crz1 * my1) * l3, cry2 = (crz1 * mx1 - crx1 * mz1) * l3, crz2 = (crx1 * my1 - cry1 * mx1) * l3;
    const double crx3 = (my2 * crz1 - mz2 * cry1) * l2, cry3 = (mz2 * crx1 - mx2 * crz1) * l2, crz3 = (mx2 * cry1 - my2 * crx1) * l2;

    *xr = (float)((crx2 + crx3) / l1); *yr = (float)((cry2 + cry3) / l1); *zr = (float)((crz2 + crz3) / l1);
    *l = sqrtf(*xr * *xr + *yr * *yr + *zr * *zr);
    *xr += x1; *yr += y1; *zr += z1;
}

char vbapf_3d_prepare(t_vbapf* vbap, size_t const nls, float const * angles)
{
    char valid, err = 0;
    float x1, y1, z1, x2, y2, z2, x3, y3, z3, xc, yc, zc, xr, yr, zr, dc, deta, ra, re;
    size_t i = 0, j = 0, k = 0, l = 0;
    size_t const max = vbap_factorial(nls);
    
    vbap->v_dim     = 3;
    if(vbap->v_n && vbap->v_matrices)
    {
        free(vbap->v_matrices);
    }
    if(vbap->v_n && vbap->v_indices)
    {
        free(vbap->v_indices);
    }
    
    vbap->v_nls      = nls;
    vbap->v_n        = 0;
    vbap->v_matrices = (float *)malloc(max * 9 * sizeof(float));
    vbap->v_indices  = (size_t *)malloc(max * 3 * sizeof(size_t));
    
    if(!vbap->v_matrices || !vbap->v_indices)
    {
        // LCOV_EXCL_START
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
        // LCOV_EXCL_STOP
    }
    
    for(i = 0; i < nls - 2; ++i)
    {
        for(j = i+1; j < nls - 1; ++j)
        {
            for(k = j+1; k < nls; ++k)
            {
                ra = angles[i*2]   * VBAP_DEG_TO_RAD_F;
                re = angles[i*2+1] * VBAP_DEG_TO_RAD_F;
                x1 = -sinf(ra) * cosf(re);
                y1 = cosf(ra)  * cosf(re);
                z1 = sinf(re);
                ra = angles[j*2]   * VBAP_DEG_TO_RAD_F;
                re = angles[j*2+1] * VBAP_DEG_TO_RAD_F;
                x2 = -sinf(ra) * cosf(re);
                y2 = cosf(ra)  * cosf(re);
                z2 = sinf(re);
                ra = angles[k*2]   * VBAP_DEG_TO_RAD_F;
                re = angles[k*2+1] * VBAP_DEG_TO_RAD_F;
                x3 = -sinf(ra) * cosf(re);
                y3 = cosf(ra)  * cosf(re);
                z3 = sinf(re);
                
                vbapf_circumcircle(x3, y3, z3, x1, y1, z1, x2, y2, z2, &xr, &yr, &zr, &dc);
                valid = (char)(sqrtf(xr * xr + yr * yr + zr * zr) > FLT_EPSILON);
                
                for(l = 0; l < nls && valid; ++l)
                {
                    if(l != i && l != j && l != k)
                    {
                        ra = angles[l*2]   * VBAP_DEG_TO_RAD_F;
                        re = angles[l*2+1] * VBAP_DEG_TO_RAD_F;
                        xc = -sinf(ra) * cosf(re) - xr;
                        yc = cosf(ra) * cosf(re) - yr;
                        zc = sinf(re) - zr;
                        
                        valid = (char)((sqrtf(xc * xc + yc * yc + zc * zc) + FLT_EPSILON) > dc);
                    }
                }
                if(valid)
                {
                    deta = x1 * y2 * z3 + y1 * z2 * x3 + z1 * x2 * y3 - z1 * y2 * x3 - x1 * z2 * y3 - y1 * x2 * z3;
                    if(fabsf(deta) < FLT_EPSILON)
                    {
                        deta = 1.f;
                        err++;
                    }
                    vbap->v_indices[vbap->v_n*3]   = i;
                    vbap->v_indices[vbap->v_n*3+1] = j;
                    vbap->v_indices[vbap->v_n*3+2] = k;
                    
                    vbap->v_matrices[vbap->v_n*9]   = (y2 * z3 - z2 * y3) / deta;
                    vbap->v_matrices[vbap->v_n*9+1] = (z1 * y3 - y1 * z3) / deta;
                    vbap->v_matrices[vbap->v_n*9+2] = (y1 * z2 - z1 * y2) / deta;
                    vbap->v_matrices[vbap->v_n*9+3] = (z2 * x3 - x2 * z3) / deta;
                    vbap->v_matrices[vbap->v_n*9+4] = (x1 * z3 - z1 * x3) / deta;
                    vbap->v_matrices[vbap->v_n*9+5] = (z1 * x2 - x1 * z2) / deta;
                    vbap->v_matrices[vbap->v_n*9+6] = (x2 * y3 - y2 * x3) / deta;
                    vbap->v_matrices[vbap->v_n*9+7] = (y1 * x3 - x1 * y3) / deta;
                    vbap->v_matrices[vbap->v_n*9+8] = (x1 * y2 - y1 * x2) / deta;
                    vbap->v_n++;
                }
            }
        }
    }
    
    return err;
}

void vbapf_3d_perform(t_vbapf const* vbap, float const azimuth, float const elevation, float * gains)
{
    size_t i = 0, index = SIZE_MAX;
    float r1, r2, r3, s1 = 0.f, s2 = 0.f, s3 = 0.f, powr, ref = 0.f;
    float const ra = azimuth * VBAP_DEG_TO_RAD_F;
    float const re = elevation * VBAP_DEG_TO_RAD_F;
    float const x = -sinf(ra) * cosf(re);
    float const y = cosf(ra) * cosf(re);
    float const z = sinf(re);
    
    for(i = 0; i < vbap->v_n; ++i)
    {
        r1 = vbap->v_matrices[i*9] * x + vbap->v_matrices[i*9+3] * y + vbap->v_matrices[i*9+6] * z;
        r2 = vbap->v_matrices[i*9+1] * x + vbap->v_matrices[i*9+4] * y + vbap->v_matrices[i*9+7] * z;
        r3 = vbap->v_matrices[i*9+2] * x + vbap->v_matrices[i*9+5] * y + vbap->v_matrices[i*9+8] * z;
        if(r1 >= 0.f && r2 >= 0.f && r3 >= 0.f)
        {
            powr = sqrtf(r1 * r1 + r2 * r2 + r3 * r3);
            if(powr > ref)
            {
                ref = powr;
                index = i;
                s1 = r1;
                s2 = r2;
                s3 = r3;
            }
        }
    }
    for(i = 0; i < vbap->v_nls; ++i)
    {
        gains[i] = 0.f;
    }
    
    if(ref != 0.f)
    {
        gains[vbap->v_indices[index*3]]   = s1 / ref;
        gains[vbap->v_indices[index*3+1]] = s2 / ref;
        gains[vbap->v_indices[index*3+2]] = s3 / ref;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

#define VBAP_DEG_TO_RAD_D 0.01745329238474369049072266

struct _vbap
{
    size_t  v_nls;
    size_t  v_n;
    double* v_matrices;
    size_t* v_indices;
    size_t  v_dim;
};


t_vbap* vbap_new(void)
{
    t_vbap* vbap = (t_vbap *)malloc(sizeof(t_vbap));
    if(vbap)
    {
        vbap->v_nls         = 0;
        vbap->v_n           = 0;
        vbap->v_matrices    = NULL;
        vbap->v_indices     = NULL;
        vbap->v_dim        = 0;
        return vbap;
    }
    return NULL;
}

void vbap_free(t_vbap* vbap)
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

size_t vbap_nls(t_vbap const* vbap)
{
    return vbap->v_nls;
}

unsigned char vbap_dimension(t_vbap const* vbap)
{
    return (unsigned char)(vbap->v_dim);
}

char vbap_2d_prepare(t_vbap* vbap, size_t const nls, double const * angles)
{
    char valid, err = 0;
    double x1, y1, x2, y2, xc, yc, xr, yr, dc, deta, ra;
    size_t i = 0, j = 0, k = 0;
    size_t const max = vbap_factorial(nls);
    
    vbap->v_dim = 2;
    if(vbap->v_n && vbap->v_matrices)
    {
        free(vbap->v_matrices);
    }
    if(vbap->v_n && vbap->v_indices)
    {
        free(vbap->v_indices);
    }
    
    vbap->v_nls      = nls;
    vbap->v_n        = 0;
    vbap->v_matrices = (double *)malloc(max * 4 * sizeof(double));
    vbap->v_indices  = (size_t *)malloc(max * 2 * sizeof(size_t));
    
    if(!vbap->v_matrices || !vbap->v_indices)
    {
        // LCOV_EXCL_START
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
        // LCOV_EXCL_STOP
    }
    
    for(i = 0; i < nls - 1; ++i)
    {
        for(j = i+1; j < nls; ++j)
        {
            ra = angles[i] * VBAP_DEG_TO_RAD_D;
            x1 = -sin(ra);
            y1 = cos(ra);
            ra = angles[j] * VBAP_DEG_TO_RAD_D;
            x2 = -sin(ra);
            y2 = cos(ra);
            xc = (x1 + x2) / 2.;
            yc = (y1 + y2) / 2.;
            
            xr = xc - x2;
            yr = yc - y2;
            dc = sqrt(xr * xr + yr * yr);
            valid = (char)(fabs(xc) >= DBL_EPSILON || fabs(yc) >= DBL_EPSILON);
            for(k = 0; k < nls && valid; ++k)
            {
                if(k != i && k != j)
                {
                    ra = angles[k] * VBAP_DEG_TO_RAD_D;
                    xr = xc + sin(ra);
                    yr = yc - cos(ra);
                    valid = (char)(sqrt(xr * xr + yr * yr) > dc);
                }
            }
            if(valid)
            {
                /* Transposed of the inverse of the matrix */
                deta = (x1 * y2 - x2 * y1);
                if(fabs(deta) < DBL_EPSILON)
                {
                    deta = 1.;
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

void vbap_2d_perform(t_vbap const* vbap, double const azimuth, double * gains)
{
    size_t i = 0, index = SIZE_MAX;
    double r1, r2, s1 = 0., s2 = 0., powr, ref = 0.;
    double const ra = azimuth * VBAP_DEG_TO_RAD_D;
    double const x = -sin(ra);
    double const y = cos(ra);
    
    for(i = 0; i < vbap->v_n; ++i)
    {
        r1 = vbap->v_matrices[i*4] * x + vbap->v_matrices[i*4+1] * y;
        r2 = vbap->v_matrices[i*4+2] * x + vbap->v_matrices[i*4+3] * y;
        
        if(r1 >= 0. && r2 >= 0.)
        {
            powr = sqrt(r1 * r1 + r2 * r2);
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
        gains[i] = 0.;
    }
    
    if(ref != 0.)
    {
        gains[vbap->v_indices[index*2]]   = s1 / ref;
        gains[vbap->v_indices[index*2+1]] = s2 / ref;
    }
}

static void vbap_circumcircle(double const x1, double const y1, double const z1,
                               double const x2, double const y2, double const z2,
                               double const x3, double const y3, double const z3,
                               double* xr, double* yr, double* zr, double* l)
{
    const double mx1 = (double)x2 - (double)x1, my1 = (double)y2 - (double)y1, mz1 = (double)z2 - (double)z1;
    const double mx2 = (double)x3 - (double)x1, my2 = (double)y3 - (double)y1, mz2 = (double)z3 - (double)z1;
    const double crx1 = my1 * mz2 - mz1 * my2, cry1 = mz1 * mx2 - mx1 * mz2, crz1 = mx1 * my2 - my1 * mx2;
    
    const double l1 = pow(sqrt(crx1*crx1+cry1*cry1+crz1*crz1), 2.) * 2.;
    const double l2 = pow(sqrt(mx1*mx1+my1*my1+mz1*mz1), 2.);
    const double l3 = pow(sqrt(mx2*mx2+my2*my2+mz2*mz2), 2.);
    
    const double crx2 = (cry1 * mz1 - crz1 * my1) * l3, cry2 = (crz1 * mx1 - crx1 * mz1) * l3, crz2 = (crx1 * my1 - cry1 * mx1) * l3;
    const double crx3 = (my2 * crz1 - mz2 * cry1) * l2, cry3 = (mz2 * crx1 - mx2 * crz1) * l2, crz3 = (mx2 * cry1 - my2 * crx1) * l2;
    
    *xr = (double)((crx2 + crx3) / l1); *yr = (double)((cry2 + cry3) / l1); *zr = (double)((crz2 + crz3) / l1);
    *l = sqrt(*xr * *xr + *yr * *yr + *zr * *zr);
    *xr += x1; *yr += y1; *zr += z1;
}

char vbap_3d_prepare(t_vbap* vbap, size_t const nls, double const * angles)
{
    char valid, err = 0;
    double x1, y1, z1, x2, y2, z2, x3, y3, z3, xc, yc, zc, xr, yr, zr, dc, deta, ra, re;
    size_t i = 0, j = 0, k = 0, l = 0;
    size_t const max = vbap_factorial(nls);
    
    vbap->v_dim     = 3;
    if(vbap->v_n && vbap->v_matrices)
    {
        free(vbap->v_matrices);
    }
    if(vbap->v_n && vbap->v_indices)
    {
        free(vbap->v_indices);
    }
    
    vbap->v_nls      = nls;
    vbap->v_n        = 0;
    vbap->v_matrices = (double *)malloc(max * 9 * sizeof(double));
    vbap->v_indices  = (size_t *)malloc(max * 3 * sizeof(size_t));
    
    if(!vbap->v_matrices || !vbap->v_indices)
    {
        // LCOV_EXCL_START
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
        // LCOV_EXCL_STOP
    }
    
    for(i = 0; i < nls - 2; ++i)
    {
        for(j = i+1; j < nls - 1; ++j)
        {
            for(k = j+1; k < nls; ++k)
            {
                ra = angles[i*2]   * VBAP_DEG_TO_RAD_D;
                re = angles[i*2+1] * VBAP_DEG_TO_RAD_D;
                x1 = -sin(ra) * cos(re);
                y1 = cos(ra)  * cos(re);
                z1 = sin(re);
                ra = angles[j*2]   * VBAP_DEG_TO_RAD_D;
                re = angles[j*2+1] * VBAP_DEG_TO_RAD_D;
                x2 = -sin(ra) * cos(re);
                y2 = cos(ra)  * cos(re);
                z2 = sin(re);
                ra = angles[k*2]   * VBAP_DEG_TO_RAD_D;
                re = angles[k*2+1] * VBAP_DEG_TO_RAD_D;
                x3 = -sin(ra) * cos(re);
                y3 = cos(ra)  * cos(re);
                z3 = sin(re);
                
                vbap_circumcircle(x3, y3, z3, x1, y1, z1, x2, y2, z2, &xr, &yr, &zr, &dc);
                valid = (char)(sqrtf(xr * xr + yr * yr + zr * zr) > FLT_EPSILON);
                
                for(l = 0; l < nls && valid; ++l)
                {
                    if(l != i && l != j && l != k)
                    {
                        ra = angles[l*2]   * VBAP_DEG_TO_RAD_D;
                        re = angles[l*2+1] * VBAP_DEG_TO_RAD_D;
                        xc = -sin(ra) * cos(re) - xr;
                        yc = cos(ra) * cos(re) - yr;
                        zc = sin(re) - zr;
                        
                        valid = (char)((sqrt(xc * xc + yc * yc + zc * zc) + DBL_EPSILON) > dc);
                    }
                }
                if(valid)
                {
                    deta = x1 * y2 * z3 + y1 * z2 * x3 + z1 * x2 * y3 - z1 * y2 * x3 - x1 * z2 * y3 - y1 * x2 * z3;
                    if(fabs(deta) < DBL_EPSILON)
                    {
                        deta = 1.;
                        err++;
                    }
                    vbap->v_indices[vbap->v_n*3]   = i;
                    vbap->v_indices[vbap->v_n*3+1] = j;
                    vbap->v_indices[vbap->v_n*3+2] = k;
                    
                    vbap->v_matrices[vbap->v_n*9]   = (y2 * z3 - z2 * y3) / deta;
                    vbap->v_matrices[vbap->v_n*9+1] = (z1 * y3 - y1 * z3) / deta;
                    vbap->v_matrices[vbap->v_n*9+2] = (y1 * z2 - z1 * y2) / deta;
                    vbap->v_matrices[vbap->v_n*9+3] = (z2 * x3 - x2 * z3) / deta;
                    vbap->v_matrices[vbap->v_n*9+4] = (x1 * z3 - z1 * x3) / deta;
                    vbap->v_matrices[vbap->v_n*9+5] = (z1 * x2 - x1 * z2) / deta;
                    vbap->v_matrices[vbap->v_n*9+6] = (x2 * y3 - y2 * x3) / deta;
                    vbap->v_matrices[vbap->v_n*9+7] = (y1 * x3 - x1 * y3) / deta;
                    vbap->v_matrices[vbap->v_n*9+8] = (x1 * y2 - y1 * x2) / deta;
                    vbap->v_n++;
                }
            }
        }
    }
    
    return err;
}

void vbap_3d_perform(t_vbap const* vbap, double const azimuth, double const elevation, double * gains)
{
    size_t i = 0, index = SIZE_MAX;
    double r1, r2, r3, s1 = 0., s2 = 0., s3 = 0., powr, ref = 0.;
    double const ra = azimuth * VBAP_DEG_TO_RAD_D;
    double const re = elevation * VBAP_DEG_TO_RAD_D;
    double const x = -sin(ra) * cos(re);
    double const y = cos(ra) * cos(re);
    double const z = sin(re);
    
    for(i = 0; i < vbap->v_n; ++i)
    {
        r1 = vbap->v_matrices[i*9] * x + vbap->v_matrices[i*9+3] * y + vbap->v_matrices[i*9+6] * z;
        r2 = vbap->v_matrices[i*9+1] * x + vbap->v_matrices[i*9+4] * y + vbap->v_matrices[i*9+7] * z;
        r3 = vbap->v_matrices[i*9+2] * x + vbap->v_matrices[i*9+5] * y + vbap->v_matrices[i*9+8] * z;
        if(r1 >= 0. && r2 >= 0. && r3 >= 0.)
        {
            powr = sqrt(r1 * r1 + r2 * r2 + r3 * r3);
            if(powr > ref)
            {
                ref = powr;
                index = i;
                s1 = r1;
                s2 = r2;
                s3 = r3;
            }
        }
    }
    for(i = 0; i < vbap->v_nls; ++i)
    {
        gains[i] = 0.;
    }
    
    if(ref != 0.)
    {
        gains[vbap->v_indices[index*3]]   = s1 / ref;
        gains[vbap->v_indices[index*3+1]] = s2 / ref;
        gains[vbap->v_indices[index*3+2]] = s3 / ref;
    }
}


