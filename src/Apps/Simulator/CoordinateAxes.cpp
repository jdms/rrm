#include "CoordinateAxes.h"
#include <QMatrix4x4>

CoordinateAxes::CoordinateAxes()
{
    init();
}


void CoordinateAxes::init()
{
    va_coneaxes = 0;
    bf_faces_coneaxes = 0;
    nfaces_cone = 0;

    va_cylinderaxes = 0;
    bf_faces_cylinderaxes = 0;
    nfaces_cylinder = 0;

}


void CoordinateAxes::initShader( std::string directory )
{


    shader_axes = new Tucano::Shader( "shader_axes", ( directory + "shaders/vertex_instanced_shader.vert" ), ( directory + "shaders/fragment_shader.frag" ), "", "", "" );
    shader_axes->initialize();

    current_directory = directory;

}


void CoordinateAxes::reloadShader()
{
   if( shader_axes )
        shader_axes->reloadShaders();
}


void CoordinateAxes::load()
{
    loadCylinderAxes();
    loadConeAxes();

}


void CoordinateAxes::loadConeAxes()
{



    vector< float > vertices;
    vector< float > colors;
    vector< float > normals;
    vector< unsigned int > faces;


    readPlyFile( ( current_directory + "Models/cone.ply" ), vertices, colors, faces, normals );


    glGenVertexArrays( 1, &va_coneaxes );
    glBindVertexArray( va_coneaxes );



        if( vertices.empty() == false )
        {

            size_t vvertices_size = vertices.size();
            GLint nvertices =  (GLint) vvertices_size/3;


            vector< Eigen::Vector3f > vertices_eigen;
            for( int it = 0; it < nvertices; ++it )
                vertices_eigen.push_back( Eigen::Vector3f( vertices[ 3*it ], vertices[ 3*it + 1 ], vertices[ 3*it + 2 ] ) );

            Celer::BoundingBox3< float > box;
            box.fromPointCloud( vertices_eigen.begin(), vertices_eigen.end() );


            for( int it = 0; it < nvertices; ++it )
                vertices_eigen[ it ] = ( vertices_eigen[ it ] - box.center() )/box.diagonal();

            box.fromPointCloud( vertices_eigen.begin(), vertices_eigen.end() );


            GLuint bf_points_coneaxes;
            glGenBuffers( 1, &bf_points_coneaxes );
            glBindBuffer( GL_ARRAY_BUFFER, bf_points_coneaxes );
            glBufferData( GL_ARRAY_BUFFER, vvertices_size*sizeof( GLfloat ), vertices_eigen.data(), GL_STATIC_DRAW );
            glVertexAttribPointer( 0, 3, GL_FLOAT, GL_TRUE, 0, NULL );
            glEnableVertexAttribArray( 0 );

        }


        if( faces.empty() == false )
        {
            std::size_t vfaces_size = faces.size();
            nfaces_cone = (GLint) vfaces_size;

            glGenBuffers( 1, &bf_faces_coneaxes );
            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces_coneaxes );
            glBufferData( GL_ELEMENT_ARRAY_BUFFER, vfaces_size*sizeof( GLuint ), faces.data(), GL_STATIC_DRAW );

        }


        if( normals.empty() == false )
        {

            std::size_t vnormals_size = normals.size();

            GLuint bf_normals_coneaxes;
            glGenBuffers( 1, &bf_normals_coneaxes );
            glBindBuffer( GL_ARRAY_BUFFER, bf_normals_coneaxes );
            glBufferData( GL_ARRAY_BUFFER, vnormals_size*sizeof(  GLfloat ), normals.data(), GL_STATIC_DRAW );
            glVertexAttribPointer( 2, 3, GL_FLOAT, GL_TRUE, 0, NULL );
            glEnableVertexAttribArray( 2 );
        }


        const std::size_t NAXES = 3;

        Eigen::Affine3f* M = new Eigen::Affine3f[ NAXES ];

        for( std::size_t it = 0; it < NAXES; ++it )
        {
            Eigen::Affine3f matrix;
            matrix.setIdentity();

            if( it == 0 )
            {
                matrix.translate( Eigen::Vector3f( 0.0f, 0.38f, 0.0f ) );
                matrix.rotate( Eigen::AngleAxisf( (float)M_PI, Eigen::Vector3f( 1.0f, 0.0f, 0.0f ) ) );
            }
            else if( it == 1 )
            {
                matrix.translate( Eigen::Vector3f( 0.38f , 0.0f, 0.0f ) );
                matrix.rotate( Eigen::AngleAxisf( (float)M_PI*0.5f, Eigen::Vector3f( 0.0f, 0.0f, 1.0f ) ) );
            }
            else
            {
                matrix.translate( Eigen::Vector3f( 0.0f , 0.0f, 0.38f ) );
                matrix.rotate( Eigen::AngleAxisf((float) -M_PI*0.5f, Eigen::Vector3f( 1.0f, 0.0f, 0.0f ) ) );
            }

            matrix.scale(  Eigen::Vector3f( 0.08f, 0.18f, 0.08f )  );


            M[ it ] = matrix;
        }

        GLuint bf_mmatrix_axes;
        glGenBuffers( 1, &bf_mmatrix_axes );
        glBindBuffer( GL_ARRAY_BUFFER, bf_mmatrix_axes );
        glBufferData( GL_ARRAY_BUFFER, NAXES*sizeof( Eigen::Affine3f ), &M[ 0 ], GL_STATIC_DRAW );

        glEnableVertexAttribArray( 3 );
        glVertexAttribPointer( 3, 4, GL_FLOAT, GL_FALSE, sizeof( Eigen::Affine3f ), (GLvoid*)0 );

        glEnableVertexAttribArray(4);
        glVertexAttribPointer( 4, 4, GL_FLOAT, GL_FALSE, sizeof( Eigen::Affine3f ), (GLvoid*)( sizeof( Eigen::Vector4f ) ) );

        glEnableVertexAttribArray( 5 );
        glVertexAttribPointer( 5, 4, GL_FLOAT, GL_FALSE, sizeof( Eigen::Affine3f ), (GLvoid*)( 2 * sizeof( Eigen::Vector4f ) ) );

        glEnableVertexAttribArray( 6 );
        glVertexAttribPointer( 6, 4, GL_FLOAT, GL_FALSE, sizeof( Eigen::Affine3f ), (GLvoid*)( 3 * sizeof( Eigen::Vector4f ) ) );

        glVertexAttribDivisor( 3, 1 );
        glVertexAttribDivisor( 4, 1 );
        glVertexAttribDivisor( 5, 1 );
        glVertexAttribDivisor( 6, 1 );


        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );


    glBindVertexArray( 0 );


}


void CoordinateAxes::loadCylinderAxes()
{


    vector< float > vertices;
    vector< float > colors;
    vector< float > normals;
    vector< unsigned int > faces;


    readPlyFile( ( current_directory + "Models/cylinder.ply" ), vertices, colors, faces, normals );


    glGenVertexArrays( 1, &va_cylinderaxes );
    glBindVertexArray( va_cylinderaxes );



        if( vertices.empty() == false )
        {

            size_t vvertices_size = vertices.size();
            GLint nvertices =  (GLint) vvertices_size/3;


            vector< Eigen::Vector3f > vertices_eigen;
            for( int it = 0; it < nvertices; ++it )
                vertices_eigen.push_back( Eigen::Vector3f( vertices[ 3*it ], vertices[ 3*it + 1 ], vertices[ 3*it + 2 ] ) );

            Celer::BoundingBox3< float > box;
            box.fromPointCloud( vertices_eigen.begin(), vertices_eigen.end() );


            for( int it = 0; it < nvertices; ++it )
                vertices_eigen[ it ] = ( vertices_eigen[ it ] - box.center() )/box.diagonal();

            box.fromPointCloud( vertices_eigen.begin(), vertices_eigen.end() );


            GLuint bf_points_cylinderaxes;
            glGenBuffers( 1, &bf_points_cylinderaxes );
            glBindBuffer( GL_ARRAY_BUFFER, bf_points_cylinderaxes );
            glBufferData( GL_ARRAY_BUFFER, vvertices_size*sizeof( GLfloat ), vertices_eigen.data(), GL_STATIC_DRAW );
            glVertexAttribPointer( 0, 3, GL_FLOAT, GL_TRUE, 0, NULL );
            glEnableVertexAttribArray( 0 );

        }


        if( faces.empty() == false )
        {
            std::size_t vfaces_size = faces.size();
            nfaces_cylinder = (GLint) vfaces_size;

            glGenBuffers( 1, &bf_faces_cylinderaxes );
            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces_cylinderaxes );
            glBufferData( GL_ELEMENT_ARRAY_BUFFER, vfaces_size*sizeof( GLuint ), faces.data(), GL_STATIC_DRAW );

        }


        if( normals.empty() == false )
        {

            std::size_t vnormals_size = normals.size();

            GLuint bf_normals_cylinderaxes;
            glGenBuffers( 1, &bf_normals_cylinderaxes );
            glBindBuffer( GL_ARRAY_BUFFER, bf_normals_cylinderaxes );
            glBufferData( GL_ARRAY_BUFFER, vnormals_size*sizeof(  GLfloat ), normals.data(), GL_STATIC_DRAW );
            glVertexAttribPointer( 2, 3, GL_FLOAT, GL_TRUE, 0, NULL );
            glEnableVertexAttribArray( 2 );
        }



        const std::size_t NAXES = 3;

        Eigen::Affine3f* M = new Eigen::Affine3f[ NAXES ];

        for( std::size_t it = 0; it < NAXES; ++it )
        {
            Eigen::Affine3f matrix;
            matrix.setIdentity();


            Eigen::Vector3f vector_translation( 0.0f, 0.0f, 0.0f );
            if( it == 0 )
                vector_translation[ 1 ] += 0.18f;

            else if( it == 1 )
                vector_translation[ 0 ] += 0.18f;

            else
                vector_translation[ 2 ] += 0.18f;

            matrix.translate( vector_translation );

            Eigen::Vector3f axis_rotation( 0.0f, 0.0f, 0.0f );
            axis_rotation[ it ] = 1.0f;

            matrix.rotate( Eigen::AngleAxisf( (float)0.5f*M_PI, axis_rotation ) );
            matrix.scale( Eigen::Vector3f( 0.032f, 0.032f, 0.8f ) );

            M[ it ] = matrix;
        }


        GLuint bf_mmatrix_axes;
        glGenBuffers( 1, &bf_mmatrix_axes );
        glBindBuffer( GL_ARRAY_BUFFER, bf_mmatrix_axes );
        glBufferData( GL_ARRAY_BUFFER, NAXES*sizeof( Eigen::Affine3f ), &M[ 0 ], GL_STATIC_DRAW );

        glEnableVertexAttribArray( 3 );
        glVertexAttribPointer( 3, 4, GL_FLOAT, GL_FALSE, sizeof( Eigen::Affine3f ), (GLvoid*)0 );

        glEnableVertexAttribArray(4);
        glVertexAttribPointer( 4, 4, GL_FLOAT, GL_FALSE, sizeof( Eigen::Affine3f ), (GLvoid*)( sizeof( Eigen::Vector4f ) ) );

        glEnableVertexAttribArray( 5 );
        glVertexAttribPointer( 5, 4, GL_FLOAT, GL_FALSE, sizeof( Eigen::Affine3f ), (GLvoid*)( 2 * sizeof( Eigen::Vector4f ) ) );

        glEnableVertexAttribArray( 6 );
        glVertexAttribPointer( 6, 4, GL_FLOAT, GL_FALSE, sizeof( Eigen::Affine3f ), (GLvoid*)( 3 * sizeof( Eigen::Vector4f ) ) );

        glVertexAttribDivisor( 3, 1 );
        glVertexAttribDivisor( 4, 1 );
        glVertexAttribDivisor( 5, 1 );
        glVertexAttribDivisor( 6, 1 );


        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );


    glBindVertexArray( 0 );


}


void CoordinateAxes::draw( const Eigen::Matrix3f& R, const Eigen::Vector3f& T )
{



    Eigen::Affine3f V;
    V.setIdentity();

    for( int i = 0; i < 3; ++i )
        for( int j = 0; j < 3; ++j )
            V( i, j ) = R( i, j );

    V( 0, 3 ) = T( 0 );
    V( 1, 3 ) = T( 1 );
    V( 2, 3 ) = T( 2 );


    Eigen::Affine3f matrix;
    matrix.setIdentity();
    matrix.translate( Eigen::Vector3f( 3.1f, -1.7f, 0.0f ) );


    QMatrix4x4 Q;
    Q.perspective( 60.0 , (float) 1024/(float) 600 , 0.1f , 10000.0f );
//    Q.ortho( 0, 1024, 0, 600, 0.1, 1000 );

    Eigen::Matrix4f P1;
    for( int i = 0; i < 4; ++i )
        for( int j = 0; j < 4; ++j )
            P1( i, j ) = Q( i, j );




    shader_axes->bind();

    shader_axes->setUniform( "vmatrix", matrix*V );
    shader_axes->setUniform( "pmatrix", P1 );

        glBindVertexArray( va_coneaxes );


            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces_coneaxes );
            glDrawElementsInstanced( GL_TRIANGLES, nfaces_cone, GL_UNSIGNED_INT, 0, 3 );

        glBindVertexArray( 0 );


        glBindVertexArray( va_cylinderaxes );


            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces_cylinderaxes );
            glDrawElementsInstanced( GL_TRIANGLES, nfaces_cylinder, GL_UNSIGNED_INT, 0, 3 );


        glBindVertexArray( 0 );

    shader_axes->unbind();


}


void CoordinateAxes::resetBuffers()
{
    deleteShaders();

    if( va_coneaxes )
    {
        glDeleteVertexArrays( 1, &va_coneaxes );
        if( bf_faces_coneaxes )
            glDeleteBuffers(1, &bf_faces_coneaxes);

    }

    if( va_cylinderaxes )
    {
        glDeleteVertexArrays( 1, &va_cylinderaxes );
        if( bf_faces_cylinderaxes )
            glDeleteBuffers(1, &bf_faces_cylinderaxes);

    }


    va_coneaxes = 0;
    bf_faces_coneaxes = 0;
    bf_faces_cylinderaxes = 0;
    va_cylinderaxes = 0;;

    nfaces_cone = 0;
    nfaces_cylinder = 0;


}

void CoordinateAxes::deleteShaders()
{

    if (shader_axes)
    {
        delete (shader_axes);
        shader_axes = nullptr;
    }


}

