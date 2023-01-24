%module colorwrap
%{
#include "../include/colorwrap.hpp"
%}

%ignore COLORWRAPLIB_DLL_HANDLER;

%include "std_vector.i"

namespace std {
    %template(IntVector) vector<int>;
    %template(FloatVector) vector<float>;
}

/* %include "../include/colorwrap.hpp" */

class Colorwrap
{
    public:

        ///////////////////////////////////////////////////////////////
        // Sequential colormaps
        ///////////////////////////////////////////////////////////////

        static std::vector<int> Blues   ( std::size_t num_colors =  9 );
        static std::vector<int> BuGn    ( std::size_t num_colors =  9 );
        static std::vector<int> BuPu    ( std::size_t num_colors =  9 );
        static std::vector<int> GnBu    ( std::size_t num_colors =  9 );
        static std::vector<int> Greens  ( std::size_t num_colors =  9 );
        static std::vector<int> Greys   ( std::size_t num_colors =  9 );
        static std::vector<int> Oranges ( std::size_t num_colors =  9 );
        static std::vector<int> OrRd    ( std::size_t num_colors =  9 );
        static std::vector<int> PuBu    ( std::size_t num_colors =  9 );
        static std::vector<int> PuBuGn  ( std::size_t num_colors =  9 );
        static std::vector<int> PuRd    ( std::size_t num_colors =  9 );
        static std::vector<int> Purples ( std::size_t num_colors =  9 );
        static std::vector<int> RdPu    ( std::size_t num_colors =  9 );
        static std::vector<int> Reds    ( std::size_t num_colors =  9 );
        static std::vector<int> YlGn    ( std::size_t num_colors =  9 );
        static std::vector<int> YlGnBu  ( std::size_t num_colors =  9 );
        static std::vector<int> YlOrBr  ( std::size_t num_colors =  9 );
        static std::vector<int> YlOrRd  ( std::size_t num_colors =  9 );


        ///////////////////////////////////////////////////////////////
        // Diverging colormaps
        ///////////////////////////////////////////////////////////////

        static std::vector<int> BrBG    ( std::size_t num_colors = 11 );
        static std::vector<int> PiYG    ( std::size_t num_colors = 11 );
        static std::vector<int> PRGn    ( std::size_t num_colors = 11 );
        static std::vector<int> PuOr    ( std::size_t num_colors = 11 );
        static std::vector<int> RdBu    ( std::size_t num_colors = 11 );
        static std::vector<int> RdGy    ( std::size_t num_colors = 11 );
        static std::vector<int> RdYlBu  ( std::size_t num_colors = 11 );
        static std::vector<int> RdYlGn  ( std::size_t num_colors = 11 );
        static std::vector<int> Spectral( std::size_t num_colors = 11 );


        ///////////////////////////////////////////////////////////////
        // Qualitative colormaps
        ///////////////////////////////////////////////////////////////

        static std::vector<int> Accent  ( std::size_t num_colors =  8 );
        static std::vector<int> Dark2   ( std::size_t num_colors =  8 );
        static std::vector<int> Paired  ( std::size_t num_colors = 12 );
        static std::vector<int> Pastel1 ( std::size_t num_colors =  9 );
        static std::vector<int> Pastel2 ( std::size_t num_colors =  8 );
        static std::vector<int> Set1    ( std::size_t num_colors =  9 );
        static std::vector<int> Set2    ( std::size_t num_colors =  8 );
        static std::vector<int> Set3    ( std::size_t num_colors = 12 );


        ///////////////////////////////////////////////////////////////
        // Convert colormap to a vector of floats
        ///////////////////////////////////////////////////////////////

        static std::vector<float> asFloat( const std::vector<int> & );
        static std::vector<float> asFloat( std::vector<int> && );


    protected:
        static std::vector<int> get_cbrewer( std::string cmap_name, std::size_t min_colors, std::size_t max_colors, std::size_t num_colors );
        static std::vector<int> interpolate_color_list( const std::vector<std::string> &clist_hex_codes, size_t num_colors );
        static void getRGB( std::string hex_code, int &r, int &g, int &b );

        static std::vector<double> linspace(double min, double max, size_t steps);
        static std::vector<double> interpolate_coord( const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &x_new );
};

