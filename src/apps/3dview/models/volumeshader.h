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



#ifndef VOLUMESHADER_H
#define VOLUMESHADER_H


#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "./core/models/shader.h"
#include "./core/models/volume.h"


/**
 *  A graphical representation to the volume using opengl to the scene 3d.
 *  It always is represent by a cube.
 *  It inherits from the class Shader.
 */

class VolumeShader: public Shader
{

    public:


        /**
        * Default Constructor.
        */
        VolumeShader();


        /**
        * Constructor. It builds a VolumeShader given a Volume instance.
        * @param raw_ a shared pointer to a Volume instance
        */
        VolumeShader( const std::shared_ptr< Volume >& raw_ );


        /**
        * Destructor.
        */
        ~VolumeShader();


        /**
        * Method to set a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved.
        * @param raw_ a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved
        * @see Volume
        * @return void.
        */
        void setVolume( const std::shared_ptr< Volume >& raw_ );



        //TODO: finish to document the parameters.
        /**
        * Method to render the volume
        * @param V it is the view matrix
        * @param P it is the projection matrix
        * @param w it is the canvas width
        * @param h it is the canvas height
        * @return void.
        */
        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                                 const int& h );



        /**
        * This method calls the method createVolumeMesh() in order to update the mesh.
        * @see createVolumeMesh()
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


    protected:


        /**
        * This method reset the shared pointer of the raw volume and set the default values to the class members.
        * @return void.
        */
        void setDefaultValues();


        /**
        * This method is the responsible to the mesh creation.
        * @return void.
        */
        void createVolumeMesh();


        /**
        * An auxialiary and transitory method to compute the normals of the volume vertices.
        * @return std::vector< float > a vector of the computed normals.
        */
        std::vector< float > defineVolumeNormals() const;


        /**
        * This method loads the buffers with the required geometry
        * @param vertices_ normalized vertices
        * @param normals_  normalized normals
        * @return void.
        */
        void updateGeometryBuffers( const std::vector< GLfloat >& vertices_,
                                    const std::vector< GLfloat >& normals_ );


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

        GLuint va_volume;                                           /**< Vertex array object of volumes */

        GLuint vb_vertices;                                         /**< Vertex buffer array object */

        GLuint vb_normals;                                          /**< Normal buffer array object */

        GLuint number_of_vertices;                                  /**< Number total of vertices in the volume*/

        std::shared_ptr< Volume > raw;                              /**< Shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved. */

};

#endif // VOLUMESHADER_H
