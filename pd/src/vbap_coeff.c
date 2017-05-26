/*
// Copyright (c) 2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include <m_pd.h>
#include <vbap.h>

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

static void vbap_coeff_bang(t_vbap_coeff *x)
{
    outlet_list(x->v_out, &s_list, (int)vbapf_nls(x->v_vbap), x->v_list);
}

static void vbap_coeff_set(t_vbap_coeff *x, t_floatarg azimuth, t_floatarg elevation)
{
    size_t i, n = vbapf_nls(x->v_vbap);
    if(vbapf_dimension(x->v_vbap) == 2)
    {
        vbapf_2d_perform(x->v_vbap, azimuth, x->v_coeffs);
    }
    else
    {
        vbapf_3d_perform(x->v_vbap, azimuth, elevation, x->v_coeffs);
    }
    
    for(i = 0; i < n; ++i)
    {
        x->v_list[i].a_w.w_float = x->v_coeffs[i];
    }
}

static void vbap_coeff_float(t_vbap_coeff *x, t_floatarg azimuth)
{
    vbap_coeff_set(x, azimuth, 0.f);
    vbap_coeff_bang(x);
}

static void vbap_coeff_list(t_vbap_coeff *x, t_symbol* s, int argc, t_atom* argv)
{
    vbap_coeff_set(x, atom_getfloatarg(0, argc, argv), atom_getfloatarg(1, argc, argv));
    vbap_coeff_bang(x);
}

static void vbap_coeff_free_pointers(t_vbap_coeff *x)
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
}

static void vbap_coeff_allocate_pointers(t_vbap_coeff *x, size_t const n)
{
    x->v_coeffs = (t_float *)getbytes(n * sizeof(t_float));
    if(!x->v_coeffs)
    {
        pd_error(x, "vbap.coeff can't allocate memory for the coefficients.");
    }
    x->v_list = (t_atom *)getbytes(n * sizeof(t_atom));
    if(!x->v_list)
    {
        freebytes(x->v_coeffs, n * sizeof(t_float));
        x->v_coeffs = NULL;
        pd_error(x, "vbap.coeff can't allocate memory for the list.");
    }
}

static void vbap_coeff_configure(t_vbap_coeff *x, t_symbol* s, int argc, t_atom* argv)
{
    int i; char err = 0;
    t_symbol const* mode = atom_getsymbolarg(0, argc, argv);
    vbap_coeff_free_pointers(x);
    vbap_coeff_allocate_pointers(x, (size_t)(argc-1));
    if(!x->v_list)
    {
        return;
    }
    if(mode == vbap_coeff_sym_3d)
    {
        for(i = 1; i < argc; i++)
        {
            if(argv[i].a_type == A_FLOAT)
            {
                x->v_coeffs[i-1] = argv[i].a_w.w_float;
            }
            else
            {
                x->v_coeffs[i-1] = 0.f;
                pd_error(x, "vbap.coeff received a bad type of argument for the %s %i.", i%2 ? "elevation" : "azimuth", i-1);
            }
            x->v_list[i-1].a_type = A_FLOAT;
            x->v_list[i-1].a_w.w_float = 0.f;
        }
        err = vbapf_3d_prepare(x->v_vbap, (size_t)(argc - 1) / 2, x->v_coeffs);
    }
    else if(mode == vbap_coeff_sym_2d)
    {
       for(i = 1; i < argc; ++i)
        {
            if(argv[i].a_type == A_FLOAT)
            {
                x->v_coeffs[i-1] = argv[i].a_w.w_float;
            }
            else
            {
                x->v_coeffs[i-1] = 0.f;
                pd_error(x, "vbap.coeff received a bad type of argument for the azimuth %i.", i-1);
            }
            x->v_list[i-1].a_type = A_FLOAT;
            x->v_list[i-1].a_w.w_float = 0.f;
        }
        err = vbapf_2d_prepare(x->v_vbap, (size_t)(argc-1), x->v_coeffs);
    }
    else
    {
        pd_error(x, "vbap.coeff expects the dimension (2d or 3d) before the list of angles.");
    }
    if(err)
    {
        pd_error(x, "vbap.coeff something wrong happened while preparing the vbap %i.", (int)err);
    }
    vbap_coeff_set(x, 0.f, 0.f);
}

static void *vbap_coeff_new(t_symbol* s, int argc, t_atom* argv)
{
    t_vbap_coeff *x = (t_vbap_coeff *)pd_new(vbap_coeff_class);
    if(x)
    {
        x->v_coeffs     = NULL;
        x->v_list       = NULL;
        x->v_vbap       = vbapf_new();
        if(x->v_vbap)
        {
            x->v_out = outlet_new((t_object *)x, &s_list);
            vbap_coeff_configure(x, &s_list, argc, argv);
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

static void vbap_coeff_free(t_vbap_coeff *x)
{
    vbap_coeff_free_pointers(x);
    vbapf_free(x->v_vbap);
}

EXTERN void setup_vbap0x2ecoeff(void)
{
    t_class* c = class_new(gensym("vbap.coeff"), (t_newmethod)vbap_coeff_new, (t_method)vbap_coeff_free,
                           sizeof(t_vbap_coeff), CLASS_DEFAULT, A_GIMME, 0);
    if(c)
    {
        class_addbang(c, (t_method)vbap_coeff_bang);
        class_addfloat(c, (t_method)vbap_coeff_float);
        class_addlist(c, (t_method)vbap_coeff_list);
        class_addmethod(c, (t_method)vbap_coeff_set, gensym("set"), A_FLOAT, A_DEFFLOAT, 0);
        class_addmethod(c, (t_method)vbap_coeff_configure, gensym("configure"), A_GIMME, 0);
    }
    vbap_coeff_class = c;
    vbap_coeff_sym_2d = gensym("2d");
    vbap_coeff_sym_3d = gensym("3d");
    
    verbose(0, "vbap.coeff %s by %s", VBAP_STR_VERSION, VBAP_STR_CREDITS);
}
