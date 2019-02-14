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
 * @file surfaceshader.h
 * @author Clarissa C. Marques
 * @brief File containing the class SurfaceShader
 */



#ifndef SURFACESHADER_H
#define SURFACESHADER_H

#include <memory>
#include <iostream>
#include <string>
#include <vector>

#include "./core/models/shader.h"
#include "./core/models/stratigraphy.h"


/**
 *  A graphical representation to the stratigraphies and structurals using opengl to the scene 3d.
 *  It always is represent by a surface.
 *  It inherits from the class Shader.
 */


class SurfaceShader: public Shader
{

    public:


        /**
        * Default Constructor.
        */
        SurfaceShader();


        /**
        * Constructor. It builds a SurfaceShader given a Stratigraphy instance.
        * @param raw a shared pointer to a Stratigraphy instance
        */
        SurfaceShader( const std::shared_ptr< Stratigraphy >& raw_ );


        /**
        * Destructor.
        */
        ~SurfaceShader();


        /**
        * Method to set a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved.
        * @param raw a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved
        * @see Stratigraphy
        * @return void.
        */
        void setObject( const std::shared_ptr< Stratigraphy >& raw_ );


        /**
        * Method to render the surface
        * @param V it is the view matrix
        * @param P it is the projection matrix
        * @param w it is the canvas width
        * @param h it is the canvas height
        * @return void.
        */
        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
              const int& h );


        /**
        * This method calls the method 'loadBuffers' in order to update the mesh.
        * @see loadBuffers()
        * @return void.
        */
        void update();


        /**
        * This method calls the method setDefaultValues() and reset(). This latter is from the class Shader, and is responsible for resetting thwe shaders.
        * @see setDefaultValues()
        * @see reset()
        * @return void.
        */
        void clear();


        /**
        * This method updates the color surface to a height map color. The color of each vertice will be computed by an interpolation between the z minimum value and z maximum value.
        * @param zmin the minimum z value
        * @param zmax the maximum z value
        * @return void.
        */
        void setHeightMap( double zmin_, double zmax_ );


        /**
        * Inline method to check if the surface contains any vertice.
        * @return boolean Returns true if there is at least on vertex and false otherwise.
        */
        inline bool isEmpty(){ return raw->getSurface().isEmpty(); }


        /**
        * An auxialiary and transitory method to enable the edged rendering.
        * @return void.
        */
        void enableDrawingEdges();


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
        * @param vertices normalized vertices
        * @param normals  normalized normals
        * @param faces  surface faces
        * @return void.
        */
        void updateGeometryBuffers( const std::vector< GLfloat >& vertices_,
                                                   const std::vector< GLfloat >& normals_,
                                                   const std::vector< GLuint >& faces_ );

        /**
        * This method loads the colors buffers. This methods allows different colors to each vertex.
        * @param colors vector of colors. The number of colors is the same of the number of vertices.
        * @return void.
        */
        void updateColorBuffers( const std::vector< GLfloat >& colors_ );


        /**
        * This method creates a vector of the color (r_, g_, b_) repeated to each vertex.
        * @param r red component of the color
        * @param g green component of the color
        * @param b blue component of the color
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

        GLuint va_surface;                                          /**< Vertex array object of surfaces */

        GLuint vb_vertices;                                         /**< Vertex buffer array object */

        GLuint vb_normals;                                          /**< Normal buffer array object */

        GLuint vb_colors;                                           /**< Color buffer array object */

        GLuint vb_faces;                                            /**< Faces buffer array object */

        GLuint number_of_vertices;                                  /**< Number total of vertices in the surface*/

        GLuint number_of_faces;                                     /**< Number total of faces in the surface*/

        bool draw_edge = true;                                      /**< Temporary variable to indicates if the edges should be                         rendered. Its values is true, if the edges should appear and false otherwise */

        std::shared_ptr< Stratigraphy > raw;                        /**< Shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved. */

        Tucano::Shader* shader_new;
};

#endif // SURFACESHADER_H
