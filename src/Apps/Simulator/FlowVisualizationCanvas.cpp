#include "FlowVisualizationCanvas.h"

FlowVisualizationCanvas::FlowVisualizationCanvas(QWidget *parent, QString _current_dir)
{
    

    show_axis = true;
//    show_colorbar = false;

    current_colormap = ColorMap::COLORMAP::CONSTANT;

	this->current_directory = _current_dir.toStdString();
	
	std::cout << "Current Directory !!!!!!! " << this->current_directory << std::endl;
	createRenderingMenu();

}


void FlowVisualizationCanvas::setOpenGLFormat()
{
    format.setVersion( 4, 1 );
    format.setProfile( QSurfaceFormat::CompatibilityProfile );
    format.setSamples(16);
    setFormat( format );
}


void FlowVisualizationCanvas::createRenderingMenu()
{

    rendering_menu = new FlowRenderingOptionsMenu( this );

    connect( rendering_menu, &FlowRenderingOptionsMenu::coloringVertexbyScalarProperty, this, [=]( std::string name ){ setVerticesColorbyProperty( name ); }  );
    connect( rendering_menu, &FlowRenderingOptionsMenu::coloringVertexbyVectorProperty, this, [=]( std::string name, std::string method ){ setVerticesColorbyProperty( name, method ); }  );
    connect( rendering_menu, &FlowRenderingOptionsMenu::coloringFacebyScalarProperty, this, [=]( std::string name ){ setFacesColorbyProperty( name ); }  );
    connect( rendering_menu, &FlowRenderingOptionsMenu::coloringFacebyVectorProperty, this, [=]( std::string name, std::string method ){ setFacesColorbyProperty( name, method ); }  );

    connect( rendering_menu, &FlowRenderingOptionsMenu::setConstantColormap, this, [=](){ setCurrentColormap( ColorMap::COLORMAP::CONSTANT ); setConstantColor();  }  );
    connect( rendering_menu, &FlowRenderingOptionsMenu::setJETColormap, this, [=](){ setCurrentColormap( ColorMap::COLORMAP::JET ); setColorMap(); }  );
    connect( rendering_menu, &FlowRenderingOptionsMenu::seHotColormap , this, [=](){ setCurrentColormap( ColorMap::COLORMAP::HOT ); setColorMap(); }  );
    connect( rendering_menu, &FlowRenderingOptionsMenu::setCoolColormap, this, [=](){ setCurrentColormap( ColorMap::COLORMAP::COOL ); setColorMap(); }  );
    connect( rendering_menu, &FlowRenderingOptionsMenu::setParulaColormap, this, [=](){ setCurrentColormap( ColorMap::COLORMAP::PARULA ); setColorMap();  }  );
    connect( rendering_menu, &FlowRenderingOptionsMenu::setSpringColormap, this, [=](){ setCurrentColormap( ColorMap::COLORMAP::SPRING ); setColorMap(); }  );
    connect( rendering_menu, &FlowRenderingOptionsMenu::setSummerColormap, this, [=](){ setCurrentColormap( ColorMap::COLORMAP::SUMMER ); setColorMap(); }  );
    connect( rendering_menu, &FlowRenderingOptionsMenu::setCopperColormap, this, [=](){ setCurrentColormap( ColorMap::COLORMAP::COPPER ); setColorMap(); }  );
    connect( rendering_menu, &FlowRenderingOptionsMenu::setPolarColormap, this, [=](){ setCurrentColormap( ColorMap::COLORMAP::POLAR ); setColorMap(); }  );
    connect( rendering_menu, &FlowRenderingOptionsMenu::setWinterColormap, this, [=](){ setCurrentColormap( ColorMap::COLORMAP::WINTER ); setColorMap(); }  );

    //connect( rendering_menu, &FlowRenderingOptionsMenu::reloadcrosssection, this, [=](){  emit getSurfaceCrossSection(); }  );
    //connect( rendering_menu, &FlowRenderingOptionsMenu::loadfile, this, [=](){ emit readSurfacefromFile(); } );
   // connect( rendering_menu, &FlowRenderingOptionsMenu::buildcornerpoint, this, [=](){ emit buildcornerpoint(); } );
   // connect( rendering_menu, &FlowRenderingOptionsMenu::buildunstructured, this, [=](){ emit buildunstructured(); } );
   // connect( rendering_menu, &FlowRenderingOptionsMenu::editparameters, this, [=]( ){ emit editParameters(); }  );
   // connect( rendering_menu, &FlowRenderingOptionsMenu::computeproperties, this, [=](){ controller->computeFlowProperties(); }  );
   // connect( rendering_menu, &FlowRenderingOptionsMenu::applycrosssection, this, [=](){ emit applyCrossSection(); }  );


   // connect( rendering_menu, &FlowRenderingOptionsMenu::exportSurfacetoVTK, this, [=](){  controller->exportSurfacetoVTK(); }  );
   // connect( rendering_menu, &FlowRenderingOptionsMenu::exportVolumetoVTK, this, [=](){ controller->exportVolumetoVTK(); } );
   // connect( rendering_menu, &FlowRenderingOptionsMenu::exportCornerPointtoVTK, this, [=](){ controller->exportCornerPointtoVTK(); } );
   // connect( rendering_menu, &FlowRenderingOptionsMenu::exportResultstoVTK, this, [=](){ controller->exportResultstoVTK(); } );



    connect( rendering_menu, &FlowRenderingOptionsMenu::clearAll, this, [=](){ emit clearAll(); }  );
	
}


void FlowVisualizationCanvas::setController( FlowVisualizationController *c )
{
    controller = c;
    connect( controller, SIGNAL( propertybyVertexComputed( std::string, std::string ) ), rendering_menu, SLOT( addVertexProperty( std::string, std::string ) ) );
    connect( controller, SIGNAL( propertybyFaceComputed( std::string, std::string ) ), rendering_menu, SLOT( addFaceProperty( std::string, std::string ) ) );
    connect( controller, SIGNAL( clearPropertiesMenu() ), rendering_menu, SLOT( clear() )  );

    connect( controller, SIGNAL(setColorMap()) , this, SLOT( setAleatoryColorMap() ) );


}


void FlowVisualizationCanvas::initializeGL()
{


    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if ( GLEW_OK != err )
    {
        fprintf ( stderr , "Error: %s\n" , glewGetErrorString ( err ) );
    }


    makeCurrent();

    glClearColor ( 1.0f , 1.0f , 1.0f , 1.0f );

    glEnable( GL_MULTISAMPLE );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);


    camera.setPerspectiveMatrix ( 60.0 , (float) this->width()/(float) this->height() , 0.1f , 10000.0f );

    mesh.initializeShader( current_directory );
    crosssection.initShader( current_directory );


    if( current_colormap == ColorMap::COLORMAP::CONSTANT )
        setConstantColor();
    else
        setColorMap();

    
    axes.initShader( current_directory );
	axes.load();

    mesh.load();
    initializeShader();



    //    colorbar.setColorBar( &colormap );
    //    colorbar.initShader();
    ////    colorbar->load();
}


void FlowVisualizationCanvas::initializeShader()
{
    background = new Tucano::Shader ( "BackGround" , ( current_directory + "Shaders/DummyQuad.vert" ),
                                       ( current_directory + "Shaders/DummyQuad.frag" ),
                                       ( current_directory + "Shaders/DummyQuad.geom" ), "" , "" );
    background->initialize ( );


    glGenVertexArrays ( 1 , &va_background );
    glBindVertexArray ( va_background );

    /// Requesting Vertex Buffers to the GPU
    glGenBuffers ( 1 , &vb_background );
    glBindBuffer ( GL_ARRAY_BUFFER , vb_background );
    glBufferData ( GL_ARRAY_BUFFER , 0 , 0 , GL_STATIC_DRAW );

    glEnableVertexAttribArray ( 0 );
    glVertexAttribPointer ( 0 , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

    glBindVertexArray ( 0 );


}

/// Debug Purpose
void FlowVisualizationCanvas::reloadShader()
{
	mesh.reloadShader();
}

void FlowVisualizationCanvas::loadBackGround()
{
    glDisable( GL_DEPTH_TEST );
    glDepthMask( GL_FALSE );

    background->bind();

    background->setUniform( "viewportSize", width(), height() );

    glBindVertexArray ( va_background );
        glDrawArrays ( GL_POINTS , 0 , 1 );
    glBindVertexArray ( 0 );

    background->unbind();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}



void FlowVisualizationCanvas::paintGL()
{
    makeCurrent();

    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    loadBackGround();

    Eigen::Affine3f V = camera.getViewMatrix();
    Eigen::Matrix4f P = camera.getProjectionMatrix();



//    if( show_axis == true ){

//        Eigen::Matrix3f R = camera.getRotationMatrix();
//        Eigen::Vector3f T = camera.getTranslationMatrix();


//        axes.draw( R, T );

//    }



    if( apply_crosssection == true )
    {
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        crosssection.draw( V, P, scale );

        glDisable( GL_BLEND );

    }


	mesh.draw(V, P, scale, static_cast<float>(width()), static_cast<float>(height()));


//    if( show_colorbar == true )
//        colorbar.draw();

}


void FlowVisualizationCanvas::resizeGL( int width, int height )
{
    makeCurrent();

    glViewport ( 0 , 0 , width , height );

    camera.setViewport ( Eigen::Vector2f ( (float) width , (float) height ) );
    camera.setPerspectiveMatrix ( camera.getFovy ( ) , (float) width / (float) height , 0.1f , 100.0f );

    scale = 1.5*(float)width/(float)height;


}


void FlowVisualizationCanvas::showVertices( bool status )
{
    mesh.showVertices( status );
    update();
}


void FlowVisualizationCanvas::showEdges( bool status )
{
    mesh.showEdges( status );
    update();
}


void FlowVisualizationCanvas::showFaces( bool status )
{
    mesh.showFaces( status );
    update();
}


void FlowVisualizationCanvas::showBoundingBox( bool status )
{
    mesh.showBoundingBox( status );
    update();
}


void FlowVisualizationCanvas::setConstantColor()
{
    QVector3D c = colormap.getConstantColor();
    mesh.setConstantColor( c.x(), c.y(), c.z() );

//    show_colorbar = false;

    update();
}


void FlowVisualizationCanvas::setColorMap()
{


    if( coloring_property_type.compare( "VERTEX" ) == 0 )
        setVerticesColorbyProperty( coloring_property_name, coloring_property_method );

    else if( coloring_property_type.compare( "FACE" ) == 0 )
        setFacesColorbyProperty( coloring_property_name, coloring_property_method );

    update();
}


void FlowVisualizationCanvas::setVerticesColorbyProperty( std::string name, std::string method )
{

    glShadeModel(GL_SMOOTH);


    coloring_property_name = name;
    coloring_property_method = method;
    coloring_property_type = "VERTEX";


    std::vector < double > values;
    double min = 0.0, max = 0.0;
    values = controller->getVerticesPropertyValues( name, method, min, max );

    if( values.empty() == true ) return;


    int number_of_vertices = mesh.getNumberofVertices();
    std::vector< float > colors;


    for( int i = 0; i < number_of_vertices; ++i )
    {
        QVector3D c = colormap.getColor( current_colormap, values[ i ], min, max );
        colors.push_back( c.x() );
        colors.push_back( c.y() );
        colors.push_back( c.z() );
    }

    mesh.setColor( colors );

    unsigned int nc = 0;
    colorbar->updateColorMap( colormap.getColors( current_colormap, nc ), min, max );
    //colorbar->setMinValue( min );
    //colorbar->setMaxValue( max );
    show_colorbar = true;


    update();

}



void FlowVisualizationCanvas::setColors( const std::vector< float >& colors )
{

    glShadeModel(GL_SMOOTH);

   mesh.setColor( colors );

//   show_colorbar = false;


    update();

}


void FlowVisualizationCanvas::setFacesColorbyProperty( std::string name, std::string method )
{

    glShadeModel(GL_FLAT);

    coloring_property_name = name;
    coloring_property_method = method;
    coloring_property_type = "FACE";


    std::vector < double > values;
    double min = 0.0, max = 0.0;
    values = controller->getFacesPropertyValues( name, method, min, max );

    if( values.empty() == true ) return;


    int number_of_vertices = mesh.getNumberofVertices();

    std::vector< float > colors;
    colors.resize( 3*number_of_vertices );


    int number_of_faces = mesh.getNumberofFaces();


    // get the color for each face and attribute it for its vertices.

    for( int i = 0; i < number_of_faces; ++i )
    {

        QVector3D c = colormap.getColor( current_colormap, values[ i ], min, max );

        vector< unsigned int > vertices_of_face = mesh.getFace( i );
        int number_of_vertices_by_face = vertices_of_face.size();


        for( int j = 0; j < number_of_vertices_by_face; ++j )
        {
            unsigned int id = vertices_of_face[ j ];
            colors[ 3*id ] = c.x();
            colors[ 3*id + 1 ] = c.y();
            colors[ 3*id + 2 ] = c.z();

        }

    }

//    colorbar.setMinValue( min );
//    colorbar.setMaxValue( max );
    show_colorbar = true;

    unsigned int nc = 0;
    colorbar->updateColorMap( colormap.getColors( current_colormap, nc ), min, max );

    mesh.setColor( colors );
    update();

}


void FlowVisualizationCanvas::setCrossSectionNormalCoordinates( float X, float Y, float Z )
{
    apply_crosssection = true;

    float xm, xM, ym, yM, zm, zM;
    mesh.getBoundingBox( xm, xM, ym, yM, zm, zM );

    crosssection.setNormal( QVector3D( X, Y, Z ) );
    crosssection.getCenterFromBoundingBox( QVector3D( xm, ym, zm ), QVector3D( xM, yM, zM ) );
    crosssection.init();
    crosssection.load();


    float a = 0.0f, b = 0.0f , c = 0.0f , d = 0.0f;
    crosssection.getPlaneEquation( a, b, c, d );
    mesh.setCrossSectionClippingEquation( a, b, c, d );

    update();
}


void FlowVisualizationCanvas::showRegions()
{

    std::vector< QColor > colors;
    controller->getRegionsColor( colors );

    int number_of_vertices = mesh.getNumberofVertices();

    std::vector< float > meshcolors;
    meshcolors.resize( 3*number_of_vertices );


    int number_of_faces = mesh.getNumberofFaces();


    // get the color for each face and attribute it for its vertices.

    for( int i = 0; i < number_of_faces; ++i )
    {


        QVector3D c( colors[i].red(),colors[i].green(), colors[i].blue() );
        vector< unsigned int > vertices_of_face = mesh.getFace( i );
        int number_of_vertices_by_face = vertices_of_face.size();


        for( int j = 0; j < number_of_vertices_by_face; ++j )
        {
            unsigned int id = vertices_of_face[ j ];
            meshcolors[ 3*id ] = c.x()/255;
            meshcolors[ 3*id + 1 ] = c.y()/255;
            meshcolors[ 3*id + 2 ] = c.z()/255;

        }

    }

    mesh.setColor( meshcolors );
    update();

}


void FlowVisualizationCanvas::disableCrossSection( bool status )
{

    apply_crosssection = status;

    if( status == false ){
        mesh.disableCrossSection();
        glDisable(GL_CLIP_DISTANCE0);
    }
    update();

}


void FlowVisualizationCanvas::setCurrentColormap( const ColorMap::COLORMAP& cm )
{
    current_colormap = cm;
    setColorMap();
}


void FlowVisualizationCanvas::updateMesh()
{


    std::vector< double > vertices_double;
    std::vector< unsigned int > faces;

    controller->buildSurfaceSkeleton( vertices_double, faces );

    std::vector< float > vertices;
    vertices.assign( vertices_double.begin(), vertices_double.end() );

    mesh.setMeshType( Mesh::TYPE::QUADRILATERAL );
    mesh.setVertices( vertices );
    mesh.setFaces( faces );
    mesh.buildBoundingBox();


    mesh.showVertices( rendering_menu->showVertices() );
    mesh.showEdges( rendering_menu->showEdges() );
    mesh.showFaces( rendering_menu->showFaces() );

    mesh.load();


    if( current_colormap == ColorMap::COLORMAP::CONSTANT )
        setConstantColor();
    else
        setColorMap();

    update();

}



void FlowVisualizationCanvas::updateCornerPoint()
{


    std::vector< float > vertices_double;
    std::vector< unsigned int > edges;
    std::vector< unsigned int > faces ;

    controller->updateCornerPoint( vertices_double, edges, faces );


    std::vector< float > vertices;
    vertices.assign( vertices_double.begin(), vertices_double.end() );

    mesh.setMeshType( Mesh::TYPE::QUADRILATERAL );
    mesh.setVertices( vertices );
    mesh.setWireframe( edges );
    mesh.setFaces( faces );
    mesh.buildBoundingBox();


    mesh.showVertices( rendering_menu->showVertices() );
    mesh.showEdges( rendering_menu->showEdges() );
    mesh.showFaces( rendering_menu->showFaces() );


    mesh.load();

    std::cout << "vertices_double.size()" <<vertices_double.size() << "\n" <<std::flush;

    if( current_colormap == ColorMap::COLORMAP::CONSTANT )
        setConstantColor();
    else
        setColorMap();

    update();

}



void FlowVisualizationCanvas::updateVolumetricMesh()
{


    std::vector< float > vertices;
    std::vector< unsigned int > edges;
    std::vector< unsigned int > faces ;


    controller->updateVolumetricMesh( vertices, edges, faces );

    mesh.setMeshType( Mesh::TYPE::TETRAHEDRAL );
    mesh.setVertices( vertices );
    mesh.setWireframe( edges );
    mesh.setFaces( faces );
    mesh.buildBoundingBox();


    mesh.showVertices( rendering_menu->showVertices() );
    mesh.showEdges( rendering_menu->showEdges() );
    mesh.showFaces( rendering_menu->showFaces() );


    mesh.load();

    if( current_colormap == ColorMap::COLORMAP::CONSTANT )
        setConstantColor();
    else
        setColorMap();


    update();


}


void FlowVisualizationCanvas::clear()
{
    mesh.clear();
    crosssection.clear();

    coloring_property_name.clear();
    coloring_property_method.clear();
    coloring_property_type.clear();

    apply_crosssection = false;

    camera.reset();

    rendering_menu->clear();
    colorbar->clear();


    setCurrentColormap( ColorMap::COLORMAP:: CONSTANT );

    update();
}


void FlowVisualizationCanvas::mouseMoveEvent( QMouseEvent *event )
{

    Eigen::Vector2f mouse_pos( event->x(), event->y() );


    if( ( event->modifiers() & Qt::ShiftModifier ) && ( event->button ( ) == Qt::LeftButton ) )
        camera.translateCamera( mouse_pos );

    else if ( event->buttons() & Qt::LeftButton )
        camera.rotateCamera( mouse_pos );

    else if ( event->buttons() & Qt::RightButton )
    {

        float x = 2*event->screenPos().x()/width() - 1.0f ;
        float y = -2*event->screenPos().y()/height() + 1.0f;
        float z = 1 - x*x - y*y;

        if( z >= 0 ) z = sqrt( z );
        else z = 0;

        double length = sqrt( x*x + y*y + z*z );
        if( length == 0.0f ) return;

        x /= length;
        y /= length;
        z /= length;

        Eigen::Affine3f V = camera.getViewMatrix();
        Eigen::Matrix4f P = camera.getProjectionMatrix();

        crosssection.updatePosition( QVector3D( x, y, z ) );

        float a = 0.0f, b = 0.0f , c = 0.0f , d = 0.0f;
        crosssection.getPlaneEquation( a, b, c, d );
        mesh.setCrossSectionClippingEquation( a, b, c, d );

    }

    update();

}



void FlowVisualizationCanvas::mousePressEvent(QMouseEvent *event)
{
    Eigen::Vector2f mouse_pos( event->x(), event->y() );


    setFocus();

    if( ( event->modifiers() & Qt::ShiftModifier ) && (event->button ( ) == Qt::LeftButton ) )
        camera.translateCamera( mouse_pos );

    if( ( event->buttons() & Qt::RightButton ) /*&& ( event->modifiers() == Qt::ControlModifier )*/ )
    {
        rendering_menu->exec( event->globalPos() );
    }

}


void FlowVisualizationCanvas::mouseReleaseEvent( QMouseEvent *event )
{

    if( event->button() == Qt::LeftButton )
    {
        camera.endTranslation();
        camera.endRotation();
    }

    update();
}


void FlowVisualizationCanvas::wheelEvent( QWheelEvent *event )
{
    float pos = event->delta()/ float( speed_zoom );

    if( event->modifiers() & Qt::ShiftModifier )
        camera.incrementFov( pos );

    else
    {
        if( pos > 0 ){
            camera.increaseZoom( 1.05f );
        }
        else if( pos < 0 ){
            camera.increaseZoom( 1/1.05f );
        }


    }

    update();
}


void FlowVisualizationCanvas::keyPressEvent( QKeyEvent *event )
{

    Eigen::Affine3f V = camera.getViewMatrix();
    Eigen::Matrix4f P = camera.getProjectionMatrix();

    switch( event->key() )
    {


        case Qt::Key_A:
        {
            show_axis = !show_axis;

        } break;

        case Qt::Key_C:
        {
//            show_colorbar = !show_colorbar;
        } break;


        case Qt::Key_U:
        {
//            reloadShaders();
        } break;

        case Qt::Key_R:
        {
            camera.reset();
        } break;

        case Qt::Key_L:
        {
//            emit controller->getSurfaceCrossSection();
        } break;

        case Qt::Key_F:
        {
//            emit controller->readFile();
        } break;

        case Qt::Key_E:
        {
//            emit controller->editParameters();
        } break;

        case Qt::Key_P:
        {
//            emit controller->applyCrossSection();
        } break;

        case Qt::Key_X:
        {
            if( apply_crosssection == true )
            {
                crosssection.setNormal( QVector3D( 1.0f, 0.0f, 0.0f ) );
                crosssection.init();
                crosssection.load();

                float a = 0.0f, b = 0.0f , c = 0.0f , d = 0.0f;
                crosssection.getPlaneEquation( a, b, c, d );
                mesh.setCrossSectionClippingEquation( a, b, c, d );

            }



        } break;


        case Qt::Key_Y:
        {
            if( apply_crosssection == true )
            {
                crosssection.setNormal( QVector3D( 0.0f, 1.0f, 0.0f ) );
                crosssection.init();
                crosssection.load();


                float a = 0.0f, b = 0.0f , c = 0.0f , d = 0.0f;
                crosssection.getPlaneEquation( a, b, c, d );
                mesh.setCrossSectionClippingEquation( a, b, c, d );
            }



        } break;

        case Qt::Key_Z:
        {
            if( apply_crosssection == true )
            {

                crosssection.setNormal( QVector3D(  0.0f, 0.0f, 1.0f ) );
                crosssection.init();
                crosssection.load();

                float a = 0.0f, b = 0.0f , c = 0.0f , d = 0.0f;
                crosssection.getPlaneEquation( a, b, c, d );
                mesh.setCrossSectionClippingEquation( a, b, c, d );

            }


        } break;

        case Qt::Key_Up:
        {

            if( apply_crosssection == true )
            {
                float step = 1.0f;
                crosssection.updatePosition( V, P, step );

                float a = 0.0f, b = 0.0f , c = 0.0f , d = 0.0f;
                crosssection.getPlaneEquation( a, b, c, d );
                mesh.setCrossSectionClippingEquation( a, b, c, d );
            }



        }break;
        case Qt::Key_Down:
        {

            if( apply_crosssection == true )
            {
                float step = -1.0f;
                crosssection.updatePosition( V, P, step );

                float a = 0.0f, b = 0.0f , c = 0.0f , d = 0.0f;
                crosssection.getPlaneEquation( a, b, c, d );
                mesh.setCrossSectionClippingEquation( a, b, c, d );

            }


        }break;
        case Qt::Key_Right:
        {

            if( apply_crosssection == true )
            {
                float step = 1.0f;
                crosssection.updatePosition( V, P, step );

                float a = 0.0f, b = 0.0f , c = 0.0f , d = 0.0f;
                crosssection.getPlaneEquation( a, b, c, d );
                mesh.setCrossSectionClippingEquation( a, b, c, d );

            }


        }break;
        case Qt::Key_Left:
        {

            if( apply_crosssection == true )
            {
                float step = -1.0f;
                crosssection.updatePosition( V, P, step );

                float a = 0.0f, b = 0.0f , c = 0.0f , d = 0.0f;
                crosssection.getPlaneEquation( a, b, c, d );
                mesh.setCrossSectionClippingEquation( a, b, c, d );
            }


        }break;


        case Qt::Key_Delete:
        {
            emit controller->clearAll();
        } break;

        case Qt::Key_H:
        {
//            emit controller->showToolbarhideToolbar( false );
        } break;

        default:
            break;


    }

    update();
}


void FlowVisualizationCanvas::setAleatoryColorMap()
{
    setCurrentColormap( ColorMap::COLORMAP::JET );
    setColorMap();
}


FlowVisualizationCanvas::~FlowVisualizationCanvas()
{
    mesh.resetBuffers();
    crosssection.resetBuffers();
    axes.resetBuffers();



//    colorbar.resetBuffers();
}

