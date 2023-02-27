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
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>. */

#include "coordinate_axes_3d.h"
#include <math.h>


CoordinateAxes3d::CoordinateAxes3d()
{

}


CoordinateAxes3d::~CoordinateAxes3d()
{
	resetBuffers();
}

void CoordinateAxes3d::init()
{
	vertex_array_coneaxes = 0;
	vertex_buffer_faces_indices = 0;
	vertex_buffer_vertices = 0;
	vertex_buffer_normals = 0;
	vertex_buffer_colors = 0;

    origin = Eigen::Vector3f( 0.0f, 0.0f, 0.0f );
}

void CoordinateAxes3d::initShader( std::string directory )
{
	shader_axes = new Tucano::Shader("shader_axes", (directory + "shaders/orientationAxes.vert"), (directory + "shaders/orientationAxes.frag"), (directory + "shaders/orientationAxes.geom"), "", "");
    shader_axes->initialize();

    current_directory = directory;

}

void CoordinateAxes3d::reloadShader()
{
	if (shader_axes)
	{
		shader_axes->reloadShaders();
	}
}

void CoordinateAxes3d::load()
{

	std::string filename = current_directory + "objects/orientationaxes.obj";
	std::string material_path = current_directory + "objects/";

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	//// For Vertex Buffer Intervealed @see https://www.khronos.org/opengl/wiki/Vertex_Specification_Best_Practices
	/// (VNC-VNC-VNC ...)

	std::vector<GLfloat> attributes;
	std::vector<int> indices;


	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(), material_path.c_str())) 
	{
		throw std::runtime_error(err);
	}
	else
	{
		attributes.clear();
		attributes.resize(attrib.vertices.size()*3);

		/// Assume the model as a whole. 
		/// @todo take the cone tip of the axis as a reference to a Axis letters.

		for ( auto shape : shapes) 
		{
			// Loop over faces(polygon)			
			for (auto i : shape.mesh.indices)
			{
				indices.push_back(i.vertex_index);
			}

			size_t index_offset = 0;

			for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) 
			{
				size_t fv = shape.mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) 
				{
					// access to vertex
					/// Use idx.vertex_index             | Use idx.vertex_index
					tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
					attributes[9 * idx.vertex_index + 0] = attrib.vertices[3 * idx.vertex_index + 0];
					attributes[9 * idx.vertex_index + 1] = attrib.vertices[3 * idx.vertex_index + 1];
					attributes[9 * idx.vertex_index + 2] = attrib.vertices[3 * idx.vertex_index + 2];
					///	Use idx.vertex_index			 | Use idx.normal_index

					attributes[9 * idx.vertex_index + 3] = attrib.normals[3 * idx.normal_index + 0];
					attributes[9 * idx.vertex_index + 4] = attrib.normals[3 * idx.normal_index + 1];
					attributes[9 * idx.vertex_index + 5] = attrib.normals[3 * idx.normal_index + 2];
					///	Use idx.vertex_index			 | shape.mesh.material_ids[f]
					attributes[9 * idx.vertex_index + 6] = materials[shape.mesh.material_ids[f]].diffuse[0];
					attributes[9 * idx.vertex_index + 7] = materials[shape.mesh.material_ids[f]].diffuse[1];
					attributes[9 * idx.vertex_index + 8] = materials[shape.mesh.material_ids[f]].diffuse[2];
				}
				index_offset += fv;
			}
		}

	}
	this->requestOpenGLResources();

	glBindVertexArray(vertex_array_coneaxes);

		//// - Vertex Information  (v.layout = 0, n.layout = 1, c.layout = 2)
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_vertices);
		glBufferData(GL_ARRAY_BUFFER, attributes.size() * sizeof(attributes[0]), attributes.data(), GL_STATIC_DRAW);

		/// Size of the per vertex attributes, aka (Vertex(V), Normal(N), Color(C)), each 3 floats
		int size_of_vertice = 3 * sizeof(float); 
		/// Size of per triagle attributes [ (V.x,V.y,V.z;N.x,N.y,N.z;C.x,C.y,C.z) ], each 3 floats to vertice, 3 to normal, and 3 to color
		int size_of_struct  = 3 * size_of_vertice;

		////! @link - http://www.opengl.org/wiki/Vertex_Specification
		//// Assign for vertex attribute location 0 - 3 
		for (int location = 0; location < 3; location++)
		{
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, size_of_struct, reinterpret_cast<void*>(size_of_vertice * location));
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_faces_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
		indices_size_ = indices.size();

	glBindVertexArray(0);
}

void CoordinateAxes3d::draw( const Eigen::Affine3f& V_, const Eigen::Matrix4f& P_, const int& w_, const int& h_ )
{


    Eigen::Matrix3f R = V_.rotation().matrix();
    Eigen::Vector3f T = V_.translation();

    Eigen::Affine3f V;
    V.setIdentity();

    for( int i = 0; i < 3; ++i )
        for( int j = 0; j < 3; ++j )
            V( i, j ) = R( i, j );

    V( 0, 3 ) = T( 0 );
    V( 1, 3 ) = T( 1 );
    V( 2, 3 ) = T( 2 );


    Eigen::Affine3f M;
    M.setIdentity();
    M.scale( 2.0f );


/*
    Eigen::Matrix4f P = Eigen::Matrix4f::Zero();

    float left		 = -1.0f;
    float right		 =  1.0f;
    float bottom	 = -1.0f;
    float top		 =  1.0f;
    float near_plane = 0.1f;
    float far_plane  = 100.0f;

    P(0, 0) = 2.0f / (right - left);
    P(1, 1) = 2.0f / (top - bottom);
    P(2, 2) = -2.0f / (far_plane - near_plane);
    P(3, 3) = 1.0f;
    P(0, 3) = -(right + left) / (right - left);
    P(1, 3) = -(top + bottom) / (top - bottom);
    P(2, 3) = -(far_plane + near_plane) / (far_plane - near_plane);


    Eigen::Affine3f M;
    M.setIdentity();
    M.scale( 0.7f );

    Eigen::Affine3f V;
    V.setIdentity();
    V.translate(Eigen::Vector3f(0.0f, 0.0f, -4.0f));
    V.rotate( V_.rotation().matrix() );
*/

    glViewport( 0, 0, w_*0.17, h_*0.17 );
	shader_axes->bind();

    shader_axes->setUniform( "ModelMatrix", M );
    shader_axes->setUniform( "ViewMatrix", V );
    shader_axes->setUniform( "ProjectionMatrix", P_ );

		glBindVertexArray(vertex_array_coneaxes);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_faces_indices);
			glDrawElements(GL_TRIANGLES, indices_size_, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

	shader_axes->unbind();

    glViewport(0, 0, w_, h_ );

}

void CoordinateAxes3d::resetBuffers()
{
	/// Delete Shaders
	if (shader_axes)
	{
		shader_axes->deleteShaders();
		shader_axes = nullptr;
	}

	this->releaseOpenGLResources();
}

void CoordinateAxes3d::requestOpenGLResources()
{
	this->releaseOpenGLResources();

	// Cuboid
	glGenVertexArrays(1, &vertex_array_coneaxes);		// Vertex Array
		glGenBuffers(1, &vertex_buffer_faces_indices);  // Face Indices
		glGenBuffers(1, &vertex_buffer_vertices);		// Vertices
		glGenBuffers(1, &vertex_buffer_normals);		// Normals
		glGenBuffers(1, &vertex_buffer_colors);			// Colors

}

void CoordinateAxes3d::releaseOpenGLResources()
{
	if (vertex_array_coneaxes)
	{
		glDeleteVertexArrays(1, &vertex_array_coneaxes);

		/// face indices
		if (vertex_buffer_faces_indices)
		{
			glDeleteBuffers(1, &vertex_buffer_faces_indices);
		}
		/// vertices
		if (vertex_buffer_vertices)
		{
			glDeleteBuffers(1, &vertex_buffer_vertices);
		}
		/// normals
		if (vertex_buffer_normals)
		{
			glDeleteBuffers(1, &vertex_buffer_normals);
		}
		/// colors
		if (vertex_buffer_colors)
		{
			glDeleteBuffers(1, &vertex_buffer_colors);
		}
	}

	this->init();
}

