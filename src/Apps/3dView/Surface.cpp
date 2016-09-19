#include "Surface.h"

Surface::Surface()
{
    initData();
    init();
}



void Surface::init()
{
    initShaders();
    initBuffers();
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
        glEnableVertexAttribArray ( 0 );
        glVertexAttribPointer ( 0 , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );
		
		
		glGenBuffers ( 1 , &vb_normals );
		glBindBuffer ( GL_ARRAY_BUFFER , vb_normals );
			glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( 2 );
        glVertexAttribPointer ( 2 , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );
		

		glGenBuffers ( 1 , &vb_colors );
		glBindBuffer ( GL_ARRAY_BUFFER , vb_colors );
		glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( 1 );
        glVertexAttribPointer ( 1 , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );
		
			
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
	
    std::vector< float > vertices = strat->getSurfaceVertices();
    std::vector< float > colors;
    std::vector< float > normals;
    std::vector< unsigned int > wireframes = strat->getSurfaceEdges();
    std::vector< unsigned int > faces = strat->getSurfaceFaces();
		   

    number_of_vertices = vertices.size()/3;
    glBindBuffer ( GL_ARRAY_BUFFER , vb_vertices );
    glBufferData ( GL_ARRAY_BUFFER , vertices.size ( ) * sizeof ( GLfloat ) , vertices.data() , GL_STATIC_DRAW );
	

	if( colors.empty() == false )
	{
		glBindBuffer( GL_ARRAY_BUFFER, vb_colors );
		glBufferData( GL_ARRAY_BUFFER, colors.size() * sizeof ( GLfloat ), colors.data(), GL_STATIC_DRAW );
	}
	
	if( normals.empty() == false )
	{
		glBindBuffer ( GL_ARRAY_BUFFER , vb_normals );
		glBufferData ( GL_ARRAY_BUFFER , normals.size() * sizeof ( GLfloat )  , normals.data() , GL_STATIC_DRAW );
	}
	
    if( wireframes.empty() == false )
	{
		number_of_lines = (GLuint) wireframes.size();
		
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vb_wireframes );
        glBufferData ( GL_ELEMENT_ARRAY_BUFFER , wireframes.size() * sizeof ( GLuint )  , wireframes.data() , GL_STATIC_DRAW );
		
	}
    
	if( faces.empty() == false )
	{
		number_of_faces = (GLuint) faces.size();
		
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vb_faces );
		glBufferData ( GL_ELEMENT_ARRAY_BUFFER , faces.size() * sizeof ( GLuint )  , faces.data() , GL_STATIC_DRAW );
	
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
	
    shader_surface = new Tucano::Shader( "Surface", ( shader_directory + "Shaders/vertex_mesh_shader.vert" ).toStdString(),
                                                    ( shader_directory + "Shaders/fragment_mesh_shader.frag" ).toStdString(),
                                          "", "", "" ) ;

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
	

    float scale = 1.5*(float)width/(float)height;

    shader_surface->bind();


    shader_surface->bind();
    shader_surface->setUniform( "mmatrix", M );
    shader_surface->setUniform( "vmatrix", V );
    shader_surface->setUniform( "pmatrix", P );
    shader_surface->setUniform( "scale", scale );
	


    glPointSize( 5.0f );
    glBindVertexArray( va_surface );
	
        // not ideal, just adapting to current code
        if( number_of_faces > 0 )
        {
            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
            glDrawElements ( GL_TRIANGLES , number_of_faces , GL_UNSIGNED_INT , 0 );

        }
        else
        {
            glDrawArrays ( GL_LINES_ADJACENCY , 0 , number_of_lines );
        }
	
    glBindVertexArray ( 0 );
	
    shader_surface->unbind ( );	
	
}


void Surface::update()
{
    loadBuffers();
}
