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

//<<<<<<< HEAD:src/apps/mainwindow/rules_processor.hpp
////#include "core/geometry/PolygonalCurve/polygonal_curve_2d.hpp"
//#include "PolygonalCurve/CurveN.hpp"
//#include "stratmod/stratigraphy_modeller.hpp"
//#include "stratmod/stratigraphy_utilities.hpp"
//=======
/* #include "Core/Geometry/PolygonalCurve/polygonal_curve_2d.hpp" */
/* #include "stratmod_wraper.hpp" */
/* #include "smodeller.hpp" */
/* #include "sutilities.hpp" */
//>>>>>>> origin/feature-stratmod_ref:src/Apps/MainWindow/rules_processor.hpp

#include "./libs/PolygonalCurve/CurveN.hpp"
#include "stratmod_wrapper.hpp"


//namespace RRM
//{}

    class RulesProcessor {

        public:

            struct TriangleMesh
            {
                std::vector<double> vertex_list;
                std::vector<std::size_t> face_list;
            };

            struct CurveMesh
            {
                std::vector<double> vertex_list;
                std::vector<std::size_t> edge_list;
            };


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

            void getOrigin( double &opengl_x, double &opengl_y, double &opengl_z );

            void getLenght( double &opengl_x, double &opengl_y, double &opengl_z );

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

//            bool defineAboveIsActive( std::size_t& index );
//            bool defineBelowIsActive( std::size_t& index );


            bool defineAboveIsActive();
            bool defineBelowIsActive();

            bool defineAboveIsActive( size_t &boundary_index );
            bool defineBelowIsActive( size_t &boundary_index );

            void removeAbove();
            void removeAboveIntersection();

            void removeBelow();
            void removeBelowIntersection();

            void truncate();


            template<typename CurveType>
            bool createSurface( size_t surface_index, const std::vector< std::tuple< CurveType, double  > > &curves );


            template<typename CurveType>
            bool testSurface( size_t surface_index, const std::vector< std::tuple< CurveType, double  > > &curves );


            template<typename CurveType>
            bool extrudeAlongPath( size_t surface_index,
                    const CurveType &cross_section, double cross_section_depth,
                    const CurveType &path
                    /* size_t num_extrusion_steps = 32 */
                    );

            bool canUndo();
            bool undo();

            bool canRedo();
            bool redo();


            /* Get mesh, pcl and curves for visualization */

            bool getMesh( size_t surface_id, std::vector<float> &vlist, std::vector<size_t> &flist );
            bool getMesh( size_t surface_id, std::vector<double> &vlist, std::vector<size_t> &flist );
            bool getNormals( size_t surface_id, std::vector<double> &nlist );

            bool getExtrusionPath( size_t surface_index, std::vector<double> &path);


            /* template<typename VertexList, typename FaceList> */
            /* bool getMesh( size_t surface_id, VertexList &vlist, FaceList &flist ); */

            bool getCrossSection( size_t surface_id, std::size_t depth, std::vector<float> &vlist, std::vector<size_t> &elist );
            bool getCrossSection( size_t surface_id, std::size_t depth, std::vector<double> &vlist, std::vector<size_t> &elist );
            /* template<typename VertexList, typename EdgeList> */
            /* bool getCrossSection( size_t surface_id, std::size_t depth, VertexList &vlist, EdgeList &elist ); */


            bool saveFile( std::string filename );
            bool loadFile( std::string filename );
            size_t getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps );
            /* End methods to interface with GUI */

            bool getLeftBoundaryCrossSectionCurve( std::vector< std::vector<double> >& vertices,  std::vector< std::vector<std::size_t> >& edges );
            bool getRightBoundaryCrossSectionCurve( std::vector< std::vector<double> >& vertices,  std::vector< std::vector<std::size_t> >& edges );
            bool getFrontBoundaryCrossSectionCurve( std::vector< std::vector<double> >& vertices,  std::vector< std::vector<std::size_t> >& edges );
            bool getBackBoundaryCrossSectionCurve( std::vector< std::vector<double> >& vertices,  std::vector< std::vector<std::size_t> >& edges );

            bool getTetrahedralMesh( std::vector<double> &vertex_coordinates, std::vector< std::vector<std::size_t> > &element_list );

            bool setPLCForSimulation( std::vector< TriangleMesh >& triangle_meshes,
                                    std::vector< CurveMesh >& left_curves,
                                    std::vector< CurveMesh >& right_curves,
                                    std::vector< CurveMesh > & front_curves,
                                    std::vector< CurveMesh >& back_curves, 
                                    std::size_t length_discretization = 16, 
                                    std::size_t width_discretization = 16 
                                    );

            bool getRegionsForSimulationTetrahedralMesh( const std::vector<double> &vertex_coordinates, const std::vector<std::size_t> &element_list, std::vector<int> &regions );

        private:
            SModellerWrapper modeller_;
            struct { double x, y, z; } origin_, lenght_;

            enum ModelResolution { LOW, MEDIUM, HIGH };
            ModelResolution current_resolution_;

            bool testing_surface_insertion_ = false;
            bool truncate_surface_ = false;

            template<typename CurveType>
            bool getFirstRegionCurveIntersects( const std::tuple<CurveType, double> &curve_tuple, std::vector<size_t> &lbounds, std::vector<size_t> &ubounds );

    };

    template<typename CurveType>
    bool RulesProcessor::getFirstRegionCurveIntersects( const std::tuple<CurveType, double> &curve_tuple, std::vector<size_t> &lbounds, std::vector<size_t> &ubounds )
    {
        auto &in_curve = std::get<0>(curve_tuple);
        auto &in_curve_depth = std::get<1>(curve_tuple);
        lbounds = {};
        ubounds = {};

        if ( in_curve.size() < 2 )
        {
            return false;
        }

        auto reduce_region = []( const std::vector<size_t> &c ) -> size_t {
            size_t num = 0;
            for ( auto &i : c )
            {
                num += std::pow(2, i);
            }

            return num;
        };

        std::vector<size_t> cur_region, region, cur_lbounds, cur_ubounds;
        bool first_cross = false, second_cross = false;
        size_t first_cross_index;

        SUtilitiesWrapper u(modeller_);

        std::cout << "getting intersecting surfaces indices: \n";

        /* region = u.getSurfacesIndicesBelowPoint(in_curve[0].x(), in_curve[0].y(), in_curve_depth); */
        lbounds = u.getSurfacesIndicesBelowPoint(in_curve[0].x(), in_curve[0].y(), in_curve_depth);
        ubounds = u.getSurfacesIndicesAbovePoint(in_curve[0].x(), in_curve[0].y(), in_curve_depth);

        for ( size_t i = 1; i < in_curve.size(); ++i )
        {
            std::cout << "For point: " << in_curve[i].x() << ", " << in_curve[i].y() << ", " << in_curve_depth << "\n";
            cur_lbounds = u.getSurfacesIndicesBelowPoint(in_curve[i].x(), in_curve[i].y(), in_curve_depth);
            cur_ubounds = u.getSurfacesIndicesAbovePoint(in_curve[i].x(), in_curve[i].y(), in_curve_depth);

            std::cout << "Lower boundaries: ";
            for ( auto i : cur_lbounds )
            {
                std::cout << i << ", ";
            }
            std::cout << "\n";
            std::cout << "Upper boundaries: ";
            for ( auto i : cur_ubounds )
            {
                std::cout << i << ", ";
            }
            std::cout << "\n";
            
            /* std::cout << "Index: " << i << ", current: " << reduce_region(cur_region) << ", previous: " << reduce_region(region) << "\n"; */

            if ( (cur_lbounds != lbounds) || (cur_ubounds != ubounds) )
            {
                if ( first_cross == false )
                {
                    lbounds = cur_lbounds;
                    ubounds = cur_ubounds;
                    first_cross_index = i;
                    first_cross = true;
                    std::cout << "First cross at index " << i << "\n"; 
                }
                else 
                {
                    {
                        second_cross = true;
                        std::cout << "Second cross at index " << i << "\n"; 
                        break; // for loop
                    }
                }
            }
        }

        if ( first_cross && second_cross )
        {
            /* lbounds = region; */
            /* size_t i = first_cross_index; */
            /* ubounds = u.getSurfacesIndicesAbovePoint(in_curve[i].x(), in_curve[i].y(), in_curve_depth); */
            std::cout << "Lower boundaries: ";
            for ( auto i : lbounds )
            {
                std::cout << i << ", ";
            }
            std::cout << "\n";
            std::cout << "Upper boundaries: ";
            for ( auto i : ubounds )
            {
                std::cout << i << ", ";
            }
            std::cout << "\n";

            return true;
        }

        return false;
    }

    template<typename CurveType>
    bool RulesProcessor::testSurface( size_t surface_index, const std::vector< std::tuple< CurveType, double  > > &curves )
    {
        modeller_.disableGeologicRules();
        bool status = createSurface(surface_index, curves);
        testing_surface_insertion_ = true;

        return status;
    }

    template<typename CurveType>
    bool RulesProcessor::createSurface( size_t surface_index, const std::vector< std::tuple< CurveType, double  > > &curves )
    {
        if ( testing_surface_insertion_ == true )
        {
            modeller_.undo();
        }

        std::vector<size_t> lbounds, ubounds;
        std::vector<size_t> intersected_surfaces;

        std::vector<double> surface;
        size_t num_cross_sections = curves.size();

        bool status = false;
        std::cout << "current surfaces: ";
        auto surfs = getSurfaces();
        for ( auto i : surfs )
        {
            std::cout << i << ", ";
        }
        std::cout << "\n";

        if ( num_cross_sections > 1 )
        {
            for ( auto &curve_tuple : curves )
            {
                auto &in_curve = std::get<0>(curve_tuple);
                auto &in_curve_depth = std::get<1>(curve_tuple);

                for ( size_t i = 0; i < in_curve.size(); ++i )
                {
                    surface.push_back(in_curve[i].x());
                    surface.push_back(in_curve[i].y());
                    surface.push_back(in_curve_depth);
                }
            }

            if ( truncate_surface_ )
            {
                status = modeller_.tryCreateSurface( surface_index, intersected_surfaces, surface );
                if ( intersected_surfaces.size() > 0 )
                {
                    status = getFirstRegionCurveIntersects( curves.front(), lbounds, ubounds );
                    if ( status == false )
                    {
                        return false;
                    }

                    status = modeller_.createSurface( surface_index, surface, lbounds, ubounds );
                }
            }
            else
            {
                status = modeller_.createSurface( surface_index, surface );
            }

            /* if ( intersected_surfaces.size() > 0 ) */
            /* { */
            /*     std::cout << "Intersected at least surfaces: " */ 
            /*         << intersected_surfaces.front() << ", " */ 
            /*         << intersected_surfaces.back() << "\n"; */ 
                    
            /*     getFirstRegionCurveIntersects( curves.front(), lbounds, ubounds ); */
            /*     std::cout << "First lower bound: " << (lbounds.empty() ? -1 : (int)lbounds.front()) << "\n"; */
            /*     std::cout << "First upper bound: " << (ubounds.empty() ? -1 : (int)ubounds.front()) << "\n"; */

            /*     status = modeller_.createSurface( surface_index, surface, lbounds, ubounds ); */
            /* } */
        }

        else if ( num_cross_sections == 1 )
        {
            std::cout << "Inside createSurface() \n";
            auto &curve_tuple = curves.front();

            auto &in_curve = std::get<0>(curve_tuple);
            auto &in_curve_depth = std::get<1>(curve_tuple);

            for ( size_t i = 0; i < in_curve.size(); ++i )
            {
                surface.push_back(in_curve[i].x());
                surface.push_back(in_curve[i].y());
            }

            if ( truncate_surface_ )
            {
                std::cout << "Truncate is active.\n";
                status = modeller_.tryCreateLengthwiseExtrudedSurface( surface_index, intersected_surfaces, surface );
                if ( intersected_surfaces.size() > 0 )
                {
                    std::cout << "Surface is not valid and status is: " << status << ", it intersects surfaces:";
                    for (int s = 0; s < intersected_surfaces.size(); ++s)
                        std::cout << intersected_surfaces[s] << ", ";
                    std::cout << "\n";

                    status = getFirstRegionCurveIntersects( curve_tuple, lbounds, ubounds );
                    if ( status == false )
                    {
                        return false;
                    }

                    std::vector<size_t> lb_intersect, ub_intersect;
                    std::set_intersection(intersected_surfaces.begin(), intersected_surfaces.end(), lbounds.begin(), lbounds.end(), std::back_inserter(lb_intersect));
                    std::set_intersection(intersected_surfaces.begin(), intersected_surfaces.end(), ubounds.begin(), ubounds.end(), std::back_inserter(ub_intersect));
                    status = modeller_.createLengthwiseExtrudedSurface( surface_index, surface, lb_intersect, ub_intersect );
                    std::cout << "Final status is: " << status << "\n";
                }
            }
            else
            {
                std::cout << "truncate is not active.\n";
                status = modeller_.createLengthwiseExtrudedSurface( surface_index, surface );
                std::cout << "Final status is: " << status << "\n";
                /* status = modeller_.createSurface( surface_index, surface ); */
            }
            /* status = modeller_.createLengthwiseExtrudedSurface( surface_index, surface ); */
        }

        testing_surface_insertion_ = false;

        return status;
    }

    template<typename CurveType>
    bool RulesProcessor::extrudeAlongPath(  size_t surface_index,
        const CurveType &cross_section_curve, double cross_section_depth,
        const CurveType &path_curve
        /* size_t num_extrusion_steps */
        )
    {
        if ( testing_surface_insertion_ == true )
        {
            modeller_.undo();
        }

        SUtilitiesWrapper util(modeller_);
        std::vector<double> surface;

        std::vector<double> cross_section;
        for ( size_t i = 0; i < cross_section_curve.size(); ++i )
        {
            cross_section.push_back( cross_section_curve[i].x() );
            cross_section.push_back( cross_section_curve[i].y() );
        }

        std::vector<double> path;
        for ( size_t i = 0; i < path_curve.size(); ++i )
        {
            path.push_back( path_curve[i].x() );
            path.push_back( path_curve[i].y() );
        }

        bool status;

        if ( truncate_surface_ )
        {
            std::vector<size_t> lbounds, ubounds;
            std::vector<size_t> intersected_surfaces;

            status = modeller_.tryCreateLengthwiseExtrudedSurface( surface_index, intersected_surfaces, cross_section, cross_section_depth, path );

            if ( intersected_surfaces.size() > 0 )
            {
                std::tuple<CurveType, double> curve_tuple = std::make_pair(cross_section_curve, cross_section_depth);
                status = getFirstRegionCurveIntersects( curve_tuple, lbounds, ubounds );
                if ( status == false )
                {
                    return false;
                }

                std::vector<size_t> lb_intersect, ub_intersect;
                std::set_intersection(intersected_surfaces.begin(), intersected_surfaces.end(), lbounds.begin(), lbounds.end(), std::back_inserter(lb_intersect));
                std::set_intersection(intersected_surfaces.begin(), intersected_surfaces.end(), ubounds.begin(), ubounds.end(), std::back_inserter(ub_intersect));

                status = modeller_.createLengthwiseExtrudedSurface( surface_index, cross_section, cross_section_depth, path, lb_intersect, ub_intersect );
            }
        }
        else
        {
            status = modeller_.createLengthwiseExtrudedSurface(surface_index, cross_section, cross_section_depth, path);
        }

        testing_surface_insertion_ = false;

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
