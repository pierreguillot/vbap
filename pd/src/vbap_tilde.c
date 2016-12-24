/*
// Copyright (c) 2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include <m_pd.h>

static t_class *vbap_tilde_class;

typedef struct _vbap_tilde
{
    t_object    v_obj;
    t_float     v_f;
} t_vbap_tilde;

static void *vbap_tilde_new(t_float f)
{
    t_vbap_tilde *x = (t_vbap_tilde *)pd_new(vbap_tilde_class);
    if(x)
    {
        outlet_new((t_object *)x, &s_signal);
    }
    
    return (x);
}

void vbap_tilde_free(t_vbap_tilde *x)
{
    ;
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
                           sizeof(t_vbap_tilde), CLASS_DEFAULT, A_DEFFLOAT, 0);
    if(c)
    {
        class_addmethod(c, (t_method)vbap_tilde_dsp, gensym("dsp"), A_CANT);
        CLASS_MAINSIGNALIN(c, t_vbap_tilde, v_f);
    }
    vbap_tilde_class = c;
}
