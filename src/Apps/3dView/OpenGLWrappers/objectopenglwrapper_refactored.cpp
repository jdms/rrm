
#include "objectopenglwrapper_refactored.h"

#include "./Core/Geology/Models/object_refactored.h"



ObjectOpenGLWrapper_Refactored::ObjectOpenGLWrapper_Refactored()
{
    raw = nullptr;
    setDefaultValues();
}


void ObjectOpenGLWrapper_Refactored::setShaderDirectory( const std::string& dir )
{
    shader_directory = dir;
}


void ObjectOpenGLWrapper_Refactored::setRawObject( Object_Refactored* const& obj )
{
    raw = obj;
    reloadBuffers();
}


Object_Refactored* ObjectOpenGLWrapper_Refactored::getRawObject() const
{
    return raw;
}


void ObjectOpenGLWrapper_Refactored::setMinimum( float mx, float my, float mz )
{
    minimum = Eigen::Vector3f( mx, my, mz );
}


void ObjectOpenGLWrapper_Refactored::setMaximum( float mx, float my, float mz )
{
    maximum = Eigen::Vector3f( mx, my, mz );
}


bool ObjectOpenGLWrapper_Refactored::isVisible() const
{
    if( raw == nullptr || ( number_of_faces == 0 ) || (number_of_vertices == 0 ) ) return false;
    return raw->getVisibility();
}


void ObjectOpenGLWrapper_Refactored::initShaders()
{

    if( singlepass == true )
        shader = new Tucano::Shader( "Surface", ( shader_directory + "shaders/SurfaceSinglePassWireframe.vert" ),
                                                ( shader_directory + "shaders/SurfaceSinglePassWireframe.frag" ),
                                                ( shader_directory + "shaders/SurfaceSinglePassWireframe.geom" ), "", "" );

    else
        shader = new Tucano::Shader( "Surface", ( shader_directory + "shaders/gouraud_surface.vert" ),
                                                ( shader_directory + "shaders/gouraud_surface.frag" ),
                                                "", "", "" );

    shader->initialize();
}


void ObjectOpenGLWrapper_Refactored::initBuffers()
{
    glGenVertexArrays ( 1 , &va_object );
    glBindVertexArray ( va_object );


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


void ObjectOpenGLWrapper_Refactored::resetShaders()
{
    if ( shader != nullptr )
    {
        delete( shader );
        shader = nullptr;
    }
}


void ObjectOpenGLWrapper_Refactored::resetBuffers()
{
    if( va_object != 0 )
    {
        glDeleteVertexArrays( 1, &va_object );

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


void ObjectOpenGLWrapper_Refactored::reloadShaders()
{
    shader->reloadShaders();
}



void ObjectOpenGLWrapper_Refactored::reloadBuffers()
{
    std::vector< GLfloat > vertices = normalizedVertices();
    if( vertices.empty() == true ) return;


    std::vector< GLuint > faces( raw->getSurfaceFaces() );
    std::vector< double > normals_double( raw->getSurfaceNormals() );
    std::vector< GLfloat > normals( normals_double.begin(), normals_double.end() );

    reloadGeometry( vertices, normals, faces );


    int r, g, b;
    raw->getColor( r, g, b );

    GLfloat red = static_cast<GLfloat>( r/255.0f );
    GLfloat green = static_cast<GLfloat>( g/255.0f );
    GLfloat blue = static_cast<GLfloat>( b/255.0f );

    setConstantColor( red, green, blue );

}


std::vector< GLfloat > ObjectOpenGLWrapper_Refactored::normalizedVertices()
{
    std::vector< double > vertices = raw->getSurfaceVertices();

    Eigen::Affine3f matrix = Model3DUtils::normalizePointCloud( minimum.x(), maximum.x(), minimum.y(),
                                                                maximum.y(), minimum.z(), maximum.z() );

    std::vector< float > unit_vertices;
    unit_vertices.resize( vertices.size() );

    number_of_vertices = static_cast< GLuint >( vertices.size()/3 );

    for( size_t i = 0; i < number_of_vertices; ++i )
    {
        Eigen::Vector3f v( vertices[ 3*i ], vertices[ 3*i + 1 ], vertices[ 3*i + 2 ] );
        v = matrix*v;

        unit_vertices[ 3*i ] =  v.x();
        unit_vertices[ 3*i + 1 ] = v.y();
        unit_vertices[ 3*i + 2 ] = v.z();
    }

    return unit_vertices;
}


void ObjectOpenGLWrapper_Refactored::reloadGeometry( const std::vector< GLfloat >& vertices,
                                                     const std::vector< GLfloat >& normals,
                                                     const std::vector< GLuint >& faces )
{

    glBindBuffer ( GL_ARRAY_BUFFER, vb_vertices );
    glBufferData ( GL_ARRAY_BUFFER, vertices.size() * sizeof ( GLfloat ), vertices.data() ,
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER, 0 );


    glBindBuffer ( GL_ARRAY_BUFFER , vb_normals );
    glBufferData ( GL_ARRAY_BUFFER , normals.size() * sizeof ( GLfloat ), normals.data() ,
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );

    number_of_faces = faces.size();
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof ( GLuint ), faces.data(),
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
}



void ObjectOpenGLWrapper_Refactored::setConstantColor( float r, float g, float b )
{

    std::vector< GLfloat > colors;
    colors.resize( 3*number_of_vertices );

    for( std::size_t i = 0; i < number_of_vertices; ++i )
    {
        colors[ 3*i ]     = r;
        colors[ 3*i + 1 ] = g;
        colors[ 3*i + 2 ] = b;
    }

    reloadColors( colors );
}


void ObjectOpenGLWrapper_Refactored::reloadColors( const std::vector< GLfloat >& colors )
{
    glBindBuffer( GL_ARRAY_BUFFER, vb_colors );
    glBufferData( GL_ARRAY_BUFFER, colors.size() * sizeof ( GLfloat ), colors.data(),
                  GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );
}


void ObjectOpenGLWrapper_Refactored::init()
{
    initShaders();
    initBuffers();
}


void ObjectOpenGLWrapper_Refactored::reset()
{
    resetShaders();
    resetBuffers();

    init();
    reloadBuffers();
}



void ObjectOpenGLWrapper_Refactored::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                                           const int& h )
{
    if( isVisible() == false ) return;

    Eigen::Affine3f M;
    M.setIdentity();

    shader->bind();

    shader->setUniform( "ModelMatrix" , M );
    shader->setUniform( "ViewMatrix" , V );
    shader->setUniform( "ProjectionMatrix" , P );
    shader->setUniform( "WIN_SCALE" , (float) w , (float) h );

    if( singlepass == true )
        singlePassWireFrame();
    else
        rendering();

    shader->unbind();

}


void ObjectOpenGLWrapper_Refactored::singlePassWireFrame()
{


//    shader->setUniform( "phong" , false );
//    shader->setUniform( "wireframe", true );


    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glBindVertexArray( va_object );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
        glDrawElements ( GL_TRIANGLES , number_of_faces , GL_UNSIGNED_INT , 0 );

    glBindVertexArray ( 0 );

}



void ObjectOpenGLWrapper_Refactored::rendering()
{

    if( testing == true )
    {
        glEnable( GL_BLEND );
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
        glDisable( GL_BLEND );


        glEnable( GL_DEPTH_TEST );



        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glEnable( GL_POLYGON_OFFSET_FILL );
        glPolygonOffset( 2.0f, 1.0f );


        shader->setUniform( "solid" , true );
        shader->setUniform( "testing" , testing );


        glBindVertexArray( va_object );

            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
            glDrawElements ( GL_TRIANGLES , number_of_faces , GL_UNSIGNED_INT , 0 );

        glBindVertexArray ( 0 );


    glDisable(GL_POLYGON_OFFSET_FILL);



    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    shader->setUniform( "solid" , false );

    glBindVertexArray( va_object );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
        glDrawElements ( GL_TRIANGLES , number_of_faces , GL_UNSIGNED_INT , 0 );

    glBindVertexArray ( 0 );

    glDisable( GL_BLEND );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );


}

void ObjectOpenGLWrapper_Refactored::clear()
{
    clearData();
    setDefaultValues();
}


void ObjectOpenGLWrapper_Refactored::clearData()
{

    resetShaders();
    resetBuffers();

    if( raw != nullptr )
        raw->clear();
    raw = nullptr;

}


void ObjectOpenGLWrapper_Refactored::setDefaultValues()
{
    va_object = 0;
    vb_vertices = 0;
    vb_normals = 0;
    vb_colors = 0;
    vb_faces = 0;

    number_of_faces = 0;
    number_of_vertices = 0;

    minimum = Eigen::Vector3f( 0.0f, 0.0f, 0.0f );
    maximum = Eigen::Vector3f( 0.0f, 0.0f, 0.0f );

    singlepass = false;
    testing = false;

}
