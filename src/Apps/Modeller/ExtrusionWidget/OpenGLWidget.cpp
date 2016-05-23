#include "OpenGLWidget.hpp"

GLWidget::GLWidget ( QWidget* parent ) : QOpenGLWidget ( parent )
{

	// Mesh Layout:
	// - Geometry   vec4  slot = 0
	// - Normal     vec4  slot = 1
	// - Colour     vec4  slot = 2
	// - Attributes vec4  slot = 3

	// [v0,v1,v2,v3,n0,n1,n2,n3,c0,c1,c2,c4,att0,att1,att2,att3]

	// Scene
	background_ = 0;

	// Entity
	vertexArray_cube_ = 0;
		vertexBuffer_cube_ = 0;
		// Be careful on the assignment of each slot attributes
		vertexCube_slot_ = 0;
		cube_shader_ = 0;

	// The interpolated surface
	vertexArray_patch_ = 0;
		vertexBuffer_patch_ = 0;
		// Be careful on the assignment of each slot attributes
		vertexPatch_slot_ = 0;
		patch_shader_ = 0;

        // The sketch lines
	lines_vertexArray_ = 0;
		lines_vertexBuffer_ = 0;
		// Be careful on the assignment of each slot attributes
		lines_vertexSlot_ = 0;
		lines_shader_ = 0;
}

/// OpenGL
void GLWidget::initializeGL ( )
{
	/// Key event to GLWidget not o MainWindow ! | @QtDocumentation
	setFocus ( );
	/// If mouse tracking is enabled, the widget receives mouse move events even if no buttons are pressed. | @QtDocmentation
	setMouseTracking ( true );
	setFocusPolicy ( Qt::StrongFocus );

	/// GLEW OpenGL
	/// GLEW Initialisation:
	glewExperimental = GL_TRUE;
	GLenum glewInitResult = glewInit ( );

	//Check Glew Initialisation:
	if ( GLEW_OK != glewInitResult )
	{
		std::cerr << "Error: " << glewGetErrorString ( glewInitResult ) << endl;
		exit ( EXIT_FAILURE );
	}

	glClearColor ( 0.0f , 0.0 , 1.0 , 1.0f );
	glEnable ( GL_DEPTH_TEST );

	glDisable ( GL_CULL_FACE );
	// Enable blending
	glEnable ( GL_BLEND );
	glBlendFunc ( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );


	glGenVertexArrays ( 1 , &vertexArray_cube_ );
	glBindVertexArray ( vertexArray_cube_ );

	/// Requesting Vertex Buffers to the GPU
	glGenBuffers ( 1 , &vertexBuffer_cube_ );
	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_cube_ );
	glBufferData ( GL_ARRAY_BUFFER , cube_.size ( ) * sizeof ( cube_[0] ) , cube_.data() , GL_STATIC_DRAW );
	// Set up generic attributes pointers
	glEnableVertexAttribArray ( vertexCube_slot_ );
	glVertexAttribPointer ( vertexCube_slot_ , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );

	glBindVertexArray ( 0 );

	/// Vertex Array Surface Patch
	glGenVertexArrays ( 1 , &vertexArray_patch_ );
	glBindVertexArray ( vertexArray_patch_ );

	/// Requesting Vertex Buffers to the GPU
	glGenBuffers ( 1 , &vertexBuffer_patch_ );
	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_patch_ );
	glBufferData ( GL_ARRAY_BUFFER , 0 , 0 , GL_STATIC_DRAW );
	/// Set up generic attributes pointers
	glEnableVertexAttribArray ( vertexPatch_slot_ );
	glVertexAttribPointer ( vertexPatch_slot_ , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );

	glBindVertexArray ( 0 );

	/// Vertex Array Lines Patch
	glGenVertexArrays ( 1 , &lines_vertexArray_ );
	glBindVertexArray ( lines_vertexArray_ );

	/// Requesting Vertex Buffers to the GPU
	glGenBuffers ( 1 , &lines_vertexBuffer_ );
	glBindBuffer ( GL_ARRAY_BUFFER , lines_vertexBuffer_ );
	glBufferData ( GL_ARRAY_BUFFER , 0 , 0 , GL_STATIC_DRAW );
	/// Set up generic attributes pointers
	glEnableVertexAttribArray ( lines_vertexSlot_ );
	glVertexAttribPointer ( lines_vertexSlot_ , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );

	glBindVertexArray ( 0 );

	// IMPORTANT FOR THE DEPLOY VERSION
	//loadShaderByResources ( );

	// Lost approximately 4 hours to figure out, that actually, my entire shader
	// was correct, however I was trying to upload the line's geometry before
	// create the appropriate vertex buffer on the GPU
	extrusionInitialize(0.0,0.0,0.0,700.0,400.0,400.0);
	loadShaders();

	camera.setPerspectiveMatrix ( 60.0 , (float) this->width ( ) / (float) this->height ( ) , 0.1f , 100.0f );

}

void GLWidget::reloadShaders ( )
{
	if ( cube_shader_ )
	{
		cube_shader_->reloadShaders ( );
	}
	if ( lines_shader_ )
	{
		lines_shader_->reloadShaders ( );
	}
	if ( background_ )
	{
		background_->reloadShaders ( );
	}
}

void GLWidget::loadShaderByResources ( )
{
	//! Debug Version: to load the update shaders
	qDebug ( ) << "Load by Resources ";

	QDir shadersDir = QDir ( qApp->applicationDirPath ( ) );

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

	//! Effects --
	cube_shader_ = new Tucano::Shader ( "Cube" , ( shaderDirectory + "Shaders/CubeSinglePassWireframe.vert" ).toStdString ( ),
					             ( shaderDirectory + "Shaders/CubeSinglePassWireframe.frag" ).toStdString ( ),
						     ( shaderDirectory + "Shaders/CubeSinglePassWireframe.geom" ).toStdString ( ) , "" , "" );
	cube_shader_->initialize ( );
	//! Effects --
	patch_shader_ = new Tucano::Shader ( "Patch" , ( shaderDirectory + "Shaders/SinglePassWireframe.vert" ).toStdString ( ),
					               ( shaderDirectory + "Shaders/SinglePassWireframe.frag" ).toStdString ( ),
						       ( shaderDirectory + "Shaders/SinglePassWireframe.geom" ).toStdString ( ) , "" , "" );
	patch_shader_->initialize ( );
	//! Effects --
	lines_shader_ = new Tucano::Shader ( "Lines" , ( shaderDirectory + "Shaders/SketchCurve.vert" ).toStdString ( ),
					               ( shaderDirectory + "Shaders/SketchCurve.frag" ).toStdString ( ),
						       ( shaderDirectory + "Shaders/SketchCurve.geom" ).toStdString ( ));
	lines_shader_->initialize ( );

	background_ = new Tucano::Shader ( "BackGround" , ( shaderDirectory + "Shaders/DummyQuad.vert" ).toStdString ( ),
					                  ( shaderDirectory + "Shaders/DummyQuad.frag" ).toStdString ( ),
						          ( shaderDirectory + "Shaders/DummyQuad.geom" ).toStdString ( ), "" , "" );
        background_->initialize ( );

	std::cout << " cube_shader_ " << cube_shader_->getShaderProgram ( ) << std::endl;
	std::cout << " background_ "  << background_->getShaderProgram  ( ) << std::endl;

}


// Development propose
void GLWidget::loadShaders ( )
{
	//! Binary absolute location
	QDir shadersDir = QDir ( qApp->applicationDirPath ( ) );

	//! Debug Version: to load the update shaders
	qDebug ( ) << "Directory " << shadersDir.path ( );
	shadersDir.cdUp ( );
	shadersDir.cdUp ( );
	shadersDir.cdUp ( );
	qDebug ( ) << "Directory " << shadersDir.path ( );

#if defined(_WIN32) || defined(_WIN64) // Windows Directory Style
	/* Do windows stuff */
	QString shaderDirectory (shadersDir.path ()+"\\src\\Extrusion\\GUI\\Qt\\RCC\\Shaders\\");
#elif defined(__linux__)               // Linux Directory Style
	/* Do Linux stuff */
	QString shaderDirectory ( shadersDir.path ( ) + "/src/Apps/Modeller/ExtrusionWidget/" );
#else
	/* Error, both can't be defined or undefined same time */
	std::cout << "Operate System not supported !"
	halt();
#endif

	//! Effects --
	cube_shader_ = new Tucano::Shader ( "Cube" , ( shaderDirectory + "Shaders/SinglePassWireframe.vert" ).toStdString ( ),
					             ( shaderDirectory + "Shaders/SinglePassWireframe.frag" ).toStdString ( ),
						     ( shaderDirectory + "Shaders/SinglePassWireframe.geom" ).toStdString ( ) , "" , "" );
	cube_shader_->initialize ( );
	//! Effects --
	patch_shader_ = new Tucano::Shader ( "Patch" , ( shaderDirectory + "Shaders/SinglePassWireframe.vert" ).toStdString ( ),
					               ( shaderDirectory + "Shaders/SinglePassWireframe.frag" ).toStdString ( ),
						       ( shaderDirectory + "Shaders/SinglePassWireframe.geom" ).toStdString ( ) , "" , "" );
	patch_shader_->initialize ( );
	//! Effects --
	lines_shader_ = new Tucano::Shader ( "Lines" , ( shaderDirectory + "Shaders/SketchCurve.vert" ).toStdString ( ),
					               ( shaderDirectory + "Shaders/SketchCurve.frag" ).toStdString ( ),
						       ( shaderDirectory + "Shaders/SketchCurve.geom" ).toStdString ( ));
	lines_shader_->initialize ( );


	// ! Blue BlackGround --
	background_ = new Tucano::Shader ( "BackGround" , ( shaderDirectory + "Shaders/DummyQuad.vert" ).toStdString ( ),
					                  ( shaderDirectory + "Shaders/DummyQuad.frag" ).toStdString ( ),
						          ( shaderDirectory + "Shaders/DummyQuad.geom" ).toStdString ( ) , "" , "" );
        background_->initialize ( );

	std::cout << " cube_shader_ " << cube_shader_->getShaderProgram ( ) << std::endl;
	std::cout << " background_ "  << background_->getShaderProgram ( ) << std::endl;

}

void GLWidget::backGround()
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	background_->bind();

	background_->setUniform("viewportSize", width(), height() );

	glBindVertexArray ( vertexArray_cube_ );
	/// Draw the triangle !
	glDrawArrays ( GL_POINTS , 0 , 1 );

	glBindVertexArray ( 0 );

	background_->unbind();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

void GLWidget::resizeGL ( int width , int height )
{
	/// What  for the view port transformation. keeping it updated !
	glViewport ( 0 , 0 , width , height );

	camera.setViewport ( Eigen::Vector2f ( (float) width , (float) height ) );
	camera.setPerspectiveMatrix ( camera.getFovy ( ) , (float) width / (float) height , 0.1f , 100.0f );
}
/// Real Looping
void GLWidget::paintGL ( )
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	backGround();

//	patch_shader_->bind ( );
//	/// 3rd attribute buffer : vertices
//	patch_shader_->setUniform ( "ModelMatrix" , camera.getViewMatrix ( ) );
//	patch_shader_->setUniform ( "ViewMatrix" , camera.getViewMatrix ( ) );
//	patch_shader_->setUniform ( "ProjectionMatrix" , camera.getProjectionMatrix ( ) );
//	patch_shader_->setUniform ( "WIN_SCALE" , (float) width ( ) , (float) height ( ) );
//	glBindVertexArray ( vertexArray_patch_ );
//	/// Draw the triangle !
//	glDrawArrays ( GL_LINES_ADJACENCY , 0 , patch_.size ( ) );
//
//	glBindVertexArray ( 0 );
//	patch_shader_->unbind ( );


//	cube_shader_->bind ( );
//	/// 3rd attribute buffer : vertices
//	cube_shader_->setUniform ( "ModelMatrix" , camera.getViewMatrix ( ) );
//	cube_shader_->setUniform ( "ViewMatrix" , camera.getViewMatrix ( ) );
//	cube_shader_->setUniform ( "ProjectionMatrix" , camera.getProjectionMatrix ( ) );
//	cube_shader_->setUniform ( "WIN_SCALE" , (float) width ( ) , (float) height ( ) );
//	glBindVertexArray ( vertexArray_cube_ );
//	/// Draw the triangle !
//	glDrawArrays ( GL_LINES_ADJACENCY , 0 , cube_.size ( ) );
//
//	glBindVertexArray ( 0 );
//	cube_shader_->unbind ( );


	lines_shader_->bind ( );
	/// 3rd attribute buffer : vertices
	lines_shader_->setUniform ( "ModelMatrix" , camera.getViewMatrix ( ) );
	lines_shader_->setUniform ( "ViewMatrix" , camera.getViewMatrix ( ) );
	lines_shader_->setUniform ( "ProjectionMatrix" , camera.getProjectionMatrix ( ) );

	glBindVertexArray ( lines_vertexArray_ );
	/// Draw the triangle !
	glDrawArrays ( GL_LINES , 0 , lines_.size() );

	glBindVertexArray ( 0 );
	lines_shader_->unbind ( );

}
/// KeyInput
void GLWidget::processMultiKeys ( )
{
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

		default:
			break;
	}

	update ( );
}

void GLWidget::keyReleaseEvent ( QKeyEvent * event )
{
}
/// MouseInput
void GLWidget::mousePressEvent ( QMouseEvent *event )
{
	setFocus ( );
	Eigen::Vector2f screen_pos ( event->x ( ) , event->y ( ) );
	if ( event->modifiers ( ) & Qt::ShiftModifier )
	{
		if ( event->button ( ) == Qt::LeftButton )
		{
			camera.translateCamera ( screen_pos );
		}
		else if ( event->button ( ) == Qt::RightButton )
		{
		}
	}
	else
	{
		if ( event->button ( ) == Qt::LeftButton )
		{
			camera.rotateCamera ( screen_pos );
		}
	}

	update ( );
}

void GLWidget::mouseMoveEvent ( QMouseEvent *event )
{
	Eigen::Vector2f screen_pos ( event->x ( ) , event->y ( ) );
	if ( ( event->modifiers ( ) & Qt::ShiftModifier )  )
	{

		if ( event->buttons ( ) & Qt::LeftButton )
		{
			camera.translateCamera ( screen_pos );
		}
		if ( event->buttons ( ) & Qt::RightButton )
		{
		}
	}
	else
	{
		if ( event->buttons ( ) & Qt::LeftButton )
		{
			camera.rotateCamera ( screen_pos );
		}
		if ( event->buttons ( ) & Qt::RightButton )
		{
			//light_trackball.rotateCamera(screen_pos);
		}
	}

	update ( );
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
	if ( (event->button() == Qt::RightButton) )
	{
		//light_trackball.endRotation();
	}


	update ( );
}
/// WheelInput
void GLWidget::wheelEvent ( QWheelEvent *event )
{

	const int WHEEL_STEP = 120;

	float pos = event->delta ( ) / float ( WHEEL_STEP );

	if ( event->modifiers ( ) & Qt::ShiftModifier ) // change FOV
	{
		camera.incrementFov ( pos );
	}
	else // change ZOOM
	{
		if ( ( pos > 0 ) )
		{
			camera.increaseZoom ( 1.05f );
		}

		else if ( pos < 0 )
		{
			camera.increaseZoom ( 1.0f / 1.05f );
		}
	}

	update ( );
}


/// Seismic Module
void GLWidget::updateSeismicSlices ( const SeismicSlices& _seismic_slices )
{
	this->lines_.clear();
	this->lines_ = this->extrusion_controller_.updateSeismicSlices(_seismic_slices);

	/// Send the new meshes to the GPU
	glBindVertexArray ( lines_vertexArray_ );
	glBindBuffer ( GL_ARRAY_BUFFER , lines_vertexBuffer_ );
	glBufferData ( GL_ARRAY_BUFFER , lines_.size ( ) * sizeof ( lines_[0] ) , lines_.data() , GL_STATIC_DRAW );
	glBindVertexArray ( 0 );

	update();
}


bool GLWidget::extrusionInitialize ( float _x_min,
				     float _y_min,
				     float _z_min,
				     float _x_max,
				     float _y_max,
				     float _z_max )
{
	this->extrusion_controller_.initialize(_x_min,_y_min,_z_min,_x_max,_y_max,_z_max);

//	std::vector<unsigned int> plane_positions = {5,10,30,40,60,90};
//
//	std:vector<Eigen::Vector4f> triangles;
//
//	/// Rebuild the scene bounding Box
//	cube_.clear();
//
//	cube_ = extrusion_controller_.getPlanes( plane_positions );
//
//	triangles = extrusion_controller_.getcubeMesh();
//
//	cube_.insert(cube_.end(),triangles.begin(),triangles.end());
//
//	/// Send the new meshes to the GPU
//	glBindVertexArray ( vertexArray_cube_ );
//	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_cube_ );
//	glBufferData ( GL_ARRAY_BUFFER , cube_.size ( ) * sizeof ( cube_[0] ) , cube_.data() , GL_STATIC_DRAW );
//	glBindVertexArray ( 0 );


//	lines_ = triangles;
//	/// Send the new meshes to the GPU
//	glBindVertexArray ( lines_vertexArray_ );
//	glBindBuffer ( GL_ARRAY_BUFFER , lines_vertexBuffer_ );
//	glBufferData ( GL_ARRAY_BUFFER , triangles.size ( ) * sizeof ( triangles[0] ) , triangles.data() , GL_STATIC_DRAW );
//	glBindVertexArray ( 0 );

	update();

	return false;
}
