/*
// Copyright (c) 2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include <m_pd.h>
#include "../../src/vbap.h"

static t_class *vbap_coeff_class;
static t_symbol *vbap_coeff_sym_2d;
static t_symbol *vbap_coeff_sym_3d;

typedef struct _vbap_coeff
{
    t_object    v_obj;
    t_vbapf*    v_vbap;
    t_float*    v_coeffs;
    t_atom*     v_list;
    t_outlet*   v_out;
} t_vbap_coeff;

void vbap_coeff_float(t_vbap_coeff *x, t_float f)
{
    int i, n = (int)vbapf_nls(x->v_vbap);
    vbapf_2d_perform(x->v_vbap, f, x->v_coeffs);
    for(i = 0; i < n; ++i)
    {
        x->v_list[i].a_w.w_float = x->v_coeffs[i];
    }
    outlet_list(x->v_out, &s_list, n, x->v_list);
}

void vbap_coeff_set(t_vbap_coeff *x, t_symbol* s, int argc, t_atom* argv)
{
    int i; char err;
    t_symbol const* mode = atom_getsymbolarg(0, argc, argv);
    argc--;
    argv++;
    if(mode == vbap_coeff_sym_3d)
    {
        
    }
    else if(mode == vbap_coeff_sym_2d)
    {
        if(x->v_coeffs)
        {
            freebytes(x->v_coeffs, vbapf_nls(x->v_vbap) * sizeof(t_float));
            x->v_coeffs = NULL;
        }
        if(x->v_list)
        {
            freebytes(x->v_list, vbapf_nls(x->v_vbap) * sizeof(t_atom));
            x->v_list = NULL;
        }
        x->v_coeffs = (t_float *)getbytes(argc * sizeof(t_float));
        if(x->v_coeffs)
        {
            x->v_list = (t_atom *)getbytes(argc * sizeof(t_atom));
            if(x->v_list)
            {
                for(i = 0; i < argc; i++)
                {
                    if(argv[i].a_type == A_FLOAT)
                    {
                        x->v_coeffs[i] = argv[i].a_w.w_float;
                    }
                    else
                    {
                        x->v_coeffs[i] = 0.f;
                        pd_error(x, "vbap.coeff received a bad type of argument for the angle %i.", i);
                    }
                    x->v_list[i].a_type = A_FLOAT;
                    x->v_list[i].a_w.w_float = 0.f;
                }
                err = vbapf_2d_prepare(x->v_vbap, argc, x->v_coeffs);
                if(err)
                {
                    pd_error(x, "vbap.coeff something wrong happened while preparing the vbap %i.", (int)err);
                }
            }
            else
            {
                freebytes(x->v_coeffs, argc * sizeof(t_float));
                pd_error(x, "vbap.coeff can't allocate memory for the list.");
            }
        }
        else
        {
            pd_error(x, "vbap.coeff can't allocate memory for the coefficients.");
        }
    }
    else
    {
        pd_error(x, "vbap.coeff expects the dimension (2d or 3d) before the list of angles.");
    }
}

static void *vbap_coeff_new(t_symbol* s, int argc, t_atom* argv)
{
    t_vbap_coeff *x = (t_vbap_coeff *)pd_new(vbap_coeff_class);
    if(x)
    {
        x->v_coeffs = NULL;
        x->v_list   = NULL;
        x->v_vbap   = vbapf_new();
        if(x->v_vbap)
        {
            x->v_out = outlet_new((t_object *)x, &s_list);
            vbap_coeff_set(x, &s_list, argc, argv);
        }
        else
        {
            pd_error(x, "vbap.coeff can't allocate its internal structure.");
            pd_free((t_pd *)x);
            return NULL;
        }
    }
    return (x);
}

void vbap_coeff_free(t_vbap_coeff *x)
{
    if(x->v_coeffs)
    {
        freebytes(x->v_coeffs, vbapf_nls(x->v_vbap) * sizeof(t_float));
    }
    if(x->v_list)
    {
        freebytes(x->v_list, vbapf_nls(x->v_vbap) * sizeof(t_atom));
    }
    vbapf_free(x->v_vbap);
}

EXTERN void setup_vbap0x2ecoeff(void)
{
    t_class* c = class_new(gensym("vbap.coeff"), (t_newmethod)vbap_coeff_new, (t_method)vbap_coeff_free,
                           sizeof(t_vbap_coeff), CLASS_DEFAULT, A_GIMME, 0);
    if(c)
    {
        class_addfloat(c, (t_method)vbap_coeff_float);
        class_addmethod(c, (t_method)vbap_coeff_set, gensym("set"), A_GIMME, 0);
    }
    vbap_coeff_class = c;
    vbap_coeff_sym_2d = gensym("2d");
    vbap_coeff_sym_3d = gensym("3d");
}
