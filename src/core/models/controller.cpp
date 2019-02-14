/****************************************************************************
 * RRM - Rapid Reservoir Modeling Project                                   *
 * Copyright (C) 2015                                                       *
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
 * @file controller.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class Controller
 */

#include <algorithm>
#include <random>


#include "controller.h"
#include "./apps/mainwindow/rrmapplication.h"

#include "volume.h"
#include "object.h"


Controller::Controller()
{
}


Controller::Controller(const Controller & cont_)
{
    this->app = cont_.app;
    this->csection = cont_.csection;
    this->model = cont_.model;

}


Controller& Controller::operator=(const Controller & cont_)
{

    this->app = cont_.app;
    this->csection = cont_.csection;
    this->model = cont_.model;

    return *this;
}


Controller::~Controller()
{
}


void Controller::setApplication( RRMApplication* const& app_)
{
    app = app_;
}


void Controller::init()
{
    createVolume();
    setVolumeDiscretization();

    createMainCrossSection();
    createTopViewCrossSection();

    addObject();
}



///
/// Volumes Methods
///


void Controller::createVolume()
{
    model.volume = std::make_shared<Volume>();
    model.volume->setDimensions( 500., 500., 500. );
    initRulesProcessor();
}


void Controller::resizeVolume(double width_, double height_, double depth_)
{
    model.volume->setDimensions( width_, height_, depth_ );
    updateBoundingBoxInModel();
    setVolumeDiscretization();

}


void Controller::getVolumeGeometry( double& ox_, double& oy, double& oz, double& w_, double& h_, double& d_ ) const
{
    model.volume->getGeometry( ox_, oy, oz, w_, h_, d_ );
}


void Controller::setVolumeWidth( double width_ )
{
    model.volume->setWidth( width_ );
    updateBoundingBoxInModel();
    setVolumeDiscretization();

}


void Controller::setVolumeHeight( double height_ )
{
    model.volume->setHeight( height_ );
    updateBoundingBoxInModel();
    setVolumeDiscretization();

}


void Controller::setVolumeLenght( double lenght_ )
{
    model.volume->setLenght( lenght_ );
    updateBoundingBoxInModel();
    setVolumeDiscretization();

}


void Controller::updateBoundingBoxInModel()
{
    double ox_ = 0.0, oy_ = 0.0, oz_ = 0.0;
    double w_ = 0.0, h_ = 0.0, d_ = 0.0;


    model.volume->getGeometry( ox_, oy_, oz_, w_, h_, d_ );

    // updating the bounding box of each object
    for( auto it: model.objects )
    {
        (it.second)->setBoundingBox( ox_, ox_ + w_, oy_, oy_ + h_, oz_, oz_ + d_ );
    }

    // updating the RulesProcessor bounding box
    updateBoundingBoxRulesProcessor();
}


void Controller::setVolumeName( const std::string& name_ )
{
    model.volume->setName(name_);
}


void Controller::setVolumeVisibility( bool status_ )
{
    model.volume->setVisible(status_);
}




///
/// Cross-Sections Methods
///


void Controller::createMainCrossSection()
{
    csection = std::make_shared<CrossSection>();
    csection->setVolume( model.volume );
    csection->setDirection( Settings::CrossSection::CrossSectionDirections::Z );
    csection->setDepth( model.volume->getLenght() );
}


void Controller::changeMainCrossSectionDirection( const Settings::CrossSection::CrossSectionDirections& dir_ )
{
    current_direction = dir_;

    // if cross-section is on the 'HEIGHT'direction, move the top-view cross-section to the top of the volume
    if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        moveTopViewCrossSection( model.volume->getHeight() );
        return;
    }


    // otherwise, move the main cross-section
    csection->setDirection( dir_ );

    if ( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        csection->setDepth( model.volume->getWidth() );
    }

    else if ( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
        csection->setDepth( model.volume->getLenght() );
    }

    // updating all curves of existents objects (stratigraphies and structurals), only if they are done
    for( auto it_: model.objects )
    {
        ObjectPtr obj_ = it_.second;
        if( obj_->isDone() == true )
            obj_->removeCrossSectionCurves();
    }

    // updating all the model with the new direction
    updateModel();

}


void Controller::moveMainCrossSection( double depth_ )
{
    csection->setDepth( depth_ );

    // update the curves to the current cross-section position
    updateObjectsCurvesInCrossSection( depth_ );

    // check if there is any image in the current position
    updateImageInMainCrossSection();

    // update the regions area in the current position
    updateRegions();
}


const CrossSectionPtr& Controller::getMainCrossSection() const
{
    return csection;
}


void Controller::createTopViewCrossSection()
{
    topview = std::make_shared<CrossSection>();
    topview->setVolume( model.volume );
    topview->setDirection( Settings::CrossSection::CrossSectionDirections::Y );
    topview->setDepth( model.volume->getHeight() );
}


const CrossSectionPtr& Controller::getTopViewCrossSection() const
{
    return topview;
}


void Controller::moveTopViewCrossSection( double depth_ )
{
    topview->setDepth( depth_ );
    updateObjectsCurvesInCrossSection( depth_ );
    updateImageInTopViewCrossSection();
}


void Controller::addCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{

     // creating fix cross-sections
    CrossSectionPtr csection_ = std::make_shared< CrossSection >();
    csection_->setVolume( model.volume );
    csection_->setDirection( dir_ );
    csection_->setDepth( depth_ );

    ImageData image_;

    // if already exist an image in this position, set the image
    if ( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {

        if( images_csectionsX.find( depth_ ) != images_csectionsX.end() )
        {
            image_ = images_csectionsX[ depth_ ];
            csection_->setImage( image_.file, image_.ox, image_.oy, image_.w, image_.h );
        }

        model.csectionsX[ depth_ ] = csection_;
    }

    else if ( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        if( images_csectionsY.find( depth_ ) != images_csectionsY.end() )
        {
            csection_->setImage( image_topview.file, image_topview.ox, image_topview.oy, image_topview.w, image_topview.h );
        }
        model.csectionsY[ depth_ ] = csection_;



    }

    else if ( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
        if( images_csectionsZ.find( depth_ ) != images_csectionsZ.end() )
        {
            image_ = images_csectionsZ[ depth_ ];
            csection_->setImage( image_.file, image_.ox, image_.oy, image_.w, image_.h );
        }
        model.csectionsZ[ depth_ ] = csection_;
    }


    // update the curves to this cross-section
    updateObjectsCurvesInCrossSection( depth_ );



}


bool Controller::getCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, CrossSectionPtr& csection_ )
{

    if ( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        if ( model.csectionsX.find( depth_ ) == model.csectionsX.end() ) return false;
        csection_ = model.csectionsX.at( depth_ );
        return true;
    }

    else if ( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        if ( model.csectionsY.find( depth_ ) == model.csectionsY.end() ) return false;
        csection_ = model.csectionsY.at( depth_ );
        return true;
    }

    else if ( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
        if ( model.csectionsZ.find( depth_ ) == model.csectionsZ.end() ) return false;
        csection_ = model.csectionsZ.at( depth_ );
        return true;
    }

    return false;
}


void Controller::removeCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    if ( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        model.csectionsX.erase( depth_ );
    }

    else if ( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        model.csectionsY.erase( depth_ );
    }

    else if ( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
        model.csectionsZ.erase( depth_ );
    }


}


void Controller::setImageToCrossSection( const std::string& file_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, double ox_, double oy_, double w_, double h_ )
{
    ImageData image_;
    image_.file = file_;
    image_.ox = ox_;
    image_.oy = oy_;
    image_.w = w_;
    image_.h = h_;

    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        images_csectionsX[ depth_ ] = std::move( image_ );
        if ( model.csectionsX.find( depth_ ) != model.csectionsX.end() )
        {
            CrossSectionPtr csection1_ = model.csectionsX[ depth_ ];
            csection1_->setImage( file_, ox_, oy_, w_, h_ );
        }
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        // the image is constant to all cross-section in the 'HEIGHT' direction
        image_topview.file = image_.file;
        image_topview.ox = image_.ox;
        image_topview.oy = image_.oy;
        image_topview.w = image_.w;
        image_topview.h = image_.h;

        if ( model.csectionsY.find( depth_ ) != model.csectionsY.end() )
        {
            CrossSectionPtr csection1_ = model.csectionsY[ depth_ ];
            csection1_->setImage( image_topview.file, image_topview.ox, image_topview.oy, image_topview.w, image_topview.h );
        }

    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
        images_csectionsZ[ depth_ ] = std::move( image_ );
        if ( model.csectionsZ.find( depth_ ) != model.csectionsZ.end() )
        {
            CrossSectionPtr csection1_ = model.csectionsZ[ depth_ ];
            csection1_->setImage( file_, ox_, oy_, w_, h_ );
        }

    }

    if( ( csection->getDirection() == dir_ ) && ( csection->getDepth() == depth_ ) )
        csection->setImage( file_, ox_, oy_, w_, h_ );

    Settings::CrossSection::CrossSectionDirections dir1_ = topview->getDirection();
    double depth1_ = topview->getDepth();


    if( ( dir1_ == dir_ ) &&  ( depth1_ == depth_ ) )
        topview->setImage( file_, ox_, oy_, w_, h_ );

}


void Controller::clearImageInCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{

    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        // removing from the data structure
        images_csectionsX.erase( depth_ );
        updateImageInMainCrossSection();

        // removing from the fixed cross-section
        if ( model.csectionsX.find( depth_ ) != model.csectionsX.end() )
        {
            CrossSectionPtr csection1_ = model.csectionsX[ depth_ ];
            csection1_->clearImage();
        }
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        image_topview.file.clear();
        image_topview.ox = 0.0;
        image_topview.oy = 0.0;
        image_topview.w = 0.0;
        image_topview.h = 0.0;

        updateImageInTopViewCrossSection();
        if ( model.csectionsY.find( depth_ ) != model.csectionsY.end() )
        {
            CrossSectionPtr csection1_ = model.csectionsY[ depth_ ];
            csection1_->clearImage();

        }
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
        images_csectionsZ.erase( depth_ );
        updateImageInMainCrossSection();
        if ( model.csectionsZ.find( depth_ ) != model.csectionsZ.end() )
        {
            CrossSectionPtr csection1_ = model.csectionsZ[ depth_ ];
            csection1_->clearImage();
        }
    }

}


void Controller::updateImageInMainCrossSection()
{
    const Settings::CrossSection::CrossSectionDirections& dir_ = csection->getDirection();
    double depth_ = csection->getDepth();

    bool has_image_ = false;
    ImageData image_;

    // if there is a fixed cross-section in this position, also update image on it
    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        if( images_csectionsX.find( depth_ ) != images_csectionsX.end() )
        {
            image_ = images_csectionsX[ depth_ ];
            has_image_ = true;
        }

    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
        if( images_csectionsZ.find( depth_ ) != images_csectionsZ.end() )
        {
            image_ = images_csectionsZ[ depth_ ];
            has_image_ = true;
        }
    }

    // updating the image on the main cross-section
    if( has_image_ == true )
        csection->setImage( image_.file, image_.ox, image_.oy, image_.w, image_.h );
    else
        csection->clearImage();

}


void Controller::updateImageInTopViewCrossSection()
{
    const Settings::CrossSection::CrossSectionDirections& dir_ = topview->getDirection();


    if( dir_ != Settings::CrossSection::CrossSectionDirections::Y )
        return;

    // if there is no image on this position of the top-view cross-section, just clear it
    if( image_topview.file.empty() == true )
    {
        topview->clearImage();
        return;
    }

    topview->setImage( image_topview.file, image_topview.ox, image_topview.oy, image_topview.w, image_topview.h );

}


///
/// Objects Methods
///


bool Controller::addObject( std::size_t index_ )
{

    ObjectPtr obj_ = std::make_shared< Object >();

    double ox_ = 0.0, oy_ = 0.0, oz_ = 0.0;
    double w_ = 0.0, h_ = 0.0, d_ = 0.0;

    // since all objects is inside of the volume, the volume will be the bounding box of all of them
    model.volume->getGeometry( ox_, oy_, oz_, w_, h_, d_ );
    obj_->setBoundingBox( ox_, ox_ + w_, oy_, oy_ + h_, oz_, oz_ + d_ );

    if( index_ != UNDEFINED_INDEX )
        obj_->setIndex( index_ );

    current_object = obj_->getIndex();
    obj_->setType( current_object_type );
    model.objects[ current_object ] = std::move( obj_ );


    return true;
}


const ObjectPtr& Controller::getObject( std::size_t index_ ) const
{
    if ( model.objects.find( index_ ) == model.objects.end() ) return nullptr;
    return model.objects.at( index_ );

}


const ObjectPtr& Controller::getCurrentObject() const
{
    return getObject( current_object );
}


const std::map< std::size_t, ObjectPtr >& Controller::getObjects()
{
    return model.objects;
}


void  Controller::setCurrentObjectType( const Settings::Objects::ObjectType& type_ )
{
    ObjectPtr& obj_ = model.objects[ current_object ];
    obj_->setType( type_ );

    current_object_type = type_;

}


Settings::Objects::ObjectType  Controller::getCurrentObjectType() const
{
    return current_object_type;
}


Settings::Objects::ObjectType  Controller::getObjectType( const std::size_t& index_ ) const
{
    if ( model.objects.find( index_ ) == model.objects.end() ) return Settings::Objects::ObjectType::NONE;
    return model.objects.at( index_ )->getType();

}


void Controller::setObjectName( std::size_t index_, const std::string& name_ )
{
    if ( model.objects.find( index_ ) == model.objects.end() ) return;
    model.objects[ index_ ]->setName( name_ );
}


std::string Controller::getObjectName( std::size_t index_) const
{
    if ( model.objects.find( index_ ) == model.objects.end() ) return std::string();
    return model.objects.at( index_ )->getName();
}


void  Controller::setObjectVisibility( std::size_t index_, bool status_ )
{
    if ( model.objects.find( index_ ) == model.objects.end() ) return;
    model.objects[ index_ ]->setVisible( status_ );
}


void Controller::setObjectsVisibility( bool status_ )
{
    for (auto it : model.objects)
    {
        setObjectVisibility( it.first, status_ );
    }
}


void Controller::setObjectColor( std::size_t index_, int r_, int g_, int b_)
{

    if ( model.objects.find( index_ ) == model.objects.end() ) return;
    model.objects[ index_ ]->setColor( r_, g_, b_ );
}


void Controller::setObjectsActive( bool status_ )
{
    for ( auto it : model.objects )
    {
        setObjectActive( it.first, status_ );
    }
}


void Controller::setObjectActive(std::size_t index_, bool status_)
{
    if ( model.objects.find(index_) == model.objects.end() ) return;
    model.objects[ index_ ]->setActive(status_);
}


bool Controller::isObjectActive(std::size_t index_) const
{
    if (model.objects.find(index_) == model.objects.end()) return false;
    return model.objects.at(index_)->isActive();
}


void Controller::setObjectSelectable(std::size_t index_, bool status_)
{
    if (model.objects.find(index_) == model.objects.end()) return;
    model.objects[index_]->setSelectable(status_);
}


bool Controller::isObjectSelectable(std::size_t index_) const
{
    if (model.objects.find(index_) == model.objects.end()) return false;
    return model.objects.at(index_)->isSelectable();
}


void Controller::setObjectSelected(std::size_t index_, bool status_)
{
    if (model.objects.find(index_) == model.objects.end()) return;
    model.objects[index_]->setSelected(status_);
}


bool Controller::isObjectSelected(std::size_t index_) const
{
    if (model.objects.find(index_) == model.objects.end()) return false;
    return model.objects.at(index_)->isSelected();
}


void Controller::setObjectLog( std::size_t index_, const QString& log_ )
{
    if (model.objects.find(index_) == model.objects.end()) return;
    model.objects.at(index_)->saveInformation( log_.toStdString() );
}


QString Controller::getObjectLog( std::size_t index_ ) const
{
    if (model.objects.find(index_) == model.objects.end()) return QString();
    return QString( model.objects.at(index_)->getInformation().c_str() );
}


///
/// Creating Curves and Surfaces of Objects
///



bool Controller::addCurveToObject( Settings::CrossSection::CrossSectionDirections dir_, double depth_, const PolyCurve& curve_ )
{
    if( curve_.isEmpty() == true ) return false;


    ObjectPtr& obj_ = model.objects[ current_object ];
    obj_->setCrossSectionDirection( dir_ );

    Curve2D curve_proc_ = curve_.getCurves2D()[0];

    // is it necessary?
    if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
        depth_ = topview->getDepth();
    else
        //if the curve is on the 'WIDTH' or 'DEPTH' direction, turn the curve monotonic in X
        curve_proc_= SketchLibrary1::monotonicInX( curve_proc_ );


    // smooth the curve before add it
    curve_proc_ = SketchLibrary1::smooth( curve_proc_ );

    bool added_curve_ = obj_->addCurve( depth_, PolyCurve( curve_proc_ ) );
    if ( added_curve_ == false ) return false;

    // create or update the preview surface
    updatePreviewSurface();

    return true;


}


bool Controller::removeCurveFromObject( Settings::CrossSection::CrossSectionDirections dir_, double depth_ )
{

    ObjectPtr& obj_ = model.objects[ current_object ];
    obj_->removeCurve( depth_ );

    CrossSectionPtr csection_;
    bool status_ = getCrossSection( dir_, depth_, csection_ );
    if (status_ == false)
    {
        return false;
    }

    //TODO: check this if. Remove cross-section only of vector of used csections, not from the opened fixed csections
    // the user may want to keep to view the fixed cross-section
    //    if ( csection_->isEmpty() == true )
    //        removeCrossSection( dir_, depth_ );

    return true;

}


void Controller::addTrajectoryToObject( const PolyCurve& curve_ )
{

    if( curve_.isEmpty() == true ) return;

    ObjectPtr& obj_ = model.objects[ current_object ];

    Curve2D curve_proc_;


    if( current_direction == Settings::CrossSection::CrossSectionDirections::X )
    {
        // if the current_direction is in the direction X, the trajectory is along the X direction in the top-view plane,
        // so it needs to be monotonic on the direction X
        curve_proc_ = SketchLibrary1::monotonicInX( curve_.getCurves2D()[0] );
    }
    else {

        // if the current_direction is in the direction Z, the trajectory is along the Y direction in the top-view plane,
        // so it needs to be monotonic on the direction Y
        curve_proc_ = SketchLibrary1::monotonicInY( curve_.getCurves2D()[0] );
    }

    curve_proc_ = SketchLibrary1::smooth( curve_proc_ );

    obj_->addTrajectory( PolyCurve( curve_proc_ ) );

}


void Controller::addLastTrajectoryToObject()
{
    addTrajectoryToObject( last_trajectory );
}


void Controller::removeTrajectoryFromObject()
{

    ObjectPtr& obj_ = model.objects[ current_object ];
    obj_->removeTrajectory();

}


bool Controller::commitObjectSurface()
{

    rules_processor.stopTestSurfaceInsertion();

    applyStratigraphicRule();

    bool status_ = createObjectSurface();
    if( status_ == false )
    {
        // it was not possible to create the surface
        // needs to do something else?
        return false;
    }

    ObjectPtr obj_ = model.objects[ current_object ];

    // the surface was created, so it is not more possible to add any curve/trajectory
    obj_->setDone( true );

    updateModel();

    // create a new object, with the current type of object (stratigraphy or strutural)
    addObject();
    return true;

}


bool Controller::createObjectSurface()
{

    ObjectPtr obj_ = model.objects[ current_object ];
    bool surface_created_ = false;

    if( obj_->getNumberOfCrossSections() == 0 ) return false;

    // if there is only one curve
    bool single_csection_ = ( obj_->getNumberOfCrossSections() == 1 );

    // if the object is being created in the 'HEIGHT' direction, so the curves are level curves
    bool is_level_curves_ = ( obj_->getCrossSectionDirection() == Settings::CrossSection::CrossSectionDirections::Y );

    // there more than one curve or they are level curves?
    bool is_general_surface_ = ( !single_csection_ || is_level_curves_ );


    // there more than one curve or they are level curves, create a surface using more general interpolation
    if( is_general_surface_ == true )
        surface_created_ = createGeneralSurface();
    else
    {
        // if the surface has not a trajectory, create a simple extruded surface
        if( obj_->hasTrajectory() == false )
            surface_created_ = createLinearExtrudedSurface();
        else
            // if the surface has a trajectory, create an extruded guided surface
            surface_created_ = createExtrudedSurface();
    }


    return surface_created_;

}


bool Controller::createGeneralSurface()
{

    ObjectPtr obj_ = model.objects[ current_object ];

    std::vector< double > curves_ = obj_->getCurves3D();

    bool surface_created_ = rules_processor.createSurface( current_object, curves_ );
    return surface_created_;

}


bool Controller::createExtrudedSurface()
{
    ObjectPtr obj_ = model.objects[ current_object ];
    if( obj_->hasTrajectory() == false ) return false;

    // return all the curves existents in the object, since the method is a guided extrusion
    // should exist only one curve
    std::vector< double > curves_ = obj_->getCurves2D();


    std::map< double, PolyCurve > curves_map_ = obj_->getCurves();
    double depth_ = curves_map_.begin()->first;

    // return the trajectory
    const PolyCurve& path_ = obj_->getTrajectory();


    bool surface_created_ = false;

    // creating the surface based on the directio which the object was created
    if( obj_->getCrossSectionDirection() == Settings::CrossSection::CrossSectionDirections::X )
    {
        surface_created_ = rules_processor.createWidthwiseExtrudedSurface( current_object, curves_, depth_, path_.getPointsSwapped() );
    }

    else if( obj_->getCrossSectionDirection() == Settings::CrossSection::CrossSectionDirections::Z )
    {
        surface_created_ = rules_processor.createLengthwiseExtrudedSurface( current_object, curves_, depth_, path_.getPoints() );
    }

    // saving the trajectory if surface creation was successful, so that the user can reuse it to create other object
    if( surface_created_ == true )
    {
        last_trajectory =  path_;
    }

    return surface_created_;

}


bool Controller::createLinearExtrudedSurface()
{

    ObjectPtr obj_ = model.objects[ current_object ];

    std::vector< double > curves_ = obj_->getCurves2D();
    bool surface_created_ = false;

    // creating the surface based on the directio which the object was created
    if( obj_->getCrossSectionDirection() == Settings::CrossSection::CrossSectionDirections::X )
    {
        surface_created_ = rules_processor.createWidthwiseExtrudedSurface( current_object, curves_ );
    }

    else if( obj_->getCrossSectionDirection() == Settings::CrossSection::CrossSectionDirections::Z )
    {
        surface_created_ = rules_processor.createLengthwiseExtrudedSurface( current_object, curves_ );

    }

    return surface_created_;

}


///
/// Updating Models Methods
///


void Controller::updateModel()
{

    // setting all objects as inactive, so that we view only the actives
    setObjectsActive( false );

    std::vector< std::size_t > actives_ = rules_processor.getActiveSurfaces();

    std::size_t number_of_actives_ = actives_.size();
    for ( std::size_t j = 0; j < number_of_actives_; ++j )
    {
        std::size_t id_ = actives_.at( j );

        setObjectActive( id_, true );

        // update the surface and the curves
        updateObjectSurface( id_ );
        updateObjectCurves( id_ );
    }

    // check if it is needed
    setObjectActive( current_object, true );

}


void Controller::updateObjectCurveInCrossSection( const std::size_t& index_, double depth_ )
{
    ObjectPtr obj_ = model.objects[ index_ ];

    // if the object is current,only update the cross-section curve (if existent), if it was not the user that added it
    // if it was created automatically it can be updated
    if( index_ == current_object )
    {
        bool has_curve_ = obj_->hasCurve( depth_ );
        if( has_curve_ == true ) return;
    }

    clearAndSetCurveinCrossSectionFromRulesProcessor( index_ , depth_ );

}


void Controller::updateObjectsCurvesInCrossSection( double depth_ )
{
    // updating all curves in certain depth ( using the current direction)
    for( auto it_: model.objects )
    {
        updateObjectCurveInCrossSection( it_.first, depth_ );
    }

}


void Controller::updateCrossSectionsX()
{

    // updating all curves in all depths in the 'WIDTH' direction
    for( auto it_: model.csectionsX )
    {
        updateObjectsCurvesInCrossSection( it_.first );
    }

}


void Controller::updateCrossSectionsY()
{
    // updating all curves in all depths in the 'HEIGHT' direction
    for( auto it_: model.csectionsY )
    {
        updateObjectsCurvesInCrossSection( it_.first );
    }

}


void Controller::updateCrossSectionsZ()
{
    // updating all curves in all depths in the 'DEPTH' direction
    for( auto it_: model.csectionsZ )
    {
        updateObjectsCurvesInCrossSection( it_.first );
    }

}


void Controller::clearAndSetCurveinCrossSectionFromRulesProcessor( const std::size_t& index_, double depth_ )
{

    // updating the curve of the object which index is 'index_', added in the cross-section 'depth'in the current direction
    clearAndSetCurveinCrossSectionFromRulesProcessor( index_, current_direction, depth_ );

}


void Controller::clearAndSetCurveinCrossSectionFromRulesProcessor( const std::size_t& index_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    ObjectPtr& obj_ = model.objects[ index_ ];

    bool has_curve_ = false;

    std::vector< double > vertices_;
    std::vector< std::size_t > edges_;

    // updating the curve of the object which index is 'index_', added in the cross-section 'depth'in the given direction
    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        has_curve_ = rules_processor.getWidthCrossSectionCurve( index_, indexCrossSectionX( depth_ ), vertices_, edges_ );
    }

    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
        has_curve_ = rules_processor.getLengthCrossSectionCurve( index_, indexCrossSectionZ( depth_ ), vertices_, edges_ );
    }

    // rules processor did not find an intersection of the surface with the given cross-section
    if( has_curve_ == false )
    {
        std::cout << "No curve for object " << index_ << " in cross-section " << depth_ << std::endl << std::flush;
        return;
    }

    // rules processor did find an intersection of the surface with the given cross-section
    PolyCurve curve_( vertices_, edges_ );

    // remove the existent and update with the new one
    obj_->removeCurve( depth_ );
    obj_->updateCurve( depth_, curve_ );
}


///// Updating object index in all directions
void Controller::updateObjectCurves( const std::size_t& index_ )
{

    updateObjectCurveInCrossSection( index_, csection->getDepth() );

    // the fixed cross-section depends on the current direction
    // so if the current cross-section is 'WIDTH', update the object curves in all the fixed cross-section
    // in the 'WIDTH' direction, do the same for any other direction

    Settings::CrossSection::CrossSectionDirections dir_ = current_direction;
    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        for( auto it_: model.csectionsX )
            updateObjectCurveInCrossSection( index_, it_.first );
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        clearAndSetCurveinCrossSectionFromRulesProcessor( index_, csection->getDirection(), csection->getDepth() );
        for( auto it_: model.csectionsY )
            updateObjectCurveInCrossSection( index_, it_.first );
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
        for( auto it_: model.csectionsZ )
            updateObjectCurveInCrossSection( index_, it_.first );
    }

}


void Controller::updateObjectSurface( const std::size_t& index_ )
{
    std::vector< double > vertices_;
    std::vector< std::size_t > faces_;

    ObjectPtr obj_ = model.objects[ index_ ];

    // if there is no surface
    bool has_surface_ = rules_processor.getMesh( index_, vertices_, faces_ );
    if( has_surface_  == false )
    {
        obj_->removeSurface();
        return;
    }

    std::vector< double > normals_;
    rules_processor.getNormals( index_, normals_ );

    // creating the surface
    Surface surface_;
    surface_.setVertices( vertices_ );
    surface_.setFaces( faces_ );
    surface_.setNormals( normals_ );

    obj_->setSurface( surface_ );

    // getting the trajectory back
    std::vector< double > trajectory_;
    bool has_trajectory_ = rules_processor.getExtrusionPath( index_, trajectory_ );
    if( has_trajectory_ == true )
    {
        PolyCurve traj_ = PolyCurve( trajectory_ );

////        if( csection->getDirection() != Settings::CrossSection::CrossSectionDirections::Y )
//        if( current_direction != Settings::CrossSection::CrossSectionDirections::Y )
//        {
//            //            if( csection->getDirection() != obj_->getCrossSectionDirection() )
//            //                traj_ = PolyCurve( traj_.getPointsSwapped() );
//        }

        obj_->removeTrajectory();
        obj_->addTrajectory( traj_ );

    }



}


void Controller::updatePreviewSurface()
{

    // creating the preview surface.
    rules_processor.testSurfaceInsertion();

    bool surface_created_ = createObjectSurface();
    if( surface_created_ == false )
        return;

    updateObjectSurface( current_object );
}



///
/// Regions Methods
///


std::vector<std::size_t > Controller::defineRegions()
{
    model.regions.clear();

    // get tetrahedral mesh of the regions

    std::vector< double > vertices_;
    std::vector< std::vector< std::size_t > > regions_;
    bool status_ = rules_processor.getTetrahedralMesh( vertices_, regions_ );

    // there is no region mesh
    if( status_ == false ) return std::vector<std::size_t >();


    // the region should be limited by the volume bounding box
    double w_ = 0, h_ = 0,  l_ = 0;
    double ox_ = 0, oy_ = 0, oz_ = 0;
    model.volume->getGeometry( ox_, oy_, oz_, w_, h_, l_ );


    // creating region colors using a color library
    std::size_t number_of_regions_ = regions_.size();
    std::vector< int > colors_ = rules_processor.getRegionsColor( number_of_regions_ );

    // getting the volumes of each region
    std::vector< double > volumes_;
    rules_processor.getRegionVolumeList( volumes_ );


    // variable to determine the total volume, i.e., the sum of each region volume
    double volume_sum_ = 0.0;
    for ( unsigned int i = 0; i < number_of_regions_; ++i)
    {
        Color color_;
        color_.red = colors_[ 3*i ];
        color_.green = colors_[ 3*i + 1 ];
        color_.blue = colors_[ 3*i + 2 ];

        RegionsPtr region_ = std::make_shared< Regions >();
        region_->setIndex( i );
        region_->setVertices( vertices_ );
        region_->setTetrahedralCells( regions_[ i ] );
        region_->setColor( color_.red, color_.green, color_.blue );
        region_->setVolume( volumes_[ i ] );
        region_->setMaxMin( ox_ + w_, oy_ + h_, oz_ + l_, ox_, oy_, oz_ );

        volume_sum_ += volumes_[ i ];

        model.regions[region_->getIndex()] = region_;

        // determining the boundary of the region intersected with the current cross-section
        getRegionCrossSectionBoundary( i );
    }

     model.volume->setVolume( volume_sum_ );

     // returning the order of the surfaces
     // maybe it needs to be placed in other method
    std::vector< std::size_t > surfaces_indexes_ = rules_processor.getOrderedActiveSurfaces();
    return surfaces_indexes_;
}

std::vector<std::size_t > Controller::getOrderedSurfacesIndices()
{
    std::vector< std::size_t > surfaces_indexes_ = rules_processor.getOrderedSurfaces();
    return surfaces_indexes_;
}

std::vector<std::size_t > Controller::getOrderedActiveSurfacesIndices()
{
    std::vector< std::size_t > surfaces_indexes_ = rules_processor.getOrderedActiveSurfaces();
    return surfaces_indexes_;
}


bool Controller::getRegionCrossSectionBoundary( std::size_t index_ )
{

    RegionsPtr region_ = model.regions[ index_ ];

    std::vector<double> vertices_upper_;
    std::vector<size_t> edges_upper_;
    std::vector<double> vertices_lower_;
    std::vector<size_t> edges_lower_;



    if( current_direction == Settings::CrossSection::CrossSectionDirections::X )
    {
        rules_processor.getRegionCurveBoxesAtWidth( index_, indexCrossSectionX( csection->getDepth() ), vertices_lower_, edges_lower_, vertices_upper_, edges_upper_ );
    }

    else if( current_direction == Settings::CrossSection::CrossSectionDirections::Z )
    {
        rules_processor.getRegionCurveBoxesAtLength( index_, indexCrossSectionZ( csection->getDepth() ), vertices_lower_, edges_lower_, vertices_upper_, edges_upper_ );
    }


    // defining the boundary of the region intersected with the current cross-section
    PolyCurve lower_, upper_;
    lower_.fromVector( vertices_lower_, edges_lower_ );
    upper_.fromVector( vertices_upper_, edges_upper_ );

    region_->setLowerBound( lower_ );
    region_->setUpperBound( upper_ );

    return true;
}



void Controller::updateRegions()
{
    // defining the boundary of each region intersected with the current cross-section
    for( auto it_: model.regions )
        getRegionCrossSectionBoundary( it_.first );
}


void Controller::setRegionsVisible(bool status_)
{
    for (auto it : model.regions)
    {
        it.second->setVisible(status_);
    }

}


void Controller::setRegionVisible(std::size_t index_, bool status_)
{
    if ( model.regions.find( index_ ) == model.regions.end() ) return;
    model.regions[ index_ ]->setVisible(status_);
}


bool Controller::isRegionVisible( std::size_t index_ ) const
{
    if (model.regions.find(index_) == model.regions.end()) return false;
    return model.regions.at(index_)->isVisible();
}


void Controller::setRegionColor(std::size_t index_, int r_, int g_, int b_ )
{
    if (model.regions.find(index_) == model.regions.end()) return;
    model.regions[index_]->setColor( r_, g_, b_ );

}


void Controller::getRegionColor(std::size_t index_, int& r_, int& g_, int& b_)
const
{
    if (model.regions.find(index_) == model.regions.end()) return;
    model.regions.at(index_)->getColor(r_, g_, b_);
}


void Controller::setRegionsActive(bool status_)
{
    for (auto it : model.regions)
    {
        it.second->setActive(status_);
    }
}


void Controller::setRegionActive(std::size_t index_, bool status_)
{
    if (model.regions.find(index_) == model.regions.end()) return;
    model.regions[index_]->setActive(status_);
}


bool Controller::isRegionActive(std::size_t index_) const
{
    if (model.regions.find(index_) == model.regions.end()) return false;
    return model.regions.at(index_)->isActive();
}


void Controller::setRegionSelectable(std::size_t index_, bool status_)
{
    if (model.regions.find(index_) == model.regions.end()) return;
    model.regions[index_]->setSelectable(status_);
}


bool Controller::isRegionSelectable(std::size_t index_) const
{
    if (model.regions.find(index_) == model.regions.end()) return false;
    return model.regions.at(index_)->isSelectable();
}


void Controller::setRegionSelected(std::size_t index_, bool status_)
{
    if (model.regions.find(index_) == model.regions.end()) return;
    model.regions[index_]->setSelected(status_);
}


bool Controller::isRegionSelected(std::size_t index_) const
{
    if (model.regions.find(index_) == model.regions.end()) return false;
    return model.regions.at(index_)->isSelected();
}



const std::map< std::size_t, RegionsPtr >& Controller::getRegions() const
{
    return model.regions;
}


void Controller::removeRegions()
{
    for( auto it: model.regions )
        (it.second).reset();
    model.regions.clear();

    for( auto it: model.domains )
        (it.second).clear();
    model.domains.clear();
    regions_in_domains.clear();
}



///
/// Domains Methods
///


std::size_t Controller::createDomain( std::set< std::size_t > indexes_ )
{
    std::size_t id_ = 0;

    // creating the index of the domains depending on the already existents domains
    if( model.domains.empty() == false )
    {
        for( auto it_: model.domains )
        {
            id_ = it_.first;
        }

        ++id_;
    }

    model.domains[ id_ ] = Domain();

    // if indexes_ is not empty, add each region inside the created domain
    for( auto it_: indexes_ )
        addRegionToDomain( it_, id_ );

    return id_;

}


void Controller::setDomainName( std::size_t index_, const std::string& name_ )
{
    if (model.domains.find(index_) == model.domains.end()) return;
    model.domains[ index_ ].setName( name_ );
}

void Controller::setDomainColor( std::size_t index_, int red_, int green_, int blue_ )
{
    if (model.domains.find(index_) == model.domains.end()) return;
    model.domains[ index_ ].setColor( red_, green_, blue_ );
}


bool Controller::addRegionToDomain( std::size_t region_id_, std::size_t domain_id_ )
{


    if (model.regions.find(region_id_) == model.regions.end()) return false;
    if (model.domains.find(domain_id_) == model.domains.end()) return false;
    if( regions_in_domains.find( region_id_ ) != regions_in_domains.end() ) return false;

    // if region_id_ is a region index valid
    // if domain_id_ is a domain index valid
    // if the regions is not added already in another domain, assign the region to the domain

    model.domains[ domain_id_ ].addRegion( region_id_ );
    regions_in_domains.insert( region_id_ );

    RegionsPtr reg_ = model.regions[region_id_];
    reg_->setDomain(domain_id_);

    double volume_ = model.domains[ domain_id_ ].getDomainVolume();
    volume_ += model.regions[region_id_]->getVolume();

    model.domains[ domain_id_ ].setDomainVolume( volume_ );
    return true;
}


bool Controller::removeRegionFromDomain(std::size_t region_id_, std::size_t domain_id_)
{
    if (model.regions.find(region_id_) == model.regions.end()) return false;
    if (model.domains.find(domain_id_) == model.domains.end()) return false;
    if( regions_in_domains.find( region_id_ ) == regions_in_domains.end() ) return false;

    // if region_id_ is a region index valid
    // if domain_id_ is a domain index valid
    // if the regions belongs to the domain, remove the region from the domain

    model.domains[domain_id_].removeRegion( region_id_ );
    regions_in_domains.erase( region_id_ );

    RegionsPtr reg_ = model.regions[region_id_];
    reg_->removeFromDomain();

    // update the volume of the domain
    double volume_ = model.domains[ domain_id_ ].getDomainVolume();
    volume_ -= model.regions[region_id_]->getVolume();

    model.domains[ domain_id_ ].setDomainVolume( volume_ );

    return true;
}


std::set< std::size_t> Controller::getRegionsFromDomain(std::size_t domain_id_) const
{
    if (model.domains.find(domain_id_) == model.domains.end()) return std::set< std::size_t>();

    const Domain& domain_ = model.domains.at( domain_id_ );
    return domain_.getRegions();

}


void Controller::removeDomain(std::size_t domain_id_)
{

    if (model.domains.find(domain_id_) == model.domains.end()) return;

    Domain& domain_ = model.domains[ domain_id_ ];
    domain_.clear();

    model.domains.erase( domain_id_ );
}


std::vector< std::size_t > Controller::getDomains()
{

    std::vector< std::size_t > indexes_;

    for( auto it_: model.domains )
    {
        indexes_.push_back( it_.first );
    }

    return indexes_;


}



std::vector< std::size_t > Controller::getDomainsToFlowDiagnostics()
{

    std::vector< std::size_t > indexes_;
    std::vector<int> diff_;

    for( auto it_: model.regions )
    {
        std::size_t id_ = it_.first;

        if( regions_in_domains.find( id_ ) != regions_in_domains.end() ) continue;

        // if the region does not belong to any domain, create a domain to it
        std::size_t domain_id_ = createDomain();
        addRegionToDomain( id_, domain_id_ );
    }

    for( auto it_: model.domains )
    {
        indexes_.push_back( it_.first );
    }

    return indexes_;

}



void Controller::getDomainColor( std::size_t domain_id_, int &red_, int &green_, int& blue_ )
{
    if (model.domains.find(domain_id_) == model.domains.end()) return;

    std::size_t reg_id_ = *model.domains[ domain_id_].getRegions().begin();

    RegionsPtr reg_ = model.regions[ reg_id_ ];
    reg_->getColor( red_, green_, blue_ );

}



///
/// Rules-Processor Methods
///


void Controller::initRulesProcessor()
{
    updateBoundingBoxRulesProcessor();
    rules_processor.removeAboveIntersection();
    setMeshResolution( Controller::MeshResolution::MEDIUM );

}


void Controller::updateBoundingBoxRulesProcessor()
{
    if( model.volume == nullptr ) return;

    double ox = 0.0, oy = 0.0, oz = 0.0;
    double width_ = 0.0, height_ = 0.0, lenght_ = 0.0;

    model.volume->getGeometry( ox, oy, oz, width_, height_, lenght_ );

    rules_processor.setOrigin( ox, oy, oz );
    rules_processor.setLenght( width_, height_, lenght_ );

}



///
/// Discretization Methods
///


void Controller::setVolumeDiscretization()
{
    std::size_t width_disc_ = rules_processor.getWidthResolution();
    std::size_t lenght_disc_ = rules_processor.getLengthResolution();
    std::size_t height_disc_ = 100;

    csection_stepx = static_cast< double >( model.volume->getWidth()/width_disc_ );
    csection_stepy = static_cast< double >( model.volume->getHeight()/height_disc_ );
    csection_stepz = static_cast< double >( model.volume->getLenght()/lenght_disc_ );
}


std::size_t Controller::getCurrentDiscretization() const
{

    if( current_direction == Settings::CrossSection::CrossSectionDirections::X )
    {
        return rules_processor.getWidthResolution();
    }
    else if( current_direction == Settings::CrossSection::CrossSectionDirections::Y )
    {
        // so far, a hard code ... it should be modified
        return 100;
    }
    else
    {
        return rules_processor.getLengthResolution();
    }

}


void Controller::getCurrentRange( double& min_, double& max_ ) const
{
    double ox_ = 0.0, oy_ = 0.0, oz_ = 0.0;
    double width_ = 0.0, height_ = 0.0, lenght_ = 0.0;

    model.volume->getGeometry( ox_, oy_, oz_, width_, height_, lenght_ );

    if( current_direction == Settings::CrossSection::CrossSectionDirections::X )
    {
        min_ = ox_;
        max_ = ox_ + width_;
    }
    else if( current_direction == Settings::CrossSection::CrossSectionDirections::Y )
    {
        min_ = oy_;
        max_ = oy_ + height_;
    }
    else
    {
        min_ = oz_;
        max_ = oz_ + lenght_;

    }

}


std::size_t Controller::indexCrossSectionX( double value_ ) const
{
    return static_cast< std::size_t > ( value_/csection_stepx );
}


std::size_t Controller::indexCrossSectionZ( double value_ ) const
{
    return static_cast< std::size_t > ( value_/csection_stepz );
}



///
/// Activing Rules Methods
///


void Controller::setRemoveAbove()
{
    current_rule = Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE;
}


void Controller::setRemoveAboveIntersection()
{
    current_rule = Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE_INTERSECTION;
}


void Controller::setRemoveBelow()
{
    current_rule = Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW;
}


void Controller::setRemoveBelowIntersection()
{
    current_rule = Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW_INTERSECTION;
}


void Controller::applyStratigraphicRule()
{
    if( current_rule == Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE )
        rules_processor.removeAbove();
    else if( current_rule == Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE_INTERSECTION )
        rules_processor.removeAboveIntersection();
    else if( current_rule == Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW )
        rules_processor.removeBelow();
    else if( current_rule == Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW_INTERSECTION )
        rules_processor.removeBelowIntersection();

}


///
/// Preserve Methods
///



Settings::Objects::BoundaryRegion Controller::getCurrentBoundaryRegion() const
{
    return boundary_region;
}




void Controller::stopCreateAbove()
{
    rules_processor.stopDefineAbove();

    // if stop create above, all the objects should be not more selectable
    for( std::size_t id_: selectable_objects )
    {
        ObjectPtr& obj_ = model.objects[ id_ ];
        obj_->setSelectable( false );
    }
    selectable_objects.clear();

    // neither should exist a selected object
    setObjectSelected( upper_index, false );

}




void Controller::stopCreateBelow()
{
    rules_processor.stopDefineBelow();

    // if stop create below, all the objects should be not more selectable
    for( std::size_t id_: selectable_objects )
    {
        ObjectPtr& obj_ = model.objects[ id_ ];
        obj_->setSelectable( false );
    }
    selectable_objects.clear();

    // neither should exist a selected object
    setObjectSelected( bottom_index, false );

}




void Controller::stopCreateRegion()
{

    //    rules_processor.stopDefineRegion();

    for( std::size_t id_: selectable_objects )
    {
        ObjectPtr& obj_ = model.objects[ id_ ];
        obj_->setSelectable( false );
    }
    //    setObjectSelected( bottom_index, false );
    //    selectable_objects.clear();


}


// Enable/Stop Preserve Above -- new methods

void Controller::enablePreserveAbove( bool status_ )
{

    // if status is true, set the boundary region option as 'ABOVE', otherwise
    // call the stop preserve above

    if( status_ == true )
        boundary_region = Settings::Objects::BoundaryRegion::ABOVE;
    else
        rules_processor.stopPreserveAbove();
}


// Enable/Stop Preserve Below -- new methods

void Controller::enablePreserveBelow( bool status_ )
{
    // if status is true, set the boundary region option as 'BELOW', otherwise
    // call the stop preserve below

    if( status_ == true )
        boundary_region = Settings::Objects::BoundaryRegion::BELOW;
    else
        rules_processor.stopPreserveBelow();

}





void Controller::setObjectSelectedAsBoundary( const std::size_t& index_ )
{

    // if the boundary region option as 'ABOVE' pass it to rules processor as the defineAbove, and
    // mark all of the rest as not selectable anymore
    if( boundary_region == Settings::Objects::BoundaryRegion::ABOVE )
    {
        if( model.objects.find( index_ ) == model.objects.end() ) return;

        upper_index = index_;
        rules_processor.defineAbove( index_ );
        model.objects[ index_ ] ->setSelected( true );

        for( std::size_t id_: selectable_objects )
        {
            ObjectPtr& obj_ = model.objects[ id_ ];
            obj_->setSelectable( false );
        }
        selectable_objects.clear();

    }

    // if the boundary region option as 'BELOW' pass it to rules processor as the defineBelow, and
    // mark all of the rest as not selectable anymore
    else if( boundary_region == Settings::Objects::BoundaryRegion::BELOW )
    {
        if( model.objects.find( index_ ) == model.objects.end() ) return;

        bottom_index = index_;
        rules_processor.defineBelow( index_ );
        model.objects[ index_ ] ->setSelected( true );

        for( std::size_t id_: selectable_objects )
        {
            ObjectPtr& obj_ = model.objects[ id_ ];
            obj_->setSelectable( false );
        }
        selectable_objects.clear();

    }
}


bool Controller::setRegionBySketchAsBoundary( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, PolyCurve& boundary_ )
{

    std::vector< double > curve3d_;
    std::vector< double > vertices_;
    std::vector< std::size_t > edges_;


    if( boundary_region == Settings::Objects::BoundaryRegion::ABOVE )
    {

        // Rules Processor needs the sketch curve as a 3d curve, so turn the 2d sketch into a 3d curve using the depth of
        // the cross-section

        if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
        {
            curve3d_ = curve_.addXCoordinate( depth_, true );
            bool status_ = rules_processor.requestPreserveAbove( curve3d_ );
            if( status_ == true )
            {
                rules_processor.getPreserveAboveCurveBoxAtWidth( indexCrossSectionX( depth_ ), vertices_, edges_ );
            }
            else
                return false;
        }
        else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
        {
            curve3d_ = curve_.addZCoordinate( depth_, false );
            bool status_ = rules_processor.requestPreserveAbove( curve3d_ );
            if( status_ == true )
            {
                rules_processor.getPreserveAboveCurveBoxAtLength( indexCrossSectionZ( depth_ ), vertices_, edges_ );
            }
            else
                return false;
        }

    }

    else if( boundary_region == Settings::Objects::BoundaryRegion::BELOW )
    {
        // pass curve to rules_processor and get the region

        if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
        {
            curve3d_ = curve_.addXCoordinate( depth_, true );
            bool status_ = rules_processor.requestPreserveBelow( curve3d_ );
            if( status_ == true )
            {
                rules_processor.getPreserveBelowCurveBoxAtLength( indexCrossSectionX( depth_ ), vertices_, edges_ );
            }
            else
                return false;
        }
        else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
        {
            curve3d_ = curve_.addZCoordinate( depth_, false );
            bool status_ = rules_processor.requestPreserveBelow( curve3d_ );
            if( status_ == true )
            {
                rules_processor.getPreserveBelowCurveBoxAtWidth( indexCrossSectionZ( depth_ ), vertices_, edges_ );
            }
            else
                return false;

        }

    }

    boundary_.fromVector( vertices_, edges_ );
    return true;

}


bool Controller::updateRegionBoundary( PolyCurve& boundary_ )
{

    bool status_ = false;

    boundary_.clear();

    // if preserve 'ABOVE' is active, so hightlight the lower curve that delimit  the area
    if( rules_processor.preserveAboveIsActive() == true )
    {
        getLowerBoundary( boundary_ );
        status_ = true;
    }

    // if preserve 'BELOW' is active, so hightlight the upper curve that delimit  the area
    else if( rules_processor.preserveBelowIsActive() == true )
    {
        getUpperBoundary( boundary_ );
        status_ = true;
    }

    return status_;
}


bool Controller::setRegionByPointAsBoundary( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ )
{
    std::vector< double > point_;


    // Rules Processor needs the point as a 3d point, so turn the 2d point into a 3d point using the depth of
    // the cross-section
    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        point_.push_back( depth_ );
        point_.push_back( static_cast< double >( py_ ) );
        point_.push_back( static_cast< double >( px_ ) );
    }

    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        point_.push_back( static_cast< double >( px_ ) );
        point_.push_back( depth_ );
        point_.push_back( static_cast< double >( py_ ) );


    }

    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
        point_.push_back( static_cast< double >( px_ ) );
        point_.push_back( static_cast< double >( py_ ) );
        point_.push_back( depth_ );

    }

    bool request_ = rules_processor.requestPreserveRegion( point_ );
    return request_;

}



bool Controller::isDefineAboveActive( PolyCurve& boundary_ )
{

    bool status_ = rules_processor.preserveAboveIsActive();
    if( status_ == false ) return false;

    getLowerBoundary( boundary_ );
    return true;
}


bool Controller::isDefineBelowActive( PolyCurve& boundary_ )
{

    bool status_ = rules_processor.preserveBelowIsActive();
    if( status_ == false ) return false;

    getUpperBoundary( boundary_ );
    return true;

}


void Controller::getLowerBoundary( PolyCurve& boundary_ )
{

    std::vector< double > vertices_;
    std::vector< std::size_t > edges_;

    if(  csection->getDirection() == Settings::CrossSection::CrossSectionDirections::X )
    {
        rules_processor.getPreserveAboveCurveBoxAtWidth( indexCrossSectionX( csection->getDepth() ), vertices_, edges_ );
    }
    else if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::Z )
    {
        rules_processor.getPreserveAboveCurveBoxAtLength( indexCrossSectionZ( csection->getDepth() ), vertices_, edges_ );
    }

    boundary_.fromVector( vertices_, edges_ );
}


void Controller::getUpperBoundary( PolyCurve& boundary_ )
{

    std::vector< double > vertices_;
    std::vector< std::size_t > edges_;

    if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::X )
    {
        rules_processor.getPreserveBelowCurveBoxAtWidth( indexCrossSectionX( csection->getDepth() ), vertices_, edges_ );

    }

    else if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::Z )
    {
        rules_processor.getPreserveBelowCurveBoxAtLength( indexCrossSectionZ( csection->getDepth() ), vertices_, edges_ );

    }

    boundary_.fromVector( vertices_, edges_ );
}


void Controller::setPointGuidedExtrusion( float , float , double , const Settings::CrossSection::CrossSectionDirections&  )
{
}


///
/// Application Methods
///

void Controller::setCurrentColor( int red_, int green_, int blue_ )
{
    current_color.red = red_;
    current_color.green = green_;
    current_color.blue = blue_;

    ObjectPtr& obj_ = model.objects[ current_object ];
    obj_->setColor( red_, green_, blue_ );

}


bool Controller::undo()
{
    bool undo_done = rules_processor.undo();
    if( undo_done == false ) return false;

    updateModel();
    return true;
}

bool Controller::redo()
{
    bool redo_done = rules_processor.redo();
    if( redo_done == false ) return false;

    updateModel();
    return true;
}


bool Controller::canUndo()
{
    return rules_processor.canUndo();
}


bool Controller::canRedo()
{
    return rules_processor.canRedo();
}



void Controller::saveFile( const std::string& filename )
{
    bool status_ = rules_processor.saveFile( filename );

    if( status_ == false )
    {
        std::cout << "Error saving file." << std::endl << std::flush;
        return;
    }

    saveObjectsMetaData( filename );
}


bool Controller::saveObjectsMetaData( const std::string& filename )
{
    //change to something that it is not Qt
    // method too long, create new methods to modularize it

    std::string complete_filename = filename + ".json";
    QFile save_file( QString( complete_filename.c_str() ) );

    if ( !save_file.open( QIODevice::WriteOnly ) ) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject metadatas;

    // saving the objects ( stratigraphies/structurals)
    QJsonArray objects_array_;
    for( auto it: model.objects )
    {
        if( it.first == current_object ) continue;

        const ObjectPtr& obj_ = it.second;
        QJsonObject object_;
        obj_->write( object_ );
        objects_array_.append( object_ );
    }
    metadatas["objects"] = objects_array_;


    // saving regions
    if( model.regions.empty() == false )
    {
        QJsonArray regions_array_;
        for( auto it: model.regions )
        {
            const RegionsPtr& reg_ = it.second;
            QJsonObject region_;
            reg_->write( region_ );
            regions_array_.append( region_ );
        }
        metadatas["regions"] = regions_array_;


    }


    // saving domains
    if( model.domains.empty() == false )
    {

        QJsonArray domains_array_;
        for( auto it_: model.domains )
        {
            const Domain& dom_ = it_.second;

            QJsonArray regions_set_array_;
            std::set< std::size_t > regions_ = dom_.getRegions();
            for( auto itd_: regions_ )
                regions_set_array_.push_back( static_cast< int >( itd_ ) );

            QJsonObject domain_;
            domain_[ "index" ] = static_cast< int >( it_.first );
            domain_[ "regions" ] = regions_set_array_;


            domains_array_.append( domain_ );
        }
        metadatas["domains"] = domains_array_;

    }

    QJsonDocument save_doc( metadatas );
    save_file.write( save_doc.toJson() );

    return true;
}


void Controller::loadFile( const std::string& filename, Controller::MeshResolution& resol_ )
{

    // clear the current session...
    clear();

    // initialize a new session
    init();


    bool status_ = rules_processor.loadFile( filename );

    if( status_ == false )
    {
        std::cout << "Error opening file." << std::endl << std::flush;
        return;
    }

    loadObjects( filename, resol_ );
}


void Controller::loadObjects( const std::string& filename, Controller::MeshResolution& resol_ )
{


    double ox, oy, oz;
    double width, height, depth;


    // loading the volume information
    rules_processor.getOrigin( ox, oy, oz );
    rules_processor.getLenght( width, height, depth );
    model.volume->setGeometry( ox, oy, oz, width, height, depth );
    setVolumeDiscretization();

    // loading the resolution
    if ( rules_processor.isLowResolution() == true )
    {
        resol_ = Controller::MeshResolution::LOW;
    }
    else if ( rules_processor.isMediumResolution() == true )
    {
        resol_ = Controller::MeshResolution::MEDIUM;
    }
    else if ( rules_processor.isHighResolution() == true )
    {
        resol_ = Controller::MeshResolution::HIGH;
    }

    // if already exist an object, remove it
    if( model.objects.find( current_object ) != model.objects.end() )
    {
        model.objects[ current_object ]->clear();
        model.objects[ current_object ].reset();
        model.objects[ current_object ] = nullptr;
        model.objects.clear();
    }


    std::string complete_filename = filename + ".json";

    // loading only the surfaces and curves with no metadata
    QFile load_file( QString( complete_filename.c_str() ) );
    if ( !load_file.open( QIODevice::ReadOnly ) ) {
        loadObjectNoMetaDatas();
    }
    else
        // loading the surfaces and curves with metadata
        loadObjectMetaDatas( load_file );

}


void Controller::loadObjectNoMetaDatas()
{
    std::random_device rd;
    std::mt19937 eng( rd() );
    std::uniform_int_distribution< size_t > distr( 0, 255 );

    // TODO: comment this code
    int counter_ = 0;
    while( rules_processor.canRedo() )
    {
        rules_processor.redo();
        counter_++;
    }

    // creating randomly colors to each object
    std::vector< std::size_t > actives = rules_processor.getActiveSurfaces();
    for( auto id: actives )
    {
        int r_ = static_cast< int >( distr( eng ) );
        int g_ = static_cast< int >( distr( eng ) );
        int b_ = static_cast< int >( distr( eng ) );

        addObject( id );
        setObjectColor( id, r_, g_, b_ );

        model.objects[ id ]->setDone( true );

    }

    // TODO: comment this code
    for( int i = 0; i < counter_; ++i )
    {
        rules_processor.undo();
    }

    addObject();
    updateModel();

}


void Controller::loadObjectMetaDatas( QFile& load_file )
{

    std::random_device rd;
    std::mt19937 eng( rd() );
    std::uniform_int_distribution< size_t > distr( 0, 255 );

    QByteArray save_data = load_file.readAll();
    QJsonDocument load_doc( QJsonDocument::fromJson(save_data) );
    const QJsonObject &json = load_doc.object();

    if ( json.contains("objects") && json["objects"].isArray() )
    {

        int counter_ = 0;
        while( rules_processor.canRedo() )
        {
            rules_processor.redo();
            counter_++;
        }

        QJsonArray objects_array_ = json["objects"].toArray();

        std::vector< std::size_t > actives = rules_processor.getActiveSurfaces();

        int obj_id_ = 0;
        for( auto id: actives )
        {
            int r_= static_cast< int >( distr( eng ) );
            int g_= static_cast< int >( distr( eng ) );
            int b_= static_cast< int >( distr( eng ) );

            addObject( id );

            if( obj_id_ < objects_array_.size() )
            {
                QJsonObject object_ = objects_array_[obj_id_].toObject();

                const ObjectPtr& obj_ = model.objects[ id ];
                obj_->read( object_ );
                obj_->getColor( r_, g_, b_ );

                obj_id_++;
            }
            else
            {
                setObjectColor( id, r_, g_, b_ );
            }

        }


        for( int i = 0; i < counter_; ++i )
        {
            rules_processor.undo();
        }

        addObject();
        updateModel();

    }

    if( json.contains("regions") && json["regions"].isArray() )
    {
        // first, create the regions and after loading the metadata to each one
        defineRegions();

        QJsonArray regions_array_ = json["regions"].toArray();
        int nregions_ = regions_array_.size();
        for( int i = 0; i < nregions_; ++i )
        {
            // if it was not save the index, it is not valid
            QJsonObject region_ = regions_array_[ i ].toObject();
            if( region_.contains( "index" ) == false ) return;

            std::size_t id_ = static_cast< std::size_t>( json["index"].toInt() );
            if( model.regions.find( id_ ) == model.regions.end() ) return;

            RegionsPtr reg_ = model.regions[ id_ ];
            reg_->read( region_ );

        }
    }

    if( json.contains("domains") && json["domains"].isArray() )
    {

        QJsonArray domains_array_ = json["domains"].toArray();
        int ndomains_ = domains_array_.size();
        for( int i = 0; i < ndomains_; ++i )
        {
            QJsonObject domain_ = domains_array_[ i ].toObject();
            if( domain_.contains( "index" ) == false ) return;

            // if it was not save the index and also the regions, it is not valid
            std::size_t id_ = static_cast< std::size_t>( domain_["index"].toInt() );
            if( ( domain_.contains( "regions" ) == false ) ||
                    ( domain_["regions"].isArray()  == false ) ) return;

            std::size_t index_ = createDomain();

            QJsonArray regions_set_array_ = domain_["regions"].toArray();
            for( auto it_: regions_set_array_ )
                addRegionToDomain( static_cast< std::size_t >( it_.toInt() ), index_ );

        }
    }


}


void Controller::setMeshResolution( const Controller::MeshResolution& resolution_ )
{
    if( resolution_ == Controller::MeshResolution::LOW )
    {
        rules_processor.setLowResolution();
    }
    else if( resolution_ == Controller::MeshResolution::MEDIUM )
    {
        rules_processor.setMediumResolution();
    }
    else if( resolution_ == Controller::MeshResolution::HIGH )
    {
        rules_processor.setHighResolution();
    }

    // after change the resolution, update the discretization information
    setVolumeDiscretization();
}


void Controller::exportToIrapGrid()
{

    IrapGridExporter exporter;
    std::vector< float > points;

    std::vector< std::size_t > actives_ = rules_processor.getOrderedActiveSurfaces();

    // to know how much digits should be filled with zero to help to sort the surfaces properly
    int num_digits = std::floor( std::log( actives_.size() ) );
    num_digits = ( num_digits > 0 ? num_digits : 1 );

    for ( size_t i = 0; i < actives_.size(); ++i )
    {
        size_t id_ = actives_[i];
        QString prefix_ = QString( "%1" ).arg( i, num_digits, 10, QChar('0') ).append("_"); 
        QString surface_filename = QString( "Surface_%1.IRAPG").arg( id_ ).prepend(prefix_);

        std::vector< double > points_list;
        std::vector< bool > valid_points;
        std::size_t nu;
        std::size_t nv;

        rules_processor.getQuadMesh( id_, points_list, valid_points, nu, nv );

        // defining the maximum and minimum points
        double xmax = points_list[ 0 ], xmin = points_list[ 0 ];
        double ymax = points_list[ 1 ], ymin = points_list[ 1 ];
        double zmax = points_list[ 2 ], zmin = points_list[ 2 ];

        std::size_t number_elements = nu*nv;

        for( std::size_t i = 0; i < number_elements; ++i )
        {

            double x = points_list[ 3*i + 0 ];
            double y = points_list[ 3*i + 1 ];
            double z = points_list[ 3*i + 2 ];

            if( x >= xmax ) xmax = x;
            else if( x < xmin ) xmin = x;

            if( y >= ymax ) ymax = y;
            else if( y < ymin ) ymin = y;


            if( z >= zmax ) zmax = z;
            else if( z < zmin ) zmin = z;

            points.push_back( static_cast<float>( z ) );

        }

        // defining the discretization of the grid
        float dx = static_cast< float>( xmax - xmin )/( nu - 1 );
        float dy = static_cast< float>( ymax - ymin )/( nv - 1 );


        //filling the file with the required inforation
        exporter.setBoundingBox( static_cast< float>(xmin), static_cast< float>(xmax), static_cast< float>(ymin),
                                 static_cast< float>(ymax), static_cast< float>(zmin), static_cast< float>(zmax));
        exporter.setVectorValues( points );
        exporter.setSize( static_cast< int>( nu ),  static_cast< int>( nv ) );
        exporter.setSpacing( dx, dy );


        std::string name_ = getObjectName( id_ ).append( ".IRAPG" );
        if( name_.empty() == true )
            exporter.writeGridData( surface_filename.toStdString() );
        else
            exporter.writeGridData( prefix_.toStdString().append(name_) );

        exporter.clearData();

        points.clear();
        surface_filename.clear();
    }

}


void Controller::setGuidedExtrusion( float , float , float , const PolyCurve&  )
{

}


void Controller::getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps )
{
    rules_processor.getLegacyMeshes( points, nu, nv, num_extrusion_steps );
}


void Controller::setSurfacesMeshes( std::vector< TriangleMesh >& triangles_meshes,
                                    std::vector< CurveMesh >& left_curves,
                                    std::vector< CurveMesh >& right_curves,
                                    std::vector< CurveMesh > & front_curves,
                                    std::vector< CurveMesh >& back_curves )
{
    rules_processor.setPLCForSimulation(triangles_meshes, left_curves, right_curves, front_curves, back_curves);
}


std::vector<int> Controller::getTetrahedronsRegions( const std::vector< float >& vertices, const std::vector< unsigned int >& faces )
{

    std::vector< double > points;
    for( auto v: vertices )
        points.push_back( static_cast< double >( v ) );

    std::vector< std::size_t > elements_;
    for( auto e: faces )
        elements_.push_back( static_cast< std::size_t >( e ) );

    std::vector<int> regions_;
    rules_processor.getRegionsForSimulationTetrahedralMesh( points, elements_, regions_ );

    //std::set<int> domains_set_;
    std::vector< int > domains_( regions_.size() );
    std::size_t id_;
    
    //for( auto it: regions_ )
    for ( std::size_t i = 0; i < regions_.size(); ++i )
    {
        //std::size_t id_; // every time a variable is created space must be allocated in the stack,
        // move all vars to outside loops if possible for efficiency

        RegionsPtr reg_ = model.regions[ regions_[i] ];
        if (reg_->getDomain(id_) == false)
        {
            domains_[i] = -1;
        }
        else
        {
            //domains_set_.insert( id_ );
            domains_[i] = id_;
        }
    }

    //std::vector< int > domains_;
    //std::copy(domains_set_.begin(), domains_set_.end(),
    //              std::back_inserter(domains_));
    return domains_;
}

void Controller::clear()
{

    if( csection != nullptr )
        csection.reset();
    csection = nullptr;

    if( topview != nullptr )
        topview.reset();
    topview = nullptr;

    if( model.volume != nullptr )
        model.volume.reset();
    model.volume = nullptr;

    for( auto it: model.csectionsX )
        (it.second).reset();
    model.csectionsX.clear();

    for( auto it: model.csectionsY )
        (it.second).reset();
    model.csectionsY.clear();

    for( auto it: model.csectionsZ )
        (it.second).reset();
    model.csectionsZ.clear();

    for( auto it: model.objects )
        (it.second).reset();
    model.objects.clear();

    for( auto it: model.regions )
        (it.second).reset();
    model.regions.clear();

    for( auto it: model.domains )
    {
        (it.second).clear();
    }
    model.domains.clear();
    regions_in_domains.clear();


    selectable_objects.clear();
    boundary_region = Settings::Objects::BoundaryRegion::NONE ;
    upper_index = 0;
    bottom_index = 0;

    images_csectionsX.clear();
    images_csectionsY.clear();
    images_csectionsZ.clear();

    image_topview.file.clear();
    image_topview.ox = 0.0;
    image_topview.oy = 0.0;
    image_topview.w = 0.0;
    image_topview.h = 0.0;


    current_object = 0;
    current_object_type = Settings::Objects::ObjectType::STRATIGRAPHY;

    current_rule = Settings::Stratigraphy::DEFAULT_STRAT_RULES;
    csection_stepx = 1.0;
    csection_stepz = 1.0;
    csection_stepy = 1.0;

    last_trajectory.clear();
    rules_processor.clear();

    current_color.red = 255;
    current_color.green = 0;
    current_color.blue = 0;

}
