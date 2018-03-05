#ifndef __COLOR_MAP_WRAPPER_HPP__
#define __COLOR_MAP_WRAPPER_HPP__

#include <vector>

#include "cppcolormap.h"

namespace ColorMapWrapper
{
    std::vector<int> getRegionsColor(std::size_t numColors)
    {
        auto data = cppcolormap::colormap("Spectral", numColors);
        std::size_t data_shape = data.shape(1);

        std::vector<int> out_colors = std::vector<int>( data.size() );

        for ( size_t i = 0; i < numColors; ++i )
            for ( size_t j = 0; j < data_shape; ++j )
            {
                {
                    out_colors[i + j] = data(i,j);
                }
            }

        return out_colors;
    }
};

#endif
