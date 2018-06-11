/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */



#ifndef REGION_H
#define REGION_H

#include <iostream>
#include <vector>

//NOTE: think about if cells are vector of indexes of faces or indexes of vertices
//NOTE: think about if the cells should be stored here.

class Region1
{
    public:

        Region1()
        {
            initialize();
        }

        inline void setId( std::size_t id_ )
        {
            index = id_;
        }

        inline std::size_t getId() const
        {
            return index;
        }

        inline void setPointPosition( double x_, double y_, double z_ )
        {
            point_position_x = x_;
            point_position_y = y_;
            point_position_z = z_;
        }

        inline void getPointPosition( double& x_, double& y_, double& z_ ) const
        {
            x_ = point_position_x;
            y_ = point_position_y;
            z_ = point_position_z;
        }


        inline void setVisibility( bool option )
        {
            is_visible = option;
        }

        inline bool getVisibility() const
        {
            return is_visible;
        }


        inline void addCrossSectionofDepth( double depth_ )
        {
            depth_of_cross_sections.push_back( depth_ );
        }

        inline double getCrossSectionofId( std::size_t id_) const
        {
            try
            {
                return depth_of_cross_sections.at( id_ );
            }
            catch ( const std::exception& e )
            {
                std::cout << "a standard exception was caught as trying to access cross-section of "
                          << id_ << ", with message '"
                          << e.what() << "'\n";
            }
        }

        inline const std::vector< double >& getDepthofAllCrossSections() const
        {
            return depth_of_cross_sections;
        }

        inline void setCellIndexesofRegion( const std::vector< std::size_t >& cells_ )
        {
            index_of_cells = cells_;
        }

        inline const std::vector< std::size_t >& getCellIndexesofRegion() const
        {
            return index_of_cells;
        }


        inline void initialize()
        {
            setDefaultValues();
        }

        inline void clear()
        {
            index_of_cells.clear();
            depth_of_cross_sections.clear();
        }

        inline void setDefaultValues()
        {
            index = 0;
            is_visible = true;
            index_of_cells.clear();
            depth_of_cross_sections.clear();

            point_position_x = 0.0f;
            point_position_y = 0.0f;
            point_position_z = 0.0f;

        }


    protected:

        std::size_t index;
        bool is_visible;

        double point_position_x;
        double point_position_y;
        double point_position_z;

        std::vector< std::size_t > index_of_cells;
        std::vector< double > depth_of_cross_sections;

};

#endif // REGION_H
