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
//    output_volume = nullptr;
//    main_csection = nullptr;
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
    stratigraphies[ index_ ]->update();

    emit updateCanvas();

}


void Scene3d::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                               const int& h )
{

    if( volume != nullptr )
        volume->draw( V, P, w, h );

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
