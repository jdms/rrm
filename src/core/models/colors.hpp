/****************************************************************************
 * RRM - Rapid Reservoir Modeling Project                                   *
 * Copyright (C) 2015-2023                                                     *
 * UofC - University of Calgary                                             *
 *                                                                          *
 * This file is part of RRM Software.                                       *
 *                                                                          *
 * RRM is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * RRM is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.             *
 ****************************************************************************/

/**
 * @file colors.hpp
 * @author Julio D. Silva
 * @brief Colors for objects
 */

#pragma once

#include "colorwrap.hpp"

/// Quick and dirty class to create colors on the fly using the Colorwrap class
/// (Colorwrap is a wraper for colorbrewer: https://colorbrewer2.org/)
class Colors {
    public:
        Colors(std::vector<int> colormap = Colorwrap::Spectral())
        {
            this->num_colors = colormap.size()/3;
            this->colormap = colormap;
        }

        QColor color(int i)
        {
            int r = 255, g = 0, b = 0;

            i = i % num_colors;
            if (invert_colors)
            {
                i = (num_colors -1) - i;
            }

            r = colormap[3*i + 0];
            g = colormap[3*i + 1];
            b = colormap[3*i + 2];

            return QColor(r, g, b);
        }

        void invert() { invert_colors = !invert_colors; }

        std::size_t size() { return num_colors; }

    private:
        int num_colors{};
        std::vector<int> colormap{};
        bool invert_colors = false;
};

static Colors DomainsColors(Colorwrap::Pastel2(7));
static Colors StratigraphicSurfaces(Colorwrap::Set1());
static Colors StructuralSurfacesColors(Colorwrap::Set3());



