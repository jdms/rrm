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
 * @file surfaceshader.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class SurfaceShader
 */

#include "surfaceshader.h"
//#include "TriMesh.h"

SurfaceShader::SurfaceShader()
{
    setDefaultValues();
}


SurfaceShader::SurfaceShader( const std::shared_ptr< Stratigraphy >& raw_ )
{
    setDefaultValues();
    init();
    setObject( raw_ );
}



void SurfaceShader::setObject( const std::shared_ptr< Stratigraphy >& raw_ )
{
    raw = raw_;
    loadBuffers();
}


void SurfaceShader::loadBuffers()
{

    Surface surface_ = raw->getSurface();

    std::vector< float > vertices_ = Shader::convertToFloat( surface_.getVertices() );
    if( vertices_.empty() == true )
    {
        return;
    }


    double maxx_ = 0, maxy_ = 0, maxz_ = 0, minx_ = 0, miny_ = 0, minz_ = 0;
    raw->getBoundingBox( minx_, maxx_, miny_, maxy_, minz_, maxz_ );

    // getting the max and min points to normalize the vertices
    Eigen::Vector3f min( static_cast< float >( minx_ ), static_cast< float >( miny_ ), static_cast< float >( minz_ ) );
    Eigen::Vector3f max( static_cast< float >( maxx_ ), static_cast< float >( maxy_ ), static_cast< float >( maxz_ ) );
    vertices_ = Shader::normalize( vertices_, max, min, 3 );
    std::vector< GLuint > faces_ = Shader::convertToUnsignedInt( surface_.getFaces() );
    std::vector< GLfloat > normals_ = Shader::convertToFloat( surface_.getNormals() );

    int r, g, b;
    raw->getColor( r, g, b );

    std::size_t nvertices = vertices_.size()/3;

    //uploading the vertices and colors buffers
    updateGeometryBuffers( vertices_, normals_, faces_ );
    updateColorBuffers( nvertices, r, g, b );
}


void SurfaceShader::initShaders()
{
    shader = new Tucano::Shader( "Surface", ( shader_directory + "shaders/mesh.vert" ),
                                 ( shader_directory + "shaders/mesh.frag" ),
                                 "", "", "" );

    shader->initialize();

    shader_new = new Tucano::Shader( "Surface New", ( shader_directory + "shaders/SurfaceDefault.vert" ),
                                     ( shader_directory + "shaders/SurfaceDefault.frag" ),
                                     ( shader_directory + "shaders/SurfaceDefault.geom" ),
                                     "", "" );

    shader_new->initialize();

}


void SurfaceShader::initBuffers()
{
    glGenVertexArrays ( 1 , &va_surface );
    glBindVertexArray ( va_surface );


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



void SurfaceShader::resetShaders()
{
    if ( shader == nullptr ) return;

    delete( shader );
    shader = nullptr;
}


void SurfaceShader::resetBuffers()
{

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

        if ( vb_faces != 0 )
        {
            glDeleteBuffers( 1, &vb_faces );
        }
    }

}


void SurfaceShader::updateGeometryBuffers( const std::vector< GLfloat >& vertices_,
                                           const std::vector< GLfloat >& normals_,
                                           const std::vector< GLuint >& faces_ )
{
    glBindBuffer ( GL_ARRAY_BUFFER, vb_vertices );
    glBufferData ( GL_ARRAY_BUFFER, vertices_.size() * sizeof ( GLfloat ), vertices_.data() ,
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER, 0 );

    glBindBuffer ( GL_ARRAY_BUFFER , vb_normals );
    glBufferData ( GL_ARRAY_BUFFER , normals_.size() * sizeof ( GLfloat ), normals_.data() ,
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );

    number_of_faces = faces_.size();
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, faces_.size() * sizeof ( GLuint ), faces_.data(),
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
}


void SurfaceShader::updateColorBuffers( const std::vector< GLfloat >& colors_ )
{

    GLfloat r_ = colors_[ 0 ];
    GLfloat g_ = colors_[ 1 ];
    GLfloat b_ = colors_[ 2 ];

    glBindBuffer( GL_ARRAY_BUFFER, vb_colors );
    glBufferData( GL_ARRAY_BUFFER, colors_.size() * sizeof ( GLfloat ), colors_.data(),
                  GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );
}


void SurfaceShader::updateColorBuffers( std::size_t nvertices_, int r_, int g_, int b_ )
{
    std::vector< GLfloat > color_;
    color_.resize( 3*nvertices_ );

    // converting colors from int to GLfloat
    for( std::size_t i = 0; i < nvertices_; ++i )
    {
        color_[ 3* i ] = static_cast<GLfloat>( r_/255.0f );
        color_[ 3* i + 1 ] = static_cast<GLfloat>( g_/255.0f );
        color_[ 3* i + 2 ] = static_cast<GLfloat>( b_/255.0f );
    }


    updateColorBuffers( color_ );
}


void SurfaceShader::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                          const int& h )
{

    // if the object is not active neither visible it shouldn't be rendered
    if( raw->isActive() == false ) return;
    if( raw->isVisible() == false ) return;


    bool is_preview_ = !raw->isDone();

    Eigen::Affine3f M;
    M.setIdentity();

    glEnable(GL_DEPTH_TEST);
    glEnable( GL_BLEND );
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_new->bind();
    shader_new->setUniform("ModelMatrix", M);
    shader_new->setUniform("ViewMatrix", V);
    shader_new->setUniform("ProjectionMatrix", P);
    shader_new->setUniform("viewportSize", static_cast<float>(w), static_cast<float>(h));

    // if the object is the current, a preview surface is created with a transparent color
    if (is_preview_)
    {
        shader_new->setUniform("alfa",0.5f);
    }else
    {
        shader_new->setUniform("alfa", 1.0f);
    }

    glBindVertexArray( va_surface );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
    glDrawElements ( GL_TRIANGLES , number_of_faces , GL_UNSIGNED_INT , 0 );

    glBindVertexArray ( 0 );

    shader_new->unbind();

    glDisable(GL_BLEND);
}



void SurfaceShader::update()
{
    loadBuffers();
}


void SurfaceShader::clear()
{
    reset();
    setDefaultValues();
}


void SurfaceShader::setDefaultValues()
{
    va_surface = 0;
    vb_vertices = 0;
    vb_normals = 0;
    vb_colors = 0;
    vb_faces = 0;

    number_of_faces = 0;
    number_of_vertices = 0;

    raw.reset();
    draw_edge = true;

}


void SurfaceShader::setHeightMap(double zmin_, double zmax_)
{
    draw_edge = false;

    Surface surface_ = raw->getSurface();

    std::vector< double > coords_ = surface_.getVertices();
    std::vector< float > zcoords_;
    for( std::size_t i = 0; i < coords_.size()/3; ++i )
    {
        zcoords_.push_back( static_cast< float >( coords_[ 3*i + 1 ] ) );
    }

    std::vector< float > colors_ = Shader::getHeightMapColor( zmin_, zmax_, zcoords_ );

    updateColorBuffers( colors_ );
}


void SurfaceShader::enableDrawingEdges()
{
    draw_edge = true;
}

SurfaceShader::~SurfaceShader()
{
    clear();
}
