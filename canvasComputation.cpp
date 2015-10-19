#include "canvasComputation.h"

CanvasComputation::CanvasComputation( QGLFormat format, QWidget* parent ) : QGLWidget( format, parent )
{
    this->makeCurrent();
    initializeOpenGLFunctions();

    flowvisualizationc = new FlowVisualizationController();
}

void CanvasComputation::initializeGL()
{
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glEnable( GL_DEPTH_TEST );

    initializeShaders();
}

void CanvasComputation::resizeGL( int width, int height )
{
    height = height? height:1;
    glViewport( 0, 0, (GLint) width, (GLint) height );
}

void CanvasComputation::paintGL()
{
    this->makeCurrent();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

}


void CanvasComputation::initializeShaders()
{

    std::string vertex_shader_string = read_shader_file( "/Users/Clarissa/Dropbox/Work/Projects/Lidar/MeshViewer/vertex_shader.vert" );
    std::string fragment_shader_string = read_shader_file( "/Users/Clarissa/Dropbox/Work/Projects/Lidar/MeshViewer/fragment_shader.frag" );

    const char *vertex_shader_source = vertex_shader_string.c_str();
    const char *fragment_shader_source = fragment_shader_string.c_str();


    GLuint vertex_shader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex_shader, 1, &vertex_shader_source, NULL );
    glCompileShader( vertex_shader );


    // verifying if the vertex compilation was ok

    GLint params_vertex;
    glGetShaderiv( vertex_shader, GL_COMPILE_STATUS, &params_vertex );
    if( params_vertex == GL_FALSE )
        cout << "Compile vertex failed!" << endl;


    GLint fragment_shader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment_shader, 1, &fragment_shader_source, NULL );
    glCompileShader( fragment_shader );


    // verifying if the fragment compilation was ok

    GLint params_fragment;
    glGetShaderiv( fragment_shader, GL_COMPILE_STATUS, &params_fragment );
    if( params_fragment == GL_FALSE )
        cout << "Compile fragment failed!" << endl;


    program = glCreateProgram();
    glAttachShader( program, vertex_shader );
    glAttachShader( program, fragment_shader );

    glLinkProgram( program );
    glUseProgram( program );


    glDeleteShader( vertex_shader );
    glDeleteShader( fragment_shader );
}


void CanvasComputation::sendMeshGPU()
{


    glPointSize( 3.0f );
    glGenVertexArrays( 1, &vao_mesh );
    glBindVertexArray( vao_mesh );


    GLuint *bf_mesh = new GLuint[ 2 ];
    glGenBuffers( 2, bf_mesh );


        vector< GLfloat > vertices;
        flowvisualizationc->getVertices( vertices );
        number_of_vertices = vertices.size();

        glBindBuffer( GL_ARRAY_BUFFER, bf_mesh[ 0 ] );
        glBufferData( GL_ARRAY_BUFFER, number_of_vertices*sizeof( GL_FLOAT ), vertices.data(), GL_STATIC_DRAW );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( 0 );


        vector< GLuint > faces;
        flowvisualizationc->getTriangles( faces );

        if( faces.empty() == false )
        {
            number_of_faces = (GLint) faces.size();

            glGenBuffers( 1, &bf_faces );
            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces );
            glBufferData( GL_ELEMENT_ARRAY_BUFFER, number_of_faces*sizeof( GLuint ) , faces.data(), GL_STATIC_DRAW );

        }


        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

        delete [] bf_mesh;

    glBindVertexArray( 0 );


}


void CanvasComputation::sendColorsGPU()
{

    glBindVertexArray( vao_mesh );

        GLuint bf_color;
        vector< GLfloat > colors;
        flowvisualizationc->getColors( colors );
        GLint ncolors = colors.size();

        glBindBuffer( GL_ARRAY_BUFFER, bf_color );
        glBufferData( GL_ARRAY_BUFFER, ncolors*sizeof( GL_FLOAT ), colors.data(), GL_STATIC_DRAW );
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( 1 );


    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );



}


void CanvasComputation::deleteBuffers()
{
    if( glIsBuffer( bf_faces ) == true )
        glDeleteBuffers( 1, &bf_faces );

    if( glIsVertexArray( vao_mesh ) == true )
        glDeleteVertexArrays( 1, &vao_mesh );

}


//void CanvasComputation::drawModel()
