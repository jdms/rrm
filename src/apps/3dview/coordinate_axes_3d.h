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


#ifndef COORDINATEAXES3D_H
#define COORDINATEAXES3D_H

#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"
#include "Tucano/BoundingBox3.hpp"


#include "./libs/tinyobjloader/tiny_obj_loader.h"

class CoordinateAxes3d
{
public:

    CoordinateAxes3d();
    ~CoordinateAxes3d();

    void init();
    void load( std::string directory );

    void draw(  const Eigen::Affine3f& V_, const Eigen::Matrix4f& P_, const int& w_, const int& h_ );

    void reloadShader();

    void initShader( std::string directory );

    void resetBuffers();
    inline void setCenter( float ox_, float oy_, float oz_ )
    {
        origin = Eigen::Vector3f( ox_, oy_, oz_ );
        centered = true;
    }


    inline void setNonCentered()
    {
        origin = Eigen::Vector3f( 0.0f, 0.0f, 0.0f );
        centered = false;
    }

private:

    void requestOpenGLResources();
    void releaseOpenGLResources();

    Tucano::Shader* shader_axes; // create a shader to shader_axis, since there will be shaders only to axes
    Eigen::Affine3f viewMatrix_;
    Eigen::Matrix4f projetionMatrix;

    GLuint vertex_array_coneaxes;
    GLuint vertex_buffer_faces_indices;
    GLuint vertex_buffer_vertices;
    GLuint vertex_buffer_normals;
    GLuint vertex_buffer_colors;

    int indices_size_;

    std::string current_directory;

    bool centered = true;
    Eigen::Vector3f origin;

};

#endif // COORDINATEAXES3D_H
