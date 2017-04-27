#include "ObjectOpenGLWrapper.h"

ObjectOpenGLWrapper::ObjectOpenGLWrapper()
{
    object = nullptr;
    shader = nullptr;

    va_object = 0;
    vb_vertices = 0;
    vb_normals = 0;
    vb_colors = 0;
    vb_faces = 0;

    nvertices = 0;
    nfaces = 0;

}


void ObjectOpenGLWrapper::initShaders()
{
    shader = new Tucano::Shader( "Surface", ( shader_directory + "shaders/Seismic.vert" ),
                                            ( shader_directory + "shaders/Seismic.frag" ),
                                            ( shader_directory + "shaders/Seismic.geom" ), "", "" );


    shader->initialize();
}

void ObjectOpenGLWrapper::initBuffers()
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



void ObjectOpenGLWrapper::reloadVertices( const std::vector< double >& vertices_ )
{

    nvertices = vertices_.size()/NCOORD;

    Eigen::Affine3f matrix = Model3DUtils::normalizePointCloud( minx, maxx, miny, maxy, minz, maxz );
    std::vector< float > unit_vertices_;

    for( size_t i = 0; i < nvertices; ++i )
    {
        Eigen::Vector3f v( vertices_[ 3*i ], vertices_[ 3*i + 1 ], vertices_[ 3*i + 2 ] );
        v = matrix*v;
        unit_vertices_.push_back( v.x() );
        unit_vertices_.push_back( v.y() );
        unit_vertices_.push_back( v.z() );
    }


    glBindBuffer ( GL_ARRAY_BUFFER, vb_vertices );
    glBufferData ( GL_ARRAY_BUFFER, unit_vertices_.size() * sizeof ( float ), unit_vertices_.data() ,
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER, 0 );


    //TODO: fix so that whenever initialize, it initializes with normal and colour
    std::vector< float > normals;
    for( size_t i = 0; i < nvertices; ++i )
    {
        normals.push_back( 1.0f );
        normals.push_back( 0.0f );
        normals.push_back( 0.0f );
    }

    glBindBuffer ( GL_ARRAY_BUFFER , vb_normals );
    glBufferData ( GL_ARRAY_BUFFER , normals.size() * sizeof ( GLfloat )  , normals.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );

}


void ObjectOpenGLWrapper::reloadFaces( const std::vector< std::size_t >& faces_sizet_ )
{
    nfaces = faces_sizet_.size();
    if( faces_sizet_.empty() == true ) return;

    std::vector< unsigned int > faces_;
    faces_.assign( faces_sizet_.begin(), faces_sizet_.end() );

    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, faces_.size() * sizeof ( GLuint ), faces_.data(),
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
}


void ObjectOpenGLWrapper::reloadColors( std::vector<float> &colors_ )
{
    if( colors_.empty() == true ){

        colors_.resize( 3*nvertices );
        for( std::size_t i = 0; i < nvertices; ++i )
        {
            colors_[ 3*i ] = current_color.red;
            colors_[ 3*i + 1 ] = current_color.green;
            colors_[ 3*i + 2 ] = current_color.blue;
        }
    }

    glBindBuffer( GL_ARRAY_BUFFER, vb_colors );
    glBufferData( GL_ARRAY_BUFFER, colors_.size() * sizeof ( GLfloat ), colors_.data(),
                  GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );
}



void ObjectOpenGLWrapper::resetShaders()
{
    if ( shader != nullptr )
    {
        delete( shader );
        shader = nullptr;
    }
}

void ObjectOpenGLWrapper::resetBuffers()
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

    va_object = 0;
    vb_vertices = 0;
    vb_normals = 0;
    vb_colors = 0;
    vb_faces = 0;

    nvertices = 0;
    nfaces = 0;
}



void ObjectOpenGLWrapper::draw( const Eigen::Affine3f& V_, const Eigen::Matrix4f& P_, const int& w_,
                                                                                   const int& h_ )
{

    if( nfaces == 0 ) return;

    Eigen::Affine3f M_;
    M_.setIdentity();

    shader->bind();

    shader->setUniform( "ModelMatrix" , M_ );
    shader->setUniform( "ViewMatrix" , V_ );
    shader->setUniform( "ProjectionMatrix" , P_ );
    shader->setUniform( "WIN_SCALE" , (float) w_ , (float) h_ );

        glBindVertexArray( va_object );

            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
            glDrawElements ( GL_TRIANGLES , nfaces , GL_UNSIGNED_INT , 0 );

        glBindVertexArray ( 0 );

    shader->unbind();

}

