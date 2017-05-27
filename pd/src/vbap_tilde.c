/*
// Copyright (c) 2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include <m_pd.h>
#include <g_canvas.h>
#include <vbap.h>

static t_class *vbap_tilde_class;
static t_symbol *vbap_tilde_sym_2d;
static t_symbol *vbap_tilde_sym_3d;

typedef struct _vbap_outlist
{
    t_outlet*             v_out;
    struct _vbap_outlist* v_next;
} t_vbap_outlist;

typedef struct _vbap_tilde
{
    t_object    v_obj;
    t_float     v_f;
    t_vbapf*    v_vbap;
    t_float*    v_coeffs;
    size_t      v_nsamples;
    t_sample*   v_inputs[3];
    size_t      v_noutlets;
    t_sample**  v_outputs;
    
    t_inlet*    v_inlet;
    t_outlet**  v_outlets;
    t_canvas*   v_canvas;
    char        v_loaded;
    
} t_vbap_tilde;

static void vbap_tilde_redraw(t_vbap_tilde *x)
{
    if(glist_isvisible(x->v_canvas) && x->v_loaded)
    {
        gobj_vis((t_gobj *)x, x->v_canvas, 0);
        canvas_fixlinesfor(x->v_canvas, (t_text *)x);
        gobj_vis((t_gobj *)x, x->v_canvas, 1);
        
        canvas_redraw(x->v_canvas);
    }
}

static void vbap_tilde_resize_inlets(t_vbap_tilde *x)
{
    if(x->v_inlet && vbapf_dimension(x->v_vbap) == 2)
    {
        inlet_free(x->v_inlet);
        x->v_inlet = NULL;
    }
    else if(!x->v_inlet && vbapf_dimension(x->v_vbap) == 3)
    {
        x->v_inlet = signalinlet_new((t_object *)x, 0);
    }
}

static void vbap_tilde_resize_outlets(t_vbap_tilde *x)
{
    size_t const oldsize = x->v_noutlets;
    size_t const newsize = vbapf_nls(x->v_vbap);
    if(oldsize < newsize)
    {
        x->v_outlets = (t_outlet **)resizebytes(x->v_outlets, oldsize * sizeof(t_outlet *), newsize * sizeof(t_outlet *));
        x->v_outputs = (t_sample **)resizebytes(x->v_outputs, oldsize * sizeof(t_sample *), newsize * sizeof(t_sample *));

        while(x->v_noutlets < newsize)
        {
            x->v_outlets[x->v_noutlets] = outlet_new((t_object *)x, &s_signal);
            x->v_noutlets++;
        }
    }
    else if(oldsize > newsize)
    {
        while(x->v_noutlets > newsize)
        {
            outlet_free(x->v_outlets[x->v_noutlets-1]);
            x->v_noutlets--;
        }
        x->v_outlets = (t_outlet **)resizebytes(x->v_outlets, oldsize * sizeof(t_outlet *), newsize * sizeof(t_outlet *));
        x->v_outputs = (t_sample **)resizebytes(x->v_outputs, oldsize * sizeof(t_sample *), newsize * sizeof(t_sample *));
    }
}

static void vbap_tilde_free_pointers(t_vbap_tilde *x)
{
    if(x->v_coeffs)
    {
        freebytes(x->v_coeffs, vbapf_nls(x->v_vbap) * sizeof(t_float));
        x->v_coeffs = NULL;
    }
    if(x->v_outputs)
    {
        freebytes(x->v_outputs, vbapf_nls(x->v_vbap) * sizeof(t_sample *));
        x->v_outputs = NULL;
    }
}

static void vbap_tilde_allocate_pointers(t_vbap_tilde *x, size_t const n)
{
    if(n)
    {
        x->v_coeffs = (t_float *)getbytes(n * sizeof(t_float));
        if(!x->v_coeffs)
        {
            pd_error(x, "vbap~ can't allocate memory for the coefficients.");
        }
        x->v_outputs = (t_sample **)getbytes(n * sizeof(t_sample *));
        if(!x->v_outputs)
        {
            freebytes(x->v_coeffs, vbapf_nls(x->v_vbap) * sizeof(t_float));
            x->v_coeffs = NULL;
            pd_error(x, "vbap~ can't allocate memory for the outputs.");
        }
    }
}

static void vbap_tilde_configure(t_vbap_tilde *x, t_symbol* s, int argc, t_atom* argv)
{
    int i; char err = 0;
    t_symbol const* mode = atom_getsymbolarg(0, argc, argv);
    int dspstate = canvas_suspend_dsp();
    vbap_tilde_free_pointers(x);
    vbap_tilde_allocate_pointers(x, (size_t)(argc-1));
    if(!x->v_outputs)
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
    vbap_tilde_resize_inlets(x);
    vbap_tilde_resize_outlets(x);
    vbap_tilde_redraw(x);
    canvas_resume_dsp(dspstate);
}

static void *vbap_tilde_new(t_symbol* s, int argc, t_atom* argv)
{
    t_vbap_tilde *x = (t_vbap_tilde *)pd_new(vbap_tilde_class);
    if(x)
    {
        x->v_coeffs     = NULL;
        x->v_vbap       = vbapf_new();
        x->v_outputs    = NULL;
        x->v_outlets    = NULL;
        x->v_inlet      = NULL;
        x->v_noutlets   = 0;
        x->v_canvas     = canvas_getcurrent();
        x->v_loaded     = 0;
        signalinlet_new((t_object *)x, 0);
        if(!x->v_vbap)
        {
            pd_error(x, "vbap.coeff can't allocate its internal structure.");
            pd_free((t_pd *)x);
            return NULL;
        }
        else if(argc > 1)
        {
            vbap_tilde_configure(x, &s_list, argc, argv);
        }
        x->v_loaded = 1;
    }
    
    return (x);
}

void vbap_tilde_free(t_vbap_tilde *x)
{
    vbap_tilde_free_pointers(x);
    freebytes(x->v_outlets, x->v_noutlets * sizeof(t_outlet *));
    freebytes(x->v_outputs, x->v_noutlets * sizeof(t_sample *));
    x->v_noutlets = 0;
    vbapf_free(x->v_vbap);
}

t_int *vbap_tilde_perform_2d(t_int *w)
{
    t_vbap_tilde *x     = (t_vbap_tilde *)(w[1]);
    t_vbapf      *vbap  = x->v_vbap;
    t_sample*   coeffs  = x->v_coeffs;
    
    size_t n            = x->v_nsamples;
    t_sample**  inputs  = x->v_inputs;
    t_sample**  outputs = x->v_outputs;
    size_t noutlets     = x->v_noutlets;
    
    size_t i;
    while(n--)
    {
        const t_sample f  = inputs[0][n];
        vbapf_2d_perform(vbap, inputs[1][n], coeffs);
        for(i = 0; i < noutlets; ++i)
        {
            outputs[i][n] = f * coeffs[i];
        }
    }
    return (w+2);
}

t_int *vbap_tilde_perform_3d(t_int *w)
{
    t_vbap_tilde *x     = (t_vbap_tilde *)(w[1]);
    t_vbapf      *vbap  = x->v_vbap;
    t_sample*   coeffs  = x->v_coeffs;
    
    size_t n            = x->v_nsamples;
    t_sample**  inputs  = x->v_inputs;
    size_t noutlets     = x->v_noutlets;
    t_sample**  outputs = x->v_outputs;
    
    size_t i;
    while(n--)
    {
        const t_sample f = inputs[0][n];
        vbapf_3d_perform(vbap, inputs[1][n], inputs[2][n], coeffs);
        for(i = 0; i < noutlets; ++i)
        {
            outputs[i][n] = f * coeffs[i];
        }
    }
    return (w+2);
}

void vbap_tilde_dsp(t_vbap_tilde *x, t_signal **sp)
{
    size_t i;
    x->v_nsamples   = sp[0]->s_n;
    x->v_inputs[0]  = sp[0]->s_vec;
    x->v_inputs[1]  = sp[1]->s_vec;
    if(vbapf_dimension(x->v_vbap) == 2)
    {
        for(i = 0; i < x->v_noutlets; ++i)
        {
            x->v_outputs[i] = sp[2+i]->s_vec;
        }
        dsp_add((t_perfroutine)vbap_tilde_perform_2d, 1, x);
    }
    else if(vbapf_dimension(x->v_vbap) == 3)
    {
        x->v_inputs[2]  = sp[2]->s_vec;
        for(i = 0; i < x->v_noutlets; ++i)
        {
            x->v_outputs[i] = sp[3+i]->s_vec;
        }
        dsp_add((t_perfroutine)vbap_tilde_perform_3d, 1, x);
    }
}

EXTERN void vbap_tilde_setup(void)
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
