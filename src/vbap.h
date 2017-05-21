/*
// Copyright (c) 2016-2017 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef VBAP_INCLUDE_C
#define VBAP_INCLUDE_C

#include <stddef.h>

#ifdef _WIN32
#define VBAP_EXTERN __declspec(dllexport) extern
#else
#define VBAP_EXTERN extern
#endif

#if defined(_MSC_VER) && !defined(_LANGUAGE_C_PLUS_PLUS) && !defined(__cplusplus)
#define VBAP_EXTERN_STRUCT extern struct
#else
#define VBAP_EXTERN_STRUCT struct
#endif

//! @defgroup vbapf vbapf
//! @brief The single point precision part of the library.

//! @addtogroup vbapf
//! @{

VBAP_EXTERN_STRUCT _vbapf;

//! @brief The opaque type used to compute vbap coefficients.
typedef struct _vbapf t_vbapf;

//! @brief Allocates a new vbap structure.
VBAP_EXTERN t_vbapf* vbapf_new(void);

//! @brief Frees a vbap structure.
VBAP_EXTERN void vbapf_free(t_vbapf* vbap);

//! @brief Gets the current number of loudspeakers.
VBAP_EXTERN size_t vbapf_nls(t_vbapf const* vbap);

//! @brief Prepares the vbap structure for a specific configuration.
VBAP_EXTERN char vbapf_2d_prepare(t_vbapf* vbap, size_t const nangles, float const * angles);

//! @brief Computes the coefficients of the loudspeakers for a specified angle.
VBAP_EXTERN void vbapf_2d_perform(t_vbapf const* vbap, float const angle, float * coefficients);

//! @}

#endif // VBAP_INCLUDE_C
