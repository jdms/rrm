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


#ifndef CROSSSECTIONOPENGLWRAPPER_H
#define CROSSSECTIONOPENGLWRAPPER_H

#include <string>

#include "Tucano/Shader.hpp"
#include "3dview/model3d_utils.hpp"


class CrossSectionOpenGLWrapper
{
    public:

        CrossSectionOpenGLWrapper();

        void setShaderDirectory( const std::string& dir );

        void setDepth( double d );
        double getDepth() const ;

        bool isVisible() const;
        void setVisible( bool status );



        void setMinimum( float mx, float my, float mz );
        void setMaximum( float mx, float my, float mz );


        void createPlane();


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

        std::string shader_directory;

        GLuint va_csection;
        GLuint vb_csection_lines;
        GLuint vb_csection_normals;
        GLuint number_of_vertices;

        Tucano::Shader* shader;

        Eigen::Vector3f minimum;
        Eigen::Vector3f maximum;

        float depth;
        bool is_visible;


};

#endif // CROSSSECTIONOPENGLWRAPPER_H
