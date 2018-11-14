/* Copyright (c) 2018 Julio Daniel Machado Silva */

/* Licensed under the Apache License, Version 2.0 (the "License"); */
/* you may not use this file except in compliance with the License. */
/* You may obtain a copy of the License at */

/* http://www.apache.org/licenses/LICENSE-2.0 */

/* Unless required by applicable law or agreed to in writing, software */
/* distributed under the License is distributed on an "AS IS" BASIS, */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/* See the License for the specific language governing permissions and */
/* limitations under the License. */


#ifndef __COLORWRAP_C_H__
#define __COLORWRAP_C_H__


#ifdef __cplusplus
extern "C" 
{
#endif


#include "colorwrap_win_dll_export_handler.h"


    ///////////////////////////////////////////////////////////////
    // Sequential colormaps
    ///////////////////////////////////////////////////////////////

    COLORWRAPLIB_DLL_HANDLER int colorwrapBlues   ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapBuGn    ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapBuPu    ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapGnBu    ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapGreens  ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapGreys   ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapOranges ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapOrRd    ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapPuBu    ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapPuBuGn  ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapPuRd    ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapPurples ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapRdPu    ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapReds    ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapYlGn    ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapYlGnBu  ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapYlOrBr  ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapYlOrRd  ( int num_colors, int rgb_colormap[] );


    ///////////////////////////////////////////////////////////////
    // Diverging colormaps
    ///////////////////////////////////////////////////////////////

    COLORWRAPLIB_DLL_HANDLER int colorwrapBrBG    ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapPiYG    ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapPRGn    ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapPuOr    ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapRdBu    ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapRdGy    ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapRdYlBu  ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapRdYlGn  ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapSpectral( int num_colors, int rgb_colormap[] );


    ///////////////////////////////////////////////////////////////
    // Qualitative colormaps
    ///////////////////////////////////////////////////////////////

    COLORWRAPLIB_DLL_HANDLER int colorwrapAccent  ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapDark2   ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapPaired  ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapPastel1 ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapPastel2 ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapSet1    ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapSet2    ( int num_colors, int rgb_colormap[] );
    COLORWRAPLIB_DLL_HANDLER int colorwrapSet3    ( int num_colors, int rgb_colormap[] );


#ifdef __cplusplus
} // extern "C"
#endif

#endif
