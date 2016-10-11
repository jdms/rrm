#include "Surface.h"


Surface::Surface()
{
    is_initialized = false;
    id = 0;
}

Surface::Surface( int id_ )
{
    is_initialized = false;
    id = id_;
}


void Surface::init()
{
    initData();
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
	
    std::vector< float >& vertices = strat->getSurfaceVertices();
    std::vector< float > normals;
    std::vector< unsigned int > wireframes;// = strat->getSurfaceEdges();
    std::vector< unsigned int >& faces = strat->getSurfaceFaces();


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


    for( unsigned int i = 0; i < number_of_vertices; ++i )
    {
        normals.push_back( 1.0f );
        normals.push_back( 0.0f );
        normals.push_back( 0.0f );
    }

    if( colors.size() == 3 )
    {

        for( unsigned int i = 1; i < number_of_vertices; ++i )
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
    std::vector< unsigned int > wireframes;  // = strat->getSurfaceEdges();
    std::vector< unsigned int >& faces = strat->getSurfaceFaces();


    number_of_vertices = vertices.size()/3;



    glBindBuffer ( GL_ARRAY_BUFFER , vb_vertices );
    glBufferData ( GL_ARRAY_BUFFER , vertices.size() * sizeof ( float ) , vertices.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );


    for( unsigned int i = 0; i < number_of_vertices; ++i )
    {
        normals.push_back( 1.0f );
        normals.push_back( 0.0f );
        normals.push_back( 0.0f );
    }


    if( colors.size() == 3 )
    {

        for( unsigned int i = 1; i < number_of_vertices; ++i )
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
    M.translate( origin );
	

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

    origin = ( -1 )*c;
    loadBuffers();


}
