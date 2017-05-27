/*
// Copyright (c) 2016-2017 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef VBAP_INCLUDE_H
#define VBAP_INCLUDE_H

#include <stddef.h>

#ifdef _WIN32
#define VBAP_EXTERN __declspec(dllexport) extern
#else
#define VBAP_EXTERN extern
#endif

#define VBAP_STR_VERSION "v0.0.1"
#define VBAP_STR_CREDITS "Pierre Guillot | CICM - Université Paris 8"




//! @defgroup vbapf vbapf
//! @brief The single point precision part of the library.

//! @addtogroup vbapf
//! @{

struct _vbapf;

//! @brief The opaque type used to compute vbap.
typedef struct _vbapf t_vbapf;

//! @brief Allocates a new vbap structure.
//! @details The function allocates and initializes a vbap structure that can be used for both<br>
//! 2d and 3d applications. 
//! @return A pointer to a new vbap structure or NULL if the allocation failed.
VBAP_EXTERN t_vbapf* vbapf_new(void);

//! @brief Frees a vbap structure.
//! @details The function frees a vbap structure. For convenience, the pointer is verified so<br>
//! it can be NULL without generating any error.
//! @param vbap The pointer to the vbap structure to free.
VBAP_EXTERN void vbapf_free(t_vbapf* vbap);

//! @brief Gets the current number of loudspeakers.
//! @param vbap The pointer to the vbap structure.
//! @return The current number of loudspeakers.
VBAP_EXTERN size_t vbapf_nls(t_vbapf const* vbap);

//! @brief Gets the current dimension.
//! @details The dimension can be 0 if the vbap structure has not been prepared yet, 2 if the<br>
//! the vbap structure has been prepared for a 2 dimensional application and 3 if thee vbap<br>
//! structure has been prepared for a 2 dimensional application.
//! @param vbap The pointer to the vbap structure.
//! @return The dimension.
VBAP_EXTERN unsigned char vbapf_dimension(t_vbapf const* vbap);

//! @brief Prepares the vbap structure for a specific 2d configuration.
//! @details The function finds the functional pairs of loudspeakers and computes their<br>
//! matrices. The function expects a vector of angles in degrees that defines the azimuth of<br>
//! the loudspkeakers.
//! @param vbap The pointer to the vbap structure.
//! @param nls The number of loudspeakers.
//! @param angles The angles of the loudspeakers.
VBAP_EXTERN char vbapf_2d_prepare(t_vbapf* vbap, size_t const nls, float const * angles);

//! @brief Computes the gains of the loudspeakers for a specified angle.
//! @details The function finds the best pair of loudspeakers depending on the azimuth of the<br>
//! virtual source and computes the gains coeffcients of this loudspeakers. The vector of<br>
//! gains must be at least the size of the number of loudspeakers.
//! @param vbap The pointer to the vbap structure.
//! @param azimuth The azimuth of the virtual source.
//! @param gains The vector to fill with the gains of the loudspkeakers.
VBAP_EXTERN void vbapf_2d_perform(t_vbapf const* vbap, float const azimuth, float * gains);

//! @brief Prepares the vbap structure for a specific 3d configuration.
//! @details The function finds the functional triplets of loudspeakers and computes their<br>
//! matrices. The function expects a vector of pair of angles in degrees that defines the<br>
//! azimuth and the elevation of the loudspkeakers.
//! @param vbap The pointer to the vbap structure.
//! @param nls The number of loudspeakers.
//! @param angles The angles of the loudspeakers.
//! @todo Check the variable max
VBAP_EXTERN char vbapf_3d_prepare(t_vbapf* vbap, size_t const nls, float const * angles);

//! @brief Computes the gains of the loudspeakers for a specified angle.
//! @details The function finds the best triplet of loudspeakers depending on the azimuth of<br>
//! the virtual source and computes the gains coeffcients of this loudspeakers. The vector of<br>
//! gains must be at least the size of the number of loudspeakers.
//! @param vbap The pointer to the vbap structure.
//! @param azimuth The azimuth of the virtual source.
//! @param elevation The elevation of the virtual source.
//! @param gains The vector to fill with the gains of the loudspkeakers.
VBAP_EXTERN void vbapf_3d_perform(t_vbapf const* vbap, float const azimuth, float const elevation, float * gains);

//! @}







//! @defgroup vbap vbap
//! @brief The double point precision part of the library.

//! @addtogroup vbap
//! @{

struct _vbap;

//! @brief The opaque type used to compute vbap.
typedef struct _vbap t_vbap;

//! @brief Allocates a new vbap structure.
//! @details The function allocates and initializes a vbap structure that can be used for both<br>
//! 2d and 3d applications.
//! @return A pointer to a new vbap structure or NULL if the allocation failed.
VBAP_EXTERN t_vbap* vbap_new(void);

//! @brief Frees a vbap structure.
//! @details The function frees a vbap structure. For convenience, the pointer is verified so<br>
//! it can be NULL without generating any error.
//! @param vbap The pointer to the vbap structure to free.
VBAP_EXTERN void vbap_free(t_vbap* vbap);

//! @brief Gets the current number of loudspeakers.
//! @param vbap The pointer to the vbap structure.
//! @return The current number of loudspeakers.
VBAP_EXTERN size_t vbap_nls(t_vbap const* vbap);

//! @brief Gets the current dimension.
//! @details The dimension can be 0 if the vbap structure has not been prepared yet, 2 if the<br>
//! the vbap structure has been prepared for a 2 dimensional application and 3 if thee vbap<br>
//! structure has been prepared for a 2 dimensional application.
//! @param vbap The pointer to the vbap structure.
//! @return The dimension.
VBAP_EXTERN unsigned char vbap_dimension(t_vbap const* vbap);

//! @brief Prepares the vbap structure for a specific 2d configuration.
//! @details The function finds the functional pairs of loudspeakers and computes their<br>
//! matrices. The function expects a vector of angles in degrees that defines the azimuth of<br>
//! the loudspkeakers.
//! @param vbap The pointer to the vbap structure.
//! @param nls The number of loudspeakers.
//! @param angles The angles of the loudspeakers.
VBAP_EXTERN char vbap_2d_prepare(t_vbap* vbap, size_t const nls, double const * angles);

//! @brief Computes the gains of the loudspeakers for a specified angle.
//! @details The function finds the best pair of loudspeakers depending on the azimuth of the<br>
//! virtual source and computes the gains coeffcients of this loudspeakers. The vector of<br>
//! gains must be at least the size of the number of loudspeakers.
//! @param vbap The pointer to the vbap structure.
//! @param azimuth The azimuth of the virtual source.
//! @param gains The vector to fill with the gains of the loudspkeakers.
VBAP_EXTERN void vbap_2d_perform(t_vbap const* vbap, double const azimuth, double * gains);

//! @brief Prepares the vbap structure for a specific 3d configuration.
//! @details The function finds the functional triplets of loudspeakers and computes their<br>
//! matrices. The function expects a vector of pair of angles in degrees that defines the<br>
//! azimuth and the elevation of the loudspkeakers.
//! @param vbap The pointer to the vbap structure.
//! @param nls The number of loudspeakers.
//! @param angles The angles of the loudspeakers.
//! @todo Check the variable max
VBAP_EXTERN char vbap_3d_prepare(t_vbap* vbap, size_t const nls, double const * angles);

//! @brief Computes the gains of the loudspeakers for a specified angle.
//! @details The function finds the best triplet of loudspeakers depending on the azimuth of<br>
//! the virtual source and computes the gains coeffcients of this loudspeakers. The vector of<br>
//! gains must be at least the size of the number of loudspeakers.
//! @param vbap The pointer to the vbap structure.
//! @param azimuth The azimuth of the virtual source.
//! @param elevation The elevation of the virtual source.
//! @param gains The vector to fill with the gains of the loudspkeakers.
VBAP_EXTERN void vbap_3d_perform(t_vbap const* vbap, double const azimuth, double const elevation, double * gains);

//! @}

#endif // VBAP_INCLUDE_C
