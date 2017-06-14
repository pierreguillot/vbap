/*
// Copyright (c) 2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include <m_pd.h>

EXTERN void setup_vbap0x2ecoeff(void);
EXTERN void vbap_tilde_setup(void);

EXTERN void vbap_setup(void)
{
    setup_vbap0x2ecoeff();
    vbap_tilde_setup();
}
