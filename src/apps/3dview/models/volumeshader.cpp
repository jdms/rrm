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



#include "volumeshader.h"


VolumeShader::VolumeShader()
{
    setDefaultValues();
}



VolumeShader::VolumeShader( const std::shared_ptr< Volume >& raw_ )
{
    setDefaultValues();
    init();
    setVolume( raw_ );
}

void VolumeShader::setVolume( const std::shared_ptr< Volume >& raw_ )
{
    raw = raw_;
    if( raw->isEmpty() == true)
        createVolumeMesh();
}



void VolumeShader::createVolumeMesh()
{
    double ox = 0.0, oy = 0.0, oz = 0.0;
    raw->getOrigin( ox, oy, oz );

    float minx = static_cast< GLfloat > ( ox );
    float miny = static_cast< GLfloat > ( oy );
    float minz = static_cast< GLfloat > ( oz );

    float maxx = static_cast< GLfloat > ( ox + raw->getWidth() );
    float maxy = static_cast< GLfloat > ( oy + raw->getHeight() );
    float maxz = static_cast< GLfloat > ( oz + raw->getLenght() );


    Eigen::Vector3f min_real( minx, miny, minz );
    Eigen::Vector3f max_real( maxx, maxy, maxz );

    Eigen::Vector3f minimum = Shader::normalize( min_real, max_real, min_real );
    Eigen::Vector3f maximum = Shader::normalize( max_real, max_real, min_real );


    Eigen::Vector3f A( minimum.x(), minimum.y(), maximum.z() );
    Eigen::Vector3f B( maximum.x(), minimum.y(), maximum.z() );
    Eigen::Vector3f C( maximum.x(), maximum.y(), maximum.z() );
    Eigen::Vector3f D( minimum.x(), maximum.y(), maximum.z() );
    Eigen::Vector3f E( maximum.x(), minimum.y(), minimum.z() );
    Eigen::Vector3f F( minimum.x(), minimum.y(), minimum.z() );
    Eigen::Vector3f G( minimum.x(), maximum.y(), minimum.z() );
    Eigen::Vector3f H( maximum.x(), maximum.y(), minimum.z() );

    // since the volume is render only as a box, the code belows determines the vertices of each box face
    std::vector< float > wireframe =
    {
        maximum.x(), maximum.y(), maximum.z(),1.0,
        minimum.x(), maximum.y(), maximum.z(),1.0,
        maximum.x(), maximum.y(), minimum.z(),1.0,
        minimum.x(), maximum.y(), minimum.z(),1.0,
        // Bottom Face
        maximum.x(), minimum.y(), maximum.z(),1.0,
        minimum.x(), minimum.y(), maximum.z(),1.0,
        maximum.x(), minimum.y(), minimum.z(),1.0,
        minimum.x(), minimum.y(), minimum.z(),1.0,
        // Front Face
        maximum.x(), maximum.y(), maximum.z(),1.0,
        minimum.x(), maximum.y(), maximum.z(),1.0,
        maximum.x(), minimum.y(), maximum.z(),1.0,
        minimum.x(), minimum.y(), maximum.z(),1.0,
        // Back Face
        maximum.x(), maximum.y(), minimum.z(),1.0,
        minimum.x(), maximum.y(), minimum.z(),1.0,
        maximum.x(), minimum.y(), minimum.z(),1.0,
        minimum.x(), minimum.y(), minimum.z(),1.0,
        // Left Face
        maximum.x(), maximum.y(), minimum.z(),1.0,
        maximum.x(), maximum.y(), maximum.z(),1.0,
        maximum.x(), minimum.y(), minimum.z(),1.0,
        maximum.x(), minimum.y(), maximum.z(),1.0,
        // Right Face
        minimum.x(), maximum.y(), maximum.z(),1.0,
        minimum.x(), maximum.y(), minimum.z(),1.0,
        minimum.x(), minimum.y(), maximum.z(),1.0,
        minimum.x(), minimum.y(), minimum.z(),1.0
    };

    // load the buffers with a temporary normal
    updateGeometryBuffers( wireframe, defineVolumeNormals() );
}





std::vector< float > VolumeShader::defineVolumeNormals() const
{

    // A, B ... H are the vertices of the box
    // this method needs to be improve
    Eigen::Vector3f normal_A( -1.0f, -1.0f,  1.0f );
    Eigen::Vector3f normal_B(  1.0f, -1.0f,  1.0f );
    Eigen::Vector3f normal_C(  1.0f,  1.0f,  1.0f );
    Eigen::Vector3f normal_D( -1.0f,  1.0f,  1.0f );
    Eigen::Vector3f normal_E(  1.0f, -1.0f, -1.0f );
    Eigen::Vector3f normal_F( -1.0f, -1.0f, -1.0f );
    Eigen::Vector3f normal_G( -1.0f,  1.0f, -1.0f );
    Eigen::Vector3f normal_H(  1.0f,  1.0f, -1.0f );

    normal_A /= 3;
    normal_B /= 3;
    normal_C /= 3;
    normal_D /= 3;
    normal_E /= 3;
    normal_F /= 3;
    normal_G /= 3;
    normal_H /= 3;

    normal_A.normalize();
    normal_B.normalize();
    normal_C.normalize();
    normal_D.normalize();
    normal_E.normalize();
    normal_F.normalize();
    normal_G.normalize();
    normal_H.normalize();


    std::vector< float > normals =
    {
        normal_A.x(), normal_A.y(), normal_A.z(),
        normal_B.x(), normal_B.y(), normal_B.z(),
        normal_D.x(), normal_D.y(), normal_D.z(),
        normal_C.x(), normal_C.y(), normal_C.z(),

        normal_B.x(), normal_B.y(), normal_B.z(),
        normal_E.x(), normal_E.y(), normal_E.z(),
        normal_C.x(), normal_C.y(), normal_C.z(),
        normal_H.x(), normal_H.y(), normal_H.z(),

        normal_E.x(), normal_E.y(), normal_E.z(),
        normal_F.x(), normal_F.y(), normal_F.z(),
        normal_H.x(), normal_H.y(), normal_H.z(),
        normal_G.x(), normal_G.y(), normal_G.z(),

        normal_F.x(), normal_F.y(), normal_F.z(),
        normal_A.x(), normal_A.y(), normal_A.z(),
        normal_G.x(), normal_G.y(), normal_G.z(),
        normal_D.x(), normal_D.y(), normal_D.z(),

        normal_D.x(), normal_D.y(), normal_D.z(),
        normal_C.x(), normal_C.y(), normal_C.z(),
        normal_G.x(), normal_G.y(), normal_G.z(),
        normal_H.x(), normal_H.y(), normal_H.z(),

        normal_F.x(), normal_F.y(), normal_F.z(),
        normal_E.x(), normal_E.y(), normal_E.z(),
        normal_A.x(), normal_A.y(), normal_A.z(),
        normal_B.x(), normal_B.y(), normal_B.z()
    };

    return normals;
}




void VolumeShader::initShaders()
{
    shader = new Tucano::Shader( "VolumeOpenGLWrap", ( shader_directory + "shaders/CubeSinglePassWireframe.vert" ), ( shader_directory + "shaders/CubeSinglePassWireframe.frag"),
                                             ( shader_directory + "shaders/CubeSinglePassWireframe.geom" ), "", "" );
    shader->initialize();
}


void VolumeShader::initBuffers()
{
    glGenVertexArrays( 1 , &va_volume );
    glBindVertexArray( va_volume );

        glGenBuffers( 1 , &vb_vertices );
        glBindBuffer( GL_ARRAY_BUFFER , vb_vertices );
        glBufferData( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0 , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );


        glGenBuffers ( 1 , &vb_normals );
        glBindBuffer ( GL_ARRAY_BUFFER , vb_normals );
        glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( 1 );
        glVertexAttribPointer ( 1 , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

    glBindVertexArray( 0 );
}



void VolumeShader::resetShaders()
{
    if ( shader == nullptr ) return;

    delete( shader );
    shader = nullptr;
}


void VolumeShader::resetBuffers()
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
    }
}




void VolumeShader::updateGeometryBuffers( const std::vector< GLfloat >& vertices_,
                                          const std::vector< GLfloat >& normals_ )
{
    number_of_vertices =  static_cast< GLuint > (vertices_.size()/4);

    glBindBuffer ( GL_ARRAY_BUFFER , vb_vertices );
    glBufferData ( GL_ARRAY_BUFFER , vertices_.size() * sizeof ( GLfloat ) ,
                                     vertices_.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );


    glBindBuffer( GL_ARRAY_BUFFER , vb_normals );
    glBufferData( GL_ARRAY_BUFFER , normals_.size() * sizeof ( GLfloat )  , normals_.data() ,
                  GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER , 0 );

}


void VolumeShader::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                         const int& h )
{
    if( raw->isVisible() == false ) return;


    Eigen::Affine3f M;
    M.setIdentity();

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glDisable( GL_DEPTH_TEST );

        shader->bind();

            shader->setUniform( "ModelMatrix", M );
            shader->setUniform( "ViewMatrix",  V );
            shader->setUniform( "ProjectionMatrix", P );
            shader->setUniform( "WIN_SCALE", (float) w, (float) h );
            shader->setUniform ( "color_plane" , 0.5f, 0.5f, 0.5f, 0.2f );

            glBindVertexArray( va_volume );
                glDrawArrays( GL_LINES_ADJACENCY , 0 , number_of_vertices );
            glBindVertexArray( 0 );

        shader->unbind();

    glDisable( GL_BLEND );

}


void VolumeShader::clear()
{
    reset();
    setDefaultValues();
}



void VolumeShader::setDefaultValues()
{
    va_volume = 0;
    vb_vertices = 0;
    vb_normals = 0;

    number_of_vertices = 0;
    raw.reset();
}

void VolumeShader::update()
{
    createVolumeMesh();
}

VolumeShader::~VolumeShader()
{
    clear();
}
