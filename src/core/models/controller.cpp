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


///==========================================================================


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



///==========================================================================

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


    for( auto it: model.objects )
    {
        (it.second)->setBoundingBox( ox_, ox_ + w_, oy_, oy_ + h_, oz_, oz_ + d_ );
    }

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



///==========================================================================


///
/// Cross-Sections Methods
///


void Controller::createMainCrossSection()
{
    csection = std::make_shared<CrossSection>();
    csection->setVolume( model.volume );
    csection->setDirection(     Settings::CrossSection::CrossSectionDirections::Z );
    csection->setDepth( model.volume->getLenght() );
}


void Controller::changeMainCrossSectionDirection( const Settings::CrossSection::CrossSectionDirections& dir_ )
{
    current_direction = dir_;

    if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        moveTopViewCrossSection( model.volume->getHeight() );
        return;
    }


    csection->setDirection( dir_ );

    if ( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        csection->setDepth( model.volume->getWidth() );
    }

    else if ( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
        csection->setDepth( model.volume->getLenght() );
    }

    for( auto it_: model.objects )
    {
        ObjectPtr obj_ = it_.second;
        if( obj_->isDone() == true )
            obj_->removeCrossSectionCurves();
    }

    updateModel();


//    csection->setDirection( dir_ );

//    if ( dir_ == Settings::CrossSection::CrossSectionDirections::X )
//    {
//        csection->setDepth( model.volume->getWidth() );
//    }

//    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
//    {
//        csection->setDepth( model.volume->getHeight() );
//    }

//    else if ( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
//    {
//        csection->setDepth( model.volume->getLenght() );
//    }

//    for( auto it_: model.objects )
//    {
//        ObjectPtr obj_ = it_.second;
//        if( obj_->isDone() == true )
//            obj_->removeCrossSectionCurves();
//    }

//    updateModel();
}


void Controller::moveMainCrossSection( double depth_ )
{
    std::cout << "csection depth: " << depth_ << std::endl << std::flush;
    csection->setDepth( depth_ );

    updateObjectsCurvesInCrossSection( depth_ );
    updateImageInMainCrossSection();
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
    CrossSectionPtr csection_ = std::make_shared< CrossSection >();
    csection_->setVolume( model.volume );
    csection_->setDirection( dir_ );
    csection_->setDepth( depth_ );

    ImageData image_;

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
            image_ = images_csectionsY[ depth_ ];
            csection_->setImage( image_.file, image_.ox, image_.oy, image_.w, image_.h );
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
        images_csectionsY[ depth_ ] = std::move( image_ );
        if ( model.csectionsY.find( depth_ ) != model.csectionsY.end() )
        {
            CrossSectionPtr csection1_ = model.csectionsY[ depth_ ];
            csection1_->setImage( file_, ox_, oy_, w_, h_ );
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

    if( csection->getDirection() == dir_ && csection->getDepth() == depth_ )
        csection->setImage( file_, ox_, oy_, w_, h_ );

    Settings::CrossSection::CrossSectionDirections dir1_ = topview->getDirection();
    double depth1_ = topview->getDepth();


    if(  dir1_ == dir_ &&  depth1_ == depth_ )
        topview->setImage( file_, ox_, oy_, w_, h_ );

}


void Controller::clearImageInCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    std::cout <<" Entering in clearImageInCrossSection method" << std::endl << std::flush;

    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        images_csectionsX.erase( depth_ );
        updateImageInMainCrossSection();
        if ( model.csectionsX.find( depth_ ) != model.csectionsX.end() )
        {
            CrossSectionPtr csection1_ = model.csectionsX[ depth_ ];
            csection1_->clearImage();
        }
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        images_csectionsY.erase( depth_ );
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

    if( has_image_ == true )
        csection->setImage( image_.file, image_.ox, image_.oy, image_.w, image_.h );
    else
        csection->clearImage();

}


void Controller::updateImageInTopViewCrossSection()
{
    const Settings::CrossSection::CrossSectionDirections& dir_ = topview->getDirection();
    double depth_ = topview->getDepth();


    if( dir_ != Settings::CrossSection::CrossSectionDirections::Y )
        return;

    if( images_csectionsY.find( depth_ ) == images_csectionsY.end() )
    {
        topview->clearImage();
        return;
    }

    ImageData image_= images_csectionsY[ depth_ ];
    topview->setImage( image_.file, image_.ox, image_.oy, image_.w, image_.h );

}


///==========================================================================

///
/// Objects Methods
///


bool Controller::addObject( std::size_t index_ )
{

    ObjectPtr obj_ = std::make_shared< Object >();

    double ox_ = 0.0, oy_ = 0.0, oz_ = 0.0;
    double w_ = 0.0, h_ = 0.0, d_ = 0.0;

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



///==========================================================================

///
/// Creating Curves and Surfaces of Objects
///



bool Controller::addCurveToObject( Settings::CrossSection::CrossSectionDirections dir_, double depth_, const PolyCurve& curve_ )
{
    if( curve_.isEmpty() == true ) return false;


    ObjectPtr& obj_ = model.objects[ current_object ];
    obj_->setCrossSectionDirection( dir_ );

    Curve2D curve_proc_ = curve_.getCurves2D()[0];

    if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
        depth_ = topview->getDepth();
    else
        curve_proc_= SketchLibrary1::monotonicInX( curve_proc_ );


    curve_proc_ = SketchLibrary1::smooth( curve_proc_ );

    bool added_curve_ = obj_->addCurve( depth_, PolyCurve( curve_proc_ ) );
    if ( added_curve_ == false ) return false;

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
    //    if ( csection_->isEmpty() == true )
    //        removeCrossSection( dir_, depth_ );

    return true;

}


void Controller::addTrajectoryToObject( const PolyCurve& curve_ )
{

    if( curve_.isEmpty() == true ) return;

    ObjectPtr& obj_ = model.objects[ current_object ];

    Curve2D curve_proc_;
//    if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::X )
    if( current_direction == Settings::CrossSection::CrossSectionDirections::X )
    {
        curve_proc_ = SketchLibrary1::monotonicInX( curve_.getCurves2D()[0] );
        //        curve_proc_
    }
    else {
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

    ObjectPtr obj_ = model.objects[ current_object ];
    if( status_ == false )
    {
        return false;
    }

    obj_->setDone( true );


    updateModel();
    addObject();
    return true;

}


bool Controller::createObjectSurface()
{

    ObjectPtr obj_ = model.objects[ current_object ];
    bool surface_created_ = false;

    if( obj_->getNumberOfCrossSections() == 0 ) return false;

    bool single_csection_ = ( obj_->getNumberOfCrossSections() == 1 );
    bool is_level_curves_ = ( obj_->getCrossSectionDirection() == Settings::CrossSection::CrossSectionDirections::Y );
    bool is_general_surface_ = ( !single_csection_ || is_level_curves_ );


    if( is_general_surface_ == true )
        surface_created_ = createGeneralSurface();
    else
    {
        if( obj_->hasTrajectory() == false )
            surface_created_ = createLinearExtrudedSurface();
        else
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

    std::vector< double > curves_ = obj_->getCurves2D();

    std::map< double, PolyCurve > curves_map_ = obj_->getCurves();
    double depth_ = curves_map_.begin()->first;
    const PolyCurve& path_ = obj_->getTrajectory();

    bool surface_created_ = false;

    if( obj_->getCrossSectionDirection() == Settings::CrossSection::CrossSectionDirections::X )
    {
        surface_created_ = rules_processor.createWidthwiseExtrudedSurface( current_object, curves_, depth_, path_.getPointsSwapped() );
    }

    else if( obj_->getCrossSectionDirection() == Settings::CrossSection::CrossSectionDirections::Z )
    {
        surface_created_ = rules_processor.createLengthwiseExtrudedSurface( current_object, curves_, depth_, path_.getPoints() );
    }

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


///==========================================================================

///
/// Updating Models Methods
///


void Controller::updateModel()
{

    setObjectsActive( false );

    std::vector< std::size_t > actives_ = rules_processor.getActiveSurfaces();

    std::size_t number_of_actives_ = actives_.size();
    for ( std::size_t j = 0; j < number_of_actives_; ++j )
    {
        std::size_t id_ = actives_.at( j );

        setObjectActive( id_, true );
        updateObjectSurface( id_ );
        updateObjectCurves( id_ );
    }

    setObjectActive( current_object, true );

}


void Controller::updateObjectCurveInCrossSection( const std::size_t& index_, double depth_ )
{
    ObjectPtr obj_ = model.objects[ index_ ];

    if( index_ == current_object )
    {
        bool has_curve_ = obj_->hasCurve( depth_ );
        if( has_curve_ == true ) return;
    }

    clearAndSetCurveinCrossSectionFromRulesProcessor( index_ , depth_ );
//    clearAndSetCurveinCrossSectionFromRulesProcessor( index_ , csection->getDirection(), csection->getDepth() );

}


void Controller::updateObjectsCurvesInCrossSection( double depth_ )
{

    for( auto it_: model.objects )
    {
        updateObjectCurveInCrossSection( it_.first, depth_ );
    }

}


void Controller::updateCrossSectionsX()
{

    for( auto it_: model.csectionsX )
    {
        updateObjectsCurvesInCrossSection( it_.first );
    }

}


void Controller::updateCrossSectionsY()
{
    for( auto it_: model.csectionsY )
    {
        updateObjectsCurvesInCrossSection( it_.first );
    }

}


void Controller::updateCrossSectionsZ()
{
    for( auto it_: model.csectionsZ )
    {
        updateObjectsCurvesInCrossSection( it_.first );
    }

}


void Controller::clearAndSetCurveinCrossSectionFromRulesProcessor( const std::size_t& index_, double depth_ )
{

    clearAndSetCurveinCrossSectionFromRulesProcessor( index_, current_direction, depth_ );

//    ObjectPtr& obj_ = model.objects[ index_ ];

//    bool has_curve_ = false;

//    std::vector< double > vertices_;
//    std::vector< std::size_t > edges_;

//    if( current_direction == Settings::CrossSection::CrossSectionDirections::X )
//    {
//        has_curve_ = rules_processor.getWidthCrossSectionCurve( index_, indexCrossSectionX( depth_ ), vertices_, edges_ );
//    }

//    else if( current_direction == Settings::CrossSection::CrossSectionDirections::Z )
//    {
//        has_curve_ = rules_processor.getLengthCrossSectionCurve( index_, indexCrossSectionZ( depth_ ), vertices_, edges_ );
//    }


////    if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::X )
////    {
////        has_curve_ = rules_processor.getWidthCrossSectionCurve( index_, indexCrossSectionX( depth_ ), vertices_, edges_ );
////    }

////    else if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::Z )
////    {
////        has_curve_ = rules_processor.getLengthCrossSectionCurve( index_, indexCrossSectionZ( depth_ ), vertices_, edges_ );
////    }

//    if( has_curve_ == false )
//    {
//        std::cout << "No curve for object " << index_ << " in cross-section " << depth_ << std::endl << std::flush;
//        return;
//    }

//    PolyCurve curve_( vertices_, edges_ );

//    obj_->removeCurve( depth_ );
//    obj_->updateCurve( depth_, curve_ );

}


void Controller::clearAndSetCurveinCrossSectionFromRulesProcessor( const std::size_t& index_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    ObjectPtr& obj_ = model.objects[ index_ ];

    bool has_curve_ = false;

    std::vector< double > vertices_;
    std::vector< std::size_t > edges_;

    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        has_curve_ = rules_processor.getWidthCrossSectionCurve( index_, indexCrossSectionX( depth_ ), vertices_, edges_ );
    }

    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
        has_curve_ = rules_processor.getLengthCrossSectionCurve( index_, indexCrossSectionZ( depth_ ), vertices_, edges_ );
    }

    if( has_curve_ == false )
    {
        std::cout << "No curve for object " << index_ << " in cross-section " << depth_ << std::endl << std::flush;
        return;
    }

    PolyCurve curve_( vertices_, edges_ );

    obj_->removeCurve( depth_ );
    obj_->updateCurve( depth_, curve_ );
}


///// Updating object index in all directions
void Controller::updateObjectCurves( const std::size_t& index_ )
{

    //    Settings::CrossSection::CrossSectionDirections dir_ = csection->getDirection();
    Settings::CrossSection::CrossSectionDirections dir_ = current_direction;

    updateObjectCurveInCrossSection( index_, csection->getDepth() );

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




////    Settings::CrossSection::CrossSectionDirections dir_ = csection->getDirection();
//    Settings::CrossSection::CrossSectionDirections dir_ = current_direction;

//    updateObjectCurveInCrossSection( index_, csection->getDepth() );

//    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
//    {
//        for( auto it_: model.csectionsX )
//            updateObjectCurveInCrossSection( index_, it_.first );
//    }
//    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
//    {
//        for( auto it_: model.csectionsY )
//            updateObjectCurveInCrossSection( index_, it_.first );
//    }
//    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
//    {
//        for( auto it_: model.csectionsZ )
//            updateObjectCurveInCrossSection( index_, it_.first );
//    }

}


void Controller::updateObjectSurface( const std::size_t& index_ )
{
    std::vector< double > vertices_;
    std::vector< std::size_t > faces_;

    ObjectPtr obj_ = model.objects[ index_ ];

    bool has_surface_ = rules_processor.getMesh( index_, vertices_, faces_ );
    if( has_surface_  == false )
    {
        obj_->removeSurface();
        //        obj_->setActive( false );
        return;
    }

    std::vector< double > normals_;
    rules_processor.getNormals( index_, normals_ );

    Surface surface_;
    surface_.setVertices( vertices_ );
    surface_.setFaces( faces_ );
    surface_.setNormals( normals_ );

    obj_->setSurface( surface_ );
    //    obj_->setActive( true );



    std::vector< double > trajectory_;
    bool has_trajectory_ = rules_processor.getExtrusionPath( index_, trajectory_ );
    if( has_trajectory_ == true )
    {
        PolyCurve traj_ = PolyCurve( trajectory_ );

//        if( csection->getDirection() != Settings::CrossSection::CrossSectionDirections::Y )
        if( current_direction != Settings::CrossSection::CrossSectionDirections::Y )
        {
            //            if( csection->getDirection() != obj_->getCrossSectionDirection() )
            //                traj_ = PolyCurve( traj_.getPointsSwapped() );
        }

        obj_->removeTrajectory();
        obj_->addTrajectory( traj_ );

    }



}


void Controller::updatePreviewSurface()
{
    rules_processor.testSurfaceInsertion();

    bool surface_created_ = createObjectSurface();
    if( surface_created_ == false )
        return;

    updateObjectSurface( current_object );
}



///==========================================================================

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
    if( status_ == false ) return std::vector<std::size_t >();

    double w_ = 0, h_ = 0,  l_ = 0;
    double ox_ = 0, oy_ = 0, oz_ = 0;

    model.volume->getGeometry( ox_, oy_, oz_, w_, h_, l_ );

    std::size_t number_of_regions_ = regions_.size();
    std::vector< int > colors_ = rules_processor.getRegionsColor( number_of_regions_ );
    std::vector< double > volumes_;
    rules_processor.getRegionVolumeList( volumes_ );

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
        getRegionCrossSectionBoundary( i );
    }

     model.volume->setVolume( volume_sum_ );

    std::vector< std::size_t > surfaces_indexes_ = rules_processor.getOrderedActiveSurfaces();
    return surfaces_indexes_;
}


bool Controller::getRegionCrossSectionBoundary( std::size_t index_ )
{
    //    if( model.regions.find( index_ ) == model.regions.end() )
    //        return false;

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


//    if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::X )
//    {
//        rules_processor.getRegionCurveBoxesAtWidth( index_, indexCrossSectionX( csection->getDepth() ), vertices_lower_, edges_lower_, vertices_upper_, edges_upper_ );
//    }

//    else if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::Z )
//    {
//        rules_processor.getRegionCurveBoxesAtLength( index_, indexCrossSectionZ( csection->getDepth() ), vertices_lower_, edges_lower_, vertices_upper_, edges_upper_ );
//    }



    PolyCurve lower_, upper_;
    lower_.fromVector( vertices_lower_, edges_lower_ );
    upper_.fromVector( vertices_upper_, edges_upper_ );

    region_->setLowerBound( lower_ );
    region_->setUpperBound( upper_ );

    return true;
}



void Controller::updateRegions()
{
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

    for( auto it: model.domains1 )
        (it.second).clear();
    model.domains1.clear();
    regions_in_domains.clear();
}




///==========================================================================

///
/// Domains Methods
///


std::size_t Controller::createDomain1( std::set< std::size_t > indexes_ )
{
    std::size_t id_ = 0;

    if( model.domains1.empty() == false )
    {
        for( auto it_: model.domains1 )
        {
            id_ = it_.first;
        }

        ++id_;
    }

    model.domains1[ id_ ] = Domain();
    for( auto it_: indexes_ )
        addRegionToDomain1( it_, id_ );

    return id_;

}


void Controller::setDomainName( std::size_t index_, const std::string& name_ )
{
    if (model.domains1.find(index_) == model.domains1.end()) return;
    model.domains1[ index_ ].setName( name_ );
}

void Controller::setDomainColor( std::size_t index_, int red_, int green_, int blue_ )
{
    if (model.domains1.find(index_) == model.domains1.end()) return;
    model.domains1[ index_ ].setColor( red_, green_, blue_ );
}


bool Controller::addRegionToDomain1( std::size_t region_id_, std::size_t domain_id_ )
{
    if (model.regions.find(region_id_) == model.regions.end()) return false;
    if (model.domains1.find(domain_id_) == model.domains1.end()) return false;
    if( regions_in_domains.find( region_id_ ) != regions_in_domains.end() ) return false;

    model.domains1[ domain_id_ ].addRegion( region_id_ );
    regions_in_domains.insert( region_id_ );

    RegionsPtr reg_ = model.regions[region_id_];
    reg_->setDomain(domain_id_);

    double volume_ = model.domains1[ domain_id_ ].getDomainVolume();
    volume_ += model.regions[region_id_]->getVolume();

    model.domains1[ domain_id_ ].setDomainVolume( volume_ );
    return true;
}


bool Controller::removeRegionFromDomain1(std::size_t region_id_, std::size_t domain_id_)
{
    if (model.regions.find(region_id_) == model.regions.end()) return false;
    if (model.domains1.find(domain_id_) == model.domains1.end()) return false;
    if( regions_in_domains.find( region_id_ ) == regions_in_domains.end() ) return false;

    model.domains1[domain_id_].removeRegion( region_id_ );
    regions_in_domains.erase( region_id_ );

    RegionsPtr reg_ = model.regions[region_id_];
    reg_->removeFromDomain();

    double volume_ = model.domains1[ domain_id_ ].getDomainVolume();
    volume_ -= model.regions[region_id_]->getVolume();
    model.domains1[ domain_id_ ].setDomainVolume( volume_ );

    return true;
}


std::set< std::size_t> Controller::getRegionsFromDomain1(std::size_t domain_id_) const
{
    if (model.domains1.find(domain_id_) == model.domains1.end()) return std::set< std::size_t>();

    const Domain& domain_ = model.domains1.at( domain_id_ );
    return domain_.getRegions();

}


void Controller::removeDomain1(std::size_t domain_id_)
{

    if (model.domains1.find(domain_id_) == model.domains1.end()) return;

    Domain& domain_ = model.domains1[ domain_id_ ];
    domain_.clear();

    model.domains1.erase( domain_id_ );
}


std::vector< std::size_t > Controller::getDomains()
{

    std::vector< std::size_t > indexes_;
    std::vector<int> diff_;

    for( auto it_: model.regions )
    {
        std::size_t id_ = it_.first;
        if( regions_in_domains.find( id_ ) != regions_in_domains.end() ) continue;
        std::size_t domain_id_ = createDomain1();
        addRegionToDomain1( id_, domain_id_ );
    }

    for( auto it_: model.domains1 )
    {
        indexes_.push_back( it_.first );
    }

    return indexes_;


    //    std::vector< std::size_t > indexes_;
    //    std::vector<int> diff_;

    //    for( auto it_: model.regions )
    //    {
    //        std::size_t id_ = it_.first;
    //        if( regions_in_domains.find( id_ ) != regions_in_domains.end() ) continue;
    //        std::size_t domain_id_ = createDomain1();
    //        addRegionToDomain1( id_, domain_id_ );
    //    }

    //    for( auto it_: model.domains )
    //    {
    //        indexes_.push_back( it_.first );
    //    }

    //    return indexes_;
}

//=== old methods


void Controller::createDomain( std::size_t index_, std::set< std::size_t > indexes_ )
{
    std::size_t id_ = index_;
    model.domains[id_].regions_set = indexes_;

    for( auto it: indexes_ )
    {
        RegionsPtr reg_ = model.regions[ it ];
        reg_->setDomain( index_ );
    }
}


void Controller::addRegionToDomain( std::size_t region_id_, std::size_t domain_id_ )
{
    if (model.regions.find(region_id_) == model.regions.end()) return;
    if (model.domains.find(domain_id_) == model.domains.end()) return;

    model.domains[domain_id_].regions_set.insert(region_id_);
    RegionsPtr reg_ = model.regions[ region_id_ ];
    reg_->setDomain( domain_id_ );


}


void Controller::removeRegionFromDomain(std::size_t region_id_, std::size_t domain_id_)
{
    if (model.regions.find(region_id_) == model.regions.end()) return;
    if (model.domains.find(domain_id_) == model.domains.end()) return;

    model.domains[domain_id_].regions_set.erase(region_id_);
    RegionsPtr reg_ = model.regions[ region_id_ ];
    reg_->removeFromDomain();
}


std::set< std::size_t> Controller::getRegionsFromDomain(std::size_t domain_id_) const
{
    if (model.domains.find(domain_id_) == model.domains.end()) return std::set< std::size_t>();
    return model.domains.at(domain_id_).regions_set;
}


void Controller::getDomainColor( std::size_t domain_id_, int &red_, int &green_, int& blue_ )
{
    if (model.domains.find(domain_id_) == model.domains.end()) return;
    if( model.domains[domain_id_].regions_set.empty() == true ) return;

    std::size_t reg_id_ = *model.domains[domain_id_].regions_set.begin();

    RegionsPtr reg_ = model.regions[ reg_id_ ];
    reg_->getColor( red_, green_, blue_ );

}


void Controller::removeDomain(std::size_t domain_id_)
{
    if (model.domains.find(domain_id_) == model.domains.end()) return;

    for( auto it_: model.domains )
        removeRegionFromDomain( it_.first, domain_id_ );

    model.domains.erase( domain_id_ );
}

///==========================================================================

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



///==========================================================================

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
//    if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::X )
//    {
//        return rules_processor.getWidthResolution();
//    }
//    else if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::Y )
    {
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

//    if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::X )
//    {
//        min_ = ox_;
//        max_ = ox_ + width_;
//    }
//    else if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::Y )
//    {
//        min_ = oy_;
//        max_ = oy_ + height_;
//    }

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



///==========================================================================

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


///==========================================================================

///
/// Preserve Methods
///



Settings::Objects::BounderingRegion Controller::getCurrentBoundaryRegion() const
{
    return boundering_region;
}




// Request/Stop Create Above -- old methods

bool Controller::requestCreateAbove()
{

    bool request_ = rules_processor.requestCreateAbove( selectable_objects );

    if( request_ == true )
    {
        std::cout << "Request create accepted" << std::endl << std::flush;

        boundering_region = Settings::Objects::BounderingRegion::ABOVE;

        for( std::size_t id_: selectable_objects )
        {
            ObjectPtr& obj_ = model.objects[ id_ ];
            obj_->setSelectable( true );
        }

    }
    else
        std::cout << "Request create denied" << std::endl << std::flush;

    return request_ ;

}


void Controller::stopCreateAbove()
{
    std::cout << "Stop create above accepted" << std::endl << std::flush;
    rules_processor.stopDefineAbove();

    for( std::size_t id_: selectable_objects )
    {
        ObjectPtr& obj_ = model.objects[ id_ ];
        obj_->setSelectable( false );
    }
    selectable_objects.clear();
    setObjectSelected( upper_index, false );

}


// Request/Stop Create Below -- old methods

bool Controller::requestCreateBelow()
{

    boundering_region = Settings::Objects::BounderingRegion::BELOW;
    return true;


    /*

    bool request_ = rules_processor.requestCreateBelow( selectable_objects );

    if( request_ == true )
    {
        std::cout << "Request create accepted" << std::endl << std::flush;

        boundering_region = Settings::Objects::BounderingRegion::BELOW;

        for( std::size_t id_: selectable_objects )
        {
            ObjectPtr& obj_ = model.objects[ id_ ];
            obj_->setSelectable( true );
        }

    }
    else
        std::cout << "Request create denied" << std::endl << std::flush;

    return request_ ;

    */

}


void Controller::stopCreateBelow()
{
    std::cout << "Stop create below accepted" << std::endl << std::flush;
    rules_processor.stopDefineBelow();

    for( std::size_t id_: selectable_objects )
    {
        ObjectPtr& obj_ = model.objects[ id_ ];
        obj_->setSelectable( false );
    }
    setObjectSelected( bottom_index, false );
    selectable_objects.clear();

}



// deprecated
bool Controller::requestCreateRegion()
{
    //    bool request_ = rules_processor.requestPreserveRegion()


    //    bool request_ = rules_processor.requestCreateRegion( selectable_objects );

    //    if( request_ == true )
    //    {
    //        std::cout << "Request create accepted" << std::endl << std::flush;

    //        boundering_region = Settings::Objects::BounderingRegion::BELOW;

    //        for( std::size_t id_: selectable_objects )
    //        {
    //            ObjectPtr& obj_ = model.objects[ id_ ];
    //            obj_->setSelectable( true );
    //        }

    //    }
    //    else
    //        std::cout << "Request create denied" << std::endl << std::flush;

    return false;
}


void Controller::stopCreateRegion()
{

    std::cout << "Stop create region accepted" << std::endl << std::flush;
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
    if( status_ == true )
        boundering_region = Settings::Objects::BounderingRegion::ABOVE;
    else
        rules_processor.stopPreserveAbove();
}


// Enable/Stop Preserve Below -- new methods

void Controller::enablePreserveBelow( bool status_ )
{
    if( status_ == true )
        boundering_region = Settings::Objects::BounderingRegion::BELOW;
    else
        rules_processor.stopPreserveBelow();

}





void Controller::setObjectSelectedAsBoundering( const std::size_t& index_ )
{

    if( boundering_region == Settings::Objects::BounderingRegion::ABOVE )
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

    else if( boundering_region == Settings::Objects::BounderingRegion::BELOW )
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


bool Controller::setRegionBySketchAsBoundering( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, PolyCurve& boundary_ )
{

    std::vector< double > curve3d_;
    std::vector< double > vertices_;
    std::vector< std::size_t > edges_;


    if( boundering_region == Settings::Objects::BounderingRegion::ABOVE )
    {


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

    else if( boundering_region == Settings::Objects::BounderingRegion::BELOW )
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

    if( rules_processor.preserveAboveIsActive() == true )
    {
        getLowerBoundering( boundary_ );
        status_ = true;
    }

    else if( rules_processor.preserveBelowIsActive() == true )
    {

        getUpperBoundering( boundary_ );
        status_ = true;
    }

    return status_;
}


bool Controller::setRegionByPointAsBoundering( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ )
{
    std::vector< double > point_;

    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        point_.push_back( depth_ );
        point_.push_back( py_ );
        point_.push_back( px_ );


    }

    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        point_.push_back( px_ );
        point_.push_back( depth_ );
        point_.push_back( py_ );


    }

    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
        point_.push_back( px_ );
        point_.push_back( py_ );
        point_.push_back( depth_ );

    }

    bool request_ = rules_processor.requestPreserveRegion( point_ );

    // get items to be selected

    return request_;

}


void Controller::getRegionByPointAsBoundering()
{



}


void Controller::clearBounderingArea()
{

}



bool Controller::isDefineAboveActive( PolyCurve& boundary_ )
{
    std::size_t index_ = 0;

    bool status_ = rules_processor.preserveAboveIsActive();
    if( status_ == false ) return false;

    getLowerBoundering( boundary_ );
    return true;
}


bool Controller::isDefineBelowActive( PolyCurve& boundary_ )
{
    std::size_t index_ = 0;

    bool status_ = rules_processor.preserveBelowIsActive();
    if( status_ == false ) return false;

    getUpperBoundering( boundary_ );
    return true;

}


void Controller::getLowerBoundering( PolyCurve& boundary_ )
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

//    if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::X )
//    {
//        rules_processor.getPreserveAboveCurveBoxAtWidth( indexCrossSectionX( csection->getDepth() ), vertices_, edges_ );
//    }
//    else if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::Z )
//    {
//        rules_processor.getPreserveAboveCurveBoxAtLength( indexCrossSectionZ( csection->getDepth() ), vertices_, edges_ );
//    }

    boundary_.fromVector( vertices_, edges_ );
}


void Controller::getUpperBoundering( PolyCurve& boundary_ )
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


//    if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::X )
//    {
//        rules_processor.getPreserveBelowCurveBoxAtWidth( indexCrossSectionX( csection->getDepth() ), vertices_, edges_ );

//    }

//    else if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::Z )
//    {
//        rules_processor.getPreserveBelowCurveBoxAtLength( indexCrossSectionZ( csection->getDepth() ), vertices_, edges_ );

//    }

    boundary_.fromVector( vertices_, edges_ );
}


void Controller::setPointGuidedExtrusion( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ )
{
}

///==========================================================================

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

    std::string complete_filename = filename + ".json";
    QFile save_file( QString( complete_filename.c_str() ) );

    if ( !save_file.open( QIODevice::WriteOnly ) ) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject metadatas;

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

    if( model.domains1.empty() == false )
    {

        QJsonArray domains_array_;
        for( auto it_: model.domains1 )
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

    clear();
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

    //    init();

    double ox, oy, oz;
    double width, height, depth;

    rules_processor.getOrigin( ox, oy, oz );
    rules_processor.getLenght( width, height, depth );
    model.volume->setGeometry( ox, oy, oz, width, height, depth );
    setVolumeDiscretization();

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

    if( model.objects.find( current_object ) != model.objects.end() )
    {
        model.objects[ current_object ]->clear();
        model.objects[ current_object ].reset();
        model.objects[ current_object ] = nullptr;
        model.objects.clear();
    }


    std::string complete_filename = filename + ".json";

    QFile load_file( QString( complete_filename.c_str() ) );
    if ( !load_file.open( QIODevice::ReadOnly ) ) {
        loadObjectNoMetaDatas();
    }
    else
        loadObjectMetaDatas( load_file );

}


void Controller::loadObjectNoMetaDatas()
{
    std::random_device rd;
    std::mt19937 eng( rd() );
    std::uniform_int_distribution< size_t > distr( 0, 255 );

    int counter_ = 0;
    while( rules_processor.canRedo() )
    {
        rules_processor.redo();
        counter_++;
    }


    std::vector< std::size_t > actives = rules_processor.getActiveSurfaces();
    for( auto id: actives )
    {
        int r_ = distr( eng );
        int g_ = distr( eng );
        int b_ = distr( eng );

        addObject( id );
        setObjectColor( id, r_, g_, b_ );

        model.objects[ id ]->setDone( true );

    }

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
        defineRegions();

        QJsonArray regions_array_ = json["regions"].toArray();
        int nregions_ = regions_array_.size();
        for( int i = 0; i < nregions_; ++i )
        {
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


            std::size_t id_ = static_cast< std::size_t>( domain_["index"].toInt() );
            if( ( domain_.contains( "regions" ) == false ) ||
                    ( domain_["regions"].isArray()  == false ) ) return;

            std::size_t index_ = createDomain1();

            QJsonArray regions_set_array_ = domain_["regions"].toArray();
            for( auto it_: regions_set_array_ )
                addRegionToDomain1( static_cast< std::size_t >( it_.toInt() ), index_ );

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

    setVolumeDiscretization();
}


void Controller::exportToIrapGrid()
{

    IrapGridExporter exporter;
    std::vector< float > points;

    std::vector< std::size_t > actives_ = rules_processor.getOrderedActiveSurfaces();
    for ( std::size_t id_: actives_ )
    {
        QString surface_filename = QString( "Surface %1.IRAPG").arg( id_ );

        std::vector< double > points_list;
        std::vector< bool > valid_points;
        std::size_t nu;
        std::size_t nv;

        rules_processor.getQuadMesh( id_, points_list, valid_points, nu, nv );

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

        float dx = (float)( xmax - xmin )/( nu - 1 );
        float dy = (float)( ymax - ymin )/( nv - 1 );

        exporter.setBoundingBox( (float )xmin, (float)xmax, (float)ymin, (float) ymax, (float) zmin, (float) zmax );
        exporter.setVectorValues( points );
        exporter.setSize( (int) nu,  (int) nv );
        exporter.setSpacing( dx, dy );

        std::string name_ = getObjectName( id_ ).append( ".IRAPG" );
        if( name_.empty() == true )
            exporter.writeGridData( surface_filename.toStdString() );
        else
            exporter.writeGridData( name_ );

        exporter.clearData();

        points.clear();
        surface_filename.clear();
    }

}


void Controller::setGuidedExtrusion( float px_, float py_, float pz_, const PolyCurve& curve_ )
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


std::vector<int> Controller::getTetrahedronsRegions( const std::vector< float >& vertices, const std::vector< unsigned int >& edges, const std::vector< unsigned int >& faces )
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

    for( auto it: model.domains1 )
    {
        (it.second).clear();
    }
    model.domains1.clear();
    regions_in_domains.clear();


    selectable_objects.clear();
    boundering_region = Settings::Objects::BounderingRegion::NONE ;
    upper_index = 0;
    bottom_index = 0;

    images_csectionsX.clear();
    images_csectionsY.clear();
    images_csectionsZ.clear();

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
