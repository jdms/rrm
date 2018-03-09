#ifndef __COLOR_MAP_WRAPPER_HPP__
#define __COLOR_MAP_WRAPPER_HPP__

#include <vector>
#include <memory>

#include <cstdio>

#include <QColor>

#include "colorbrewer.h" 
#include "nspline.h"

class ColorMapWrapper
{
    public:

        std::vector<int> getSpectral(std::size_t num_colors)
        {
            size_t spectral_min_colors = 3;
            size_t spectral_max_colors = 10;

            // For some weird reason the QColor object returns weird colours
            // when we pass all 11 possible colours of the "Spectral" colormap
            // brewer
            //
            // This bug disapears when setting building to debug
            //
            /* size_t spectral_max_colors = 11; */
            std::string cmap_name = "Spectral";

            return get("Spectral", spectral_min_colors, spectral_max_colors, num_colors);
        }

    private:

        std::vector<int> get( std::string cmap_name, std::size_t min_colors, std::size_t max_colors, std::size_t num_colors )
        {
            //
            // Get base colors
            // 

            std::vector<QColor> colors;

            if ( num_colors < min_colors )
            {
                colors = brew<QColor>(cmap_name, min_colors);
            }
            else if ( num_colors > max_colors )
            {
                colors = brew<QColor>(cmap_name, max_colors);
            }
            else 
            {
                colors = brew<QColor>(cmap_name, num_colors);
            }


            //
            // Return RGB colors
            //

            return interpolate_color_list(colors, num_colors);
        }

        std::vector<int> interpolate_color_list( const std::vector<QColor> &colors, size_t num_colors )
        {
            std::vector<int> out_colors;

            if ( colors.size() == 0 )
            {
                return out_colors;
            }

            std::unique_ptr<int> r = std::unique_ptr<int>( new int );
            std::unique_ptr<int> g = std::unique_ptr<int>( new int );
            std::unique_ptr<int> b = std::unique_ptr<int>( new int );

            if ( num_colors <= colors.size() )
            {
                /* QColor qc; */

                /* std::cout << "num_colors <= 11\n"; */
                for ( size_t i = 0; i < num_colors; ++i )
                {
                    /* qc = colors[i]; */
                    /* qc.getRgb(&r, &g, &b); */
                    colors[i].getRgb( r.get(), g.get(), b.get() );

                    out_colors.push_back(*r);
                    out_colors.push_back(*g);
                    out_colors.push_back(*b);
                    /* std::cout << "(r, g, b) = (" << *r << ", " << *g << ", " << *b << ")\n" << std::flush; */

                }

                return out_colors;
            }

            std::vector<double> rc( colors.size() ), gc( colors.size() ), bc( colors.size() ), t;
            t = linspace( 0.0, 1.0, colors.size() );

            /* std::cout << "raw colors\n" << std::flush; */
            for ( size_t i = 0; i < colors.size(); ++i )
            {
                colors[i].getRgb( r.get(), g.get(), b.get() );
                /* std::cout << "(r, g, b) = (" << *r << ", " << *g << ", " << *b << ")\n" << std::flush; */

                rc[i] = static_cast<double>(*r)/255.0;
                gc[i] = static_cast<double>(*g)/255.0;
                bc[i] = static_cast<double>(*b)/255.0;
            }

            std::vector<double> rc_new, gc_new, bc_new, t_new;
            t_new = ColorMapWrapper::linspace(0.0, 1.0, num_colors);

            rc_new = ColorMapWrapper::interpolate_coord(t, rc, t_new);
            gc_new = ColorMapWrapper::interpolate_coord(t, gc, t_new);
            bc_new = ColorMapWrapper::interpolate_coord(t, bc, t_new);

            /* std::cout << "num_colors > 11\n"; */
            for ( size_t i = 0; i < num_colors; ++i )
            {
                *r = rc_new[i] * 255.0;
                *g = gc_new[i] * 255.0;
                *b = bc_new[i] * 255.0;

                out_colors.push_back( static_cast<int>(*r) );
                out_colors.push_back( static_cast<int>(*g) );
                out_colors.push_back( static_cast<int>(*b) );

                /* std::cout << "(r, g, b) = (" << *r << ", " << *g << ", " << *b << ")\n" << std::flush; */
            }

            return out_colors;
        }

        std::vector<double> linspace(double min, double max, size_t steps)
        {
            std::vector<double> out;
            out.push_back(min);

            if ( steps <= 1 )
            {
                out.push_back(max);

                return out;
            }

            for ( size_t i = 1; i < steps; ++i )
            {
                out.push_back( min + (max-min) * static_cast<double>(i)/static_cast<double>(steps-1) );
            }

            return out;
        }

        std::vector<double> interpolate_coord( const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &x_new )
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

        void getRGB( std::string hex_code, int &r, int &g, int &b )
        {
            hex_code.erase(0,1);

            sscanf_s( hex_code.data(), "%02x%02x%02x", &r, &g, &b );
            
            std::cout << "hex = (r, g, b) >>> " << hex_code << " = (";
            std::cout << r << ", " << g << ", " << b << ")\n" << std::flush;
        }

        //
        // Deprecated
        //
        /* std::vector<int> getRegionsColor(std::size_t num_colors) */
        /* { */
        /*     auto data = cppcolormap::colormap("Spectral", num_colors); */
        /*     std::size_t data_shape = data.shape(1); */

        /*     std::vector<int> out_colors = std::vector<int>( data.size() ); */

        /*     for ( size_t i = 0; i < num_colors; ++i ) */
        /*         for ( size_t j = 0; j < data_shape; ++j ) */
        /*         { */
        /*             { */
        /*                 out_colors[i + j] = data(i,j); */
        /*             } */
        /*         } */

        /*     return out_colors; */
        /* } */

};

#endif
