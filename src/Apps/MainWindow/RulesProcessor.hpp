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


#ifndef __RULES_PROCESSOR__
#define __RULES_PROCESSOR__

#include <iostream>
#include <vector>
#include <map>


#include "Core/Geometry/PolygonalCurve/PolygonalCurve2D.hpp"
#include "stratmod/stratigraphy_modeller.hpp"
#include "stratmod/stratigraphy_utilities.hpp"



//namespace RRM
//{}

    class RulesProcessor {

        public:


            RulesProcessor();
            ~RulesProcessor() = default;

            std::vector<std::size_t> getSurfaces();

            //
            // brief:
            // Verifies whether ir is possible to define a new 'drawing' region,
            // either above or below.
            // Argument: `eligible_surfaces` stores the indices of surfaces that can
            // be used as input either for an `defineAbove()` or a `defineBelow()`.
            // Return: true if at least one elegible surface was found.
            //
            /* bool requestDefineRegion( std::vector<size_t> &eligible_surfaces ); */

            bool requestCreateAbove( std::vector<size_t> &eligible_surfaces );
            bool requestCreateBelow( std::vector<size_t> &eligible_surfaces );




            /* Change the model's properties */



            bool setLowResolution();

            bool setMediumResolution();

            bool setHighResolution();


            void setOrigin( double opengl_x, double opengl_y, double opengl_z );

            bool setLenght( double opengl_x, double opengl_y, double opengl_z );


            /* Begin methods to interface with GUI */


            std::size_t getWidthResolution();

            std::size_t getDepthResolution();


            /* Clean up */
            void clear();


            /* Query or modify the automatum state */


            //
            // brief:
            // Define new input region above surface which index is `surface_index`.
            //
            bool defineAbove( size_t surface_index );


            //
            // brief:
            // Clear any previous `defineAbove()` call.
            // Safe to call anytime.
            //
            void stopDefineAbove();

            //
            // brief:
            // Define new input region below surface which index is `surface_index`.
            //
            bool defineBelow( size_t surface_index );

            //
            // brief:
            // Clear any previous `defineBelow()` call.
            // Safe to call anytime.
            //
            void stopDefineBelow();

            bool defineAboveIsActive();
            bool defineBelowIsActive();


            void removeAbove();
            void removeAboveIntersection();

            void removeBelow();
            void removeBelowIntersection();


            template<typename CurveType>
            bool createSurface( size_t surface_index, const std::vector< std::tuple< CurveType, double  > > &curves );

            template<typename CurveType>
            bool extrudeAlongPath( size_t surface_index,
                    const CurveType &cross_section, double cross_section_depth, 
                    const CurveType &path, 
                    size_t num_extrusion_steps = 32 
                    );

            bool canUndo();
            bool undo();

            bool canRedo();
            bool redo();


            /* Get mesh, pcl and curves for visualization */

            bool getMesh( size_t surface_id, std::vector<float> &vlist, std::vector<size_t> &flist );
            bool getMesh( size_t surface_id, std::vector<double> &vlist, std::vector<size_t> &flist );

            /* template<typename VertexList, typename FaceList> */
            /* bool getMesh( size_t surface_id, VertexList &vlist, FaceList &flist ); */

            bool getCrossSection( size_t surface_id, std::size_t depth, std::vector<float> &vlist, std::vector<size_t> &elist );
            bool getCrossSection( size_t surface_id, std::size_t depth, std::vector<double> &vlist, std::vector<size_t> &elist );
            /* template<typename VertexList, typename EdgeList> */
            /* bool getCrossSection( size_t surface_id, std::size_t depth, VertexList &vlist, EdgeList &elist ); */



            /* End methods to interface with GUI */


        private:

            StratigraphyModeller modeller_;
            struct { double x, y, z; } origin_, lenght_;

    };

    template<typename CurveType>
    bool RulesProcessor::createSurface( size_t surface_index, const std::vector< std::tuple< CurveType, double  > > &curves )
    {
        std::vector<double> surface;
        size_t num_cross_sections = 0;

        for ( auto &curve_tuple : curves )
        {
            num_cross_sections++;

            auto &in_curve = std::get<0>(curve_tuple);
            auto &in_curve_depth = std::get<1>(curve_tuple);

            for ( size_t i = 0; i < in_curve.size(); ++i )
            {
                surface.push_back(in_curve[i].x());
                surface.push_back(in_curve[i].y());
                surface.push_back(in_curve_depth);
            }
        }

        bool status = false;

        if ( num_cross_sections > 1 )
        {
            status = modeller_.createSurface( surface_index, surface );
        }

        else if ( num_cross_sections == 1 )
        {
            status = modeller_.createExtrudedSurface( surface_index, surface );
        }

        return status;
    }

    template<typename CurveType>
    bool RulesProcessor::extrudeAlongPath(  size_t surface_index, 
        const CurveType &cross_section_curve, double cross_section_depth, 
        const CurveType &path_curve, 
        size_t num_extrusion_steps
        )
    {
        /* std::cout << "Got into extrudeAlongPath()\n\n" << std::flush; */
        StratigraphyUtilities util(modeller_);
        std::vector<double> surface;

        std::vector<double> cross_section;
        /* std::cout << "RulesProcessor: cross section size = " << cross_section_curve.size() << "\n\n"; */
        for ( size_t i = 0; i < cross_section_curve.size(); ++i )
        {
            cross_section.push_back( cross_section_curve[i].x() );
            cross_section.push_back( cross_section_curve[i].y() );
        }

        std::vector<double> path;
        /* std::cout <<  "RulesProcessor: path size = " << path_curve.size() << "\n\n"; */
        for ( size_t i = 0; i < path_curve.size(); ++i )
        {
            path.push_back( path_curve[i].x() );
            path.push_back( path_curve[i].y() );
        }

        /* util.extrudeAlongPath(surface, cross_section, cross_section_depth, path, num_extrusion_steps, 32); */
        /* std::cout << "Creating surface "<< surface_index << " with " << surface.size()/3.0 << " points.\n\n"; */
        /* std::cout << std::flush; */
        bool status = modeller_.createSurface(surface_index, surface);

        /* std::cout << "Result of creating extrudeAlongPath(): " << status << "\n\n" << std::flush; */

        return status;
    }

    /* template<typename VertexList, typename FaceList> */
    /* bool RulesProcessor::getMesh(size_t surface_index, VertexList &vlist, FaceList &flist) */
    /* { */

    /* return modeller_.getMesh(surface_index, vlist, flist); */
    /* } */

    /* template<typename VertexList, typename EdgeList> */
    /* bool RulesProcessor::getCrossSection(size_t surface_id, std::size_t depth, VertexList &vlist, EdgeList &elist) */
    /* { */
    /*     return modeller_.getCrossSectionDepth(surface_id, vlist, elist, depth); */
    /* } */

//{} // End of Namespace RRM

#endif
