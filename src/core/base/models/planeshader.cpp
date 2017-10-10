#include "planeshader.h"
#include "crosssection.h"

PlaneShader::PlaneShader()
{
    setDefaultValues();
}


PlaneShader::PlaneShader( CrossSection* const& raw_ )
{
    setDefaultValues();
    init();
    setCrossSection( raw_ );
}


void PlaneShader::setCrossSection( CrossSection* const& raw_ )
{
    csection = raw_;
    createPlane();
}


void PlaneShader::createPlane()
{
    std::vector< double > verticesd_;
    csection->getCoordinates( verticesd_ );
    std::vector< float >  vertices_ = Shader::convertToFloat( verticesd_ );

    double maxx_ = 0, maxy_ = 0, maxz_ = 0, minx_ = 0, miny_ = 0, minz_ = 0;
    csection->getMaxMin( maxx_, maxy_, maxz_, minx_, miny_, minz_ );

    Eigen::Vector3f min( static_cast< float >( minx_ ), static_cast< float >( miny_ ), static_cast< float >( minz_ ) );
    Eigen::Vector3f max( static_cast< float >( maxx_ ), static_cast< float >( maxy_ ), static_cast< float >( maxz_ ) );

    Eigen::Vector3f A( vertices_[ 0 ], vertices_[ 1 ], vertices_[ 2 ] );
    Eigen::Vector3f B( vertices_[ 3 ], vertices_[ 4 ], vertices_[ 5 ] );
    Eigen::Vector3f C( vertices_[ 6 ], vertices_[ 7 ], vertices_[ 8 ] );
    Eigen::Vector3f D( vertices_[ 9 ], vertices_[ 10 ], vertices_[ 11 ] );


    A = Shader::normalize( A, max, min );
    B = Shader::normalize( B, max, min );
    C = Shader::normalize( C, max, min );
    D = Shader::normalize( D, max, min );


    std::vector< float > plane =
    {
        C.x(), C.y(), C.z(), 1.0f,
        D.x(), D.y(), D.z(), 1.0f,
        B.x(), B.y(), B.z(), 1.0f,
        A.x(), A.y(), A.z(), 1.0f
    };

    number_of_vertices = static_cast< GLuint > (plane.size()/4 );
    updateGeometryBuffers( plane, definePlaneNormals() );


}


std::vector< float > PlaneShader::definePlaneNormals() const
{
    Eigen::Vector3f normal_A( -1.0f, -1.0f,  1.0f );
    Eigen::Vector3f normal_B(  1.0f, -1.0f,  1.0f );
    Eigen::Vector3f normal_C(  1.0f,  1.0f,  1.0f );
    Eigen::Vector3f normal_D( -1.0f,  1.0f,  1.0f );

    normal_A /= 3;
    normal_B /= 3;
    normal_C /= 3;
    normal_D /= 3;

    normal_A.normalize();
    normal_B.normalize();
    normal_C.normalize();
    normal_D.normalize();

    std::vector< float > normals =
    {
        normal_A.x(), normal_A.y(), normal_A.z(),
        normal_B.x(), normal_B.y(), normal_B.z(),
        normal_D.x(), normal_D.y(), normal_D.z(),
        normal_C.x(), normal_C.y(), normal_C.z()
    };

    return normals;
}




void PlaneShader::initShaders()
{
    shader = new Tucano::Shader( "Plane", ( shader_directory + "shaders/CubeSinglePassWireframe.vert" ),
                                            ( shader_directory + "shaders/CubeSinglePassWireframe.frag" ),
                                            ( shader_directory + "shaders/CubeSinglePassWireframe.geom" ), "", "" );

    shader->initialize();
}

void PlaneShader::initBuffers()
{
    glGenVertexArrays( 1 , &va_plane );
    glBindVertexArray( va_plane );

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


void PlaneShader::resetShaders()
{
    if ( shader != nullptr )
    {
        delete( shader );
        shader = nullptr;
    }
}

void PlaneShader::resetBuffers()
{
    if( va_plane != 0 )
    {
        glDeleteVertexArrays( 1, &va_plane );

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



void PlaneShader::updateGeometryBuffers( const std::vector< GLfloat >& vertices_,
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


void PlaneShader::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                        const int& h )
{

    Eigen::Affine3f M;
    M.setIdentity();

    glEnable( GL_BLEND );
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        shader->bind();

            shader->setUniform( "ModelMatrix", M );
            shader->setUniform( "ViewMatrix",  V );
            shader->setUniform( "ProjectionMatrix", P );
            shader->setUniform( "WIN_SCALE", (float) w, (float) h );
            shader->setUniform ( "color_plane" , 0.5f, 0.5f, 0.5f, 0.2f );

            glBindVertexArray( va_plane );
                glDrawArrays( GL_LINES_ADJACENCY , 0 , number_of_vertices );
            glBindVertexArray( 0 );

        shader->unbind();

    glDisable( GL_BLEND );
}



void PlaneShader::update()
{
    createPlane();
}


void PlaneShader::clear()
{
    reset();
    setDefaultValues();
}


void PlaneShader::setDefaultValues()
{
    va_plane = 0;
    vb_vertices = 0;
    vb_normals = 0;

    number_of_vertices = 0;
}
