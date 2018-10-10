#ifndef __COLORMAP_HPP__
#define __COLORMAP_HPP__

#include <vector>


class Colormap
{
    public:
        static std::vector<int> Spectral( std::size_t num_colors = 11 );

    protected:
        static std::vector<int> get_cbrewer( std::string cmap_name, std::size_t min_colors, std::size_t max_colors, std::size_t num_colors );

        static std::vector<int> interpolate_color_list( const std::vector<std::string> &clist_hex_codes, size_t num_colors );

        static std::vector<double> linspace(double min, double max, size_t steps);
        static std::vector<double> interpolate_coord( const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &x_new );

        static void getRGB( std::string hex_code, int &r, int &g, int &b );
};


#include <cstdio>
#include <cmath>

#include "colorbrewer.h" 
#include "nspline.h"

std::vector<int> Colormap::Spectral(std::size_t num_colors)
{
    size_t spectral_min_colors = 3;
    size_t spectral_max_colors = 11;
    std::string cmap_name = "Spectral";

    return get_cbrewer(cmap_name, spectral_min_colors, spectral_max_colors, num_colors);
}

std::vector<int> Colormap::get_cbrewer( std::string cmap_name, std::size_t min_colors, std::size_t max_colors, std::size_t num_colors )
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

            return interpolate_color_list(colors, num_colors);
}

std::vector<int> Colormap::interpolate_color_list( const std::vector<std::string> &colors, size_t num_colors )
{
            std::vector<int> out_colors;

            if ( colors.size() == 0 )
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
            t_new = Colormap::linspace(0.0, 1.0, num_colors);

            rc_new = Colormap::interpolate_coord(t, rc, t_new);
            gc_new = Colormap::interpolate_coord(t, gc, t_new);
            bc_new = Colormap::interpolate_coord(t, bc, t_new);

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

std::vector<double> Colormap::linspace(double min, double max, size_t steps)
{
            std::vector<double> out;
            out.push_back(min);

            if ( steps <= 1 )
            {
                out.push_back(max);

                return out;
            }

            out.resize(steps);
            for ( size_t i = 1; i < steps; ++i )
            {
                /* out.push_back( min + (max-min) * static_cast<double>(i)/static_cast<double>(steps-1) ); */
                out[i] = min + (max-min) * static_cast<double>(i)/static_cast<double>(steps-1);
            }

            return out;
}

std::vector<double> Colormap::interpolate_coord( const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &x_new )
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

void Colormap::getRGB( std::string hex_code, int &r, int &g, int &b )
{
            hex_code.erase(0,1);

            #ifdef _WIN32
                sscanf_s( hex_code.c_str(), "%02x%02x%02x", &r, &g, &b );
            #else
                sscanf( hex_code.c_str(), "%02x%02x%02x", &r, &g, &b );
            #endif

            /* std::cout << "hex = (r, g, b) >>> " << hex_code << " = ("; */
            /* std::cout << r << ", " << g << ", " << b << ")\n" << std::flush; */
}

#endif
