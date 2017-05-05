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
#include <set>

#include "MainWindow/RulesProcessor.hpp"

#include "Core/Geology/Models/Volume.h"
#include "Core/Geology/Models/CrossSection.h"
#include "Core/Geology/Models/Object.h"
#include "Core/Geology/Models/Region1.h"

#include "3dView/Scene3D.h"
#include "Sketching/SketchScene.h"
#include "Sketching/PathScene.h"
#include "Object_Tree/ObjectTree.h"



class Controller: public QObject
{

    Q_OBJECT

	public: 
	
    enum class State : int {
        UNDEFINED = -1,
        NO_GEOLOGIC_RULE,
        REMOVE_ABOVE, // Remove above
        REMOVE_ABOVE_INTERSECTION, // Remove above intersection
        REMOVE_BELOW, // Remove below
        REMOVE_BELOW_INTERSECTION, // Remove below intersection
        DEFINE_ABOVE, // Define above
        DEFINE_BELOW, // Define below
        DEFINE_REGION // Define region
    };


        enum class ReconstructionMode { EXTRUSION, INTERPOLATION };
        enum class RequestRegion { ABOVE, BELOW, NONE };
        enum class RuleStatus { RA_SKETCHING, RAI_SKETCHING, RB_SKETCHING, RBI_SKETCHING };

        Controller();


        void init();

        inline void setScene3D( Scene3D* const& sc_ ){ scene3d = sc_; }

        inline void setSketchScene( SketchScene* const& sc_ ){ sketch_scene = sc_; }

        inline void setPathScene( PathScene* const& sc_ ){ path_scene = sc_; }

        inline void setObjectTree( ObjectTree* const& tree_ ){ object_tree = tree_; }



        void addInputVolume();


        inline void setInputVolumeDimensions( double width_, double height_, double depth_ )
        {
            input_volume.setDimensions( width_, height_, depth_ );
        }

        inline void setInputVolumeWidthHeight( double width_, double height_ )
        {
            input_volume.setDimensions( width_, height_, input_volume.getDepth() );
        }

        inline void setInputVolumeWidthDepth( double width_, double depth_ )
        {
            input_volume.setDimensions( width_, input_volume.getHeight(), depth_ );
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

        void setCurrentCrossSection( double depth_ );

        inline double getCurrentCrossSection() const
        {
            return current_depth_csection;
        }

        inline size_t getNumberofCrossSectionsUsed() const
        {
            return used_cross_sections.size();
        }


        inline void createObject()
        {
            Object* obj = new Object( current_object_type );
            obj->setColor( current_color.red, current_color.green, current_color.blue );

            current_object = obj->getId();
            objects[ current_object ] = obj;
        }


        inline void setTypeCurrentObject( Object::TYPE type_ )
        {
            //TODO: if object is a channel set object in path scene
            current_object_type = type_;
            objects[ current_object ]->setType( current_object_type );

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

        void updateObjects();



        inline void addRegion()
        {
            Region1* rg_ = new Region1();
            std::size_t id_ = rg_->getId();

            regions[ id_ ] = rg_;

            scene3d->addRegion( rg_ );
            //object_tree->add( rg_, "Region" );
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



        bool enableCreateAbove( bool status_ );

        bool enableCreateBelow( bool status_ );


        inline bool undo()
        {
            bool undo_done = rules_processor.undo();
            if( undo_done == false ) return false;

            updateObjects();
            return true;
        }

        inline bool redo()
        {
            bool redo_done = rules_processor.redo();
            if( redo_done == false ) return false;

            updateObjects();
            return true;
        }


        inline bool canUndo()
        {
            return rules_processor.canUndo();
        }

        inline bool canRedo()
        {
            return rules_processor.canRedo();
        }



        inline void setupDepthResolution( std::size_t& disc_depth_, std::size_t& step_ )
        {
            disc_depth_ = rules_processor.getDepthResolution();
            disc_depth = disc_depth_;
            step_ = input_volume.getDepth()/( disc_depth_);
            step_depth = (double) step_ ;
        }

        inline std::size_t rowIndexfromDepth( double depth_ )
        {
            return std::size_t ( disc_depth*depth_/input_volume.getDepth() );
        }

        inline double depthFromRowIndex( std::size_t index_ )
        {
            return (double)( index_*input_volume.getDepth()/( disc_depth ) );
        }

        void sendSelectedSurface( const std::size_t& id_ );

        inline std::size_t getUpperSurface(){ return boundering_above; }

        inline std::size_t getLowerSurface(){ return boundering_below; }

        void clear();


        inline void setCurrentColor( int r_, int g_, int b_ )
        {
            current_color.red =   r_;
            current_color.green = g_;
            current_color.blue =  b_;

            sketch_scene->setCurrentColor( r_, g_, b_ );
            scene3d->setCurrentColor( r_, g_, b_ );
        }


    private:

        void addInputVolumeToScenes();


        inline void setCurrentCrossSectionAsUsed()
        {
            path_scene->addCrossSection( current_depth_csection );
            used_cross_sections.insert( current_depth_csection );
        }

        inline std::set< double > getCrossSections() const
        {
             return used_cross_sections;
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
            clearCrossSectionScenes();
            updateScenesCurvesOfCurrentCrossSection();
        }


        inline void clearCrossSectionScenes()
        {
            //NOTE: don't need to clear scene3d?
            sketch_scene->resetData();
        }

        inline void updateScenesCurvesOfCurrentCrossSection()
        {

            for( auto& it_: objects )
            {
                Object* obj_ = it_.second;
                if( getObjectCurvesFromCurrentCrossSection( obj_ ) == false )
                    continue;
                sketch_scene->addObject( it_.second );
            }

            sketch_scene->setCrossSection( current_depth_csection );
            scene3d->updateCrossSection( current_depth_csection );

        }


        inline bool getObjectCurvesFromCurrentCrossSection( Object* obj_ )
        {

            std::size_t index_ = rowIndexfromDepth( current_depth_csection );

            std::cout << "cross-section: " << index_ << std::endl << std::flush;

            std::vector< float > curve_vertices;
            std::vector< std::size_t > curve_edges;
            bool has_curve = rules_processor.getCrossSection( obj_->getId(), index_ ,
                                             curve_vertices, curve_edges );

            std::cout << "Object " << obj_->getId() << " has curve " << has_curve << std::endl << std::flush;

            if( has_curve == false )
            {
                return false;
            }

            std::vector< double > curve_vertices1( curve_vertices.begin(), curve_vertices.end() );
            obj_->addInputCurve( current_depth_csection,
                                       Model3DUtils::convertToCurve2D( curve_vertices1 ) );
            obj_->addInputEdges( current_depth_csection, curve_edges );

            return true;
        }


        inline bool getObjectSurface( Object* obj_ )
        {
            std::vector< float > surface_vertices;
            std::vector< std::size_t > surface_faces;

            bool getmesh_ok = rules_processor.getMesh( obj_->getId(), surface_vertices, surface_faces );
            if( getmesh_ok  == false ) return false;

            std::vector< double > surface_vertices1( surface_vertices.begin(), surface_vertices.end() );
            obj_->updateSurface( surface_vertices1, surface_faces );

        }



        inline bool isValidObject( std::size_t id_ )
        {
            auto search = objects.find( id_ );
            if( search != objects.end() )
                return true;
            else
                return false;

        }


        inline void addCurrentObjectToScenes()
        {
            Object* const& obj = objects[ current_object ];

            scene3d->addObject( obj );
            sketch_scene->addObject( obj );
            object_tree->addObject( obj->getId() );
        }


        inline void initRulesProcessor()
        {
            double ox_ = 0.0f, oy_ = 0.0f, oz_ = 0.0f;
            double w_ = 0.0f, h_ = 0.0f, d_ = 0.0f;

            input_volume.getOrigin( ox_, oy_, oz_ );
            input_volume.getDimensions( w_, h_, d_ );

            rules_processor.setOrigin( ox_, oy_, oz_ );
            rules_processor.setLenght(  w_, h_, d_ );
            rules_processor.setMediumResolution();
            rules_processor.removeAbove();
        }


        void setObjectsAsAllowed( std::vector< std::size_t >& objects_ );
        void unsetObjectsAsAllowed( std::vector< std::size_t >& objects_ );

        void unSelectObject( const std::size_t& id_ );

        void cleaningScenes();
        void cleaningRawData();
        void cleaningRules();

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


        struct Color { int red, green, blue; } current_color;

        Scene3D *scene3d;
        SketchScene* sketch_scene;
        PathScene* path_scene;
        ObjectTree* object_tree;

        Volume input_volume;
        std::size_t disc_width = 10;
        std::size_t disc_depth = 10;
        double step_depth = 10;

        double current_depth_csection;
        std::set< double > used_cross_sections;

        Object::TYPE current_object_type = Object::TYPE::Stratigraphy;
        std::size_t current_object;
        std::map< std::size_t, Object* > objects;

        std::map< std::size_t, Region1* > regions;

        RuleStatus current_rule = RuleStatus::RA_SKETCHING;
        RequestRegion current_region = RequestRegion::NONE;
        RulesProcessor rules_processor;

        std::size_t boundering_above;
        std::size_t boundering_below;

        std::vector< std::size_t > allowed_upper;
        std::vector< std::size_t > allowed_below;

        //        SolverRegistration register_solver;

        /**/


};


#endif
