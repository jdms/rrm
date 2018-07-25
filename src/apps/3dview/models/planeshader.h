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



#ifndef PLANESHADER_H
#define PLANESHADER_H


#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "./core/models/shader.h"

class CrossSection;

class PlaneShader: public Shader
{
    public:

        PlaneShader();
        PlaneShader( const std::shared_ptr< CrossSection>& raw_ );
        ~PlaneShader();

        void setCrossSection( const std::shared_ptr< CrossSection>& raw_ );

        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                                const int& h );

        void update();
        void clear();


    protected:


        void updateGeometryBuffers( const std::vector< GLfloat >& vertices_,
                                    const std::vector< GLfloat >& normals_ );



        void setDefaultValues();
        void createPlane();
        std::vector< float > definePlaneNormals() const;


        virtual void initShaders();
        virtual void initBuffers();

        virtual void resetShaders();
        virtual void resetBuffers();



    private:

        GLuint va_plane;
        GLuint vb_vertices;
        GLuint vb_normals;

        GLuint number_of_vertices;

        std::shared_ptr< CrossSection> csection;
};

#endif // PLANESHADER_H
