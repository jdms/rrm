#include "canvasComputation.h"

CanvasComputation::CanvasComputation( QGLFormat format, QWidget* parent ) : QGLWidget( format, parent )
{
    this->makeCurrent();
    initializeOpenGLFunctions();

//    flowvisualizationc = new FlowVisualizationController();
    flowvisualizationc = NULL;

    resetSetup();

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

    pmatrix.setToIdentity();

    // maybe change this projection matrix to ortho
    pmatrix.perspective( 80.0f, (float) width/height, 0.001f, 1800.0f );

}


void CanvasComputation::paintGL()
{
    this->makeCurrent();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glUseProgram( program );

    setupMatrices();
    drawModel();

}


void CanvasComputation::initializeShaders()
{

    std::string vertex_shader_string = read_shader_file( "/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/vertex_shader.vert" );
    std::string fragment_shader_string = read_shader_file( "/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/fragment_shader.frag" );

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


void CanvasComputation::deleteBuffers()
{
    if( glIsBuffer( bf_faces ) == true )
        glDeleteBuffers( 1, &bf_faces );

    if( glIsVertexArray( vao_mesh ) == true )
        glDeleteVertexArrays( 1, &vao_mesh );

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


void CanvasComputation::setPositionModel()
{
    float xm, xM, ym, yM, zm, zM;
    flowvisualizationc->getBoundingBox( xm, xM, ym, yM, zm, zM );

    model_center.setX( ( xm + xM )*0.5f );
    model_center.setY( ( ym + yM )*0.5f );
    model_center.setZ( ( zm + zM )*0.5f );

//    float max_val = std::max( std::max( xM - xm, yM - ym ), ( zM - zm ) );
//    diameter = sqrt( 2*max_val*max_val );
}


void CanvasComputation::setupMatrices()
{

    QVector3D up_direction( 0.0f, 1.0f, 0.0f );
    QVector3D axis_beta( 1.0f, 0.0f, 0.0f );

    QVector4D camera_position( 0.0f, 0.0f, zoom, 1.0f );
    QVector4D camera_lookat( 0.0f, 0.0f, 0.0f, 0.0f );
    QVector4D camera_up( up_direction.x(), up_direction.y(), up_direction.z(), 1.0f );

    QMatrix4x4 rotation_matrix;
    rotation_matrix.setToIdentity();
    rotation_matrix.rotate( alpha, up_direction );
    rotation_matrix.rotate( beta, axis_beta );

    camera_position = rotation_matrix*camera_position;
    camera_up = rotation_matrix*camera_up;


    QMatrix4x4 vmatrix;
    vmatrix.lookAt( QVector3D( camera_position.x(), camera_position.y(), camera_position.z() ),
                    QVector3D( camera_lookat.x(), camera_lookat.y(), camera_lookat.z() ),
                    QVector3D( camera_up.x(), camera_up.y(), camera_up.z() ) );



    QVector4D translation_vector = rotation_matrix* QVector4D( translation_vector.x(), translation_vector.y(), 0.0f, 1.f );
    QMatrix4x4 mmatrix;
    mmatrix.setToIdentity();
    mmatrix.translate( QVector3D( -model_center.x() - translation_vector.x(), -model_center.y() - translation_vector.y(), -model_center.z()/* - translation_vector.z() */) );


    QMatrix4x4 mvp = pmatrix * vmatrix * mmatrix;
    GLint loc_mvp = glGetUniformLocation( program, "mvp_matrix" );
    glUniformMatrix4fv( loc_mvp, 1, GL_FALSE, mvp.data() );

}


void CanvasComputation::drawModel()
{
    glBindVertexArray( vao_mesh );

    if( show_vertices == true )
        glDrawArrays( GL_POINTS, 0, number_of_vertices );

    if( show_faces == true )
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces );
        glDrawElements(GL_TRIANGLES, number_of_faces, GL_UNSIGNED_INT, NULL );
    }


    glBindVertexArray( 0 );
}


void CanvasComputation::resetSetup()
{
    resetVisualization();
    resetCamera();
    resetData();
}


void CanvasComputation::resetData()
{

    // empty controller

    number_of_vertices = 0;
    number_of_faces = 0;
    model_center.setX( 0.0f );
    model_center.setY( 0.0f );
    model_center.setZ( 0.0f );

//    deleteBuffers();

    if( flowvisualizationc != NULL )
    {
        flowvisualizationc->clear();
        delete flowvisualizationc;
    }
    flowvisualizationc = new FlowVisualizationController();

}


void CanvasComputation::resetCamera()
{
    previous_mouse.setX( 0.0f );
    previous_mouse.setY( 0.0f );
    previous_mouse.setZ( 0.0f );

    translation_vector.setX( 0.0f );
    translation_vector.setY( 0.0f );
    translation_vector.setZ( 0.0f );

    alpha = 0.0f;
    beta = 0.0f;
    zoom = 1.0f;

}


void CanvasComputation::resetVisualization()
{
    speed_mouse = 0.0015f;
    speed_rotation = 0.5f;

    show_vertices = true;
    show_faces = true;
}


void CanvasComputation::mousePressEvent( QMouseEvent *m )
{
    previous_mouse.setX( m->x() );
    previous_mouse.setY( m->y() );

}


void CanvasComputation::mouseMoveEvent( QMouseEvent *m )
{


    int deltaX = m->x() - previous_mouse.x();
    int deltaY = m->y() - previous_mouse.y();


    if( m->buttons() & Qt::LeftButton )
    {

        alpha -= speed_rotation*deltaX;
        if( alpha < 0.0f )
            alpha = 360.0f;
        else if( alpha > 360.0f )
            alpha = 0.0f;

        beta -= speed_rotation*deltaY;
        if( beta < 0.0f )
            beta = 360.0f;
        if( beta > 360.0f )
            beta = 0.0f;

    }
    else if( m->buttons() & Qt::RightButton )
    {
        float transX = translation_vector.x() - speed_mouse*deltaX;
        float transY = translation_vector.y() + speed_mouse*deltaY;

        translation_vector.setX( transX );
        translation_vector.setY( transY );
    }

    previous_mouse.setX( m->x() );
    previous_mouse.setY( m->y() );

    updateGL();

}


void CanvasComputation::wheelEvent( QWheelEvent *m )
{

    if( m->orientation() == Qt::Vertical )
    {

        int delta = m->delta();

        if( delta < 0 )
            zoom *= 1.1f;
        else
            zoom *= 0.9f;

        updateGL();

    }
}
