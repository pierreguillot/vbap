/*
// Copyright (c) 2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include <m_pd.h>

static t_class *leeloo_class;

typedef struct _leeloo
{
    t_object    l_obj;
    t_float     l_f;
    size_t      l_nios;
    t_sample*   l_vtemp;
    size_t      l_vsize;
} t_leeloo;

static void *leeloo_new(t_float f)
{
    size_t i;
    t_leeloo *x = (t_leeloo *)pd_new(leeloo_class);
    if(x)
    {
        x->l_nios = (f < 1) ? 1 : (size_t)f;
        for(i = 1; i < x->l_nios; ++i)
        {
            signalinlet_new((t_object *)x, 0);
            outlet_new((t_object *)x, &s_signal);
        }
        outlet_new((t_object *)x, &s_signal);
        x->l_vsize = 0;
        x->l_vtemp = NULL;
    }
    
    return (x);
}

void leeloo_free(t_leeloo *x)
{
    if(x->l_vsize && x->l_vtemp)
    {
        freebytes(x->l_vtemp, x->l_vsize);
        x->l_vtemp = NULL;
        x->l_vsize = 0;
    }
}

t_int *leeloo_perform(t_int *w)
{
    size_t i, j;
    size_t nios     = (size_t)(w[1]);
    size_t n        = (size_t)(w[2]);
    t_sample  *vec  = (t_sample *)(w[3]);
    t_sample  *in, *out, *temp;
    
    temp = vec;
    for(i = 4; i < nios+4; ++i)
    {
        in = (t_sample *)(w[i]);
        for(j = 0; j < n; ++j) {
            *temp++ = *in++;
        }
    }
    
    temp = vec;
    for(; i < 2*nios+4; ++i)
    {
        out = (t_sample *)(w[i]);
        for(j = 0; j < n; ++j) {
            *out++ = *temp++;
        }
    }
    
    return (w+nios*2+4);
}

void leeloo_dsp(t_leeloo *x, t_signal **sp)
{
    size_t i;
    t_int* vec;
    if(x->l_vsize && x->l_vtemp)
    {
        freebytes(x->l_vtemp, x->l_vsize);
        x->l_vtemp = NULL;
        x->l_vsize = 0;
    }
    x->l_vsize = sizeof(t_sample) * x->l_nios * (size_t)sp[0]->s_n;
    x->l_vtemp = (t_sample *)getbytes(x->l_vsize);
    if(x->l_vtemp)
    {
        vec = (t_int *)getbytes((x->l_nios * 2 + 3) * sizeof(t_int));
        if(vec)
        {
            vec[0] = (t_int)x->l_nios;
            vec[1] = (t_int)sp[0]->s_n;
            vec[2] = (t_int)x->l_vtemp;
            for(i = 0; i < x->l_nios * 2; ++i)
            {
                vec[i+3] = (t_int)sp[i]->s_vec;
            }
            dsp_addv(leeloo_perform, (x->l_nios * 2 + 3), vec);
            freebytes(vec, (x->l_nios * 2 + 2) * sizeof(t_int));
        }
        else
        {
            pd_error(x, "can't allocate temporary vectors.");
        }
    }
    else
    {
        pd_error(x, "can't allocate temporary vectors.");
    }
    
}

EXTERN void leeloo_tilde_setup(void)
{
    t_class* c = class_new(gensym("leeloo~"), (t_newmethod)leeloo_new, NULL, sizeof(t_leeloo), CLASS_DEFAULT, A_DEFFLOAT, 0);
    if(c)
    {
        class_addmethod(c, (t_method)leeloo_dsp, gensym("dsp"), A_CANT);
        CLASS_MAINSIGNALIN(c, t_leeloo, l_f);
    }
    leeloo_class = c;
}
