#include "BoundingBox3D.h"

BoundingBox3D::BoundingBox3D()
{
	slot_vertices = 0;
}


void BoundingBox3D::resetBuffers()
{
	
	if( va_boundingbox != 0 )
	{
        glDeleteVertexArrays( 1, &va_boundingbox );
		
        if ( vb_vertices != 0 )
        {
            glDeleteBuffers( 1, &vb_vertices );
        }
    }
		
	va_boundingbox = 0;	
	vb_vertices = 0;
	
}


void BoundingBox3D::initBuffers()
{
	
	glGenVertexArrays ( 1 , &va_boundingbox );
    glBindVertexArray ( va_boundingbox );
	
		glGenBuffers ( 1 , &vb_vertices );
		glBindBuffer ( GL_ARRAY_BUFFER , vb_vertices );
			glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
		glEnableVertexAttribArray ( slot_vertices );
		glVertexAttribPointer ( slot_vertices , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );
		
    glBindVertexArray ( 0 );
	
}



void BoundingBox3D::loadBuffers()
{
	
	std::vector< float > wireframe;
    number_of_lines = (GLuint) wireframe.size();
	
    glBindBuffer ( GL_ARRAY_BUFFER , vb_vertices );
    glBufferData ( GL_ARRAY_BUFFER , wireframe.size() * sizeof ( GLfloat ) , wireframe.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );
		
}


void BoundingBox3D::resetShaders()
{
	
	if ( shader_boundingbox != nullptr )
    {
        delete( shader_boundingbox );
        shader_boundingbox = nullptr;
    }
	
}


void BoundingBox3D::initShaders()
{
	
    shader_boundingbox = new Tucano::Shader( "BoundingBox3D", ( shader_directory + "Shaders/BlankScreenCube.vert" ).toStdString(), ( shader_directory + "Shaders/BlankScreenCube.frag").toStdString(),
                                                     ( shader_directory + "Shaders/BlankScreenCube.geom" ).toStdString(), "", "" );
    shader_boundingbox->initialize();
	
}


void BoundingBox3D::reloadShaders()
{
    shader_boundingbox->reloadShaders();
}



void BoundingBox3D::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& width, const int& height )
{
	
	Eigen::Affine3f M;
    M.setIdentity();
	
    shader_boundingbox->bind ( );
    
	shader_boundingbox->setUniform ( "ModelMatrix" , M );
    shader_boundingbox->setUniform ( "ViewMatrix" , V );
    shader_boundingbox->setUniform ( "ProjectionMatrix" , P );
    shader_boundingbox->setUniform ( "WIN_SCALE" , (float) width, (float) height );
	
    glBindVertexArray ( va_boundingbox );
        glDrawArrays ( GL_LINES_ADJACENCY , 0 , number_of_lines );
    glBindVertexArray ( 0 );
	
    shader_boundingbox->unbind ( );	
	
}
