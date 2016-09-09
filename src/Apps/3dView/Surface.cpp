#include "Surface.h"

Surface::Surface()
{
	slot_vertices = 0;
	slot_normals = 1 ;
	slot_colors = 2;	
}


void Surface::resetBuffers()
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
	
    std::vector< float > vertices = strat->getSurfaceVerticestoOpenGL();
    std::vector< float > colors;
    std::vector< float > normals;
    std::vector< unsigned int > wireframes = strat->getSurfaceEdges();
    std::vector< unsigned int > faces = strat->getSurfaceFaces();
		   

	number_of_vertices = vertices.size();
    glBindBuffer ( GL_ARRAY_BUFFER , vb_vertices );
    glBufferData ( GL_ARRAY_BUFFER , vertices.size ( ) * sizeof ( GLfloat ) , vertices.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );
	

	if( colors.empty() == false )
	{
		glBindBuffer( GL_ARRAY_BUFFER, vb_colors );
		glBufferData( GL_ARRAY_BUFFER, colors.size() * sizeof ( GLfloat ), colors.data(), GL_STATIC_DRAW );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
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
	
    shader_surface = new Tucano::Shader( "Surface", ( shader_directory + "Shaders/BlankScreenCube.vert" ).toStdString(), ( shader_directory + "Shaders/BlankScreenCube.frag").toStdString(),
                                                     ( shader_directory + "Shaders/BlankScreenCube.geom" ).toStdString(), "", "" );
    shader_surface->initialize();
	
}


void Surface::reloadShaders()
{
    shader_surface->reloadShaders();
}



void Surface::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& width, const int& height )
{
	
	Eigen::Affine3f M;
    M.setIdentity();
	
    shader_surface->bind ( );
    
	shader_surface->setUniform ( "ModelMatrix" , M );
    shader_surface->setUniform ( "ViewMatrix" , V );
    shader_surface->setUniform ( "ProjectionMatrix" , P );
    shader_surface->setUniform ( "WIN_SCALE" , (float) width, (float) height );
	
	
	// not ideal, just adapting to current code
    if( number_of_faces > 0 )
	{
		glDrawElements ( GL_TRIANGLES , number_of_faces , GL_UNSIGNED_INT , 0 );
	}
	else 
	{
		glDrawArrays ( GL_LINES_ADJACENCY , 0 , number_of_lines );
	}
	
    glBindVertexArray ( 0 );
	
    shader_surface->unbind ( );	
	
}
