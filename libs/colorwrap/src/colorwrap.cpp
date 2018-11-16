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


#include <cstdio>
#include <cmath>
#include <algorithm> // for std::max

#include "colorwrap.hpp"

#include "colorbrewer.h" 


#if defined(COLORWRAP_INTERPOLATE_COLORS)
    // Use natural splines to interpolate colours in RGB space -- requires Eigen3
    #include "nspline.h"
#else 
    // Define a stub
    class NSpline 
    {
        public: 
            bool init( std::vector<double> /* = centers */, std::vector<double> /* = function evaluations */  ) 
            { 
                return false; 
            }

            double eval( double ) 
            { 
                return 0;
            }
    };
#endif



///////////////////////////////////////////////////////////
// Process colormaps
///////////////////////////////////////////////////////////


std::vector<int> Colorwrap::YlGn( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "YlGn";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::YlGnBu( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "YlGnBu";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::GnBu( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "GnBu";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::BuGn( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "BuGn";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::PuBuGn( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 11;
    std::string cmap_name = "PuBuGn";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::PuBu( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "PuBu";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::BuPu( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "BuPu";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::RdPu( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "RdPu";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::PuRd( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "PuRd";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::OrRd( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "OrRd";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::YlOrRd( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "YlOrRd";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::YlOrBr( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "YlOrBr";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::Purples( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "Purples";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::Blues( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "Blues";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::Greens( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "Greens";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::Oranges( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "Oranges";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::Reds( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "Reds";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::Greys( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "Greys";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::PuOr( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 11;
    std::string cmap_name = "PuOr";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::BrBG( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 11;
    std::string cmap_name = "BrBG";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::PRGn( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 11;
    std::string cmap_name = "PRGn";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::PiYG( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 11;
    std::string cmap_name = "PiYG";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::RdBu( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 11;
    std::string cmap_name = "RdBu";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::RdGy( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 11;
    std::string cmap_name = "RdGy";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::RdYlBu( std::size_t num_colors )
{
    size_t min_colours = 3;
    size_t max_colours = 11;
    std::string cmap_name = "RdYlBu";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}


std::vector<int> Colorwrap::Spectral(std::size_t num_colors)
{
    size_t min_colours = 3;
    size_t max_colours = 11;
    std::string cmap_name = "Spectral";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}

std::vector<int> Colorwrap::RdYlGn( std::size_t num_colors)
{
    size_t min_colours = 3;
    size_t max_colours = 11;
    std::string cmap_name = "RdYlGn";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}

std::vector<int> Colorwrap::Accent( std::size_t num_colors)
{
    size_t min_colours = 3;
    size_t max_colours = 8;
    std::string cmap_name = "Accent";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}

std::vector<int> Colorwrap::Dark2( std::size_t num_colors)
{
    size_t min_colours = 3;
    size_t max_colours = 8;
    std::string cmap_name = "Dark2";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}

std::vector<int> Colorwrap::Paired( std::size_t num_colors)
{
    size_t min_colours = 3;
    size_t max_colours = 12;
    std::string cmap_name = "Paired";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}

std::vector<int> Colorwrap::Pastel1( std::size_t num_colors)
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "Pastel1";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}

std::vector<int> Colorwrap::Pastel2( std::size_t num_colors)
{
    size_t min_colours = 3;
    size_t max_colours = 8;
    std::string cmap_name = "Pastel2";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}

std::vector<int> Colorwrap::Set1( std::size_t num_colors)
{
    size_t min_colours = 3;
    size_t max_colours = 9;
    std::string cmap_name = "Set1";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}

std::vector<int> Colorwrap::Set2( std::size_t num_colors)
{
    size_t min_colours = 3;
    size_t max_colours = 8;
    std::string cmap_name = "Set2";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}

std::vector<int> Colorwrap::Set3( std::size_t num_colors)
{
    size_t min_colours = 3;
    size_t max_colours = 12;
    std::string cmap_name = "Set3";

    return get_cbrewer(cmap_name, min_colours, max_colours, num_colors);
}



///////////////////////////////////////////////////////////
// Interpolate colours and perform other menial tasks
///////////////////////////////////////////////////////////


std::vector<int> Colorwrap::get_cbrewer( std::string cmap_name, std::size_t min_colors, std::size_t max_colors, std::size_t num_colors )
{
            //
            // Get base colors
            // 

            size_t base_num_colors;

            if ( num_colors < min_colors )
            {
                base_num_colors = min_colors;
            }
            else if ( num_colors > max_colors )
            {
                base_num_colors = max_colors;
            }
            else 
            {
                base_num_colors = num_colors;
            }

            /* auto colors_ = brew<std::string>(cmap_name, base_num_colors); */
            std::vector<std::string> colors( brew<std::string>(cmap_name, base_num_colors) );

            //
            // Return RGB colors
            //

            #if !defined(COLORWRAP_INTERPOLATE_COLORS)
            // if can't interpolate colours we can only return whatever is hard coded in colormap
                if ( num_colors > max_colors )
                    num_colors = max_colors;
            #endif

            return interpolate_color_list(colors, num_colors);
}

std::vector<int> Colorwrap::interpolate_color_list( const std::vector<std::string> &colors, size_t num_colors )
{
            std::vector<int> out_colors;

            if ( (num_colors == 0) || (colors.size() == 0) )
            {
                return out_colors;
            }

            int r, g, b;

            if ( num_colors <= colors.size() )
            {

                for ( size_t i = 0; i < num_colors; ++i )
                {
                    /* std::cout << "\n Current colour: "  << colors[i] << "; "; */
                    getRGB(colors[i], r, g, b );
                    /* std::cout << "(r, g, b) = (" << r << ", " << g << ", " << b << ")\n" << std::flush; */

                    out_colors.push_back(r);
                    out_colors.push_back(g);
                    out_colors.push_back(b);
                }

                return out_colors;
            }

            std::vector<double> rc( colors.size() ), gc( colors.size() ), bc( colors.size() ), t;
            t = linspace( 0.0, 1.0, colors.size() );

            /* std::cout << "raw colors\n" << std::flush; */
            for ( size_t i = 0; i < colors.size(); ++i )
            {
                /* std::cout << "\n Current colour: "  << colors[i] << "; "; */
                getRGB(colors[i], r, g, b );
                /* std::cout << "(r, g, b) = (" << r << ", " << g << ", " << b << ")\n" << std::flush; */

                rc[i] = static_cast<double>(r)/255.0;
                gc[i] = static_cast<double>(g)/255.0;
                bc[i] = static_cast<double>(b)/255.0;
            }

            std::vector<double> rc_new, gc_new, bc_new, t_new;
            t_new = Colorwrap::linspace(0.0, 1.0, num_colors);

            rc_new = Colorwrap::interpolate_coord(t, rc, t_new);
            gc_new = Colorwrap::interpolate_coord(t, gc, t_new);
            bc_new = Colorwrap::interpolate_coord(t, bc, t_new);

            /* std::cout << "num_colors > 11\n"; */
            for ( size_t i = 0; i < num_colors; ++i )
            {
                r = static_cast<int>( std::max( std::round(rc_new[i] * 255.0), 0.) );
                g = static_cast<int>( std::max( std::round(gc_new[i] * 255.0), 0.) );
                b = static_cast<int>( std::max( std::round(bc_new[i] * 255.0), 0.) );

                out_colors.push_back( (r < 255 ? r : 255) );
                out_colors.push_back( (g < 255 ? g : 255) );
                out_colors.push_back( (b < 255 ? b : 255) );

                /* std::cout << "(r, g, b) = (" */ 
                    /* << (r < 255 ? r : 255) << ", " */ 
                    /* << (g < 255 ? g : 255) << ", " */ 
                    /* << (b < 255 ? b : 255) << ")\n" << std::flush; */
            }

            return out_colors;
}

std::vector<double> Colorwrap::linspace(double min, double max, size_t steps)
{
            std::vector<double> out;
            out.push_back(min);

            if ( steps <= 1 )
            {
                out.push_back(max);
            }
            else
            {
                out.resize(steps);
                for ( size_t i = 1; i < steps; ++i )
                {
                    /* out.push_back( min + (max-min) * static_cast<double>(i)/static_cast<double>(steps-1) ); */
                    out[i] = min + (max-min) * static_cast<double>(i)/static_cast<double>(steps-1);
                }
            }

            return out;
}

std::vector<double> Colorwrap::interpolate_coord( const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &x_new )
{
            std::vector<double> y_new( x_new.size() ); 

            NSpline spline;

            spline.init(x, y);

            for ( size_t i = 0; i < x_new.size(); ++i )
            {
                y_new[i] = spline.eval( x_new[i] );
            }

            return y_new;
}

void Colorwrap::getRGB( std::string hex_code, int &r, int &g, int &b )
{
            if ( hex_code.size() < 7 )
            {
                r = 0; g = 0; b = 0;
                return;
            }

            hex_code.erase(0,1);

            #ifdef _WIN32
                sscanf_s( hex_code.c_str(), "%02x%02x%02x", &r, &g, &b );
            #else
                sscanf( hex_code.c_str(), "%02x%02x%02x", &r, &g, &b );
            #endif

            /* std::cout << "hex = (r, g, b) >>> " << hex_code << " = ("; */
            /* std::cout << r << ", " << g << ", " << b << ")\n" << std::flush; */
}
