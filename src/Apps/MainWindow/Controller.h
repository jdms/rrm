#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <map>
#include <vector>

#include <QObject>

#include "Volume.h"
#include "CrossSection.h"
#include "Object.h"
#include "Region.h"
#include "SolverRegistration.h"


class Controller: public QObject
{
    Q_OBJECT

    public:

        Controller();

        inline void init()
        {
            addInputVolume();
            registerSolver();
        }

        inline void addInputVolume()
        {
            //TODO: create visualization volume
            //TODO: add in object tree, etc
            input_volume.initialize();
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
                depth_of_cross_sections[ depth_ ] = new CrossSection();
                setCurrentCrossSection( depth_ );
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

            CrossSection *csection = depth_of_cross_sections[ current_cross_section ];
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
            Region* rg_ = new Region();
            std::size_t id_ = rg_->getId();

            regions[ id_ ] = rg_;
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

            Region* rg_ = regions[ id_ ];
            rg_->setVisibility( option );
            return true;
        }

        inline bool getVisibilityofRegionofId( std::size_t id_ )
        {
            auto search = regions.find( id_ );
            if( search == regions.end() ) return false;

            Region* rg_ = regions[ id_ ];
            return rg_->getVisibility();
        }

        inline bool addCrossSectionDepthtoRegionofId( std::size_t id_, double depth_ )
        {
            auto search = regions.find( id_ );
            if( search == regions.end() ) return false;

            Region* rg_ = regions[ id_ ];
            rg_->addCrossSectionofDepth( depth_ );
            return true;
        }

        inline bool getDepthofAllCrossSectionsofRegionofId( std::size_t id_, std::vector< double >& csections_ )
        {
            auto search = regions.find( id_ );
            if( search == regions.end() ) return false;

            Region* rg_ = regions[ id_ ];
            csections_ = rg_->getDepthofAllCrossSections();
            return true;
        }

        inline bool setCellsIndexedtoRegionofId( std::size_t id_, const std::vector< std::size_t >& cells_ )
        {
            auto search = regions.find( id_ );
            if( search == regions.end() ) return false;

            Region* rg_ = regions[ id_ ];
            rg_->setCellIndexesofRegion( cells_ );
            return true;
        }

        inline bool getCellsIndexedtoRegionofId( std::size_t id_, std::vector< std::size_t >& cells_ )
        {
            auto search = regions.find( id_ );
            if( search == regions.end() ) return false;

            Region* rg_ = regions[ id_ ];
            cells_ = rg_->getCellIndexesofRegion();
            return true;
        }


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


    protected:

        Volume input_volume;

        double current_cross_section;
        std::map< double, CrossSection* > depth_of_cross_sections;

        Object::TYPE current_object_type = Object::TYPE::Stratigraphy;
        size_t current_object;
        std::map< std::size_t, Object* > objects;


        std::map< std::size_t, Region* > regions;

        SolverRegistration register_solver;




};

#endif // CONTROLLER_H
