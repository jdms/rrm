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



#ifndef SURFACESHADER_H
#define SURFACESHADER_H

#include <memory>
#include <iostream>
#include <string>
#include <vector>

#include "./core/models/shader.h"
#include "./core/models/stratigraphy.h"

class SurfaceShader: public Shader
{

    public:

        SurfaceShader();
        SurfaceShader( const std::shared_ptr< Stratigraphy >& raw_ );
        ~SurfaceShader();

        void setObject( const std::shared_ptr< Stratigraphy >& raw_ );

        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
              const int& h );

        void update();
        void clear();

        void setHeightMap( double zmin_, double zmax_ );

        void enableDrawingEdges();


    protected:


        void setDefaultValues();
        void loadBuffers();


        void updateGeometryBuffers( const std::vector< GLfloat >& vertices_,
                                                   const std::vector< GLfloat >& normals_,
                                                   const std::vector< GLuint >& faces_ );

        void updateColorBuffers( const std::vector< GLfloat >& colors_ );


        void updateColorBuffers( std::size_t nvertices_, int r_, int g_, int b_ );


        virtual void initShaders();
        virtual void initBuffers();

        virtual void resetShaders();
        virtual void resetBuffers();



    private:

        GLuint va_surface;
        GLuint vb_vertices;
        GLuint vb_normals;
        GLuint vb_colors;
        GLuint vb_faces;

        GLuint number_of_vertices;
        GLuint number_of_faces;

        bool draw_edge = true;

        std::shared_ptr< Stratigraphy > raw;

        Tucano::Shader* shader_new;
};

#endif // SURFACESHADER_H
