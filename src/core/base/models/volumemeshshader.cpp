#include "volumemeshshader.h"

VolumeMeshShader::VolumeMeshShader()
{
    setDefaultValues();
}


VolumeMeshShader::VolumeMeshShader( Region* const& raw_ )
{
    setDefaultValues();
    init();
    setRegion( raw_ );
}



void VolumeMeshShader::setRegion( Region* const& raw_ )
{
    raw = raw_;
    loadBuffers();
}


void VolumeMeshShader::loadBuffers()
{

    std::vector< double > vertices_double_;
    raw->getVertices( vertices_double_ );

    std::vector< float > vertices_ = Shader::convertToFloat( vertices_double_ );
    if( vertices_.empty() == true ) return;

    double maxx_ = 0, maxy_ = 0, maxz_ = 0, minx_ = 0, miny_ = 0, minz_ = 0;
    raw->getMaxMin( maxx_, maxy_, maxz_, minx_, miny_, minz_ );

    Eigen::Vector3f min( static_cast< float >( minx_ ), static_cast< float >( miny_ ), static_cast< float >( minz_ ) );
    Eigen::Vector3f max( static_cast< float >( maxx_ ), static_cast< float >( maxy_ ), static_cast< float >( maxz_ ) );
    vertices_ = Shader::normalize( vertices_, max, min, 3 );


//    std::vector< GLuint > faces_ = Shader::convertToUnsignedInt( surface_.getFaces() );



    std::vector< std::size_t > faces_size_t_;
    raw->getTetrahedralCells( faces_size_t_ );

    std::vector< GLuint > faces_ = Shader::convertToUnsignedInt( faces_size_t_ );
    std::vector< GLfloat > normals_;

    int r, g, b;
    raw->getColor( r, g, b );

    std::size_t nvertices = vertices_.size()/3;

    updateGeometryBuffers( vertices_, normals_, faces_ );
    updateColorBuffers( nvertices, r, g, b );
}



void VolumeMeshShader::initShaders()
{
    shader = new Tucano::Shader( "Surface", ( shader_directory + "shaders/gouraud_surface.vert" ),
                                            ( shader_directory + "shaders/gouraud_surface.frag" ),
                                            "", "", "" );

    shader->initialize();
}

void VolumeMeshShader::initBuffers()
{
    glGenVertexArrays ( 1 , &va_volume );
    glBindVertexArray ( va_volume );


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



void VolumeMeshShader::resetShaders()
{
    if ( shader == nullptr ) return;

    delete( shader );
    shader = nullptr;
}

void VolumeMeshShader::resetBuffers()
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



void VolumeMeshShader::updateGeometryBuffers( const std::vector< GLfloat >& vertices_,
                                           const std::vector< GLfloat >& normals_,
                                           const std::vector< GLuint >& faces_ )
{
    glBindBuffer ( GL_ARRAY_BUFFER, vb_vertices );
    glBufferData ( GL_ARRAY_BUFFER, vertices_.size() * sizeof ( GLfloat ), vertices_.data() ,
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER, 0 );


    std::vector< GLfloat > normals_default_;
    normals_default_ = normals_;
    if( normals_default_.empty() == true )
        normals_default_ = getDefaultNormals( vertices_.size() );

    glBindBuffer ( GL_ARRAY_BUFFER , vb_normals );
    glBufferData ( GL_ARRAY_BUFFER , normals_default_.size() * sizeof ( GLfloat ), normals_default_.data() ,
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );

    number_of_faces = faces_.size();
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, faces_.size() * sizeof ( GLuint ), faces_.data(),
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void VolumeMeshShader::updateColorBuffers( const std::vector< GLfloat >& colors_ )
{
    glBindBuffer( GL_ARRAY_BUFFER, vb_colors );
    glBufferData( GL_ARRAY_BUFFER, colors_.size() * sizeof ( GLfloat ), colors_.data(),
                  GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );
}


void VolumeMeshShader::updateColorBuffers( std::size_t nvertices_, int r_, int g_, int b_ )
{
    std::vector< GLfloat > color_;
    color_.resize( 3*nvertices_ );

    for( std::size_t i = 0; i < nvertices_; ++i )
    {
        color_[ 3* i ] = static_cast<GLfloat>( r_/255.0f );
        color_[ 3* i + 1 ] = static_cast<GLfloat>( g_/255.0f );
        color_[ 3* i + 2 ] = static_cast<GLfloat>( b_/255.0f );
    }

    updateColorBuffers( color_ );
}



void VolumeMeshShader::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                          const int& h )
{

    if( raw->isVisible() == false ) return;

    Eigen::Affine3f M;
    M.setIdentity();

    shader->bind();

        shader->setUniform( "ModelMatrix" , M );
        shader->setUniform( "ViewMatrix" , V );
        shader->setUniform( "ProjectionMatrix" , P );
        shader->setUniform( "WIN_SCALE" , (float) w , (float) h );

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

        glEnable( GL_POLYGON_OFFSET_FILL );
        glPolygonOffset( 2.0f, 1.0f );


            glBindVertexArray( va_volume );

                shader->setUniform( "solid" , true );
                shader->setUniform( "testing" , true );

                glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
                glDrawElements ( GL_TRIANGLES , number_of_faces , GL_UNSIGNED_INT , 0 );

            glBindVertexArray ( 0 );


        glDisable(GL_POLYGON_OFFSET_FILL);


//        glEnable( GL_DEPTH_TEST );
//        glDepthFunc( GL_LEQUAL );
//        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );


//            glBindVertexArray( va_volume );

//                shader->setUniform( "solid" , false );
//                glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
//                glDrawElements ( GL_TRIANGLES , number_of_faces , GL_UNSIGNED_INT , 0 );

//            glBindVertexArray ( 0 );

        glDisable( GL_BLEND );
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    shader->unbind();

}



void VolumeMeshShader::update()
{
    loadBuffers();
}


void VolumeMeshShader::clear()
{
    reset();
    setDefaultValues();
}


void VolumeMeshShader::setDefaultValues()
{
    va_volume = 0;
    vb_vertices = 0;
    vb_normals = 0;
    vb_colors = 0;
    vb_faces = 0;

    number_of_faces = 0;
    number_of_vertices = 0;

    raw = nullptr;

}
