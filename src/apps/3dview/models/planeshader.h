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
 * @file planeshader.h
 * @author Clarissa C. Marques
 * @date 9 Sep 2012
 * @brief File containing the class PlaneShader
 */


#ifndef PLANESHADER_H
#define PLANESHADER_H


#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "./core/models/shader.h"

class CrossSection;


/**
 *  A graphical representation to the cross-section using opengl to the scene 3d.
 *  It always is represent by a plane.
 *  It inherits from the class Shader.
 */



class PlaneShader: public Shader
{

    public:


        /**
        * Default Constructor.
        */
        PlaneShader();


        /**
        * Constructor. It builds a PlaneShader given a cross-section instance.
        * @param a shared pointer to a CrossSection instance
        */
        PlaneShader( const std::shared_ptr< CrossSection>& raw_ );


        /**
        * Destructor.
        */
        ~PlaneShader();


        /**
        * Method to set a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved.
        * @param raw_ a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved
        * @see CrossSection()
        * @return Void.
        */
        void setCrossSection( const std::shared_ptr< CrossSection>& raw_ );


        /**
        * Method to render the volume
        * @param V it is the view matrix
        * @param P it is the projection matrix
        * @param w it is the canvas width
        * @param h it is the canvas height
        * @see Eigen::Affine3f
        * @see Eigen::Matrix4f
        * @return Void
        */
        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                                const int& h );


        /**
        * This method calls the method 'createVolumeMesh' in order to update the mesh.
        * @see createVolumeMesh()
        * @return Void
        */
        void update();


        /**
        * This method calls the method setDefaultValues() and reset(). This latter is from the class Shader, and is responsible for resetting thwe shaders.
        * @see setDefaultValues()
        * @see reset()
        * @return Void
        */
        void clear();


    protected:


        /**
        * This method loads the buffers with the required geometry
        * @param vertices_ normalized vertices
        * @param normals_  normalized normals
        * @return Void
        */
        void updateGeometryBuffers( const std::vector< GLfloat >& vertices_,
                                    const std::vector< GLfloat >& normals_ );


        /**
        * This method reset the shared pointer of the raw volume and set the default values to the class members.
        * @return Void
        */
        void setDefaultValues();


        /**
        * This method is the responsible to the plane creation.
        * @return Void
        */
        void createPlane();


        /**
        * An auxialiary and transitory method to compute the normals of the plane vertices.
        * @return Void
        */
        std::vector< float > definePlaneNormals() const;


        /**
        * This is a virtual method to initialize the shaders
        * @return Void
        */
        virtual void initShaders();


        /**
        * This is a virtual method to initialize the buffers
        * @return Void
        */
        virtual void initBuffers();


        /**
        * Virtual method to reset the shaders
        * @return Void
        */
        virtual void resetShaders();


        /**
        * Virtual method to reset the buffers
        * @return Void
        */
        virtual void resetBuffers();



    private:

        GLuint va_plane;                                                    /**< Vertex array object of planes */

        GLuint vb_vertices;                                                 /**< Vertex buffer array object */

        GLuint vb_normals;                                                  /**< Normal buffer array object */

        GLuint number_of_vertices;                                          /**< Number total of vertices in the volume*/

        std::shared_ptr< CrossSection> csection;                            /**< Shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved. */
};

#endif // PLANESHADER_H
