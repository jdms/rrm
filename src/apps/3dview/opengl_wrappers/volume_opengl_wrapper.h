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


#ifndef VOLUMEOPENGLWRAPPER_H
#define VOLUMEOPENGLWRAPPER_H

#include <string>

#include "Tucano/Shader.hpp"
#include "3dview/model3d_utils.hpp"

class Volume;


class VolumeOpenGLWrapper
{
    public:

        VolumeOpenGLWrapper();


        void setShaderDirectory( const std::string& dir );


        void setRawVolume( Volume* const& vol );
        Volume* getRawVolume() const ;


        bool isVisible() const;

        void getOrigin( double& w, double& h, double& d ) const ;

        double getWidth() const ;
        double getHeight() const ;
        double getDepth() const ;

        void getMinimum( float& mx, float& my, float &mz ) const;
        void getMaximum( float& mx, float& my, float &mz ) const;


        void createVolumeBox();


        void updateGeometry();
        void defineNormals();


        void initShaders();
        void initBuffers();
        void resetShaders();
        void resetBuffers();
        void reloadShaders();
        void reloadBuffers( const std::vector< float >& wireframe );


        void init();
        void reset();
        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                   const int& h );

        void clear();
        void clearData();
        void setDefaultValues();



    private:

        Volume* raw;

        std::string shader_directory;

        GLuint va_volume;
        GLuint vb_volume_vertices;
        GLuint vb_volume_normals;
        GLuint number_of_vertices;

        Tucano::Shader* shader;

        Eigen::Vector3f minimum;
        Eigen::Vector3f maximum;


};

#endif // VOLUMEOPENGLWRAPPER_H
