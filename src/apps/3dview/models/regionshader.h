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
 * @file regionshader.h
 * @author Clarissa C. Marques
 * @date 9 Sep 2012
 * @brief File containing the class RegionShader
 */


#ifndef VOLUMEMESHSHADER_H
#define VOLUMEMESHSHADER_H


#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "./core/models/shader.h"
#include "./core/models/regions.h"


/**
 *  A graphical representation to the regions using opengl to the scene 3d.
 *  It always is represent by a volume.
 *  It inherits from the class Shader.
 */

class RegionShader: public Shader
{

    public:

        /**
        * Default Constructor.
        */
        RegionShader();


        /**
        * Constructor. It builds a RegionShader given a Regions instance
        * @param a shared pointer to a Regions instance
        */
        RegionShader( const std::shared_ptr< Regions >& raw_ );


        /**
        * Destructor.
        */
        ~RegionShader();


        /**
        * Method to set a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved.
        * @param raw_ a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved
        * @return void.
        */
        void setRegion( const std::shared_ptr< Regions >& raw_ );


        /**
        * Method to render the region
        * @param V it is the view matrix
        * @param P it is the projection matrix
        * @param w it is the canvas width
        * @param h it is the canvas height
        * @return void.
        */
        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
              const int& h );


        /**
        * This method calls the method 'loadBuffers' in order to update the region.
        * @return void.
        */
        void update();


        /**
        * This method calls the method setDefaultValues() and reset(). This latter is from the class Shader, and is responsible for resetting thwe shaders.
        * @return void.
        */
        void clear();


    protected:


        /**
        * This method reset the shared pointer of the raw volume and set the default values to the class members.
        * @return void.
        */
        void setDefaultValues();


        /**
        * This method gets the geometry from the raw data, normalize the vertices and calls the updateGeometryBuffers() and updateColorBuffers() to load the buffers.
        * @return void.
        */
        void loadBuffers();


        /**
        * This method loads the buffers with the required geometry
        * @param vertices_ normalized vertices
        * @param normals_  normalized normals
        * @param faces_  surface faces
        * @return void.
        */
        void updateGeometryBuffers( const std::vector< GLfloat >& vertices_,
                                                   const std::vector< GLfloat >& normals_,
                                                   const std::vector< GLuint >& faces_ );


        /**
        * This method loads the colors buffers. This methods allows different colors to each vertex.
        * @param colors_ vector of colors. The number of colors is the same of the number of vertices.
        * @return void.
        */
        void updateColorBuffers( const std::vector< GLfloat >& colors_ );


        /**
        * This method creates a vector of the color (r_, g_, b_) repeated to each vertex.
        * @param nvertices_ number of vertices of the mesh. It will be created the same number of colors.
        * @param r_ red component of the color
        * @param green_ green component of the color
        * @param blue_ blue component of the color
        * @return void.
        */
        void updateColorBuffers( std::size_t nvertices_, int r_, int g_, int b_ );


        /**
        * This is a virtual method to initialize the shaders
        * @return void.
        */
        virtual void initShaders();


        /**
        * This is a virtual method to initialize the buffers
        * @return void.
        */
        virtual void initBuffers();


        /**
        * Virtual method to reset the shaders
        * @return void.
        */
        virtual void resetShaders();


        /**
        * Virtual method to reset the buffers
        * @return void.
        */
        virtual void resetBuffers();


    private:

        GLuint va_volume;                                               /**< Vertex array object of volumes */
        GLuint vb_vertices;                                             /**< Vertex buffer array object */
        GLuint vb_normals;                                              /**< Normal buffer array object */
        GLuint vb_colors;                                               /**< Color buffer array object */
        GLuint vb_faces;                                                /**< Faces buffer array object */

        GLuint number_of_vertices;                                      /**< Number total of vertices in the region*/
        GLuint number_of_faces;                                         /**< Number total of faces in the region*/

        std::shared_ptr< Regions > raw;                                 /**< Shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved. */

};


#endif // VOLUMEMESHSHADER_H
