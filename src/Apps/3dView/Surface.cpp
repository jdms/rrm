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

#include "Surface.h"


Surface::Surface()
{
    is_initialized = false;
    id = 0;

    initData();
}

Surface::Surface( int id_ )
{
    is_initialized = false;
    id = id_;

    initData();

}


void Surface::init()
{

    initShaders();
    initBuffers();

    is_initialized = true;
    is_visible = true;
}


void Surface::initData()
{
    slot_vertices = 0;
    slot_normals = 1 ;
    slot_colors = 2;


    shader_surface = nullptr;

    va_surface = 0;
    vb_vertices = 0;
    vb_normals = 0;
    vb_colors = 0;
    vb_wireframes = 0;
    vb_faces = 0;

    number_of_lines = 0;
    number_of_vertices = 0;
    number_of_faces = 0;
}


void Surface::resetBuffers()
{
    resetShaders();
	
	if( va_surface != 0 )
	{
        glDeleteVertexArrays( 1, &va_surface );
		
        if ( vb_vertices != 0 )
        {
            glDeleteBuffers( 1, &vb_vertices );
        }
		
		if ( vb_normals != 0 )
        {
            glDeleteBuffers( 1, &vb_normals );
        }
		
		if ( vb_colors != 0 )
        {
            glDeleteBuffers( 1, &vb_colors );
        }
		
        if ( vb_wireframes != 0 )
        {
            glDeleteBuffers( 1, &vb_wireframes );
        }
		
		if ( vb_faces != 0 )
        {
            glDeleteBuffers( 1, &vb_faces );
        }
		
    }
		

	va_surface = 0;	
	vb_vertices = 0;	
	vb_normals = 0;
	vb_colors = 0;
	vb_wireframes = 0;
	vb_faces = 0;

    number_of_lines = 0;
    number_of_vertices = 0;
    number_of_faces = 0;

    initShaders();
    initBuffers();
    loadBuffers();
	
	
}


void Surface::initBuffers()
{
	
	glGenVertexArrays ( 1 , &va_surface );
    glBindVertexArray ( va_surface );
	
	
		glGenBuffers ( 1 , &vb_vertices );
		glBindBuffer ( GL_ARRAY_BUFFER , vb_vertices );
			glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( slot_vertices );
        glVertexAttribPointer ( slot_vertices , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );
		
		
		glGenBuffers ( 1 , &vb_normals );
		glBindBuffer ( GL_ARRAY_BUFFER , vb_normals );
			glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( slot_normals );
        glVertexAttribPointer ( slot_normals , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );
		

		glGenBuffers ( 1 , &vb_colors );
		glBindBuffer ( GL_ARRAY_BUFFER , vb_colors );
		glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( slot_colors );
        glVertexAttribPointer ( slot_colors , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );
		
			
		glGenBuffers( 1, &vb_wireframes );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vb_wireframes );
			glBufferData( GL_ELEMENT_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW );
		

		glGenBuffers( 1, &vb_faces );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
			glBufferData( GL_ELEMENT_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW );
		
				
    glBindVertexArray ( 0 );
	
}



void Surface::loadBuffers()
{
	
    std::vector< float >& vertices = strat->getSurfaceVertices();
    std::vector< float > normals;
    std::vector< size_t > wireframes;// = strat->getSurfaceEdges();
    std::vector< size_t >& faces_size_t = strat->getSurfaceFaces();

    std::vector< unsigned int > faces;
    faces.assign( faces_size_t.begin(), faces_size_t.end() );


    if( vertices.empty() == true )
    {
        is_visible = false;
        return;
    }
    else
        is_visible = true;


    number_of_vertices = vertices.size()/3;

    glBindBuffer ( GL_ARRAY_BUFFER , vb_vertices );
    glBufferData ( GL_ARRAY_BUFFER , vertices.size() * sizeof ( float ) , vertices.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );


    for( size_t i = 0; i < number_of_vertices; ++i )
    {
        normals.push_back( 1.0f );
        normals.push_back( 0.0f );
        normals.push_back( 0.0f );
    }

    if( colors.size() == 3 )
    {

        for( size_t i = 1; i < number_of_vertices; ++i )
        {
            colors.push_back( colors[ 0 ] );
            colors.push_back( colors[ 1 ] );
            colors.push_back( colors[ 2 ] );
        }

    }

	if( colors.empty() == false )
	{

        int number_of_colors = colors.size()/3;


		glBindBuffer( GL_ARRAY_BUFFER, vb_colors );
		glBufferData( GL_ARRAY_BUFFER, colors.size() * sizeof ( GLfloat ), colors.data(), GL_STATIC_DRAW );
        glBindBuffer ( GL_ARRAY_BUFFER , 0 );
	}
	
	if( normals.empty() == false )
	{
		glBindBuffer ( GL_ARRAY_BUFFER , vb_normals );
		glBufferData ( GL_ARRAY_BUFFER , normals.size() * sizeof ( GLfloat )  , normals.data() , GL_STATIC_DRAW );
        glBindBuffer ( GL_ARRAY_BUFFER , 0 );
	}
	
    if( wireframes.empty() == false )
	{
		number_of_lines = (GLuint) wireframes.size();
		
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vb_wireframes );
        glBufferData ( GL_ELEMENT_ARRAY_BUFFER , wireframes.size() * sizeof ( GLuint )  , wireframes.data() , GL_STATIC_DRAW );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 );
		
	}
    
	if( faces.empty() == false )
	{
		number_of_faces = (GLuint) faces.size();
		
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vb_faces );
		glBufferData ( GL_ELEMENT_ARRAY_BUFFER , faces.size() * sizeof ( GLuint )  , faces.data() , GL_STATIC_DRAW );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 );
	
	}
	
		
}



void Surface::loadBuffers( const std::vector< float >& vertices )
{


    std::vector< float > normals;
    std::vector< size_t > wireframes;  // = strat->getSurfaceEdges();
    std::vector< size_t >& faces = strat->getSurfaceFaces();


    number_of_vertices = vertices.size()/3;



    glBindBuffer ( GL_ARRAY_BUFFER , vb_vertices );
    glBufferData ( GL_ARRAY_BUFFER , vertices.size() * sizeof ( float ) , vertices.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );


    for( size_t i = 0; i < number_of_vertices; ++i )
    {
        normals.push_back( 1.0f );
        normals.push_back( 0.0f );
        normals.push_back( 0.0f );
    }


    if( colors.size() == 3 )
    {

        for( size_t i = 1; i < number_of_vertices; ++i )
        {
            colors.push_back( colors[ 0 ] );
            colors.push_back( colors[ 1 ] );
            colors.push_back( colors[ 2 ] );
        }

    }

    if( colors.empty() == false )
    {
        glBindBuffer( GL_ARRAY_BUFFER, vb_colors );
        glBufferData( GL_ARRAY_BUFFER, colors.size() * sizeof ( GLfloat ), colors.data(), GL_STATIC_DRAW );
        glBindBuffer ( GL_ARRAY_BUFFER , 0 );
    }

    if( normals.empty() == false )
    {
        glBindBuffer ( GL_ARRAY_BUFFER , vb_normals );
        glBufferData ( GL_ARRAY_BUFFER , normals.size() * sizeof ( GLfloat )  , normals.data() , GL_STATIC_DRAW );
        glBindBuffer ( GL_ARRAY_BUFFER , 0 );
    }

    if( wireframes.empty() == false )
    {
        number_of_lines = (GLuint) wireframes.size();

        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vb_wireframes );
        glBufferData ( GL_ELEMENT_ARRAY_BUFFER , wireframes.size() * sizeof ( GLuint )  , wireframes.data() , GL_STATIC_DRAW );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 );

    }

    if( faces.empty() == false )
    {
        number_of_faces = (GLuint) faces.size();

        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vb_faces );
        glBufferData ( GL_ELEMENT_ARRAY_BUFFER , faces.size() * sizeof ( GLuint )  , faces.data() , GL_STATIC_DRAW );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 );

    }


}



void Surface::resetShaders()
{
	
	if ( shader_surface != nullptr )
    {
        delete( shader_surface );
        shader_surface = nullptr;
    }
	
}


void Surface::initShaders()
{
	
    shader_surface = new Tucano::Shader( "Surface", ( shader_directory + "shaders/Seismic.vert" ).toStdString(),
                                                    ( shader_directory + "shaders/Seismic.frag" ).toStdString(),
                                                    ( shader_directory + "shaders/Seismic.geom" ).toStdString(), "", "" ) ;


    shader_surface->initialize();
	
}


void Surface::reloadShaders()
{
    shader_surface->reloadShaders();
}



void Surface::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& width, const int& height )
{


    if( is_visible == false ) return;
	
	Eigen::Affine3f M;
    M.setIdentity();
	

    shader_surface->bind();


    shader_surface->setUniform( "ModelMatrix" , M );
    shader_surface->setUniform( "ViewMatrix" , V );
    shader_surface->setUniform( "ProjectionMatrix" , P );
    shader_surface->setUniform( "WIN_SCALE" , (float) width , (float) height );

    glPointSize( 10.0f );

    glBindVertexArray( va_surface );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
        glDrawElements ( GL_TRIANGLES , number_of_faces , GL_UNSIGNED_INT , 0 );

    glBindVertexArray ( 0 );


	
    shader_surface->unbind();


	
}


void Surface::update( const Eigen::Vector3f& c )
{

    loadBuffers();
}
