#include "CrossPlane.h"

CrossPlane::CrossPlane()
{
	slot_vertices = 0;
}


void CrossPlane::resetBuffers()
{
	
	if( va_plane != 0 )
	{
        glDeleteVertexArrays( 1, &va_plane );
		
        if ( vb_vertices != 0 )
        {
            glDeleteBuffers( 1, &vb_vertices );
        }
    }
		
	va_plane = 0;	
	vb_vertices = 0;
	
}


void CrossPlane::initBuffers()
{
	
	glGenVertexArrays ( 1 , &va_plane );
    glBindVertexArray ( va_plane );
	
		glGenBuffers ( 1 , &vb_vertices );
		glBindBuffer ( GL_ARRAY_BUFFER , vb_vertices );
			glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
		glEnableVertexAttribArray ( slot_vertices );
		glVertexAttribPointer ( slot_vertices , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );
		
    glBindVertexArray ( 0 );
	
}



void CrossPlane::loadBuffers()
{
	
	std::vector< float > wireframe;
    number_of_lines = (GLuint) wireframe.size();
	
    glBindBuffer ( GL_ARRAY_BUFFER , vb_vertices );
    glBufferData ( GL_ARRAY_BUFFER , wireframe.size() * sizeof ( GLfloat ) , wireframe.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );
		
}


void CrossPlane::resetShaders()
{
	
	if ( shader_plane != nullptr )
    {
        delete( shader_plane );
        shader_plane = nullptr;
    }
	
}


void CrossPlane::initShaders()
{
	
    shader_plane = new Tucano::Shader( "CrossPlane", ( shader_directory + "Shaders/BlankScreenCube.vert" ).toStdString(), ( shader_directory + "Shaders/BlankScreenCube.frag").toStdString(),
                                                     ( shader_directory + "Shaders/BlankScreenCube.geom" ).toStdString(), "", "" );
    shader_plane->initialize();
	
}


void CrossPlane::reloadShaders()
{
    shader_plane->reloadShaders();
}



void CrossPlane::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& width, const int& height )
{
	
	Eigen::Affine3f M;
    M.setIdentity();
	
    shader_plane->bind ( );
    
	shader_plane->setUniform ( "ModelMatrix" , M );
    shader_plane->setUniform ( "ViewMatrix" , V );
    shader_plane->setUniform ( "ProjectionMatrix" , P );
    shader_plane->setUniform ( "WIN_SCALE" , (float) width, (float) height );
	
    glBindVertexArray ( va_plane );
        glDrawArrays ( GL_LINES_ADJACENCY , 0 , number_of_lines );
    glBindVertexArray ( 0 );
	
    shader_plane->unbind ( );	
	
}
