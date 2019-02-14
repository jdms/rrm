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
 * @file coordinate_axes_3d.h
 * @author Clarissa C. Marques
 * @date 9 Sep 2012
 * @brief File containing the class CoordinateAxes3d
 */


#ifndef COORDINATEAXES3D_H
#define COORDINATEAXES3D_H

#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"
#include "Tucano/BoundingBox3.hpp"


#include "./libs/tinyobjloader/tiny_obj_loader.h"


/**
 *  An OpenGL graphical representation to the axes in the scene 3d.
 *  The model is loaded from the file 'orientationaxes.obj'.
 */

class CoordinateAxes3d
{
    public:


        /**
        * Constructor.
        */
        CoordinateAxes3d();


        /**
        * Destructor.
        */
        ~CoordinateAxes3d();


        /**
        * Method to initialize all member data
        * @return void.
        */
        void init();


        /**
        * Method to load the geometry of the axes
        * @param directory the folder where the file 'orientationaxes.obj' is placed
        * @return void.
        */
        void load( std::string directory );


        /**
        * Method to render the axes
        * @param V it is the view matrix
        * @param P it is the projection matrix
        * @param w it is the canvas width
        * @param h it is the canvas height
        * @return void.
        */
        void draw(  const Eigen::Affine3f& V_, const Eigen::Matrix4f& P_, const int& w_, const int& h_ );


        /**
        * Method to reload the buffers
        * @return void.
        */
        void reloadShader();


        /**
        * Method to initialize the shader
        * @param directory path where the shaders are placed.
        * @return void.
        */
        void initShader( std::string directory );


        /**
        * Method to reset the buffers
        * @return void.
        */
        void resetBuffers();


        /**
        * Inline method to change the origin of the axes
        * @param ox_ x coordinate of the origin of the axes
        * @param oy_ y coordinate of the origin of the axes
        * @param oz_ z coordinate of the origin of the axes
        * @return void.
        */
        inline void setCenter( float ox_, float oy_, float oz_ )
        {
            origin = Eigen::Vector3f( ox_, oy_, oz_ );
            centered = true;
        }


        /**
        * Inline method to change the origin of the axes to the default position
        * @return void.
        */
        inline void setNonCentered()
        {
            origin = Eigen::Vector3f( 0.0f, 0.0f, 0.0f );
            centered = false;
        }


    private:


        /**
        * Method to create the vertex arrays and buffers
        * @return void.
        */
        void requestOpenGLResources();


        /**
        * Method to release the vertex arrays and buffers
        * @return void.
        */
        void releaseOpenGLResources();


        Tucano::Shader* shader_axes;                                            /**< Shader to render the axes */

        Eigen::Affine3f viewMatrix_;                                            /**< View matrix */

        Eigen::Matrix4f projetionMatrix;                                        /**< Projection matrix */

        GLuint vertex_array_coneaxes;                                           /**< Vertex array to axes*/

        GLuint vertex_buffer_faces_indices;                                     /**< Faces buffer array */

        GLuint vertex_buffer_vertices;                                          /**< Vertices buffer array */

        GLuint vertex_buffer_normals;                                           /**< Normals buffer array */

        GLuint vertex_buffer_colors;                                            /**< Colors buffer array */

        int indices_size_;                                                      /**< Total number of faces */

        bool centered = true;                                                   /**< Boolean to indicate if the axes should be centered or not. Its value is true if the axes should be centered and false otherwise. */

        Eigen::Vector3f origin;                                                 /**< Origin of the axes */

};

#endif // COORDINATEAXES3D_H
