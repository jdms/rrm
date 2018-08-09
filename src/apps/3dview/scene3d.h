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



#ifndef SCENE3D_H
#define SCENE3D_H

#include <memory>

#include <QObject>
#include <QColor>

#include "Eigen/Dense"

class QString;
class QOpenGLContext;
class QSurface;

class VolumeShader;
class PlaneShader;
class SurfaceShader;
class RegionShader;


#include "./core/models/scene.h"
#include "./core/models/crosssection.h"
#include "./core/models/stratigraphy.h"



class Scene3d: public QObject
{
    Q_OBJECT

    public:


        Scene3d();
        ~Scene3d(){}


        void addVolume( const std::shared_ptr< Volume >& raw_ );
        void updateVolume();

        void addMainCrossSection( const std::shared_ptr< CrossSection>& csection_ );
        void changeCrossSectionDirection( const std::shared_ptr< CrossSection>& raw_ );
        void updateMainCrossSection();

        void addCrossSection( const std::shared_ptr< CrossSection >& csection_ );
        void updateCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );
        void removeCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ){}
        void updateCrossSections();


        void addStratigraphy( const std::shared_ptr< Stratigraphy >& raw_ );
        void updateStratigraphy( const std::size_t& index_ );
        void updateStratigraphies();

        void addRegion( const std::shared_ptr< Regions >& region_ );
        void updateRegion( const std::size_t& index_ );
        void updateRegions();


        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w, const int& h );
        void setOpenGLContext( QOpenGLContext* ctxt );

        void clearScene();


    signals:

        void updateCanvas();


    private:

        QString shader_directory;
        QColor current_color;
        QOpenGLContext* context;
        QSurface* surface;

        std::shared_ptr < VolumeShader > volume;
        std::shared_ptr< PlaneShader > main_csection;

        std::map< std::size_t, std::shared_ptr< PlaneShader > > cross_sectionsX;
        std::map< std::size_t, std::shared_ptr< PlaneShader > > cross_sectionsY;
        std::map< std::size_t, std::shared_ptr< PlaneShader > > cross_sectionsZ;

        std::map< std::size_t, std::shared_ptr< SurfaceShader > > stratigraphies;
        std::map< std::size_t, std::shared_ptr< RegionShader > > regions;




};

#endif // SCENE3D_H
