/*
// Copyright (c) 2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include <m_pd.h>
#include <vbap.h>

static t_class *vbap_tilde_class;
static t_symbol *vbap_tilde_sym_2d;
static t_symbol *vbap_tilde_sym_3d;

typedef struct _vbap_tilde
{
    t_object    v_obj;
    t_float     v_f;
    t_vbapf*    v_vbap;
    t_float*    v_coeffs;
} t_vbap_tilde;

static void vbap_tilde_free_pointers(t_vbap_tilde *x)
{
    if(x->v_coeffs)
    {
        freebytes(x->v_coeffs, vbapf_nls(x->v_vbap) * sizeof(t_float));
        x->v_coeffs = NULL;
    }
}

static void vbap_tilde_allocate_pointers(t_vbap_tilde *x, size_t const n)
{
    x->v_coeffs = (t_float *)getbytes(n * sizeof(t_float));
    if(!x->v_coeffs)
    {
        pd_error(x, "vbap~ can't allocate memory for the coefficients.");
    }
}

static void vbap_tilde_configure(t_vbap_tilde *x, t_symbol* s, int argc, t_atom* argv)
{
    int i; char err = 0;
    t_symbol const* mode = atom_getsymbolarg(0, argc, argv);
    vbap_tilde_free_pointers(x);
    vbap_tilde_allocate_pointers(x, (size_t)(argc-1));
    if(!x->v_coeffs)
    {
        return;
    }
    if(mode == vbap_tilde_sym_3d)
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
                pd_error(x, "vbap~ received a bad type of argument for the %s %i.", i%2 ? "elevation" : "azimuth", i-1);
            }
        }
        err = vbapf_3d_prepare(x->v_vbap, (size_t)(argc - 1) / 2, x->v_coeffs);
    }
    else if(mode == vbap_tilde_sym_2d)
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
                pd_error(x, "vbap~ received a bad type of argument for the azimuth %i.", i-1);
            }
        }
        err = vbapf_2d_prepare(x->v_vbap, (size_t)(argc-1), x->v_coeffs);
    }
    else
    {
        pd_error(x, "vbap~ expects the dimension (2d or 3d) before the list of angles.");
    }
    if(err)
    {
        pd_error(x, "vbap~ something wrong happened while preparing the vbap %i.", (int)err);
    }
}

static void *vbap_tilde_new(t_symbol* s, int argc, t_atom* argv)
{
    t_vbap_tilde *x = (t_vbap_tilde *)pd_new(vbap_tilde_class);
    if(x)
    {
        x->v_coeffs     = NULL;
        x->v_vbap       = vbapf_new();
        if(x->v_vbap)
        {
            vbap_tilde_configure(x, &s_list, argc-1, argv+1);
        }
        else
        {
            pd_error(x, "vbap.coeff can't allocate its internal structure.");
            pd_free((t_pd *)x);
            return NULL;
        }
        
        signalinlet_new((t_object *)x, 0);
        signalinlet_new((t_object *)x, 0);
        outlet_new((t_object *)x, &s_signal);
    }
    
    return (x);
}

void vbap_tilde_free(t_vbap_tilde *x)
{
    vbap_tilde_free_pointers(x);
    vbapf_free(x->v_vbap);
}

t_int *vbap_tilde_perform(t_int *w)
{
    return NULL;
}

void vbap_tilde_dsp(t_vbap_tilde *x, t_signal **sp)
{
    
}

EXTERN void vbap_tilde_tilde_setup(void)
{
    t_class* c = class_new(gensym("vbap~"), (t_newmethod)vbap_tilde_new, (t_method)vbap_tilde_free,
                           sizeof(t_vbap_tilde), CLASS_DEFAULT, A_GIMME, 0);
    if(c)
    {
        class_addmethod(c, (t_method)vbap_tilde_dsp, gensym("dsp"), A_CANT);
        class_addmethod(c, (t_method)vbap_tilde_configure, gensym("configure"), A_GIMME, 0);
        CLASS_MAINSIGNALIN(c, t_vbap_tilde, v_f);
    }
    vbap_tilde_class = c;
    vbap_tilde_sym_2d = gensym("2d");
    vbap_tilde_sym_3d = gensym("3d");
    
    verbose(0, "vbap~ %s by %s", VBAP_STR_VERSION, VBAP_STR_CREDITS);
}
