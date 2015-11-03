#include "canvasComputation.h"

CanvasComputation::CanvasComputation( QGLFormat format, QWidget* parent ) : QGLWidget( format, parent )
{
    this->makeCurrent();

    flowvisualizationc = NULL;
    mn_options = NULL;
    initializeOpenGLFunctions();

    bf_mesh = new GLuint[ 2 ];


    resetSetup();
    createActions();
    createPopupMenu();

    connect( parent, SIGNAL( sendSurfaceFile( std::string ) ), flowvisualizationc, SLOT( openSurfaceFile( std::string ) ) );
    connect( parent, SIGNAL( computeVolume() ), flowvisualizationc, SLOT( computeVolumetricMesh() ) );
    connect( parent, SIGNAL( computeFlowProperties() ), flowvisualizationc, SLOT( computeFlowProperties() ) );
    connect( parent, SIGNAL( computePressureProperty() ), flowvisualizationc, SLOT( computePressure() ) );
    connect( parent, SIGNAL( computeVelocityProperty() ), flowvisualizationc, SLOT( computeVelocity() ) );
    connect( parent, SIGNAL( computeTOFProperty() ), flowvisualizationc, SLOT( computeTOF() ) );

    connect( flowvisualizationc, SIGNAL( updateComboBox( std::vector< std::string >, std::vector< std::string > ) ), parent, SLOT( updateComboBox( std::vector< std::string >, std::vector< std::string > ) ) );
    connect( parent, SIGNAL( selectFlowProperty( int, bool& ) ), flowvisualizationc, SLOT( selectFlowProperty( int, bool& ) ) );


}


void CanvasComputation::createActions()
{
    ac_reset_transformations = new QAction( tr( "Reset View" ), this );
    ac_clear_all = new QAction( tr( "Clear All" ), this );
    wa_properties = new QWidgetAction( this );
    wa_colormaps = new QWidgetAction( this );
    wa_rendering = new QWidgetAction( this );

    chk_show_points = new QCheckBox( tr( "Show Points" ) );
    chk_show_points->setChecked( show_vertices );

    chk_show_wireframe = new QCheckBox( tr( "Show WireFrame" ) );
    chk_show_wireframe->setChecked( show_lines );

    chk_show_volume = new QCheckBox( tr( "Show Volume" ) );
    chk_show_volume->setChecked( show_faces );



    connect( this, SIGNAL( sendProperty( std::string, std::string ) ), this, SLOT( sendColorsGPU( std::string, std::string ) ) );
    connect( chk_show_points, SIGNAL( clicked( bool ) ), this, SLOT( showPoints( bool ) ) );
    connect( chk_show_volume, SIGNAL( clicked( bool ) ), this, SLOT( showVolume( bool ) ) );
    connect( chk_show_wireframe, SIGNAL( clicked( bool ) ), this, SLOT( showWireframe( bool ) ) );
    connect( this, SIGNAL( sendVectorProperty( std::string, std::string, int ) ), this, SLOT( sendColorsGPU( std::string, std::string, int ) ) );
    connect( ac_reset_transformations, SIGNAL( triggered() ), this, SLOT( resetTransformations() ) );
    connect( ac_clear_all, SIGNAL( triggered() ), this, SLOT( resetSetup() ) );

}

void CanvasComputation::fillMenuProperties()
{

    fillMenuPointProperties();
    fillMenuCellProperties();

}


void CanvasComputation::fillMenuPointProperties()
{

    std::string name;
    std::string format;
    std::string type;
    int ncoords = 0;

    mn_properties->addSection( "POINTS ATTRIBUTES" );
    QActionGroup* ag_properties = new QActionGroup( this );

    int nproperties = flowvisualizationc->getNumberofPointsProperties();
    for( int i = 0, ids = 0, idv = 0; i < nproperties; ++i )
    {

        flowvisualizationc->getPointProperty( i, name, format, type, ncoords );
        if( ncoords == 1 )
        {
            ac_properties.push_back( new QAction( tr( name.c_str() ), this ) );
            mn_properties->addAction( ac_properties[ ids ] );
            ag_properties->addAction( ac_properties[ ids ] );

            connect( ac_properties[ ids ], &QAction::triggered, [=](){ emit sendProperty( name , type.c_str() ); } );
            ids++;

        }
        else if( ncoords == 3 )
        {
            createRenderingVectors( idv, name, type.c_str() );
            idv++;

        }



    }
}

void CanvasComputation::fillMenuCellProperties()
{
    std::string name;
    std::string format;
    std::string type;
    int ncoords = 0;


    mn_properties->addSection( "CELLS ATTRIBUTES" );
    QActionGroup* ag_properties_cells = new QActionGroup( this );

    int nactions = (int) ac_properties.size();
    int nproperties = flowvisualizationc->getNumberofCellsProperties();
    for( int i = 0, ids = 0, idv = 0; i < nproperties; ++i )
    {

        flowvisualizationc->getCellProperty( i, name, format, type, ncoords );
        if( ncoords == 1 )
        {
            ac_properties.push_back( new QAction( tr( name.c_str() ), this ) );
            mn_properties->addAction( ac_properties[ ids + nactions ] );
            ag_properties_cells->addAction( ac_properties[ ids + nactions ] );

            connect( ac_properties[ ids + nactions ], &QAction::triggered, [=](){ emit sendProperty( name , type.c_str() ); } );
            ids++;

        }
        else if( ncoords == 3 )
        {
            createRenderingVectors( idv, name, type.c_str() );
            idv++;

        }



    }
}


void CanvasComputation::createMenuProperties()
{
    mn_properties = new QMenu( tr( "Properties" ) );
    fillMenuProperties();

}


void CanvasComputation::createMenuColorMaps()
{
    rd_colormaps.push_back( new QRadioButton( tr( "Jet" ) ) );
    rd_colormaps.push_back( new QRadioButton( tr( "Constant" ) ) );

    QVBoxLayout *vb_layout = new QVBoxLayout;
    vb_layout->addWidget( rd_colormaps[ 0 ] );
    vb_layout->addWidget( rd_colormaps[ 1 ] );

    QGroupBox *gb_colormaps = new QGroupBox();
    gb_colormaps->setFlat( true );
    gb_colormaps->setLayout( vb_layout );

    wa_colormaps->setDefaultWidget( gb_colormaps );

    mn_colormap = new QMenu( tr( "ColorMaps" ) );
    mn_colormap->addAction( wa_colormaps );

    connect( rd_colormaps[ 0 ], SIGNAL( clicked( bool) ) , this,  SLOT( setColorMap() ) );
    connect( rd_colormaps[ 1 ], SIGNAL( clicked( bool) ) , this,  SLOT( setColorMapConstant() ) );

}


void CanvasComputation::createMenuRendering()
{


    QVBoxLayout *vb_layout = new QVBoxLayout;
    vb_layout->addWidget( chk_show_points );
    vb_layout->addWidget( chk_show_wireframe );
    vb_layout->addWidget( chk_show_volume );

    QGroupBox *gb_rendering = new QGroupBox();
    gb_rendering->setFlat( true );
    gb_rendering->setLayout( vb_layout );

    wa_rendering->setDefaultWidget( gb_rendering );

}


void CanvasComputation::createRenderingVectors( int id, std::string title, std::string type )
{

    rd_options_vector.push_back( new QRadioButton( tr( "Magnitude" ) ) );
    rd_options_vector.push_back( new QRadioButton( tr( "X" ) ) );
    rd_options_vector.push_back( new QRadioButton( tr( "Y" ) ) );
    rd_options_vector.push_back( new QRadioButton( tr( "Z" ) ) );

    id = (int) mn_vector_properties_points.size();

    QVBoxLayout *vb_layout = new QVBoxLayout( this );
    vb_layout->addWidget( rd_options_vector[ 4*id + 0 ] );
    vb_layout->addWidget( rd_options_vector[ 4*id + 1 ] );
    vb_layout->addWidget( rd_options_vector[ 4*id + 2 ] );
    vb_layout->addWidget( rd_options_vector[ 4*id + 3 ] );

    QGroupBox *gb_options_vector = new QGroupBox();
    gb_options_vector->setFlat( true );
    gb_options_vector->setLayout( vb_layout );

    wa_options_vector = new QWidgetAction( this );
    wa_options_vector->setDefaultWidget( gb_options_vector );



    mn_vector_properties_points.push_back( new QMenu() );
    mn_vector_properties_points[ id ]->addAction( wa_options_vector );
    mn_vector_properties_points[ id ]->setTitle( title.c_str() );


    mn_properties->addMenu( mn_vector_properties_points[ id ] );


    connect( rd_options_vector[ 4*id + 0 ], &QRadioButton::clicked, this, [=](){ emit sendVectorProperty( title.c_str() , type, 0 ); } );
    connect( rd_options_vector[ 4*id + 1 ], &QRadioButton::clicked, this, [=](){ emit sendVectorProperty( title.c_str() , type, 1 ); } );
    connect( rd_options_vector[ 4*id + 2 ], &QRadioButton::clicked, this, [=](){ emit sendVectorProperty( title.c_str() , type, 2 ); } );
    connect( rd_options_vector[ 4*id + 3 ], &QRadioButton::clicked, this, [=](){ emit sendVectorProperty( title.c_str() , type, 3 ); } );

}


void CanvasComputation::createPopupMenu()
{


    createMenuProperties();
    createMenuColorMaps();
    createMenuRendering();


    mn_options = new QMenu();
    mn_options->addMenu( mn_properties );
    mn_options->addMenu( mn_colormap );
    mn_options->addSeparator();
    mn_options->addAction( wa_rendering );
    mn_options->addSeparator();
    mn_options->addAction( ac_reset_transformations );
    mn_options->addAction( ac_clear_all );

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

    delete [] bf_mesh;


    if( glIsBuffer( bf_faces ) == GL_TRUE )
        glDeleteBuffers( 1, &bf_faces );

    if( glIsVertexArray( vao_mesh ) == GL_TRUE )
        glDeleteVertexArrays( 1, &vao_mesh );

}


void CanvasComputation::sendMeshGPU()
{


    glPointSize( 3.0f );
    glGenVertexArrays( 1, &vao_mesh );
    glBindVertexArray( vao_mesh );



    glGenBuffers( 2, bf_mesh );


        vector< GLfloat > vertices;
        flowvisualizationc->getVertices( vertices );
        number_of_vertices = (int) vertices.size();

        glBindBuffer( GL_ARRAY_BUFFER, bf_mesh[ 0 ] );
        glBufferData( GL_ARRAY_BUFFER, number_of_vertices*sizeof( GL_FLOAT ), vertices.data(), GL_STATIC_DRAW );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( 0 );

        vector< GLfloat > colors;
        flowvisualizationc->getColors( colors );
        GLint ncolors = (int) colors.size();

        glBindBuffer( GL_ARRAY_BUFFER, bf_mesh[ 1 ] );
        glBufferData( GL_ARRAY_BUFFER, ncolors*sizeof( GL_FLOAT ), colors.data(), GL_STATIC_DRAW );
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( 1 );



        vector< GLuint > lines;
        flowvisualizationc->getWireframe( lines );

        if( lines.empty() == false )
        {
            number_of_lines = (GLint) lines.size();
            glGenBuffers( 1, &bf_lines );
            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_lines );
            glBufferData( GL_ELEMENT_ARRAY_BUFFER, number_of_lines*sizeof( GLuint ) , lines.data(), GL_STATIC_DRAW );

        }




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


    glBindVertexArray( 0 );


}


void CanvasComputation::sendColorsGPU( std::string property, std::string type, int option )
{
    if( property != "" && type != "" )
        flowvisualizationc->setCurrentProperty( property, type );

    glBindVertexArray( vao_mesh );

        vector< GLfloat > colors;

        if( option == 0 )
            flowvisualizationc->getColors( colors );
        else
            flowvisualizationc->getColors( colors, option );

        GLint ncolors = (GLint) colors.size();

        glBindBuffer( GL_ARRAY_BUFFER, bf_mesh[ 1 ] );
        glBufferData( GL_ARRAY_BUFFER, ncolors*sizeof( GL_FLOAT ), colors.data(), GL_STATIC_DRAW );
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( 1 );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );

    updateGL();



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


    QVector4D translation = QVector4D( translation_vector.x(), translation_vector.y(), 0.0f, 1.f );
    QMatrix4x4 translation_matrix;
    translation_matrix.setToIdentity();
    translation_matrix.translate( QVector3D( -model_center.x() - translation.x(), -model_center.y() -  translation.y(), -model_center.z() - translation_vector.z() )  );
    vmatrix = translation_matrix *vmatrix;

    QMatrix4x4 mmatrix;
    mmatrix.setToIdentity();


    QMatrix4x4 mvp = pmatrix * vmatrix * mmatrix;
    GLint loc_mvp = glGetUniformLocation( program, "mvp_matrix" );
    glUniformMatrix4fv( loc_mvp, 1, GL_FALSE, mvp.data() );

}


void CanvasComputation::showData()
{
//    flowvisualizationc->readData();

//    fillMenuProperties();

//    sendMeshGPU();
//    setPositionModel();

    updateGL();
}


void CanvasComputation::drawModel()
{
//    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glBindVertexArray( vao_mesh );

    if( show_vertices == true )
        glDrawArrays( GL_POINTS, 0, number_of_vertices );

    if( show_faces == true )
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces );
        glDrawElements(GL_TRIANGLES, number_of_faces, GL_UNSIGNED_INT, NULL );
    }

    if( show_lines == true )
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_lines );
        glDrawElements(GL_LINES, number_of_lines, GL_UNSIGNED_INT, NULL );
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
    number_of_lines = 0;
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
    speed_mouse = 0.55f;
    speed_rotation = 0.5f;

    show_vertices = true;
    show_faces = true;
    show_lines = true;


}


void CanvasComputation::mousePressEvent( QMouseEvent *m )
{
    previous_mouse.setX( m->x() );
    previous_mouse.setY( m->y() );

    if( m->buttons() & Qt::RightButton )
    {
        mn_options->exec( m->globalPos() );
    }

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

void CanvasComputation::showPoints( bool option )
{
    show_vertices = option;
    updateGL();
}

void CanvasComputation::showVolume( bool option )
{
    show_faces = option;
    updateGL();
}


void CanvasComputation::showWireframe( bool option )
{
    show_lines = option;
    updateGL();
}



void CanvasComputation::resetTransformations()
{
    resetCamera();
    flowvisualizationc->setCurrentProperty( 0 );
}

void CanvasComputation::setColorMap()
{
    flowvisualizationc->setColorMap( "JET" );
    sendColorsGPU();
    updateGL();
}

void CanvasComputation::setColorMapConstant()
{
    flowvisualizationc->setColorMap( "CONSTANT" );
    sendColorsGPU();
    updateGL();
}


void CanvasComputation::sendSurfaceGPU()
{


    glPointSize( 3.0f );
    glGenVertexArrays( 1, &vao_mesh );
    glBindVertexArray( vao_mesh );



    glGenBuffers( 2, bf_mesh );


        vector< GLfloat > vertices;
        vector< GLuint > lines;
        flowvisualizationc->getSurface( vertices, lines );


        number_of_vertices = (GLuint)  vertices.size();

        glBindBuffer( GL_ARRAY_BUFFER, bf_mesh[ 0 ] );
        glBufferData( GL_ARRAY_BUFFER, number_of_vertices*sizeof( GL_FLOAT ), vertices.data(), GL_STATIC_DRAW );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( 0 );


        if( lines.empty() == false )
        {
            number_of_lines = (GLint) lines.size();
            glGenBuffers( 1, &bf_lines );
            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_lines );
            glBufferData( GL_ELEMENT_ARRAY_BUFFER, number_of_lines*sizeof( GLuint ) , lines.data(), GL_STATIC_DRAW );

        }


        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );


    glBindVertexArray( 0 );


}


void CanvasComputation::setSurfacePositionModel()
{
    float xm, xM, ym, yM, zm, zM;
    flowvisualizationc->getSurfaceBoundingBox( xm, xM, ym, yM, zm, zM );

    model_center.setX( ( xm + xM )*0.5f );
    model_center.setY( ( ym + yM )*0.5f );
    model_center.setZ( ( zm + zM )*0.5f );

}


void CanvasComputation::selectProperty( int id, bool option, int option_color )
{
    FlowProperty p = flowvisualizationc->getPropertyfromMap( id );
    std::string name;
    std::string type;

   p.getName( name );
   p.getType( type );
   if( option == true )
        sendColorsGPU( name, type, option_color );
   else
       sendColorsGPU();


}
