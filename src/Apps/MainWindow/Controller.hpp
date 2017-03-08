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

//#include "Sketching/BoundaryItem2D.hpp"
//#include "./src/Core/Geology/Models/CrossSection.hpp"
//#include "Sketching/StratigraphicItem.hpp"

//#include "MainWindow/ExtrusionRulesProcessor.hpp"

//#include "3dView/Model3DUtils.hpp"


#include <iostream>
#include <map>

#include "Volume.h"
#include "CrossSection.h"
#include "Object.h"
#include "Region1.h"

class Controller: public QObject
{

    Q_OBJECT

	public: 
	

		enum class ReconstructionMode { EXTRUSION, INTERPOLATION };
		
	
        Controller();


//        void addCrossSection( const float& d = 0.0f );
//        inline bool hasCrossSection(){ return !( crosssections_list.empty() ); }
//        float getCurrentCrossSection(){ return current_crosssection; }


//        bool addBoundary( const float& origin_x, const float& origin_y, const float& origin_z, const float& width, const float& height, const float& depth );
//        void editBoundary( const float& origin_x, const float& origin_y, const float& origin_z, const float& width, const float& height, const float& depth );
//        Boundary* getCurrentBoundary();



//        bool addCurve( const Curve2D &curve );
//        bool addStratigraphy();
//        bool interpolateStratigraphy( const std::vector< size_t >& lower_bound = std::vector< size_t >(),
//                                      const std::vector< size_t >& upper_bound = std::vector< size_t >() );

//        Stratigraphy* getCurrentStratigraphy();


//        bool defineSketchingAbove( std::vector< size_t >& allowed );
//        bool defineSketchingBelow( std::vector< size_t >& allowed );

//        bool defineRegionAbove( const std::vector< size_t >& selections );
//        bool defineRegionBelow( const std::vector< size_t >& selections );



//        bool stopSketchingAbove();
//        bool stopSketchingBelow();


//        void initRulesProcessor( const float& orig_x, const float& orig_y, const float& orig_z, const float& width, const float& height, const float& depth );
//        void editRulesProcessor( const float& orig_x, const float& orig_y, const float& orig_z, const float& width, const float& height, const float& depth );



        /**/

        inline void init()
        {
            addInputVolume();
//            registerSolver();
        }

        inline void addInputVolume()
        {
            //TODO: create visualization volume
            //TODO: add in object tree, etc
            input_volume.initialize();

//            scene.addVolume( input_volume );
//            object_tree.add( input_volume, "INPUT VOLUME" );
        }

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

        inline void addCurrentCrossSectionifNotValid()
        {
            auto search_csection_ = depth_of_cross_sections.find( current_cross_section );
            if( search_csection_ == depth_of_cross_sections.end() )
            {
                addCrossSectionofDepth( current_cross_section );
            }

        }

        inline void addCrossSectionofDepth( double depth_ )
        {
            //TODO: create visualization cross_section
            //TODO: add in object tree, etc

            double ox = 0.0f, oy = 0.0f, oz = 0.0f;
            input_volume.getOrigin( ox, oy, oz );

            if( std::fabs(  depth_ - oz ) < input_volume.getDepth() )
            {
                depth_of_cross_sections[ depth_ ] = new CrossSection1();
                setCurrentCrossSection( depth_ );

//            scene.addCrossSection( depth_of_cross_sections[ depth_ ] );
//            object_tree.add( depth_of_cross_sections[ depth_ ], "CrossSection" );

            }
            else
            {
                std::cout << "Cross-section out of range" <<std::endl;
            }
        }

        inline void setCurrentCrossSection( double depth_ )
        {
            double ox = 0.0f, oy = 0.0f, oz = 0.0f;
            input_volume.getOrigin( ox, oy, oz );

            if( std::fabs(  depth_ - oz ) < input_volume.getDepth() )
            {
                current_cross_section = depth_;
            }
            else
            {
                std::cout << "Cross-section out of range" <<std::endl;
            }
        }

        inline double getCurrentCrossSection() const
        {
            return current_cross_section;
        }

        inline bool getCurrentCrossSectionDimensions( double& width_, double& height_ )
        {
            auto search = depth_of_cross_sections.find( current_cross_section );
            if( search == depth_of_cross_sections.end() ) return false;

            CrossSection1 *csection = depth_of_cross_sections[ current_cross_section ];
            csection->getDimensions( width_, height_ );
            return true;
        }

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


        inline void addCurrentObjectifNotValid()
        {
            auto search = objects.find( current_object );
            if( search == objects.end() )
            {
                addObject();
            }
        }

        inline void addObject()
        {
            Object* obj = new Object( current_object_type );
            current_object = obj->getId();
            objects[ current_object ] = obj;

//            scene.addObject( obj );
//            object_tree.add( obj, "Object" );
        }

        inline bool setNameofCurrentObject( const std::string& name_ )
        {
            return setNameofObjectofId( current_object, name_ );
        }

        inline bool setNameofObjectofId( std::size_t id_, const std::string& name_ )
        {
            auto search = objects.find( id_ );
            if( search == objects.end() ) return false;

            Object* obj = objects[ id_ ];
            obj->setName( name_ );
            return true;
        }

        inline bool getNameofCurrentObject( std::string& name_ )
        {
            return getNameofObjectofId( current_object, name_ );
        }

        inline bool getNameofObjectofId( std::size_t id_, std::string& name_ )
        {
            auto search = objects.find( id_ );
            if( search == objects.end() ) return false;

            Object* obj = objects[ id_ ];
            name_ = obj->getName();
            return true;
        }

        inline bool getTypeofObjectofId( std::size_t id_, Object::TYPE& type_ )
        {
            auto search = objects.find( id_ );
            if( search == objects.end() ) return false;

            Object* obj = objects[ id_ ];
            type_ = obj->getType();
            return true;
        }

        inline bool setVisibilityofObjectofId( std::size_t id_, bool option )
        {
            auto search = objects.find( id_ );
            if( search == objects.end() ) return false;

            Object* obj = objects[ id_ ];
            obj->setVisibility( option );
            return true;
        }

        inline bool getVisibilityofObjectofId( std::size_t id_ )
        {
            auto search = objects.find( id_ );
            if( search == objects.end() ) return false;

            Object* obj = objects[ id_ ];
            return obj->getVisibility();
        }

        inline void addInputCurvetoCurrentObject( const Curve2d& curve_ )
        {
            addCurrentObjectifNotValid();
            addCurrentCrossSectionifNotValid();

            Object* obj_ = objects[ current_object ];
            obj_->addInputCurve( current_cross_section, (Curve2d* const& )curve_ );
        }

        inline bool getAllInputCurvesofCurrentObject( std::vector< Curve2d* >& input_curves_ )
        {
            auto search = objects.find( current_object );
            if( search == objects.end() ) return false;

            Object* obj_ = objects[ current_object ];
            input_curves_ = obj_->getAllInputCurves();
            return true;

        }

        inline std::size_t getNumberofInputCurvesinObjectofId( std::size_t id_ )
        {
            auto search = objects.find( id_ );
            if( search == objects.end() ) return false;

            Object* obj_ = objects[ id_ ];
            return obj_->getNumberofInputCurves();
        }

        inline bool getAllCrossSectionsRelatedtoCurrentObject( std::vector< double >& cross_sections_related_ )
        {
            auto search = objects.find( current_object );
            if( search == objects.end() ) return false;

            Object* obj_ = objects[ current_object ];
            cross_sections_related_ = obj_->getAllCrossSectionsRelatedtoObject();
            return true;
        }

        inline bool setPathCurvetoCurrentObject( const Curve2d& path_curve_ )
        {
            auto search = objects.find( current_object );
            if( search == objects.end() ) return false;

            Object* obj_ = objects[ current_object ];
            obj_->setPathCurve( path_curve_ );
            return true;
        }

        inline bool getPathCurveofCurrentObject( Curve2d& path_ )
        {
            auto search = objects.find( current_object );
            if( search == objects.end() ) return false;

            Object* obj_ = objects[ current_object ];
            path_ =  obj_->getPathCurve();
            return true;
        }

        inline bool setCurrentObject( Object*& obj_ )
        {
            auto id_ = obj_->getId();
            return setCurrentObject( id_ );
        }

        inline bool setCurrentObject( std::size_t id_ )
        {
            auto search = objects.find( id_ );
            if( search == objects.end() ) return false;

            current_object = id_;
            return true;
        }

        inline bool getCurrentObject( Object*& obj_ )
        {
            auto search = objects.find( current_object );
            if( search == objects.end() ) return false;

            obj_ = objects[ current_object ];
            return true;
        }


        inline void addRegion()
        {
            Region1* rg_ = new Region1();
            std::size_t id_ = rg_->getId();

            regions[ id_ ] = rg_;

//            scene.addRegion( rg_ );
//            object_tree.add( rg_, "Region" );
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


//        inline bool registerSolver()
//        {
//            map_input_property input_data;
//            map_input_property boundary_data;
//            map_input_property output_data;

//            register_solver.registerSolverData( input_data, boundary_data, output_data );

//            std::cout << "There are " << input_data.size()  << " input properties. " <<std::endl;
//            std::cout << "There are " << boundary_data.size()  << " boundary properties. " <<std::endl;
//            std::cout << "There are " << output_data.size()  << " output properties. \n" <<std::endl;


//            MeshData input_mesh;
//            MeshData output_mesh;

//            register_solver.registerSolverMeshes( input_mesh, output_mesh );

//            std::cout << "Input mesh dimension = " << input_mesh.dimension << std::endl;
//            std::cout << "Output mesh dimension = " << output_mesh.dimension << std::endl;


//            return true;

//        }


    protected:

        Volume input_volume;

        double current_cross_section;
        std::map< double, CrossSection1* > depth_of_cross_sections;

        Object::TYPE current_object_type = Object::TYPE::Stratigraphy;
        size_t current_object;
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
