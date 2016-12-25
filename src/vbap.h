/*
// Copyright (c) 2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef _VBAP_INCLUDE_C_
#define _VBAP_INCLUDE_C_

#include <stddef.h>

#define VBAP_EXTERN
#define VBAP_EXTERN_STRUCT

VBAP_EXTERN_STRUCT struct _vbapf;
typedef struct _vbapf t_vbapf;

//! @brief Creates a new single point precision vbap structure.
//! @return A pointer to the new vbap structure or NULL if the allocation failed.
VBAP_EXTERN t_vbapf* vbapf_new(void);

//! @brief Frees an allocated single point precision vbap structure.
//! @return A pointer to the new t_vbapf or NULL if the allocation failed.
VBAP_EXTERN void vbapf_free(t_vbapf* vbap);
VBAP_EXTERN size_t vbapf_nls(t_vbapf const* vbap);

VBAP_EXTERN char vbapf_2d_prepare(t_vbapf* vbap, size_t const nangles, float const * angles);
VBAP_EXTERN void vbapf_2d_perform(t_vbapf const* vbap, float const angle, float * coefficients);

#endif // _VBAP_INCLUDE_C_