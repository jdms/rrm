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

#include "GradientBackgroundShader.hpp"



GradientBackgroundShader::GradientBackgroundShader()
{

}



void GradientBackgroundShader::init()
{
	initBuffers();
	initShaders();
}


void GradientBackgroundShader::resetBuffers()
{
    if( va_background != 0 )
    {
        glDeleteVertexArrays( 1, &va_background );

        if ( vb_background != 0 )
        {
            glDeleteBuffers( 1, &vb_background );
        }
    }

    va_background = 0;
    vb_background = 0;

    init();
}


void GradientBackgroundShader::initBuffers()
{
    glGenVertexArrays ( 1 , &va_background );
    glBindVertexArray ( va_background );

    glGenBuffers ( 1 , &vb_background );
    glBindBuffer ( GL_ARRAY_BUFFER , vb_background );
    glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
    glEnableVertexAttribArray ( 0 );
    glVertexAttribPointer ( 0 , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

    glBindVertexArray ( 0 );

}	





void GradientBackgroundShader::resetShaders()
{

    if ( shader_background != nullptr )
    {
        delete( shader_background );
        shader_background = nullptr;
    }

}

void GradientBackgroundShader::initShaders()
{

    shader_background = new Tucano::Shader( "Background", ( shader_directory + "shaders/DummyQuad.vert" ).toStdString(), ( shader_directory + "shaders/DummyQuad.frag").toStdString(),
                                            ( shader_directory + "shaders/DummyQuad.geom" ).toStdString(), "", "" );


    shader_background->initialize ( );


    glGenVertexArrays ( 1 , &va_background );
    glBindVertexArray ( va_background );
    glGenBuffers ( 1 , &vb_background );
    glBindBuffer ( GL_ARRAY_BUFFER , vb_background );
    glBufferData ( GL_ARRAY_BUFFER , 0 , 0 , GL_STATIC_DRAW );

    glEnableVertexAttribArray ( 0 );
    glVertexAttribPointer ( 0 , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

    glBindVertexArray ( 0 );

}

void GradientBackgroundShader::reloadShaders()
{
        shader_background->reloadShaders();
}


void GradientBackgroundShader::draw( const int& width, const int& height )
{

    glDisable( GL_DEPTH_TEST );
    glDepthMask( GL_FALSE );

    shader_background->bind();

    shader_background->setUniform( "viewportSize", width, height );

    glBindVertexArray ( va_background );
    glDrawArrays ( GL_POINTS , 0 , 1 );
    glBindVertexArray ( 0 );

    shader_background->unbind();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);


}

