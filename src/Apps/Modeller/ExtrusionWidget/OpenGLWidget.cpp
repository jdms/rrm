#include "OpenGLWidget.hpp"

GLWidget::GLWidget ( QWidget* parent ) : QOpenGLWidget ( parent )
{

    createMenu();
    init();

    extrusion_controller_.module_ = RRM::ExtrusionController::BlankScreen;
    stepx = 1;
    stepz = 20;
    volume_width = 400;

}


GLWidget::~GLWidget()
{
    resetBuffers();
}




void GLWidget::createMenu()
{
    module_menu = new QMenu( this );

    ac_seismic_module = new QAction( tr ("Seismic Module"), module_menu );
    ac_blankscreen_module = new QAction( tr ("Black Screen Module"), module_menu );

    module_menu->addSection ( tr ( "Module" ) );
    module_menu->addAction( ac_seismic_module );
    module_menu->addAction( ac_blankscreen_module );

    connect ( ac_seismic_module , SIGNAL( triggered() ), this, SLOT( setSeismicModule() ) );
    connect ( ac_blankscreen_module , SIGNAL( triggered() ), this, SLOT( setBlackScreenModule() ) );

}


void GLWidget::init()
{


    shader_background = 0;


    // bounding box in the blanckscreen module
    shader_boundingbox_blankscreen = 0;
    va_boundingbox_blankscreen = 0;
    vb_boundingbox_blankscreen = 0;
    slot_boundingbox_vertex = 0;


    // interpolated surface
    shader_interpolated_surface = 0;
    va_interpolated_surface = 0;
    vb_surface_vertices = 0;
    vb_surface_color = 0;
    slot_surface_vertex = 0;
    slot_surface_color = 2;



    // The seismic sketches
    shader_seimisc_sketches = 0;
    va_seismic_sketches = 0;
    vb_seismic_sketches_vertex = 0;
    slot_seismic_sketches_vertex = 0;



    // The seismic meshes
    shader_seismic_mesh = 0;
    va_seismic_mesh = 0;
    vb_seismic_mesh_vertex = 0;
    vb_seismic_mesh_color = 0;
    vb_seismic_mesh_normal = 0;
    vb_seismic_mesh_idfaces = 0;
    slot_seismic_mesh_vertex = 0;
    slot_seismic_mesh_normal = 1;
    slot_seismic_mesh_color = 2;



    // bounding box seismic
    shader_boundingbox_seismic = 0;
    va_boundingbox_seismic = 0;
    vb_boundingbox_seismic_vertex = 0;
    slot_boundingbox_seismic_vertex = 0;


    // plane slice seismic
    shader_plane_seismic = 0;
    va_plane_seismic = 0;
    vb_plane_seismic_vertex = 0;
    slot_plane_seismic_vertex = 0;
    seismic_plane_sliceid = 0;
    seismic_plane_slice_position = 0.0f;



    vertexArray_for_the_Cube_ = 0;
    vertexBuffer_cube_8vertices_ = 0;
    vertices_8slot_ = 0;
    vertexBuffer_cube_8verticesIndices_ = 0;


}


void GLWidget::clear()
{


    seismic_sketches.clear();
    seismic_mesh_faces.clear();
    seismic_mesh_vertices.clear();
    seismic_mesh_normals.clear();
    seismic_mesh_colors.clear();



    glBindBuffer ( GL_ARRAY_BUFFER , vb_seismic_sketches_vertex );
    glBufferData ( GL_ARRAY_BUFFER , seismic_sketches.size() * sizeof ( seismic_sketches[0] ) , seismic_sketches.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );


    glBindBuffer ( GL_ARRAY_BUFFER , vb_seismic_mesh_vertex );
    glBufferData ( GL_ARRAY_BUFFER , seismic_mesh_vertices.size() * sizeof ( seismic_mesh_vertices[0] ) , seismic_mesh_vertices.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );

    glBindBuffer ( GL_ARRAY_BUFFER , vb_seismic_mesh_normal );
    glBufferData ( GL_ARRAY_BUFFER , seismic_mesh_normals.size() * sizeof ( seismic_mesh_normals[0] ) , seismic_mesh_normals.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );

    glBindBuffer ( GL_ARRAY_BUFFER , vb_seismic_mesh_color );
    glBufferData(GL_ARRAY_BUFFER, seismic_mesh_normals.size() * sizeof (seismic_mesh_normals[0]), seismic_mesh_normals.data(), GL_STATIC_DRAW);
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );

    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vb_seismic_mesh_idfaces );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER , seismic_mesh_faces.size() * sizeof ( seismic_mesh_faces[0] )  , seismic_mesh_faces.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 );


}





void GLWidget::initializeGL ( )
{

    connect( context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::resetBuffers );

    setFocus();
    setMouseTracking ( true );
    setFocusPolicy ( Qt::StrongFocus );



    glewExperimental = GL_TRUE;
    GLenum glewInitResult = glewInit();

    if ( GLEW_OK != glewInitResult )
    {
        std::cerr << "Error: " << glewGetErrorString ( glewInitResult ) << endl;
        exit ( EXIT_FAILURE );
    }



    glClearColor ( 0.0f , 0.0 , 1.0 , 1.0f );
    glEnable ( GL_DEPTH_TEST );
    glDisable ( GL_CULL_FACE );
    glEnable ( GL_BLEND );
    glBlendFunc ( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
    glEnable(GL_MULTISAMPLE);
    glMinSampleShading(1.0f);




    createBlankScreenBuffers();
    createSeismicBuffers();


    loadShaderByResources();



    create8VerticesIndices();
    extrusionInitialize(0.0,0.0,0.0,596.0,291.0,297.0);



    camera.setPerspectiveMatrix ( 60.0 , (float) width()/(float)height ( ) , 0.1f , 100.0f );


    updateRendering();
    updateBlackScreen( cross_section_ );


}


void GLWidget::resizeGL ( int width , int height )
{

    glViewport ( 0 , 0 , width , height );

    camera.setViewport( Eigen::Vector2f ( (float) width , (float) height ) );
    camera.setPerspectiveMatrix( camera.getFovy() , (float) width/(float)height , 0.1f , 100.0f );

}


void GLWidget::paintGL ( )
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


    background();


    if ( extrusion_controller_.module_ == RRM::ExtrusionController::Seismic )
    {
        renderSeismic();
    }
    else if ( extrusion_controller_.module_ == RRM::ExtrusionController::BlankScreen )
    {
        renderBlankScreen();
    }


}






void GLWidget::createBlankScreenBuffers()
{


    glGenVertexArrays ( 1 , &va_boundingbox_blankscreen );
    glBindVertexArray ( va_boundingbox_blankscreen );

        glGenBuffers ( 1 , &vb_boundingbox_blankscreen);
        glBindBuffer ( GL_ARRAY_BUFFER , vb_boundingbox_blankscreen );
            glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( slot_boundingbox_vertex );
        glVertexAttribPointer ( slot_boundingbox_vertex , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

    glBindVertexArray ( 0 );




    glGenVertexArrays ( 1 , &va_interpolated_surface );
    glBindVertexArray ( va_interpolated_surface );

        glGenBuffers ( 1 , &vb_surface_vertices );
        glBindBuffer ( GL_ARRAY_BUFFER , vb_surface_vertices );
            glBufferData ( GL_ARRAY_BUFFER , 0 , 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( slot_surface_vertex );
        glVertexAttribPointer ( slot_surface_vertex , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );


        glGenBuffers(1, &vb_surface_color);
        glBindBuffer(GL_ARRAY_BUFFER, vb_surface_color);
            glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
        glEnableVertexAttribArray(slot_surface_color);
        glVertexAttribPointer(slot_surface_color, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray ( 0 );


}


void GLWidget::createSeismicBuffers()
{


    glGenVertexArrays ( 1 , &va_seismic_sketches );
    glBindVertexArray ( va_seismic_sketches );

        glGenBuffers ( 1 , &vb_seismic_sketches_vertex );
        glBindBuffer ( GL_ARRAY_BUFFER , vb_seismic_sketches_vertex );
            glBufferData ( GL_ARRAY_BUFFER , 0 , 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( slot_seismic_sketches_vertex );
        glVertexAttribPointer ( slot_seismic_sketches_vertex , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );

    glBindVertexArray ( 0 );




    glGenVertexArrays ( 1 , &va_seismic_mesh );
    glBindVertexArray ( va_seismic_mesh );


        glGenBuffers ( 1 , &vb_seismic_mesh_vertex );
        glBindBuffer ( GL_ARRAY_BUFFER , vb_seismic_mesh_vertex );
            glBufferData ( GL_ARRAY_BUFFER , 0 , 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( slot_seismic_mesh_vertex );
        glVertexAttribPointer ( slot_seismic_mesh_vertex , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );


        glGenBuffers ( 1 , &vb_seismic_mesh_normal );
        glBindBuffer ( GL_ARRAY_BUFFER , vb_seismic_mesh_normal );
            glBufferData ( GL_ARRAY_BUFFER , 0 , 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( slot_seismic_mesh_normal );
        glVertexAttribPointer ( slot_seismic_mesh_normal , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );


        glGenBuffers ( 1 , &vb_seismic_mesh_color );
        glBindBuffer ( GL_ARRAY_BUFFER , vb_seismic_mesh_color );
            glBufferData ( GL_ARRAY_BUFFER , 0 , 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( slot_seismic_mesh_color );
        glVertexAttribPointer ( slot_seismic_mesh_color , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );


        glGenBuffers ( 1 , &vb_seismic_mesh_idfaces );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vb_seismic_mesh_idfaces );
            glBufferData ( GL_ELEMENT_ARRAY_BUFFER , 0 , 0 , GL_STATIC_DRAW );


    glBindVertexArray ( 0 );




    glGenVertexArrays ( 1 , &va_boundingbox_seismic );
    glBindVertexArray ( va_boundingbox_seismic );

        glGenBuffers ( 1 , &vb_boundingbox_seismic_vertex );
        glBindBuffer ( GL_ARRAY_BUFFER , vb_boundingbox_seismic_vertex );
            glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( slot_boundingbox_seismic_vertex );
        glVertexAttribPointer ( slot_boundingbox_seismic_vertex , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );

    glBindVertexArray ( 0 );


    glGenVertexArrays ( 1 , &va_plane_seismic );
    glBindVertexArray ( va_plane_seismic );

        glGenBuffers ( 1 , &vb_plane_seismic_vertex );
        glBindBuffer ( GL_ARRAY_BUFFER , vb_plane_seismic_vertex );
            glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( slot_plane_seismic_vertex );
        glVertexAttribPointer ( slot_plane_seismic_vertex , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );

    glBindVertexArray ( 0 );


}


void GLWidget::resetBuffers()
{


    std::cout << " Reseting Buffers ";


    deleteShaders();


    if (va_boundingbox_blankscreen)
    {
        glDeleteVertexArrays(1, &va_boundingbox_blankscreen);

        if (vb_boundingbox_blankscreen)
        {
            glDeleteBuffers(1, &vb_boundingbox_blankscreen);
        }
    }

    if (va_interpolated_surface)
    {
        glDeleteVertexArrays(1, &va_interpolated_surface);
        if (vb_surface_vertices)
        {
            glDeleteBuffers(1, &vb_surface_vertices);
        }
        if (vb_surface_color)
        {
            glDeleteBuffers(1, &vb_surface_color);
        }
    }

    if (va_seismic_sketches)
    {
        glDeleteVertexArrays(1, &va_seismic_sketches);
        if (vb_seismic_sketches_vertex)
        {
            glDeleteBuffers(1, &vb_seismic_sketches_vertex);
        }
    }

    if (va_seismic_mesh)
    {
        glDeleteVertexArrays(1, &va_seismic_mesh);
        if (vb_seismic_mesh_vertex)
        {
            glDeleteBuffers(1, &vb_seismic_mesh_vertex);
        }
        if (vb_seismic_mesh_normal)
        {
            glDeleteBuffers(1, &vb_seismic_mesh_normal);
        }
        if (vb_seismic_mesh_color)
        {
            glDeleteBuffers(1, &vb_seismic_mesh_color);
        }
    }

    if (va_boundingbox_seismic)
    {
        glDeleteVertexArrays(1, &va_boundingbox_seismic);
        if (vb_boundingbox_seismic_vertex)
        {
            glDeleteBuffers(1, &vb_boundingbox_seismic_vertex);
        }
    }

    if (va_plane_seismic)
    {
        glDeleteVertexArrays(1, &va_plane_seismic);
        if (vb_plane_seismic_vertex)
        {
            glDeleteBuffers(1, &vb_plane_seismic_vertex);
        }
    }

    if (vertexArray_for_the_Cube_)
    {
        glDeleteVertexArrays(1, &vertexArray_for_the_Cube_);
        if (vertexBuffer_cube_8vertices_)
        {
            glDeleteBuffers(1, &vertexBuffer_cube_8vertices_);
        }
    }

    // Mesh Layout:
    // - Geometry   vec4  slot = 0
    // - Normal     vec4  slot = 1
    // - Colour     vec4  slot = 2
    // - Attributes vec4  slot = 3

    init();

}


void GLWidget::create8VerticesIndices ()
{

    vertices.clear();
    indices.clear();

    glGenVertexArrays ( 1 , &vertexArray_for_the_Cube_);
    glBindVertexArray(vertexArray_for_the_Cube_);

    // Care about the type: GL_DOUBLE or GL_FLOAT
    GLfloat vertex_data_8[24] =
    {
     // X Y Z
          1.0, 1.0, 1.0, // vertex 0
          1.0, 1.0,-1.0, // vertex 1
         -1.0, 1.0,-1.0, // vertex 2
         -1.0, 1.0, 1.0, // vertex 3

         1.0,-1.0, 1.0, // vertex 4
        -1.0,-1.0, 1.0, // vertex 5
        -1.0,-1.0,-1.0, // vertex 6
         1.0,-1.0,-1.0  // vertex 7
    }; // 8 vertices with 3 components ( Real ) each.

    // Care about the type: GL_INT
    GLuint vertex_indices_8[] =
    {
       // Top Face 		// Bottom
       0, 1, 2, 2, 3, 0,    4, 5, 6, 6, 7, 4,

       // Front 		// Back
       0, 3, 5, 5, 4, 0,   2, 1, 7, 7, 6, 2,

       // Right   	 	// Left
       0, 4, 7, 7, 1, 0,    2, 6, 5, 5, 3, 2
    };// 2 Triangle per face.


    std::copy( vertex_data_8   , vertex_data_8    + 24  , std::back_inserter ( vertices ) );
    std::copy( vertex_indices_8, vertex_indices_8 + 36 , std::back_inserter ( indices ) );

    /// Requesting Vertex Buffers to the GPU
    glGenBuffers ( 1 , &vertexBuffer_cube_8vertices_ );
        glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_cube_8vertices_ );
        glBufferData ( GL_ARRAY_BUFFER , vertices.size( ) * sizeof(vertices[0]) , vertices.data() , GL_STATIC_DRAW );
        // Set up generic attributes pointers
        glEnableVertexAttribArray(vertices_8slot_);
        glVertexAttribPointer(vertices_8slot_, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /// Requesting Indices
    glGenBuffers ( 1 , &vertexBuffer_cube_8verticesIndices_ );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, vertexBuffer_cube_8verticesIndices_ );
        glBufferData ( GL_ELEMENT_ARRAY_BUFFER , indices.size( ) * sizeof(indices[0]) , indices.data() , GL_STATIC_DRAW );

    glBindVertexArray(0);

}





void GLWidget::loadShaderByResources()
{


    QDir shadersDir = QDir ( qApp->applicationDirPath() ) ;


#if defined(_WIN32) || defined(_WIN64) // Windows Directory Style
    /* Do windows stuff */
    QString shaderDirectory (shadersDir.path ()+"\\");
#elif defined(__linux__)               // Linux Directory Style
    /* Do linux stuff */
    QString shaderDirectory ( shadersDir.path ( ) + "/" );
#else
    /* Error, both can't be defined or undefined same time */
    std::cout << "Operate System not supported !"
    halt();
#endif


    loadShaders(shaderDirectory);

}


void GLWidget::loadShaders( const QString& shaderDirectory )
{

    shader_boundingbox_blankscreen = new Tucano::Shader( "Cube", (shaderDirectory + "Shaders/BlankScreenCube.vert").toStdString(),
        (shaderDirectory + "Shaders/BlankScreenCube.frag").toStdString(),
        (shaderDirectory + "Shaders/BlankScreenCube.geom").toStdString(), "", "");
    shader_boundingbox_blankscreen->initialize();


    shader_interpolated_surface = new Tucano::Shader("Patch", (shaderDirectory + "Shaders/SinglePassWireframe.vert").toStdString(),
        (shaderDirectory + "Shaders/SinglePassWireframe.frag").toStdString(),
        (shaderDirectory + "Shaders/SinglePassWireframe.geom").toStdString(), "", "");
    shader_interpolated_surface->initialize();


    shader_seimisc_sketches = new Tucano::Shader("Lines", (shaderDirectory + "Shaders/SketchCurve.vert").toStdString(),
        (shaderDirectory + "Shaders/SketchCurve.frag").toStdString(),
        (shaderDirectory + "Shaders/SketchCurve.geom").toStdString());
    shader_seimisc_sketches->initialize();


    shader_background = new Tucano::Shader("BackGround", (shaderDirectory + "Shaders/DummyQuad.vert").toStdString(),
        (shaderDirectory + "Shaders/DummyQuad.frag").toStdString(),
        (shaderDirectory + "Shaders/DummyQuad.geom").toStdString(), "", "");
    shader_background->initialize();


    shader_seismic_mesh = new Tucano::Shader("Seismic", (shaderDirectory + "Shaders/Seismic.vert").toStdString(),
        (shaderDirectory + "Shaders/Seismic.frag").toStdString(),
        (shaderDirectory + "Shaders/Seismic.geom").toStdString(), "", "");
    shader_seismic_mesh->initialize();


    shader_boundingbox_seismic = new Tucano::Shader("Seismic  Cube", (shaderDirectory + "Shaders/CubeSinglePassWireframe.vert").toStdString(),
        (shaderDirectory + "Shaders/CubeSinglePassWireframe.frag").toStdString(),
        (shaderDirectory + "Shaders/CubeSinglePassWireframe.geom").toStdString(), "", "");
    shader_boundingbox_seismic->initialize();


    shader_plane_seismic = new Tucano::Shader("Seismic  Plane", (shaderDirectory + "Shaders/SeismicSlicePlane.vert").toStdString(),
        (shaderDirectory + "Shaders/SeismicSlicePlane.frag").toStdString(),
        (shaderDirectory + "Shaders/SeismicSlicePlane.geom").toStdString(), "", "");
    shader_plane_seismic->initialize();
}


void GLWidget::reloadShaders ( )
{

    if ( shader_boundingbox_blankscreen )
    {
        shader_boundingbox_blankscreen->reloadShaders ( );
    }

    if ( shader_seimisc_sketches )
    {
        shader_seimisc_sketches->reloadShaders ( );
    }

    if ( shader_background )
    {
        shader_background->reloadShaders ( );
    }

    if ( shader_seismic_mesh )
    {
        shader_seismic_mesh->reloadShaders ( );
    }

    if ( shader_boundingbox_seismic )
    {
        shader_boundingbox_seismic->reloadShaders ( );
    }

    if ( shader_plane_seismic )
    {
        shader_plane_seismic->reloadShaders ( );
    }

    if ( shader_interpolated_surface )
    {
        shader_interpolated_surface->reloadShaders ( );
    }

}


void GLWidget::deleteShaders()
{


    if ( shader_background )
    {
        delete( shader_background );
        shader_background = nullptr;
    }



    if ( shader_boundingbox_blankscreen )
    {
        delete ( shader_boundingbox_blankscreen );
        shader_boundingbox_blankscreen = nullptr;
    }

    if ( shader_interpolated_surface )
    {
        delete( shader_interpolated_surface );
        shader_interpolated_surface = nullptr;
    }




    if ( shader_seimisc_sketches )
    {
        delete( shader_seimisc_sketches );
        shader_seimisc_sketches = nullptr;
    }

    if ( shader_seismic_mesh )
    {
        delete( shader_seismic_mesh );
        shader_seismic_mesh = nullptr;
    }

    if ( shader_boundingbox_seismic )
    {
        delete( shader_boundingbox_seismic );
        shader_boundingbox_seismic = nullptr;
    }

    if ( shader_plane_seismic )
    {
        delete( shader_plane_seismic );
        shader_plane_seismic = nullptr;
    }


}





void GLWidget::background()
{

    glDisable( GL_DEPTH_TEST );
    glDepthMask( GL_FALSE );

    shader_background->bind();

    shader_background->setUniform( "viewportSize", width(), height() );

    glBindVertexArray ( va_boundingbox_blankscreen );
        glDrawArrays ( GL_POINTS , 0 , 1 );
    glBindVertexArray ( 0 );

    shader_background->unbind();

    glEnable( GL_DEPTH_TEST );
    glDepthMask( GL_TRUE );


}


void GLWidget::renderBlankScreen()
{

    shader_interpolated_surface->bind ( );

    shader_interpolated_surface->setUniform ( "ModelMatrix" , camera.getViewMatrix ( ) );
    shader_interpolated_surface->setUniform ( "ViewMatrix" , camera.getViewMatrix ( ) );
    shader_interpolated_surface->setUniform ( "ProjectionMatrix" , camera.getProjectionMatrix ( ) );
    shader_interpolated_surface->setUniform ( "WIN_SCALE" , (float) width ( ) , (float) height ( ) );

    glBindVertexArray ( va_interpolated_surface );
        glDrawArrays ( GL_LINES_ADJACENCY , 0 , interpolated_surfaces.size() );
    glBindVertexArray ( 0 );

    shader_interpolated_surface->unbind ( );



    shader_boundingbox_blankscreen->bind ( );
    /// 3rd attribute buffer : vertices
    shader_boundingbox_blankscreen->setUniform ( "ModelMatrix" , camera.getViewMatrix ( ) );
    shader_boundingbox_blankscreen->setUniform ( "ViewMatrix" , camera.getViewMatrix ( ) );
    shader_boundingbox_blankscreen->setUniform ( "ProjectionMatrix" , camera.getProjectionMatrix ( ) );
    shader_boundingbox_blankscreen->setUniform ( "WIN_SCALE" , (float) width ( ) , (float) height ( ) );

    glBindVertexArray ( va_boundingbox_blankscreen );
        glDrawArrays ( GL_LINES_ADJACENCY , 0 , boundingbox_blankscreen.size() );
    glBindVertexArray ( 0 );

    shader_boundingbox_blankscreen->unbind ( );

}


void GLWidget::renderSeismic()
{

    glLineWidth(2.0);

    if ( seismic_sketches.size() > 0 )
    {
        shader_seimisc_sketches->bind ( );
        shader_seimisc_sketches->setUniform ( "ModelMatrix" , camera.getViewMatrix ( ) );
        shader_seimisc_sketches->setUniform ( "ViewMatrix" , camera.getViewMatrix ( ) );
        shader_seimisc_sketches->setUniform ( "ProjectionMatrix" , camera.getProjectionMatrix ( ) );

        glBindVertexArray ( va_seismic_sketches );
            glDrawArrays ( GL_LINES , 0 , seismic_sketches.size() );
        glBindVertexArray ( 0 );

        shader_seimisc_sketches->unbind ( );
    }


    // Triangles
    shader_seismic_mesh->bind ( );
    shader_seismic_mesh->setUniform ( "ModelMatrix" , camera.getViewMatrix() );
    shader_seismic_mesh->setUniform ( "ViewMatrix" , camera.getViewMatrix() );
    shader_seismic_mesh->setUniform ( "ProjectionMatrix" , camera.getProjectionMatrix() );
    shader_seismic_mesh->setUniform ( "WIN_SCALE" , (float) width() , (float) height() );

    glBindVertexArray ( va_seismic_mesh );
        glDrawElements ( GL_TRIANGLES , seismic_mesh_faces.size() , GL_UNSIGNED_INT , 0 );
    glBindVertexArray ( 0 );

    shader_seismic_mesh->unbind ( );



    shader_plane_seismic->bind ( );
    shader_plane_seismic->setUniform ( "ModelMatrix", camera.getViewMatrix() );
    shader_plane_seismic->setUniform ( "ViewMatrix", camera.getViewMatrix() );
    shader_plane_seismic->setUniform ( "ProjectionMatrix", camera.getProjectionMatrix() );
    shader_plane_seismic->setUniform ( "WIN_SCALE", (float) width() , (float) height() );
    shader_plane_seismic->setUniform ( "color_plane", 0.0f, 1.0f, 0.0f, 1.0f );
    shader_plane_seismic->setUniform ( "z" , seismic_plane_slice_position );

    glBindVertexArray ( va_plane_seismic );
        glDrawArrays ( GL_LINES_ADJACENCY , 0 , seismic_plane.size() );
    glBindVertexArray ( 0 );

    shader_plane_seismic->unbind ( );



    shader_boundingbox_seismic->bind ( );
    shader_boundingbox_seismic->setUniform ( "ModelMatrix", camera.getViewMatrix() );
    shader_boundingbox_seismic->setUniform ( "ViewMatrix", camera.getViewMatrix() );
    shader_boundingbox_seismic->setUniform ( "ProjectionMatrix", camera.getProjectionMatrix() );
    shader_boundingbox_seismic->setUniform ( "WIN_SCALE" , (float) width(), (float) height() );
    shader_boundingbox_seismic->setUniform ( "color_plane" , 0.5f, 0.5f, 0.5f, 0.2f );


    glBindVertexArray ( va_boundingbox_blankscreen );
        glDrawArrays ( GL_LINES_ADJACENCY , 0 , boundingbox_seismic.size() );
    glBindVertexArray ( 0 );


    shader_boundingbox_seismic->unbind ( );

}






bool GLWidget::extrusionInitialize ( float _x_min, float _y_min, float _z_min, float _x_max, float _y_max,

                                     float _z_max )
{

    extrusion_controller_.initialize( _x_min, _y_min, _z_min, _x_max, _y_max, _z_max );
    boundingbox_seismic = extrusion_controller_.getCubeMesh();
    seismic_plane = extrusion_controller_.getPlaneMesh( 0.0f );
    seismic_plane_slice_position = extrusion_controller_.slicePositon( seismic_plane_sliceid );


    glBindBuffer ( GL_ARRAY_BUFFER , vb_plane_seismic_vertex );
    glBufferData ( GL_ARRAY_BUFFER , seismic_plane.size() * sizeof ( seismic_plane[0] ), seismic_plane.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );

    glBindBuffer ( GL_ARRAY_BUFFER , vb_boundingbox_seismic_vertex );
    glBufferData ( GL_ARRAY_BUFFER , boundingbox_seismic.size ( ) * sizeof ( boundingbox_seismic[0] ) , boundingbox_seismic.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0);


    update();

    return false;

}



void GLWidget::black_screen_stepx( int x )
{

    stepx = static_cast< float >( x );
    extrusion_controller_.updateBlackScreenMesh( stepx, stepz, volume_width, boundingbox_blankscreen, interpolated_surfaces, interpolated_surfaces_colors );

    glBindBuffer ( GL_ARRAY_BUFFER , vb_boundingbox_blankscreen );
    glBufferData ( GL_ARRAY_BUFFER , boundingbox_blankscreen.size() * sizeof ( boundingbox_blankscreen[0] ) , boundingbox_blankscreen.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0);

    glBindBuffer ( GL_ARRAY_BUFFER , vb_surface_vertices );
    glBufferData ( GL_ARRAY_BUFFER , interpolated_surfaces.size () * sizeof ( interpolated_surfaces[0] ) , interpolated_surfaces.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0);

    glBindBuffer(GL_ARRAY_BUFFER, vb_surface_color);
    glBufferData(GL_ARRAY_BUFFER, interpolated_surfaces_colors.size() * sizeof (interpolated_surfaces_colors[0]), interpolated_surfaces_colors.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    update();

}


void GLWidget::black_screen_stepz ( int z )
{

    stepz = static_cast<float>( z );
    extrusion_controller_.updateBlackScreenMesh( stepx, stepz, volume_width, boundingbox_blankscreen, interpolated_surfaces, interpolated_surfaces_colors );

    glBindBuffer ( GL_ARRAY_BUFFER , vb_boundingbox_blankscreen );
    glBufferData ( GL_ARRAY_BUFFER , boundingbox_blankscreen.size() * sizeof ( boundingbox_blankscreen[0] ) , boundingbox_blankscreen.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0);

    glBindBuffer ( GL_ARRAY_BUFFER , vb_surface_vertices );
    glBufferData ( GL_ARRAY_BUFFER , interpolated_surfaces.size() * sizeof ( interpolated_surfaces[0] ) , interpolated_surfaces.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0);

    glBindBuffer(GL_ARRAY_BUFFER, vb_surface_color);
    glBufferData(GL_ARRAY_BUFFER, interpolated_surfaces_colors.size() * sizeof (interpolated_surfaces_colors[0]), interpolated_surfaces_colors.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    update();

}


void GLWidget::black_screen_volumeWidth ( int w )
{

    volume_width = static_cast< float >( w );
    extrusion_controller_.updateBlackScreenMesh( stepx, stepz, volume_width, boundingbox_blankscreen, interpolated_surfaces, interpolated_surfaces_colors );

    glBindBuffer ( GL_ARRAY_BUFFER , vb_boundingbox_blankscreen );
    glBufferData ( GL_ARRAY_BUFFER , boundingbox_blankscreen.size ( ) * sizeof ( boundingbox_blankscreen[0] ) , boundingbox_blankscreen.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0);

    glBindBuffer ( GL_ARRAY_BUFFER , vb_surface_vertices );
    glBufferData ( GL_ARRAY_BUFFER , interpolated_surfaces.size ( ) * sizeof ( interpolated_surfaces[0] ) , interpolated_surfaces.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0);

    glBindBuffer(GL_ARRAY_BUFFER, vb_surface_color);
    glBufferData(GL_ARRAY_BUFFER, interpolated_surfaces_colors.size() * sizeof (interpolated_surfaces_colors[0]), interpolated_surfaces_colors.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    update();

}



void GLWidget::setPlanePosition( int _index )
{
    seismic_plane_slice_position = extrusion_controller_.slicePositon( _index );
    update();
}






void GLWidget::updateSeismicSlices ( const SeismicSlices& _seismic_slices )
{

    extrusion_controller_.setSeismicSlices( _seismic_slices );

}


void GLWidget::updateRendering()
{


    seismic_sketches.clear();
    seismic_mesh_faces.clear();

    seismic_sketches = extrusion_controller_.updateSeismicSlices( seismic_mesh_vertices, seismic_mesh_normals, seismic_mesh_colors, list_of_faces);

    glBindBuffer ( GL_ARRAY_BUFFER , vb_seismic_sketches_vertex );
    glBufferData ( GL_ARRAY_BUFFER , seismic_sketches.size ( ) * sizeof ( seismic_sketches[0] ) , seismic_sketches.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );


    for (auto f: list_of_faces )
    {
        seismic_mesh_faces.push_back( GLuint( f ) );
    }

    glBindBuffer ( GL_ARRAY_BUFFER , vb_seismic_mesh_vertex );
    glBufferData ( GL_ARRAY_BUFFER , seismic_mesh_vertices.size() * sizeof ( seismic_mesh_vertices[0] ) , seismic_mesh_vertices.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );

    glBindBuffer ( GL_ARRAY_BUFFER , vb_seismic_mesh_normal );
    glBufferData ( GL_ARRAY_BUFFER , seismic_mesh_normals.size() * sizeof ( seismic_mesh_normals[0] ) , seismic_mesh_normals.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );

    glBindBuffer ( GL_ARRAY_BUFFER , vb_seismic_mesh_color );
    glBufferData(GL_ARRAY_BUFFER, seismic_mesh_colors.size() * sizeof (seismic_mesh_colors[0]), seismic_mesh_colors.data(), GL_STATIC_DRAW);
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );

    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vb_seismic_mesh_idfaces );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER , seismic_mesh_faces.size() * sizeof ( seismic_mesh_faces[0] )  , seismic_mesh_faces.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 );


    update();


}


void GLWidget::updateSeismicResolution( int res )
{

    seismic_mesh_faces.clear();

    extrusion_controller_.setResolution( res, seismic_mesh_vertices, seismic_mesh_normals, seismic_mesh_colors, list_of_faces );

    for (auto f: list_of_faces )
    {
        seismic_mesh_faces.push_back( GLuint( f ) );
    }

    glBindBuffer ( GL_ARRAY_BUFFER , vb_seismic_mesh_vertex );
    glBufferData ( GL_ARRAY_BUFFER , seismic_mesh_vertices.size() * sizeof ( seismic_mesh_vertices[0] ) , seismic_mesh_vertices.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );

    glBindBuffer ( GL_ARRAY_BUFFER , vb_seismic_mesh_normal );
    glBufferData ( GL_ARRAY_BUFFER , seismic_mesh_normals.size() * sizeof ( seismic_mesh_normals[0] ) , seismic_mesh_normals.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );

    glBindBuffer ( GL_ARRAY_BUFFER , vb_seismic_mesh_color );
    glBufferData(GL_ARRAY_BUFFER, seismic_mesh_colors.size() * sizeof (seismic_mesh_colors[0]), seismic_mesh_colors.data(), GL_STATIC_DRAW);
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );

    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vb_seismic_mesh_idfaces );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER , seismic_mesh_faces.size() * sizeof ( seismic_mesh_faces[0] )  , seismic_mesh_faces.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 );

    update();

}


void GLWidget::updateBlackScreen( const CrossSection& _cross_section )
{

    cross_section_ = _cross_section;

    extrusion_controller_.setBlackScreenCrossSection( _cross_section );
    extrusion_controller_.updateBlackScreenMesh( stepx, stepz, volume_width, boundingbox_blankscreen, interpolated_surfaces, interpolated_surfaces_colors );

    glBindBuffer ( GL_ARRAY_BUFFER , vb_boundingbox_blankscreen );
    glBufferData ( GL_ARRAY_BUFFER , boundingbox_blankscreen.size ( ) * sizeof ( boundingbox_blankscreen[0] ) , boundingbox_blankscreen.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0);

    glBindBuffer ( GL_ARRAY_BUFFER , vb_surface_vertices );
    glBufferData ( GL_ARRAY_BUFFER , interpolated_surfaces.size ( ) * sizeof ( interpolated_surfaces[0] ) , interpolated_surfaces.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0);

    glBindBuffer( GL_ARRAY_BUFFER, vb_surface_color );
    glBufferData( GL_ARRAY_BUFFER, interpolated_surfaces_colors.size() * sizeof ( interpolated_surfaces_colors[0]), interpolated_surfaces_colors.data(), GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );


    update();
}







void GLWidget::keyPressEvent ( QKeyEvent * event )
{
    switch ( event->key ( ) )
    {
        case Qt::Key_F5:
        {
            reloadShaders ( );
        }
        break;

        case Qt::Key_R:
        {
            camera.reset ( );
        }
        break;

        case Qt::Key_Space:
        {
            updateRendering();
        }
        break;

        case Qt::Key_F2:
        {
            updateSeismicResolution( 8 );
        }
        break;


        case Qt::Key_F3:
        {
            updateSeismicResolution( 16 );
        }
        break;


        case Qt::Key_F4:
        {
            updateSeismicResolution( 32 );
        }
        break;

        default:
            break;


    }

    update ( );
}


void GLWidget::mousePressEvent ( QMouseEvent *event )
{

    setFocus();


    if ( ( event->buttons() & Qt::RightButton ) && ( event->modifiers() & Qt::AltModifier )  )
    {
        module_menu->exec ( event->globalPos ( ) );
    }



    Eigen::Vector2f screen_pos ( event->x() , event->y() );

    if ( event->modifiers() & Qt::ShiftModifier )
    {
        if ( event->button() == Qt::LeftButton )
        {
            camera.translateCamera( screen_pos );
        }

    }
    else
    {
        if ( event->button() == Qt::LeftButton )
        {
            camera.rotateCamera( screen_pos );
        }
    }


    update();

}


void GLWidget::mouseMoveEvent ( QMouseEvent *event )
{

    Eigen::Vector2f screen_pos ( event->x() , event->y() );


    if ( ( event->modifiers() & Qt::ShiftModifier )  )
    {

        if ( event->buttons() & Qt::LeftButton )
        {
            camera.translateCamera ( screen_pos );
        }

    }
    else
    {
        if ( event->buttons() & Qt::LeftButton )
        {
            camera.rotateCamera ( screen_pos );
        }

    }

    update( );
}



/// @see http://stackoverflow.com/questions/25426356/how-to-get-the-released-button-inside-mousereleaseevent-in-qt
/// @Merlin069 There is no button pressed on the releaseEvent so the event->buttons()
/// – Othman Benchekroun Aug 21 '14 at 12:38is equal to 0
void GLWidget::mouseReleaseEvent ( QMouseEvent *event )
{

    if ( event->button ( ) == Qt::LeftButton )
    {
        camera.endTranslation ( );
        camera.endRotation ( );
    }

    update ( );

}


void GLWidget::wheelEvent ( QWheelEvent *event )
{

    const int WHEEL_STEP = 120;
    float pos = event->delta()/float( WHEEL_STEP );


    if ( event->modifiers ( ) & Qt::ShiftModifier )
    {
        camera.incrementFov ( pos );
    }

    else
    {
        if ( pos > 0 )
        {
            camera.increaseZoom ( 1.05f );
        }

        else if ( pos < 0 )
        {
            camera.increaseZoom ( 1.0f/1.05f );
        }
    }

    update();
}


