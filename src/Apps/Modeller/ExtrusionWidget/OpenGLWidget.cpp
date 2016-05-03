#include "OpenGLWidget.hpp"

GLWidget::GLWidget ( QWidget* parent ) : QOpenGLWidget ( parent )
{
	cube_shader_ = 0;
	patch_shader_ = 0;
	background_ = 0;

	vertexArray_cube_ = 0;
	vertexBuffer_cube_ = 0;
	vertexCube_slot_ = 0;

	vertexArray_patch_ = 0;
	vertexBuffer_patch_ = 0;
	vertexPatch_slot_ = 0;
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

	vertices.clear ( );

	extrusion_controller_.initialize(0.0,0.0,0.0,700.0,400.0,100.0);

	std::vector<unsigned int> positions = {0,50,100};

	std:vector<Eigen::Vector4f> x;

	cube_ = extrusion_controller_.getPlanes( positions );

	x = extrusion_controller_.getcubeMesh();

	cube_.insert(cube_.end(),x.begin(),x.end());

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

	loadShaderByResources ( );

	camera.setPerspectiveMatrix ( 60.0 , (float) this->width ( ) / (float) this->height ( ) , 0.1f , 100.0f );

}

void GLWidget::reloadShaders ( )
{
	if ( cube_shader_ )
	{
		cube_shader_->reloadShaders ( );
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

	patch_shader_ = new Tucano::Shader ( "Patch" , ( shaderDirectory + "Shaders/SinglePassWireframe.vert" ).toStdString ( ),
					               ( shaderDirectory + "Shaders/SinglePassWireframe.frag" ).toStdString ( ),
						       ( shaderDirectory + "Shaders/SinglePassWireframe.geom" ).toStdString ( ) , "" , "" );
	patch_shader_->initialize ( );

	background_ = new Tucano::Shader ( "BackGround" , ( shaderDirectory + "Shaders/DummyQuad.vert" ).toStdString ( ),
					                  ( shaderDirectory + "Shaders/DummyQuad.frag" ).toStdString ( ),
						          ( shaderDirectory + "Shaders/DummyQuad.geom" ).toStdString ( ), "" , "" );
        background_->initialize ( );

	std::cout << " cube_shader_ " << cube_shader_->getShaderProgram ( ) << std::endl;
	std::cout << " background_ "  << background_->getShaderProgram  ( ) << std::endl;

}

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
	QString shaderDirectory ( shadersDir.path ( ) + "/src/Extrusion/GUI/Qt/RCC/Shaders/" );
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

	cube_shader_->bind ( );
	/// 3rd attribute buffer : vertices
	cube_shader_->setUniform ( "ModelMatrix" , camera.getViewMatrix ( ) );
	cube_shader_->setUniform ( "ViewMatrix" , camera.getViewMatrix ( ) );
	cube_shader_->setUniform ( "ProjectionMatrix" , camera.getProjectionMatrix ( ) );
	cube_shader_->setUniform ( "WIN_SCALE" , (float) width ( ) , (float) height ( ) );
	glBindVertexArray ( vertexArray_cube_ );
	/// Draw the triangle !
	glDrawArrays ( GL_LINES_ADJACENCY , 0 , cube_.size ( ) );

	glBindVertexArray ( 0 );
	cube_shader_->unbind ( );

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
