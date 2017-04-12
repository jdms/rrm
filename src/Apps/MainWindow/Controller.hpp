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


#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QObject>

#include <vector>
#include <iostream>
#include <map>

//#include "Sketching/BoundaryItem2D.hpp"
//#include "./src/Core/Geology/Models/CrossSection.hpp"
//#include "Sketching/StratigraphicItem.hpp"

//#include "MainWindow/ExtrusionRulesProcessor.hpp"

//#include "3dView/Model3DUtils.hpp"




#include "Volume.h"
#include "CrossSection.h"
#include "Object.h"
#include "Region1.h"

#include "Scene3D.h"
#include "SketchScene.h"




class Controller: public QObject
{

    Q_OBJECT

    public:


        enum class ReconstructionMode { EXTRUSION, INTERPOLATION };
        enum class SketchStatus { ABOVE, BELOW, NONE };
        enum class RuleStatus { RA_SKETCHING, RAI_SKETCHING, RB_SKETCHING, RBI_SKETCHING };


        Controller();

        /*

                void addCrossSection( const float& d = 0.0f );
                inline bool hasCrossSection(){ return !( crosssections_list.empty() ); }
                float getCurrentCrossSection(){ return current_crosssection; }


                bool addBoundary( const float& origin_x, const float& origin_y, const float& origin_z, const float& width, const float& height, const float& depth );
                void editBoundary( const float& origin_x, const float& origin_y, const float& origin_z, const float& width, const float& height, const float& depth );
                Boundary* getCurrentBoundary();



                bool addCurve( const Curve2D &curve );
                bool addStratigraphy();
                bool interpolateStratigraphy( const std::vector< size_t >& lower_bound = std::vector< size_t >(),
                                              const std::vector< size_t >& upper_bound = std::vector< size_t >() );

                Stratigraphy* getCurrentStratigraphy();


                bool defineSketchingAbove( std::vector< size_t >& allowed );
                bool defineSketchingBelow( std::vector< size_t >& allowed );

                bool defineRegionAbove( const std::vector< size_t >& selections );
                bool defineRegionBelow( const std::vector< size_t >& selections );



                bool stopSketchingAbove();
                bool stopSketchingBelow();


                void initRulesProcessor( const float& orig_x, const float& orig_y, const float& orig_z, const float& width, const float& height, const float& depth );
                void editRulesProcessor( const float& orig_x, const float& orig_y, const float& orig_z, const float& width, const float& height, const float& depth );

*/

        /**/

        void init();

        inline void setScene3D( Scene3D* sc_ ){ scene3d = sc_; }

        inline void setSketchScene( SketchScene* sc_ ){ sketch_scene = sc_; }



        void addInputVolume();

        inline void setInputVolumeDimensions( double width_, double height_, double depth_ )
        {
            input_volume.setDimensions( width_, height_, depth_ );
        }

        inline void getInputVolumeDimensions( double& width_, double& height_, double& depth_ ) const
        {
            input_volume.getDimensions( width_, height_, depth_ );
        }

        inline void setInputVolumeOrigin( double ox_, double oy_, double oz_ )
        {
            input_volume.setOrigin( ox_, oy_, oz_ );
        }

        inline void getInputVolumeOrigin( double& ox_, double& oy_, double& oz_ )
        {
            input_volume.getOrigin( ox_, oy_, oz_ );
        }

        inline void setInputVolumeVisibility( bool option )
        {
            input_volume.setVisibility( option );
        }

        inline bool getInputVolumeVisibility() const
        {
            return input_volume.getVisibility();
        }



        //NOTE: cross-sections should be members of volumes? it makes sense to me.

        void addCurrentCrossSectionToList();

        void setCurrentCrossSection( double depth_ );

        inline double getCurrentCrossSection() const
        {
            return current_depth_csection;
        }

        bool getCurrentCrossSectionDimensions( double& width_, double& height_ );

        inline size_t getNumberofCrossSectionsUsed() const
        {
            return depth_of_cross_sections.size();
        }






        inline void setTypeCurrentObject( Object::TYPE type_ )
        {
            current_object_type = type_;
        }

        inline Object::TYPE getTypeCurrentObject() const
        {
            return current_object_type;
        }

        inline bool setNameofCurrentObject( const std::string& name_ )
        {
            return setNameofObjectofId( current_object, name_ );
        }

        bool setNameofObjectofId( std::size_t id_, const std::string& name_ );

        inline bool getNameofCurrentObject( std::string& name_ )
        {
            return getNameofObjectofId( current_object, name_ );
        }

        bool getNameofObjectofId( std::size_t id_, std::string& name_ );

        inline bool getTypeofObjectofId( std::size_t id_, Object::TYPE& type_ );

        bool setVisibilityofObjectofId( std::size_t id_, bool option );

        bool getVisibilityofObjectofId( std::size_t id_ );

        void addInputCurvetoCurrentObject( const Curve2D &curve_ );

        bool getAllInputCurvesofCurrentObject( std::vector< Curve2D >& input_curves_ );

        std::size_t getNumberofInputCurvesinObjectofId( std::size_t id_ );

        bool getAllCrossSectionsRelatedtoCurrentObject( std::vector< double >& cross_sections_related_ );

        bool setPathCurvetoCurrentObject( const Curve2D& path_curve_ );

        bool getPathCurveofCurrentObject( Curve2D& path_ );

        inline bool setCurrentObject( Object*& obj_ )
        {
            auto id_ = obj_->getId();
            return setCurrentObject( id_ );
        }

        inline bool setCurrentObject( std::size_t id_ )
        {
            if( isValidObject( current_object ) == false ) return false;
            current_object = id_;
            return true;
        }

        inline bool getCurrentObject( Object*& obj_ )
        {
            if( isValidObject( current_object ) == false ) return false;
            obj_ = objects[ current_object ];
            return true;
        }

        bool interpolate();


        inline void addRegion()
        {
            Region1* rg_ = new Region1();
            std::size_t id_ = rg_->getId();

            regions[ id_ ] = rg_;

            scene3d->addRegion( rg_ );
            //object_tree.add( rg_, "Region" );
        }

        inline bool setRegionPointPositionofId( std::size_t id_, double x_, double y_, double z_ )
        {
            auto search = regions.find( id_ );
            if( search == regions.end() ) return false;

            regions[ id_ ]->setPointPosition( x_, y_, z_ );
        }

        inline bool getRegionPointPositionofId( std::size_t id_, double& x_, double& y_, double& z_ )
        {
            auto search = regions.find( id_ );
            if( search == regions.end() ) return false;

            regions[ id_ ]->getPointPosition( x_, y_, z_ );
        }

        inline bool setVisibilityofRegionofId( std::size_t id_, bool option )
        {
            auto search = regions.find( id_ );
            if( search == regions.end() ) return false;

            Region1* rg_ = regions[ id_ ];
            rg_->setVisibility( option );
            return true;
        }

        inline bool getVisibilityofRegionofId( std::size_t id_ )
        {
            auto search = regions.find( id_ );
            if( search == regions.end() ) return false;

            Region1* rg_ = regions[ id_ ];
            return rg_->getVisibility();
        }

        inline bool addCrossSectionDepthtoRegionofId( std::size_t id_, double depth_ )
        {
            auto search = regions.find( id_ );
            if( search == regions.end() ) return false;

            Region1* rg_ = regions[ id_ ];
            rg_->addCrossSectionofDepth( depth_ );
            return true;
        }

        inline bool getDepthofAllCrossSectionsofRegionofId( std::size_t id_, std::vector< double >& csections_ )
        {
            auto search = regions.find( id_ );
            if( search == regions.end() ) return false;

            Region1* rg_ = regions[ id_ ];
            csections_ = rg_->getDepthofAllCrossSections();
            return true;
        }

        inline bool setCellsIndexedtoRegionofId( std::size_t id_, const std::vector< std::size_t >& cells_ )
        {
            auto search = regions.find( id_ );
            if( search == regions.end() ) return false;

            Region1* rg_ = regions[ id_ ];
            rg_->setCellIndexesofRegion( cells_ );
            return true;
        }

        inline bool getCellsIndexedtoRegionofId( std::size_t id_, std::vector< std::size_t >& cells_ )
        {
            auto search = regions.find( id_ );
            if( search == regions.end() ) return false;

            Region1* rg_ = regions[ id_ ];
            cells_ = rg_->getCellIndexesofRegion();
            return true;
        }


        void updateRule( const std::string &rule_ );

        void updateObjects();

        void defineSketchAbove( bool status_ );

        void defineSketchBelow( bool status_ );

        inline bool undo()
        {
//            bool undo_done = rules_processor.stopDefineBelow();
            std::cout << "Trying undo" << std::flush;
            return true;
        }



        inline bool canUndo()
        {
        //    rules_processor.canUndo();
            std::cout << "Enabling undo \n"  << std::flush;
            return true;
        }

        inline bool canRedo()
        {
        //    rules_processor.canRedo();
            std::cout << "Enabling redo \n"  << std::flush;
            return true;
        }

    private:

        inline void addCrossSectionofDepth( double depth_ )
        {
//            //TODO: create visualization cross_section
//            //TODO: add in object tree, etc

//            if( isValidCrossSection( depth_ ) == false )
//            {
//                std::cout << "Cross-section out of range" <<std::endl;
//                return;
//            }

//            depth_of_cross_sections[ depth_ ] = new CrossSection1();
//            depth_of_cross_sections[ depth_ ].setVolume( &input_volume );
//            updateScenesWithCurrentCrossSection();
        }

        inline void setCurrentCrossSectionAsUsed()
        {
            used_cross_sections.push_back( current_depth_csection );
        }

        inline bool isValidCrossSection( double depth_ )
        {
            double ox = 0.0f, oy = 0.0f, oz = 0.0f;
            input_volume.getOrigin( ox, oy, oz );

            if( std::fabs(  depth_ - oz ) <= input_volume.getDepth() )
                return true;
            else
                return false;
        }

        inline void updateScenesWithCurrentCrossSection()
        {
//            if( isCrossSectionAdded( current_depth_csection ) == false ) return;
//            depth_of_cross_sections[ current_depth_csection ] = CrossSection1();
            depth_of_cross_sections[ current_depth_csection ].setZCoordinate( current_depth_csection );

            sketch_scene->resetData();
            sketch_scene->setCrossSection( depth_of_cross_sections[ current_depth_csection ] );
        }

        inline bool isCrossSectionAdded( double depth_ )
        {
            auto search_csection_ = depth_of_cross_sections.find( depth_ );
            if( search_csection_ != depth_of_cross_sections.end() )
                return true;
            else return false;

        }


        inline void createObject()
        {
            Object* obj = new Object( current_object_type );
            current_object = obj->getId();
            objects[ current_object ] = obj;

            std::cout << "Object " << current_object << " created\n\n" << std::endl;
        }

        inline bool isValidObject( std::size_t id_ )
        {
            auto search = objects.find( id_ );
            if( search != objects.end() )
                return true;
            else
                return false;

        }

        inline void addCurrentObjectifNotValid()
        {
            //NOTE: maybe this method is not necessary too
            if( isValidObject( current_object ) == true ) return;
            createObject();
        }

        inline void addCurrentObjectToScenes()
        {
            Object* const& obj = objects[ current_object ];
            scene3d->addObject( obj );
            sketch_scene->addObject( obj );
            //object_tree.add( obj, "Object" );
        }

        inline void addCurrentObjectToCurrentCrossSection()
        {
            CrossSection1& csection_ = depth_of_cross_sections[ current_depth_csection ];
            Object* const& obj_ = objects[ current_object ];
            csection_.addObjectReferenced( obj_ );
        }







/*
                inline bool registerSolver()
                {
                    map_input_property input_data;
                    map_input_property boundary_data;
                    map_input_property output_data;

                    register_solver.registerSolverData( input_data, boundary_data, output_data );

                    std::cout << "There are " << input_data.size()  << " input properties. " <<std::endl;
                    std::cout << "There are " << boundary_data.size()  << " boundary properties. " <<std::endl;
                    std::cout << "There are " << output_data.size()  << " output properties. \n" <<std::endl;


                    MeshData input_mesh;
                    MeshData output_mesh;

                    register_solver.registerSolverMeshes( input_mesh, output_mesh );

                    std::cout << "Input mesh dimension = " << input_mesh.dimension << std::endl;
                    std::cout << "Output mesh dimension = " << output_mesh.dimension << std::endl;


                    return true;

                }
*/


    protected:

        Scene3D *scene3d;
        SketchScene* sketch_scene;

        Volume input_volume;

        double current_depth_csection;
        std::map< double, CrossSection1 > depth_of_cross_sections;
        std::vector< double > used_cross_sections;

        Object::TYPE current_object_type = Object::TYPE::Stratigraphy;
        std::size_t current_object;
        std::map< std::size_t, Object* > objects;

        std::map< std::size_t, Region1* > regions;

        //        SolverRegistration register_solver;

        /**/

    public slots:


        //        size_t getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps );

        //        void setStratigraphicRule( const std::string& rule );
        //        void changeResolution( const int numI_, const int numJ_ );

        //        void undo();
        //        void redo();


        //        void clear();
        //        void update();



    signals:

        //        void updateScene();
        //        void enableUndo( bool option );
        //        void enableRedo( bool option );
        //        void removeStratigraphy( size_t id );
        //        void waitingSelection( bool, const std::vector< size_t >& );
        //        void changeStratigraphyRulesStatus( const std::string& );
        //        void changeDefineRegionStatus( const bool, const bool );
        //        void pickingRegion( bool );


    protected:


        //        ReconstructionMode current_reconstruction;

        //        float current_crosssection;
        //        size_t current_stratigraphy;

        //        std::map< size_t, Stratigraphy* > stratigraphics_list;
        //        std::map< size_t, size_t > index_stratigraphy_map;
        //        std::map< float, CrossSection* > crosssections_list;
        //        RRM::ExtrusionRulesProcessor rules_processor;


};


#endif
