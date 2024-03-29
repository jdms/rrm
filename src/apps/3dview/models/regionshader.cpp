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
 * @file regionshader.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class RegionShader
 */

#include "regionshader.h"

RegionShader::RegionShader()
{
    setDefaultValues();
}


RegionShader::RegionShader( const std::shared_ptr< Regions >& raw_ )
{
    setDefaultValues();
    init();
    setRegion( raw_ );
}



void RegionShader::setRegion( const std::shared_ptr< Regions >& raw_ )
{
    raw = raw_;
    must_load_data = true;
    loadBuffers();
}


void RegionShader::loadBuffers()
{

    if (must_load_data || raw->geometryWasUpdatedSinceLastRead())
    {
        std::vector< double > vertices_double_;
        raw->getVertices( vertices_double_ );

        std::vector< float > vertices_ = Shader::convertToFloat( vertices_double_ );
        if( vertices_.empty() == true ) return;

        // getting the max and min points to normalize the vertices
        double maxx_ = 0, maxy_ = 0, maxz_ = 0, minx_ = 0, miny_ = 0, minz_ = 0;
        raw->getMaxMin( maxx_, maxy_, maxz_, minx_, miny_, minz_ );

        Eigen::Vector3f min( static_cast< float >( minx_ ), static_cast< float >( miny_ ), static_cast< float >( minz_ ) );
        Eigen::Vector3f max( static_cast< float >( maxx_ ), static_cast< float >( maxy_ ), static_cast< float >( maxz_ ) );
        vertices_ = Shader::normalize( vertices_, max, min, 3 );


        //getting the triangles that make the tetrahedrals cells
        std::vector< std::size_t > faces_size_t_;
        raw->getTriangleCells( faces_size_t_ );

        std::vector< GLuint > faces_ = Shader::convertToUnsignedInt( faces_size_t_ );
        std::vector< GLfloat > normals_;

        std::size_t nvertices = vertices_.size()/3;
        num_vertices = nvertices;

        //update opengl buffers
        updateGeometryBuffers( vertices_, normals_, faces_ );
    }

    if(must_load_data || raw->colorWasUpdatedSinceLastRead())
    {
        int r, g, b;
        raw->getColorFor3DView( r, g, b );

        //update opengl buffers
        updateColorBuffers( num_vertices, r, g, b );
    }

    must_load_data = false;
}



void RegionShader::initShaders()
{
    shader = new Tucano::Shader( "Surface", ( shader_directory + "shaders/mesh.vert" ),
                                            ( shader_directory + "shaders/mesh.frag" ),
                                            "", "", "" );

    shader->initialize();
}

void RegionShader::initBuffers()
{
    glGenVertexArrays ( 1 , &va_volume );
    glBindVertexArray ( va_volume );


        glGenBuffers ( 1 , &vb_vertices );
        glBindBuffer ( GL_ARRAY_BUFFER , vb_vertices );
        glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( 0 );
        glVertexAttribPointer ( 0 , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );


        glGenBuffers ( 1 , &vb_normals );
        glBindBuffer ( GL_ARRAY_BUFFER , vb_normals );
        glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( 1 );
        glVertexAttribPointer ( 1 , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );


        glGenBuffers ( 1 , &vb_colors );
        glBindBuffer ( GL_ARRAY_BUFFER , vb_colors );
        glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( 2 );
        glVertexAttribPointer ( 2 , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );


        glGenBuffers( 1, &vb_faces );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW );


    glBindVertexArray ( 0 );
}



void RegionShader::resetShaders()
{
    if ( shader == nullptr ) return;

    delete( shader );
    shader = nullptr;
}

void RegionShader::resetBuffers()
{

    if( va_volume != 0 )
    {
        glDeleteVertexArrays( 1, &va_volume );

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

        if ( vb_faces != 0 )
        {
            glDeleteBuffers( 1, &vb_faces );
        }
    }

}



void RegionShader::updateGeometryBuffers( const std::vector< GLfloat >& vertices_,
                                           const std::vector< GLfloat >& normals_,
                                           const std::vector< GLuint >& faces_ )
{
    glBindBuffer ( GL_ARRAY_BUFFER, vb_vertices );
    glBufferData ( GL_ARRAY_BUFFER, vertices_.size() * sizeof ( GLfloat ), vertices_.data() ,
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER, 0 );


    std::vector< GLfloat > normals_default_;
    normals_default_ = normals_;
    if( normals_default_.empty() == true )
        normals_default_ = getDefaultNormals( vertices_.size() );

    glBindBuffer ( GL_ARRAY_BUFFER , vb_normals );
    glBufferData ( GL_ARRAY_BUFFER , normals_default_.size() * sizeof ( GLfloat ), normals_default_.data() ,
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );

    number_of_faces = faces_.size();
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, faces_.size() * sizeof ( GLuint ), faces_.data(),
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void RegionShader::updateColorBuffers( const std::vector< GLfloat >& colors_ )
{
    glBindBuffer( GL_ARRAY_BUFFER, vb_colors );
    glBufferData( GL_ARRAY_BUFFER, colors_.size() * sizeof ( GLfloat ), colors_.data(),
                  GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );
}


void RegionShader::updateColorBuffers( std::size_t nvertices_, int r_, int g_, int b_ )
{
    std::vector< GLfloat > color_;
    color_.resize( 3*nvertices_ );

    for( std::size_t i = 0; i < nvertices_; ++i )
    {
        color_[ 3* i ] = static_cast<GLfloat>( r_/255.0f );
        color_[ 3* i + 1 ] = static_cast<GLfloat>( g_/255.0f );
        color_[ 3* i + 2 ] = static_cast<GLfloat>( b_/255.0f );
    }

    updateColorBuffers( color_ );
}



void RegionShader::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                          const int& h )
{

    if( raw->isVisible() == false ) return;

    Eigen::Affine3f M;
    M.setIdentity();

    shader->bind();

        shader->setUniform( "ModelMatrix" , M );
        shader->setUniform( "ViewMatrix" , V );
        shader->setUniform( "ProjectionMatrix" , P );
        shader->setUniform( "WIN_SCALE" , (float) w , (float) h );

        glEnable( GL_DEPTH_TEST );

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

        glEnable( GL_POLYGON_OFFSET_FILL );
        glPolygonOffset( 2.0f, 1.0f );


            glBindVertexArray( va_volume );

                shader->setUniform( "is_face" , true );
                shader->setUniform( "is_preview" , false );
                shader->setUniform( "has_shading" , false );

                glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
                glDrawElements ( GL_TRIANGLES , number_of_faces , GL_UNSIGNED_INT , 0 );

            glBindVertexArray ( 0 );


        glDisable(GL_POLYGON_OFFSET_FILL);

        glDisable( GL_DEPTH_TEST );



    shader->unbind();

}



void RegionShader::update()
{
    loadBuffers();
}


void RegionShader::clear()
{
    reset();
    setDefaultValues();
}


void RegionShader::setDefaultValues()
{
    va_volume = 0;
    vb_vertices = 0;
    vb_normals = 0;
    vb_colors = 0;
    vb_faces = 0;

    number_of_faces = 0;
    number_of_vertices = 0;

    raw.reset();

}

RegionShader::~RegionShader()
{
    clear();
}
