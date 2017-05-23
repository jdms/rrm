#include "VolumeOpenGLWrap.h"

VolumeOpenGLWrap::VolumeOpenGLWrap()
{
    shader = nullptr;
    va_volume = 0;
    vb_vertices = 0;
    nlines = 0;

    volume = nullptr;


//    csection = nullptr;
//    shader_csection = nullptr;
//    va_csection = 0;
//    vb_vertices_csection = 0;
//    nvertices_csection = 0;

}


void VolumeOpenGLWrap::initShaders()
{
    shader = new Tucano::Shader( "VolumeOpenGLWrap", ( shader_directory + "shaders/CubeSinglePassWireframe.vert" ), ( shader_directory + "shaders/CubeSinglePassWireframe.frag"),
                                             ( shader_directory + "shaders/CubeSinglePassWireframe.geom" ), "", "" );
    shader->initialize();


//    shader_csection = new Tucano::Shader( "Surface", ( shader_directory + "shaders/CubeSinglePassWireframe.vert" ),
//                                            ( shader_directory + "shaders/CubeSinglePassWireframe.frag" ),
//                                            ( shader_directory + "shaders/CubeSinglePassWireframe.geom" ), "", "" );


//    shader_csection->initialize();
}

void VolumeOpenGLWrap::initBuffers()
{
    glGenVertexArrays( 1 , &va_volume );
    glBindVertexArray( va_volume );

        glGenBuffers( 1 , &vb_vertices );
        glBindBuffer( GL_ARRAY_BUFFER , vb_vertices );
        glBufferData( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0 , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );

    glBindVertexArray( 0 );


//    glGenVertexArrays ( 1 , &va_csection );
//    glBindVertexArray ( va_csection );

//        glGenBuffers ( 1 , &vb_vertices_csection );
//        glBindBuffer ( GL_ARRAY_BUFFER , vb_vertices_csection );
//        glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
//        glEnableVertexAttribArray ( 0 );
//        glVertexAttribPointer ( 0 , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );

//    glBindVertexArray ( 0 );
}



void VolumeOpenGLWrap::resetShaders()
{

    if ( shader != nullptr )
    {
        delete( shader );
        shader = nullptr;
    }

//    if ( shader_csection != nullptr )
//    {
//        delete( shader_csection );
//        shader_csection = nullptr;
//    }
}

void VolumeOpenGLWrap::resetBuffers()
{
    if( va_volume != 0 )
    {
        glDeleteVertexArrays( 1, &va_volume );

        if ( vb_vertices != 0 )
        {
            glDeleteBuffers( 1, &vb_vertices );
        }
    }

    va_volume = 0;
    vb_vertices = 0;

//    if( va_csection != 0 )
//    {
//        glDeleteVertexArrays( 1, &va_csection );

//        if ( vb_vertices_csection != 0 )
//        {
//            glDeleteBuffers( 1, &vb_vertices_csection );
//        }

//    }

//    va_csection = 0;
//    vb_vertices_csection = 0;
//    nvertices_csection = 0;
}


void VolumeOpenGLWrap::setVolumeRaw( Volume* const& vol_ )
{
    volume = vol_;
    createVolumeBox();
}


void VolumeOpenGLWrap::createVolumeBox()
{
    double ox_ = 0.0f, oy_ = 0.0f, oz_ = 0.0f;
    double w_ = 0.0f, h_ = 0.0f, d_ = 0.0f;

    volume->getOrigin( ox_, oy_, oz_ );
    volume->getDimensions( w_, h_, d_ );

    float originx_ = (float)ox_, originy_ =  (float)oy_, originz_ = (float)oz_;
    float width_ = (float)w_, height_ = (float)h_, depth_ = (float)d_;

    Eigen::Vector3f min( originx_, originy_, originz_ );
    Eigen::Vector3f max( originx_ + width_, originy_ + height_, originz_ + depth_ );


    Eigen::Vector3f A( min.x(), min.y(), max.z());
    Eigen::Vector3f B( max.x(), min.y(), max.z());
    Eigen::Vector3f C( max.x(), max.y(), max.z());
    Eigen::Vector3f D( min.x(), max.y(), max.z());
    Eigen::Vector3f E( max.x(), min.y(), min.z());
    Eigen::Vector3f F( min.x(), min.y(), min.z());
    Eigen::Vector3f G( min.x(), max.y(), min.z());
    Eigen::Vector3f H( max.x(), max.y(), min.z());


    std::vector< float > wireframe_ =
    {
        A.x(), A.y(), A.z(), 1.0f,
        B.x(), B.y(), B.z(), 1.0f,
        D.x(), D.y(), D.z(), 1.0f,
        C.x(), C.y(), C.z(), 1.0f,

        B.x(), B.y(), B.z(), 1.0f,
        E.x(), E.y(), E.z(), 1.0f,
        C.x(), C.y(), C.z(), 1.0f,
        H.x(), H.y(), H.z(), 1.0f,

        E.x(), E.y(), E.z(), 1.0f,
        F.x(), F.y(), F.z(), 1.0f,
        H.x(), H.y(), H.z(), 1.0f,
        G.x(), G.y(), G.z(), 1.0f,

        F.x(), F.y(), F.z(), 1.0f,
        A.x(), A.y(), A.z(), 1.0f,
        G.x(), G.y(), G.z(), 1.0f,
        D.x(), D.y(), D.z(), 1.0f,

        D.x(), D.y(), D.z(), 1.0f,
        C.x(), C.y(), C.z(), 1.0f,
        G.x(), G.y(), G.z(), 1.0f,
        H.x(), H.y(), H.z(), 1.0f,

        F.x(), F.y(), F.z(), 1.0f,
        E.x(), E.y(), E.z(), 1.0f,
        A.x(), A.y(), A.z(), 1.0f,
        B.x(), B.y(), B.z(), 1.0f,
    };


    Eigen::Vector3f max_( 0.5f, 0.5f, 0.5f );
    Eigen::Vector3f min_( -0.5f, -0.5f, -0.5f );

    std::vector< float > unit_wireframe_ = Model3DUtils::normalizeMeshIntoABox( wireframe_, max_, min_ );
    reloadBuffers( unit_wireframe_ ) ;

    minimum = Model3DUtils::normalizePointCloud( min, max, min );
    std::cout << "minimum: " << minimum.x() << ", " << minimum.y() << ", " << minimum.z()
              << std::endl << std::flush;


    maximum = Model3DUtils::normalizePointCloud( max, max, min );
    std::cout << "maximum: " << maximum.x() << ", " << maximum.y() << ", " << maximum.z()
              << std::endl << std::flush;


//    updateCrossSection( depth_ );
}


void VolumeOpenGLWrap::updateCrossSection( double depth_ )
{

//    double ox_ = 0.0f, oy_ = 0.0f, oz_ = 0.0f;
//    double w_ = 0.0f, h_ = 0.0f, d_ = 0.0f;

//    volume->getOrigin( ox_, oy_, oz_ );
//    volume->getDimensions( w_, h_, d_ );

//    float M = (float)( oz_ + d_ );
//    float m = (float)( oz_ );

//    float z_ = ( depth_ - m )*( maximum.z() - minimum.z() )/( M - m ) + minimum.z();

//    Eigen::Vector3f A( minimum.x(), minimum.y(), z_ );
//    Eigen::Vector3f B( maximum.x(), minimum.y(), z_ );
//    Eigen::Vector3f C( maximum.x(), maximum.y(), z_ );
//    Eigen::Vector3f D( minimum.x(), maximum.y(), z_ );



//    std::vector< float > plane_ =
//    {
//        A.x(), A.y(), A.z(), 1.0f,
//        B.x(), B.y(), B.z(), 1.0f,
//        D.x(), D.y(), D.z(), 1.0f,
//        C.x(), C.y(), C.z(), 1.0f
//    };


//    nvertices_csection = plane_.size();

//    glBindBuffer ( GL_ARRAY_BUFFER, vb_vertices_csection );
//    glBufferData ( GL_ARRAY_BUFFER, plane_.size() * sizeof ( float ), plane_.data() ,
//                   GL_STATIC_DRAW );
//    glBindBuffer ( GL_ARRAY_BUFFER, 0 );


}


void VolumeOpenGLWrap::draw( const Eigen::Affine3f& V_, const Eigen::Matrix4f& P_, const int& w_,
                                                                                   const int& h_ )
{
    if( volume->getVisibility() == false ) return;

    Eigen::Affine3f M_;
    M_.setIdentity();


    glDisable( GL_DEPTH_TEST );

    shader->bind();

        shader->setUniform( "ModelMatrix", M_ );
        shader->setUniform( "ViewMatrix",  V_ );
        shader->setUniform( "ProjectionMatrix", P_ );
        shader->setUniform( "WIN_SCALE", (float) w_, (float) h_ );
        shader->setUniform ( "color_plane" , 0.5f, 0.5f, 0.5f, 0.2f );

        glBindVertexArray( va_volume );
            glDrawArrays( GL_LINES_ADJACENCY , 0 , nlines );
        glBindVertexArray( 0 );

    shader->unbind();

//    shader_csection->bind();

//    shader_csection->setUniform( "ModelMatrix" , M_ );
//    shader_csection->setUniform( "ViewMatrix" , V_ );
//    shader_csection->setUniform( "ProjectionMatrix" , P_ );
//    shader_csection->setUniform( "WIN_SCALE" , (float) w_ , (float) h_ );
//    shader_csection->setUniform( "color_plane", 0.5f, 0.5f, 0.5f, 0.2f );

//        glBindVertexArray( va_csection );
//        glDrawArrays ( GL_LINES_ADJACENCY , 0 , nvertices_csection );
//        glBindVertexArray ( 0 );

//    shader_csection->unbind();

    glEnable( GL_DEPTH_TEST );



}
