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

#include <cstring>

#include "colorwrap_c.h"
#include "colorwrap.hpp"


    ///////////////////////////////////////////////////////////////
    // Sequential colormaps
    ///////////////////////////////////////////////////////////////

int colorwrapBlues   ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::Blues( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapBuGn    ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::BuGn( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapBuPu    ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::BuPu( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapGnBu    ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::GnBu( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapGreens  ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::Greens( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapGreys   ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::Greys( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapOranges ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::Oranges( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapOrRd    ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::OrRd( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapPuBu    ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::PuBu( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapPuBuGn  ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::PuBuGn( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapPuRd    ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::PuRd( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapPurples ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::Purples( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapRdPu    ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::RdPu( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapReds    ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::Reds( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapYlGn    ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::YlGn( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapYlGnBu  ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::YlGnBu( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapYlOrBr  ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::YlOrBr( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapYlOrRd  ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::YlOrRd( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}



    ///////////////////////////////////////////////////////////////
    // Diverging colormaps
    ///////////////////////////////////////////////////////////////

int colorwrapBrBG    ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::BrBG( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapPiYG    ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::PiYG( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapPRGn    ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::PRGn( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapPuOr    ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::PuOr( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapRdBu    ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::RdBu( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapRdGy    ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::RdGy( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapRdYlBu  ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::RdYlBu( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapRdYlGn  ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::RdYlGn( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapSpectral( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::Spectral( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}



    ///////////////////////////////////////////////////////////////
    // Qualitative colormaps
    ///////////////////////////////////////////////////////////////

int colorwrapAccent  ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::Accent( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapDark2   ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::Dark2( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapPaired  ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::Paired( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapPastel1 ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::Pastel1( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapPastel2 ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::Pastel2( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapSet1    ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::Set1( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapSet2    ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::Set2( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

int colorwrapSet3    ( int num_colors, int colormap[] )
{
    if ( num_colors < 1 )
    {
        return 0;
    }

    if ( colormap == nullptr )
    {
        return 0;
    }

    auto colors = Colorwrap::Set3( static_cast<std::size_t>(num_colors) );
    memcpy(colormap, colors.data(), colors.size());

    return static_cast<int>(colors.size());
}

