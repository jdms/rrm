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


#ifndef RRM_CORE_MODELS_RULES_PROCESSOR
#define RRM_CORE_MODELS_RULES_PROCESSOR

#include <iostream>
#include <vector>
#include <map>

#include "./libs/PolygonalCurve/CurveN.hpp"
#include "stratmod_wrapper.hpp"
/* #include "truncate_helper.hpp" */

#ifdef _WIN32
    #define RULES_PROCESSOR_DEPRECATED_METHOD
#else
    #define RULES_PROCESSOR_DEPRECATED_METHOD [[deprecated]]
#endif

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

            stratmod::SModeller& getSModeller();

            std::vector<std::size_t> getSurfaces();
            std::vector<std::size_t> getActiveSurfaces();

            std::vector<std::size_t> getOrderedSurfaces();
            std::vector<std::size_t> getOrderedActiveSurfaces();

            //
            // brief:
            // Verifies whether ir is possible to define a new 'drawing' region,
            // either above or below.
            // Argument: `eligible_surfaces` stores the indices of surfaces that can
            // be used as input either for an `defineAbove()` or a `defineBelow()`.
            // Return: true if at least one elegible surface was found.
            //
            /* bool requestDefineRegion( std::vector<size_t> &eligible_surfaces ); */

            RULES_PROCESSOR_DEPRECATED_METHOD
            bool requestCreateAbove( std::vector<size_t> &eligible_surfaces );

            RULES_PROCESSOR_DEPRECATED_METHOD
            bool requestCreateBelow( std::vector<size_t> &eligible_surfaces );




            /* Change the model's properties */


            RULES_PROCESSOR_DEPRECATED_METHOD
            bool setLowResolution();

            RULES_PROCESSOR_DEPRECATED_METHOD
            bool setMediumResolution();

            RULES_PROCESSOR_DEPRECATED_METHOD
            bool setHighResolution();

            RULES_PROCESSOR_DEPRECATED_METHOD
            bool isLowResolution();

            RULES_PROCESSOR_DEPRECATED_METHOD
            bool isMediumResolution();

            RULES_PROCESSOR_DEPRECATED_METHOD
            bool isHighResolution();

            RULES_PROCESSOR_DEPRECATED_METHOD
            bool setModellingResolution( std::size_t width = 64, std::size_t length = 64 );

            RULES_PROCESSOR_DEPRECATED_METHOD
            bool setDiagnosticsResolution( std::size_t width = 16, std::size_t length = 16);

            void setOrigin( double opengl_x, double opengl_y, double opengl_z );

            bool setLenght( double opengl_x, double opengl_y, double opengl_z );

            void getOrigin( double &opengl_x, double &opengl_y, double &opengl_z );

            void getLenght( double &opengl_x, double &opengl_y, double &opengl_z );

            /* Begin methods to interface with GUI */


            std::size_t getWidthResolution() const;

            // DEPRECATED
            RULES_PROCESSOR_DEPRECATED_METHOD
            std::size_t getDepthResolution();
            
            // USE THIS INSTEAD:
            std::size_t getLengthResolution() const;
            // END: use this instead


            /* Clean up */
            void clear();


            /* Query or modify the automatum state */


            /////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////
            //  All old define[Above,Below] methods have become deprecated as 
            //  we move to allow structural rules on models.  
            //
            //  Refer to new preserve[Above, Below] methods
            /////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////

            //
            // brief:
            // Define new input region above surface which index is `surface_index`.
            //
            // DEPRECATED
            RULES_PROCESSOR_DEPRECATED_METHOD
            bool defineAbove( size_t surface_index );
            
            // USE THIS INSTEAD
            RULES_PROCESSOR_DEPRECATED_METHOD
            bool defineAbove( std::vector<size_t> &surface_indices );
            // END: use this instead


            //
            // brief:
            // Clear any previous `defineAbove()` call.
            // Safe to call anytime.
            //
            RULES_PROCESSOR_DEPRECATED_METHOD
            void stopDefineAbove();

            //
            // brief:
            // Define new input region below surface which index is `surface_index`.
            //
            // DEPRECATED
            RULES_PROCESSOR_DEPRECATED_METHOD
            bool defineBelow( size_t surface_index );
            
            // DEPRECATED
            RULES_PROCESSOR_DEPRECATED_METHOD
            bool defineBelow( std::vector<size_t> surface_indices );
            // END: use this instead

            //
            // brief:
            // Clear any previous `defineBelow()` call.
            // Safe to call anytime.
            //
            RULES_PROCESSOR_DEPRECATED_METHOD
            void stopDefineBelow();

//            bool defineAboveIsActive( std::size_t& index );
//            bool defineBelowIsActive( std::size_t& index );


            RULES_PROCESSOR_DEPRECATED_METHOD
            bool defineAboveIsActive();
            RULES_PROCESSOR_DEPRECATED_METHOD
            bool defineBelowIsActive();

            // DEPRECATED
            RULES_PROCESSOR_DEPRECATED_METHOD
            bool defineAboveIsActive( size_t &boundary_index );
            RULES_PROCESSOR_DEPRECATED_METHOD
            bool defineBelowIsActive( size_t &boundary_index );

            // DEPRECATED
            RULES_PROCESSOR_DEPRECATED_METHOD
            bool defineAboveIsActive( std::vector<size_t> &boundary_indices );
            RULES_PROCESSOR_DEPRECATED_METHOD
            bool defineBelowIsActive( std::vector<size_t> &boundary_indices );
            // END: use this instead

            /////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////////////
            //
            // New structure aware preserve[Above, Below]
            //
            /////////////////////////////////////////////////////////////////////////////

            void stopPreserveAbove();
            void stopPreserveBelow();
            void stopPreserveRegion();

            bool preserveAboveIsActive();
            bool preserveBelowIsActive();
            /* bool preserveRegionIsActive(); */

            bool requestPreserveRegion( std::vector<double> &point );
            bool requestPreserveAbove( std::vector<double> &curve_points );
            bool requestPreserveBelow( std::vector<double> &curve_points );

            bool preserveAbove( std::vector<std::size_t> &surface_indices_list );
            bool preserveBelow( std::vector<std::size_t> &surface_indices_list );

            // DEPRECATED
            bool getModelAboveSurface( std::vector<double> &curve_points, std::vector<size_t> &surface_indices_list );
            bool getModelBelowSurface( std::vector<double> &curve_points, std::vector<size_t> &surface_indices_list );

            RULES_PROCESSOR_DEPRECATED_METHOD
            bool getUpperBoundaryMesh( std::vector<float> &vlist, std::vector<size_t> &flist );
            bool getUpperBoundaryMesh( std::vector<double> &vlist, std::vector<size_t> &flist );

            RULES_PROCESSOR_DEPRECATED_METHOD
            bool getLowerBoundaryMesh( std::vector<float> &vlist, std::vector<size_t> &flist );
            bool getLowerBoundaryMesh( std::vector<double> &vlist, std::vector<size_t> &flist );


            RULES_PROCESSOR_DEPRECATED_METHOD
            bool getUpperBoundaryLengthwiseCrossSection( size_t cross_sec, std::vector<float> &vlist, std::vector<size_t> &flist );
            bool getUpperBoundaryLengthwiseCrossSection( size_t cross_sec, std::vector<double> &vlist, std::vector<size_t> &flist );

            RULES_PROCESSOR_DEPRECATED_METHOD
            bool getUpperBoundaryWidthwiseCrossSection( size_t cross_sec, std::vector<float> &vlist, std::vector<size_t> &flist );
            bool getUpperBoundaryWidthwiseCrossSection( size_t cross_sec, std::vector<double> &vlist, std::vector<size_t> &flist );


            RULES_PROCESSOR_DEPRECATED_METHOD
            bool getLowerBoundaryLengthwiseCrossSection( size_t cross_sec, std::vector<float> &vlist, std::vector<size_t> &flist );
            bool getLowerBoundaryLengthwiseCrossSection( size_t cross_sec, std::vector<double> &vlist, std::vector<size_t> &flist );

            RULES_PROCESSOR_DEPRECATED_METHOD
            bool getLowerBoundaryWidthwiseCrossSection( size_t cross_sec, std::vector<float> &vlist, std::vector<size_t> &flist );
            bool getLowerBoundaryWidthwiseCrossSection( size_t cross_sec, std::vector<double> &vlist, std::vector<size_t> &flist );
            // END DEPRECATED

            // KK: please, use these instead of get[Lower,Upper]Boundary...
            bool getPreserveAboveCurveBoxAtLength( size_t length, std::vector<double> &vlist, std::vector<size_t> &flist );
            bool getPreserveBelowCurveBoxAtLength( size_t length, std::vector<double> &vlist, std::vector<size_t> &flist );

            bool getPreserveAboveCurveBoxAtWidth( size_t width, std::vector<double> &vlist, std::vector<size_t> &flist );
            bool getPreserveBelowCurveBoxAtWidth( size_t width, std::vector<double> &vlist, std::vector<size_t> &flist );

            // KK: these allow you to paint the 2D cross-sections with a region color
            bool getRegionCurveBoxesAtLength( std::size_t region_id, std::size_t length, 
                    std::vector<double> &lower_bound_box_vlist, std::vector<std::size_t> &lower_bound_box_elist,
                    std::vector<double> &upper_bound_box_vlist, std::vector<std::size_t> &upper_bound_box_elist );

            bool getRegionCurveBoxesAtWidth( std::size_t region_id, std::size_t width, 
                    std::vector<double> &lower_bound_box_vlist, std::vector<std::size_t> &lower_bound_box_elist,
                    std::vector<double> &upper_bound_box_vlist, std::vector<std::size_t> &upper_bound_box_elist );


            // KK: these are internal methods, for my own use
            bool getModelInfCurveAtLength( std::vector<std::size_t> &surface_indices, std::size_t length, 
                    std::vector<double> &vlist, std::vector<std::size_t> &elist );

            bool getModelSupCurveAtLength( std::vector<std::size_t> &surface_indices, std::size_t length, 
                    std::vector<double> &vlist, std::vector<std::size_t> &elist );

            bool getModelInfCurveAtWidth( std::vector<std::size_t> &surface_indices, std::size_t width, 
                    std::vector<double> &vlist, std::vector<std::size_t> &elist );

            bool getModelSupCurveAtWidth( std::vector<std::size_t> &surface_indices, std::size_t width, 
                    std::vector<double> &vlist, std::vector<std::size_t> &elist );

            /////////////////////////////////////////////////////////////////////////////




            void removeAbove();
            void removeAboveIntersection();

            void removeBelow();
            void removeBelowIntersection();

            // DEPRECATED
            RULES_PROCESSOR_DEPRECATED_METHOD
            void truncate();


            // DEPRECATED: a new signature for this function must be discussed
            template<typename CurveType>
            RULES_PROCESSOR_DEPRECATED_METHOD
            bool createSurface( size_t surface_index, std::vector< std::tuple< CurveType, double  > > &curves );

            // SUGGESTED NEW 
            bool createSurface( size_t surface_index, std::vector<double> &points );


            // DEPRECATED: a new signature for this function must be discussed
            template<typename CurveType>
            RULES_PROCESSOR_DEPRECATED_METHOD
            bool testSurface( size_t surface_index, std::vector< std::tuple< CurveType, double  > > &curves );

            // NEW BUT ALREADY DEPRECATED
            RULES_PROCESSOR_DEPRECATED_METHOD
            bool testSurface( size_t surface_index, std::vector<double> &points );

            // SUGGESTED NEW 
            void testSurfaceInsertion();
            void stopTestSurfaceInsertion();

            // DEPRECATED: a new signature for this function must be discussed
            template<typename CurveType>
            RULES_PROCESSOR_DEPRECATED_METHOD
            bool extrudeAlongPath( size_t surface_index,
                    const CurveType &cross_section, double cross_section_depth,
                    const CurveType &path
                    /* size_t num_extrusion_steps = 32 */
                    );

            // SUGGESTED NEW SIGNATURES:
            bool createLengthwiseExtrudedSurface( size_t surface_id, 
                    const std::vector<double> &cross_section_curve_point_data
                    );

            bool createLengthwiseExtrudedSurface( size_t surface_id, 
                    const std::vector<double> &cross_section_curve_point_data, double cross_section_depth, 
                    const std::vector<double> &path_curve_point_data 
                    );

            bool createWidthwiseExtrudedSurface( size_t surface_id, 
                    const std::vector<double> &cross_section_curve_point_data
                    );

            bool createWidthwiseExtrudedSurface( size_t surface_id, 
                    const std::vector<double> &cross_section_curve_point_data, double cross_section_depth, 
                    const std::vector<double> &path_curve_point_data 
                    );

            // END SUGGESTED NEW SIGNATURES

            bool canUndo();
            bool undo();

            bool canRedo();
            bool redo();


            /* Get mesh, pcl and curves for visualization */

            RULES_PROCESSOR_DEPRECATED_METHOD
            bool getMesh( size_t surface_id, std::vector<float> &vlist, std::vector<size_t> &flist );
            bool getMesh( size_t surface_id, std::vector<double> &vlist, std::vector<size_t> &flist );
            bool getNormals( size_t surface_id, std::vector<double> &nlist );

            // DEPRECATED? We have to discuss the meaning of the 'path' object
            bool getExtrusionPath( size_t surface_index, std::vector<double> &path);


            /* template<typename VertexList, typename FaceList> */
            /* bool getMesh( size_t surface_id, VertexList &vlist, FaceList &flist ); */

            // DEPRECATED
            RULES_PROCESSOR_DEPRECATED_METHOD
            bool getCrossSection( size_t surface_id, std::size_t depth, std::vector<float> &vlist, std::vector<size_t> &elist );
            bool getCrossSection( size_t surface_id, std::size_t depth, std::vector<double> &vlist, std::vector<size_t> &elist );
            /* template<typename VertexList, typename EdgeList> */
            /* bool getCrossSection( size_t surface_id, std::size_t depth, VertexList &vlist, EdgeList &elist ); */

            // USE THIS INSTEAD
            RULES_PROCESSOR_DEPRECATED_METHOD
            bool getWidthCrossSectionCurve( size_t surface_id, size_t width, std::vector<float> &vlist, std::vector<size_t> &elist );
            bool getWidthCrossSectionCurve( size_t surface_id, size_t width, std::vector<double> &vlist, std::vector<size_t> &elist );

            RULES_PROCESSOR_DEPRECATED_METHOD
            bool getLengthCrossSectionCurve( size_t surface_id, size_t lenght, std::vector<float> &vlist, std::vector<size_t> &elist );
            bool getLengthCrossSectionCurve( size_t surface_id, size_t lenght, std::vector<double> &vlist, std::vector<size_t> &elist );
            // END: use this instead



            bool saveFile( std::string filename );
            bool loadFile( std::string filename );

            // DEPRECATED?
            size_t getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps );
            /* End methods to interface with GUI */

            bool getLeftBoundaryCrossSectionCurve( std::vector< std::vector<double> >& vertices,  std::vector< std::vector<std::size_t> >& edges );
            bool getRightBoundaryCrossSectionCurve( std::vector< std::vector<double> >& vertices,  std::vector< std::vector<std::size_t> >& edges );
            bool getFrontBoundaryCrossSectionCurve( std::vector< std::vector<double> >& vertices,  std::vector< std::vector<std::size_t> >& edges );
            bool getBackBoundaryCrossSectionCurve( std::vector< std::vector<double> >& vertices,  std::vector< std::vector<std::size_t> >& edges );

            bool getTetrahedralMesh( std::vector< std::vector<double> > &vertex_coordinates, std::vector< std::vector<std::size_t> > &element_list );

            void getRegionVolumeList( std::vector<double>& volumes );

            bool setPLCForSimulation( std::vector< TriangleMesh >& triangle_meshes,
                                    std::vector< CurveMesh >& left_curves,
                                    std::vector< CurveMesh >& right_curves,
                                    std::vector< CurveMesh > & front_curves,
                                    std::vector< CurveMesh >& back_curves, 
                                    std::size_t length_discretization = 16, 
                                    std::size_t width_discretization = 16 
                                    );

            bool getRegionsForSimulationTetrahedralMesh( const std::vector<double> &vertex_coordinates, const std::vector<std::size_t> &element_list, std::vector<int> &regions );

            std::vector<int> getRegionsColor( std::size_t numColors );

            bool getQuadMesh( std::size_t surface_id, std::vector<double> &points, std::vector<bool> &valid_points, std::size_t &num_width, std::size_t &num_length );

            using Att2RegMap = std::vector<int>;
            using Reg2AttMap = std::vector<int>;
            std::pair<Att2RegMap, Reg2AttMap> getI2VRegionMaps();

        private:
            SModellerWrapper& modeller_;
            struct { double x, y, z; } origin_, length_;

            enum ModelResolution { LOW, MEDIUM, HIGH };
            ModelResolution current_resolution_;
            std::size_t modelling_length_discretization_ = 64;
            std::size_t modelling_width_discretization_ = 64;

            std::size_t diagnostics_length_discretization_ = 32;
            std::size_t diagnostics_width_discretization_ = 32;

            bool testing_surface_insertion_ = false;
            bool last_surface_inserted_is_a_test_ = false;

            bool pa_is_active_ = false;
            bool pb_is_active_ = false;

            std::vector<size_t> lower_model_ = std::vector<size_t>();
            std::vector<size_t> upper_model_ = std::vector<size_t>();

            // DEPRECATED
            bool truncate_surface_ = false; 

            /* template<typename CurveType, typename T = std::vector<size_t>> */
            /* bool getFirstRegionCurveIntersects( const std::tuple<CurveType, double> &curve_tuple, std::vector<size_t> &lbounds, std::vector<size_t> &ubounds, T &&crossings = {} ); */

            std::vector<std::size_t> filterActiveSurfaces( std::vector<std::size_t> &surface_ids );

            void enforcePreserveRegion();

            template<typename FunctionType, typename... Args>
            bool processSurfaceCreation( FunctionType &&surfaceCreator, size_t surface_index, Args&&... args );
            
            template<typename CurveType>
            bool getFirstRegionCurveIntersects( 
                        const std::tuple<CurveType, double> &curve_tuple, 
                        std::vector<size_t> &lbounds, 
                        std::vector<size_t> &ubounds, 
                        size_t &first_index, 
                        size_t &second_index
                        );

            template<typename CurveType>
            bool truncateInputCurve( const std::vector<size_t> &intersected_surfaces, std::tuple<CurveType, double> &curve_tuple, size_t first_index, size_t second_index );

    };

    template<typename CurveType>
    bool RulesProcessor::truncateInputCurve( const std::vector<size_t> &intersect_surf, std::tuple<CurveType, double> &curve_tuple, size_t first, size_t second )
    {
        return false;
    }

    template<typename CurveType>
    bool RulesProcessor::getFirstRegionCurveIntersects( 
            const std::tuple<CurveType, double> &curve_tuple, 
            std::vector<size_t> &lbounds, 
            std::vector<size_t> &ubounds, 
            size_t &first_cross_index, 
            size_t &second_cross_index
            )
    {
        auto &in_curve = std::get<0>(curve_tuple);
        auto &in_curve_depth = std::get<1>(curve_tuple);
        lbounds = {};
        ubounds = {};
        // crossings.clear();

        if ( in_curve.size() < 2 )
        {
            return false;
        }

        /* auto reduce_region = []( const std::vector<size_t> &c ) -> size_t { */
        /*     size_t num = 0; */
        /*     for ( auto &i : c ) */
        /*     { */
        /*         num += std::pow(2, i); */
        /*     } */

        /*     return num; */
        /* }; */

        std::vector<size_t> cur_region, region, cur_lbounds, cur_ubounds;
        bool first_cross = false, second_cross = false;
        /* size_t first_cross_index; */

        SUtilitiesWrapper u(modeller_);

        /* std::cout << "getting intersecting surfaces indices: \n"; */

        /* region = u.getSurfacesIndicesBelowPoint(in_curve[0].x(), in_curve[0].y(), in_curve_depth); */
        lbounds = u.getSurfacesIndicesBelowPoint(in_curve[0].x(), in_curve[0].y(), in_curve_depth);
        ubounds = u.getSurfacesIndicesAbovePoint(in_curve[0].x(), in_curve[0].y(), in_curve_depth);

        for ( size_t i = 1; i < in_curve.size(); ++i )
        {
            /* std::cout << "For point: " << in_curve[i].x() << ", " << in_curve[i].y() << ", " << in_curve_depth << "\n"; */
            cur_lbounds = u.getSurfacesIndicesBelowPoint(in_curve[i].x(), in_curve[i].y(), in_curve_depth);
            cur_ubounds = u.getSurfacesIndicesAbovePoint(in_curve[i].x(), in_curve[i].y(), in_curve_depth);

            /* std::cout << "Lower boundaries: "; */
            /* for ( auto i : cur_lbounds ) */
            /* { */
                /* std::cout << i << ", "; */
            /* } */
            /* std::cout << "\n"; */
            /* std::cout << "Upper boundaries: "; */
            /* for ( auto i : cur_ubounds ) */
            /* { */
                /* std::cout << i << ", "; */
            /* } */
            /* std::cout << "\n"; */
            
            /* std::cout << "Index: " << i << ", current: " << reduce_region(cur_region) << ", previous: " << reduce_region(region) << "\n"; */

            if ( (cur_lbounds != lbounds) || (cur_ubounds != ubounds) )
            {
                if ( first_cross == false )
                {
                    lbounds = cur_lbounds;
                    ubounds = cur_ubounds;
                    first_cross_index = i;
                    first_cross = true;
                    /* crossings.push_back(i); */
                    /* std::cout << "First cross at index " << i << "\n"; */ 
                }
                else 
                {
                    second_cross = true;
                    second_cross_index = i-1;
                    /* crossings.push_back(i); */
                    /* std::cout << "Second cross at index " << i << "\n"; */ 
                    break; // for loop
                }
            }
        }

        if ( first_cross && second_cross )
        {
            /* lbounds = region; */
            /* size_t i = first_cross_index; */
            /* ubounds = u.getSurfacesIndicesAbovePoint(in_curve[i].x(), in_curve[i].y(), in_curve_depth); */
            /* std::cout << "Lower boundaries: "; */
            /* for ( auto i : lbounds ) */
            /* { */
                /* std::cout << i << ", "; */
            /* } */
            /* std::cout << "\n"; */
            /* std::cout << "Upper boundaries: "; */
            /* for ( auto i : ubounds ) */
            /* { */
                /* std::cout << i << ", "; */
            /* } */
            /* std::cout << "\n"; */

            return true;
        }

        return false;
    }

    template<typename CurveType>
    bool RulesProcessor::testSurface( size_t surface_index, std::vector< std::tuple< CurveType, double  > > &curves )
    {
        modeller_.disableGeologicRules();
        bool success = createSurface(surface_index, curves);
        
		if ( success )
        {
            testing_surface_insertion_ = true;
        }

        return success;
    }

    template<typename CurveType>
    bool RulesProcessor::createSurface( size_t surface_index, std::vector< std::tuple< CurveType, double  > > &curves )
    {
        return false;

        /* if ( testing_surface_insertion_ == true ) */
        /* { */
        /*     modeller_.undo(); */
			/* // testing_surface_insertion_ = false; */
        /* } */
		
        /* std::vector<size_t> lbounds, ubounds; */
        /* std::vector<size_t> intersected_surfaces; */

        /* std::vector<double> surface; */
        /* size_t num_cross_sections = curves.size(); */

        /* size_t first_index, second_index; */

        /* bool status = false; */
        /* std::cout << "current surfaces: "; */
        /* auto surfs = getSurfaces(); */
        /* for ( auto i : surfs ) */
        /* { */
        /*     std::cout << i << ", "; */
        /* } */
        /* std::cout << "\n"; */

        /* if ( num_cross_sections > 1 ) */
        /* { */
        /*     for ( auto &curve_tuple : curves ) */
        /*     { */
        /*         auto &in_curve = std::get<0>(curve_tuple); */
        /*         auto &in_curve_depth = std::get<1>(curve_tuple); */

        /*         for ( size_t i = 0; i < in_curve.size(); ++i ) */
        /*         { */
        /*             surface.push_back(in_curve[i].x()); */
        /*             surface.push_back(in_curve[i].y()); */
        /*             surface.push_back(in_curve_depth); */
        /*         } */
        /*     } */

        /*     if ( truncate_surface_ ) */
        /*     { */
        /*         status = modeller_.tryCreateSurface( surface_index, intersected_surfaces, surface ); */
        /*         if ( intersected_surfaces.size() > 0 ) */
        /*         { */
        /*             status = getFirstRegionCurveIntersects( curves.front(), lbounds, ubounds, first_index, second_index ); */
        /*             if ( status == false ) */
        /*             { */
        /*                 return false; */
        /*             } */

        /*             status = modeller_.createSurface( surface_index, surface, lbounds, ubounds ); */
        /*         } */
        /*     } */
        /*     else */
        /*     { */
        /*         status = modeller_.createSurface( surface_index, surface ); */
        /*     } */

        /*     /1* if ( intersected_surfaces.size() > 0 ) *1/ */
        /*     /1* { *1/ */
        /*     /1*     std::cout << "Intersected at least surfaces: " *1/ */ 
        /*     /1*         << intersected_surfaces.front() << ", " *1/ */ 
        /*     /1*         << intersected_surfaces.back() << "\n"; *1/ */ 
                    
        /*     /1*     getFirstRegionCurveIntersects( curves.front(), lbounds, ubounds ); *1/ */
        /*     /1*     std::cout << "First lower bound: " << (lbounds.empty() ? -1 : (int)lbounds.front()) << "\n"; *1/ */
        /*     /1*     std::cout << "First upper bound: " << (ubounds.empty() ? -1 : (int)ubounds.front()) << "\n"; *1/ */

        /*     /1*     status = modeller_.createSurface( surface_index, surface, lbounds, ubounds ); *1/ */
        /*     /1* } *1/ */
        /* } */

        /* else if ( num_cross_sections == 1 ) */
        /* { */
        /*     std::cout << "Inside createSurface() \n"; */
        /*     auto &curve_tuple = curves.front(); */

        /*     auto &in_curve = std::get<0>(curve_tuple); */
        /*     auto &in_curve_depth = std::get<1>(curve_tuple); */

        /*     for ( size_t i = 0; i < in_curve.size(); ++i ) */
        /*     { */
        /*         surface.push_back(in_curve[i].x()); */
        /*         surface.push_back(in_curve[i].y()); */
        /*     } */

        /*     if ( truncate_surface_ ) */
        /*     { */
        /*         std::cout << "Truncate is active.\n"; */
        /*         status = modeller_.tryCreateLengthwiseExtrudedSurface( surface_index, intersected_surfaces, surface ); */
        /*         if ( intersected_surfaces.size() > 0 ) */
        /*         { */
        /*             std::cout << "Surface is not valid and status is: " << status << ", it intersects surfaces:"; */
        /*             for (int s = 0; s < intersected_surfaces.size(); ++s) */
        /*                 std::cout << intersected_surfaces[s] << ", "; */
        /*             std::cout << "\n"; */

					/* TruncateHelper<CurveType> tHelper(curve_tuple, modeller_, intersected_surfaces); */
        /*             // status = getFirstRegionCurveIntersects( curve_tuple, lbounds, ubounds, first_index, second_index ); */
					/* status = tHelper.getFirstRegionCurveIntersects(lbounds, ubounds); */
					/* if ( status == false ) */
        /*             { */
        /*                 return false; */
        /*             } */

        /*             std::vector<size_t> lb_intersect, ub_intersect; */
        /*             std::set_intersection(intersected_surfaces.begin(), intersected_surfaces.end(), lbounds.begin(), lbounds.end(), std::back_inserter(lb_intersect)); */
        /*             std::set_intersection(intersected_surfaces.begin(), intersected_surfaces.end(), ubounds.begin(), ubounds.end(), std::back_inserter(ub_intersect)); */

					/* status = tHelper.truncateCurve(); */
					/* if (status == false) */
					/* { */
						/* return false; */
					/* } */

					/* std::cout << "Original curve\n"; */
					/* for (size_t i = 0; i < in_curve.size(); ++i) */
					/* { */
						/* std::cout << in_curve[i].x() << " " << in_curve[i].y() << "\n"; */
					/* } */
					/* in_curve = tHelper.getTruncatedCurve(); */
					/* std::cout << "Modified curve\n"; */
					/* for (size_t i = 0; i < in_curve.size(); ++i) */
					/* { */
						/* std::cout << in_curve[i].x() << " " << in_curve[i].y() << "\n"; */
					/* } */
					/* surface = tHelper.getTruncatedPoints2D(); */
        /*             status = modeller_.createLengthwiseExtrudedSurface( surface_index, surface, lb_intersect, ub_intersect ); */
        /*             std::cout << "Final status is: " << status << "\n"; */
        /*         } */
        /*     } */
        /*     else */
        /*     { */
        /*         std::cout << "truncate is not active.\n"; */
        /*         status = modeller_.createLengthwiseExtrudedSurface( surface_index, surface ); */
        /*         std::cout << "Final status is: " << status << "\n"; */
        /*         /1* status = modeller_.createSurface( surface_index, surface ); *1/ */
        /*     } */
        /*     /1* status = modeller_.createLengthwiseExtrudedSurface( surface_index, surface ); *1/ */
        /* } */

        /* testing_surface_insertion_ = false; */
        
        /* if ( status == true ) */
        /* { */
        /*     std::vector<double> dummy_vertices; */
        /*     std::vector<size_t> dummy_edges; */
        /*     /1* status &= (getMesh(surface_index, dummy_vertices, dummy_edges) > 0); *1/ */
        /*     status &= getMesh(surface_index, dummy_vertices, dummy_edges); */

        /*     if ( status == false ) */
        /*     { */
        /*         std::cout << "Inserted surface is vacuous, undoing...\n" << std::flush; */
        /*         modeller_.undo(); */
        /*     } */
        /* } */
        
		/* return status; */
    }

    template<typename CurveType>
    bool RulesProcessor::extrudeAlongPath(  size_t surface_index,
        const CurveType &cross_section_curve, double cross_section_depth,
        const CurveType &path_curve
        /* size_t num_extrusion_steps */
        )
    {
        return false;

        /* if ( testing_surface_insertion_ == true ) */
        /* { */
        /*     modeller_.undo(); */
        /* } */

        /* SUtilitiesWrapper util(modeller_); */
        /* std::vector<double> surface; */

        /* size_t first_index, second_index; */

        /* std::vector<double> cross_section; */
        /* for ( size_t i = 0; i < cross_section_curve.size(); ++i ) */
        /* { */
        /*     cross_section.push_back( cross_section_curve[i].x() ); */
        /*     cross_section.push_back( cross_section_curve[i].y() ); */
        /* } */

        /* std::vector<double> path; */
        /* for ( size_t i = 0; i < path_curve.size(); ++i ) */
        /* { */
        /*     path.push_back( path_curve[i].x() ); */
        /*     path.push_back( path_curve[i].y() ); */
        /* } */

        /* bool status; */

        /* if ( truncate_surface_ ) */
        /* { */
        /*     std::vector<size_t> lbounds, ubounds; */
        /*     std::vector<size_t> intersected_surfaces; */

        /*     status = modeller_.tryCreateLengthwiseExtrudedSurface( surface_index, intersected_surfaces, cross_section, cross_section_depth, path ); */

        /*     if ( intersected_surfaces.size() > 0 ) */
        /*     { */
        /*         std::tuple<CurveType, double> curve_tuple = std::make_pair(cross_section_curve, cross_section_depth); */
        /*         TruncateHelper<CurveType> tHelper(curve_tuple, modeller_, intersected_surfaces); */
        /*         status = tHelper.getFirstRegionCurveIntersects( lbounds, ubounds ); */
        /*         if ( status == false ) */
        /*         { */
        /*             return false; */
        /*         } */

        /*         std::vector<size_t> lb_intersect, ub_intersect; */
        /*         std::set_intersection(intersected_surfaces.begin(), intersected_surfaces.end(), lbounds.begin(), lbounds.end(), std::back_inserter(lb_intersect)); */
        /*         std::set_intersection(intersected_surfaces.begin(), intersected_surfaces.end(), ubounds.begin(), ubounds.end(), std::back_inserter(ub_intersect)); */

        /*         status = tHelper.truncateCurve(); */
        /*         if (status == false) */
        /*         { */
        /*             return false; */
        /*         } */

        /*         cross_section = tHelper.getTruncatedPoints2D(); */
        /*         status = modeller_.createLengthwiseExtrudedSurface( surface_index, cross_section, cross_section_depth, path, lb_intersect, ub_intersect ); */
        /*     } */
        /* } */
        /* else */
        /* { */
        /*     status = modeller_.createLengthwiseExtrudedSurface(surface_index, cross_section, cross_section_depth, path); */
        /* } */

        /* testing_surface_insertion_ = false; */

        /* return status; */

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
