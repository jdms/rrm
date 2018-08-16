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


#include "scene3d.h"


#include "./core/models/object.h"
#include "./models/volumeshader.h"
#include "./models/planeshader.h"
#include "./models/surfaceshader.h"
#include "./models/regionshader.h"


#include <QString>
#include <QOpenGLContext>
#include <QSurface>


Scene3d::Scene3d()
{
    volume = nullptr;
}



void Scene3d::addVolume( const std::shared_ptr< Volume >& raw_ )
{
    context->makeCurrent( surface );

    volume = std::make_shared< VolumeShader >( raw_ );
    emit updateCanvas();

}


void Scene3d::updateVolume()
{
    if( volume == nullptr ) return;
    volume->update();



    emit updateCanvas();
}





void Scene3d::addMainCrossSection( const std::shared_ptr< CrossSection>& raw_ )
{
    context->makeCurrent( surface );
    main_csection = std::make_shared< PlaneShader >( raw_ );
    emit updateCanvas();
}


void Scene3d::updateMainCrossSection()
{
    main_csection->update();
    emit updateCanvas();
}


void Scene3d::changeCrossSectionDirection( const std::shared_ptr< CrossSection>& raw_ )
{
    main_csection->setCrossSection( raw_ );
    emit updateCanvas();
}


void Scene3d::addCrossSection( const std::shared_ptr< CrossSection >& csection_ )
{
    context->makeCurrent( surface );

    if( csection_->getDirection() == Settings::CrossSection::CrossSectionDirections::X )
        cross_sectionsX[ csection_->getDepth() ] = std::make_shared< PlaneShader >( csection_ );

    if( csection_->getDirection() == Settings::CrossSection::CrossSectionDirections::Y )
        cross_sectionsY[ csection_->getDepth() ] = std::make_shared< PlaneShader >( csection_ );

    if( csection_->getDirection() == Settings::CrossSection::CrossSectionDirections::Z )
        cross_sectionsZ[ csection_->getDepth() ] = std::make_shared< PlaneShader >( csection_ );

    emit updateCanvas();
}


void Scene3d::updateCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        if( cross_sectionsX.find( depth_ ) == cross_sectionsX.end() ) return;
        cross_sectionsX[ depth_ ]->update();
    }

    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        if( cross_sectionsY.find( depth_ ) == cross_sectionsY.end() ) return;
        cross_sectionsY[ depth_ ]->update();
    }

    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
        if( cross_sectionsZ.find( depth_ ) == cross_sectionsZ.end() ) return;
        cross_sectionsZ[ depth_ ]->update();
    }
    emit updateCanvas();
}



void Scene3d::updateCrossSections()
{
    for( auto it: cross_sectionsX )
        (it.second)->update();
    for( auto it: cross_sectionsY )
        (it.second)->update();
    for( auto it: cross_sectionsZ )
        (it.second)->update();

    emit updateCanvas();
}


void Scene3d::addStratigraphy( const std::shared_ptr< Stratigraphy >& raw_ )
{
    context->makeCurrent( surface );
    stratigraphies[ raw_->getIndex() ] = std::make_shared< SurfaceShader >( raw_ );

    emit updateCanvas();

}


void Scene3d::updateStratigraphy( const std::size_t& index_ )
{
    context->makeCurrent( surface );
    if( stratigraphies.find( index_ ) == stratigraphies.end() )
    stratigraphies[ index_ ]->update();

    emit updateCanvas();

}

void Scene3d::updateStratigraphies()
{
    context->makeCurrent( surface );
    for( auto it: stratigraphies )
    {
        (it.second)->update();
    }
    emit updateCanvas();

}


void Scene3d::addRegion( const std::shared_ptr< Regions >& region_ )
{
    context->makeCurrent( surface );
    regions[ region_->getIndex() ] = std::make_shared< RegionShader >( region_ );
    emit updateCanvas();
}


void Scene3d::updateRegion( const std::size_t& index_ )
{
    context->makeCurrent( surface );
    if( regions.find( index_ ) == regions.end() )
    regions[ index_ ]->update();

    emit updateCanvas();

}


void Scene3d::updateRegions()
{
    context->makeCurrent( surface );
    for( auto it: regions )
        (it.second)->update();

    emit updateCanvas();
}


void Scene3d::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                               const int& h )
{

    if( volume != nullptr )
        volume->draw( V, P, w, h );

    if( main_csection != nullptr )
        main_csection->draw( V, P, w, h );

    for ( auto it: stratigraphies )
    {
        std::shared_ptr < SurfaceShader > surface_ = it.second;
        surface_->draw( V, P, w, h );
    }

}


void Scene3d::setOpenGLContext( QOpenGLContext* ctxt )
{
    context = ctxt;
    surface = ctxt->surface();
}

void Scene3d::clearScene()
{
    volume.reset();
    main_csection.reset();

    for( auto it: cross_sectionsX )
        (it.second)->reset();
    cross_sectionsX.clear();

    for( auto it: cross_sectionsY )
        (it.second)->reset();
    cross_sectionsY.clear();

    for( auto it: cross_sectionsZ )
        (it.second)->reset();
    cross_sectionsZ.clear();

    for( auto it: stratigraphies )
        (it.second)->reset();
    stratigraphies.clear();

    for( auto it: regions )
        (it.second)->reset();
    regions.clear();

    emit updateCanvas();
}
