#include "OpenGLWidget.hpp"

GLWidget::GLWidget ( QWidget* parent ) : QOpenGLWidget ( parent )
{
	cube_shader_ = 0;
	background_ = 0;

	vertexArray_cube_ = 0;
	vertexBuffer_cube_ = 0;
	/// PORRA ERA SO O SLOT QUE TAVA ERRADO !!!! Q MERDA !!!
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

	Eigen::Vector3f vertex_data[] =
	{
		//  Top Face
		Eigen::Vector3f ( 1.0f , 1.0f , 1.0f ), Eigen::Vector3f ( 1.0f , 1.0f , -1.0f ),
		Eigen::Vector3f ( -1.0f , 1.0f , -1.0f ), Eigen::Vector3f ( -1.0f , 1.0f , 1.0f ),
		// Bottom Face
		Eigen::Vector3f ( 1.0f , -1.0f , 1.0f ), Eigen::Vector3f ( -1.0f , -1.0f , 1.0f ),
		Eigen::Vector3f ( -1.0f , -1.0f , -1.0f ), Eigen::Vector3f ( 1.0f , -1.0f , -1.0f ),
		// Front Face
		Eigen::Vector3f ( 1.0f , 1.0f , 1.0f ), Eigen::Vector3f ( -1.0f , 1.0f , 1.0f ),
		Eigen::Vector3f ( -1.0f , -1.0f , 1.0f ), Eigen::Vector3f ( 1.0f , -1.0f , 1.0f ),
		// Back Face
		Eigen::Vector3f ( 1.0f , 1.0f , -1.0f ), Eigen::Vector3f ( 1.0f , -1.0f , -1.0f ),
		Eigen::Vector3f ( -1.0f , -1.0f , -1.0f ), Eigen::Vector3f ( -1.0f , 1.0f , -1.0f ),
		// Right Face
		Eigen::Vector3f ( 1.0f , 1.0f , 1.0f ), Eigen::Vector3f ( 1.0f , -1.0f , 1.0f ),
		Eigen::Vector3f ( 1.0f , -1.0f , -1.0f ), Eigen::Vector3f ( 1.0f , 1.0f , -1.0f ),
		// Left Face
		Eigen::Vector3f ( -1.0f , 1.0f , -1.0f ), Eigen::Vector3f ( -1.0f , -0.0f , -1.0f ),
		Eigen::Vector3f ( -1.0f , -1.0f , 1.0f ), Eigen::Vector3f ( -1.0f , 1.0f , 1.0f ) };

	Eigen::Vector3f vertex_data_strip[] =
	{
	// Top Face
		vertex_data[0], vertex_data[1], vertex_data[3], vertex_data[2],/* 0 - 5*/
		// Bottom Face
		vertex_data[4], vertex_data[5], vertex_data[7], vertex_data[6],/* 6 - 11 */
		// Front Face
		vertex_data[0], vertex_data[3], vertex_data[4], vertex_data[5],/* 12 - 17*/
		// Back Face
		vertex_data[1], vertex_data[7], vertex_data[2], vertex_data[6],/* 18 - 23*/
		// Right Face
		vertex_data[0], vertex_data[4], vertex_data[1], vertex_data[7],/* 24 - 29*/
		// Left Face
		vertex_data[2], vertex_data[6], vertex_data[3], vertex_data[5] /* 30 - 35*/
	};

	std::copy ( vertex_data_strip , vertex_data_strip + 24 , std::back_inserter ( vertices ) );

//	for( auto c : vertices )
//	{
//		std::cout << "Vertices : " << c << std::endl;
//	}

	glGenVertexArrays ( 1 , &vertexArray_cube_ );
	glBindVertexArray ( vertexArray_cube_ );

	/// Requesting Vertex Buffers to the GPU
	glGenBuffers ( 1 , &vertexBuffer_cube_ );
	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_cube_ );
	glBufferData ( GL_ARRAY_BUFFER , vertices.size ( ) * sizeof ( vertices[0] ) , &vertices[0] , GL_STATIC_DRAW );
	// Set up generic attributes pointers
	glEnableVertexAttribArray ( vertexCube_slot_ );
	glVertexAttribPointer ( vertexCube_slot_ , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

	glBindVertexArray ( 0 );

	/// Vertex Array Surface Patch
	glGenVertexArrays ( 1 , &vertexArray_patch_ );
	glBindVertexArray ( vertexArray_patch_ );

	/// Requesting Vertex Buffers to the GPU
	glGenBuffers ( 1 , &vertexBuffer_patch_ );
	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_patch_ );
	glBufferData ( GL_ARRAY_BUFFER , patch_.size ( ) * sizeof ( patch_[0] ) , &patch_[0] , GL_STATIC_DRAW );
	// Set up generic attributes pointers
	glEnableVertexAttribArray ( vertexPatch_slot_ );
	glVertexAttribPointer ( vertexPatch_slot_ , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

	glBindVertexArray ( 0 );

	//createPatch ( Eigen::Vector3f ( -1.0 , 0.0 , 0.5 ) , Eigen::Vector3f ( 1.0 , 0.0 , 0.5 ) , 10 );

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

void GLWidget::createSurfacePatch()
{
	patch_.clear ( );
	box.reset();

	float 		stepz = 100.0f;
	std::size_t 	stepx = 10;

	std::cout << " Felipe " << sketch_.size() <<  std::endl;

	for ( float j = 0.0f; j < 1000.0f; j+=stepz )
	{
		for ( std::size_t i = 0; i < ( sketch_.size ( ) - stepx ); i+=stepx )
		{
			// In the Curve
			std::cout << " Felipe " << sketch_[i] << std::endl;

			patch_.push_back ( Eigen::Vector3f ( sketch_[i].x ( ) , j , sketch_[i].y ( ) ) );

			patch_.push_back ( Eigen::Vector3f ( sketch_[i + stepx].x ( ) , j , sketch_[i + stepx].y ( ) ) );

			// In the Extrude
			patch_.push_back ( Eigen::Vector3f ( sketch_[i].x ( ) , j+stepz , sketch_[i].y ( ) ) );

			patch_.push_back ( Eigen::Vector3f ( sketch_[i + stepx].x ( ) , j+stepz , sketch_[i + stepx].y ( ) ) );

		}
	}

	box.fromPointCloud(patch_.begin(),patch_.end());

	std::cout << "Box Center " << box.center() << std::endl;

	for ( std::size_t it = 0; it < patch_.size ( ); it++ )
	{
		patch_[it] = (patch_[it] - box.center())/box.diagonal();
	}

	box.fromPointCloud(patch_.begin(),patch_.end());

	std::cout << "Box Diagonal " << box.diagonal() << std::endl;

	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_patch_ );
	glBufferData ( GL_ARRAY_BUFFER , patch_.size ( ) * sizeof ( patch_[0] ) , &patch_[0] , GL_STATIC_DRAW );


	update();

}

void GLWidget::createSurfacePatchies ( const std::vector<std::vector<Eigen::Vector3f> >& patchies, float stepx, float stepz, float volume_width, Eigen::Vector3f center, float diagonal )
{

	patch_.clear ( );
	box.reset();

	std::size_t last;
	std::size_t last_j;

	stepz = volume_width / stepz;

	//std::cout << " Felipe " << sketch_.size() <<  std::endl;

	for ( std::size_t it_patch = 0; it_patch < patchies.size ( ); it_patch++ )
	{
		for ( float j = 0.0f; j < volume_width; j += stepz )
		{
			for ( std::size_t i = 0; i < ( patchies[it_patch].size ( ) - stepx ); i += stepx )
			{
				// In the Curve
				//std::cout << " Felipe " << patchies[it_patch][i] << std::endl;

				patch_.push_back ( Eigen::Vector3f ( patchies[it_patch][i].x ( )         , patchies[it_patch][i].y ( )          , j   ) );

				patch_.push_back ( Eigen::Vector3f ( patchies[it_patch][i + stepx].x ( ) ,  patchies[it_patch][i + stepx].y ( ) , j  ) );
				// In the Extrude
				patch_.push_back ( Eigen::Vector3f ( patchies[it_patch][i].x ( )          , patchies[it_patch][i].y ( )         , j + stepz   ) );

				patch_.push_back ( Eigen::Vector3f ( patchies[it_patch][i + stepx].x ( )  , patchies[it_patch][i + stepx].y ( ) , j + stepz  ) );

				last = i;

			}
//
			if ( last > patchies[it_patch].size ( ) )
			{
				last -= stepx;
			}

			last_j = j;

			patch_.push_back ( Eigen::Vector3f ( patchies[it_patch][last].x ( ) , patchies[it_patch][last].y ( ), last_j   ) );

			patch_.push_back ( Eigen::Vector3f ( patchies[it_patch].back ( ).x ( ) , patchies[it_patch].back ( ).y ( ) , last_j   ) );
			// In the Extrude
			patch_.push_back ( Eigen::Vector3f ( patchies[it_patch][last].x ( ) , patchies[it_patch][last].y ( ), last_j + stepz  ) );

			patch_.push_back ( Eigen::Vector3f ( patchies[it_patch].back ( ).x ( ) , patchies[it_patch].back ( ).y ( ), last_j + stepz  ) );
		}

	}


	std::cout << "Box Center " << center << std::endl;

	for ( std::size_t it = 0; it < patch_.size ( ); it++ )
	{
		patch_[it] = (patch_[it] - center)/diagonal;
	}


	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_patch_ );
	glBufferData ( GL_ARRAY_BUFFER , patch_.size ( ) * sizeof ( patch_[0] ) , &patch_[0] , GL_STATIC_DRAW );

	update();

}

/// Left to Right
void GLWidget::createCube ( const Celer::BoundingBox3<float>& box )
{

	cube_.clear ( );


	Eigen::Vector3f min_ = box.min();
	Eigen::Vector3f max_ = box.max();


	Eigen::Vector3f vertex_data[] =
	{
		//  Top Face
		Eigen::Vector3f ( max_.x(), max_.y(), max_.z() ),
		Eigen::Vector3f ( min_.x(), max_.y(), max_.z() ),
		Eigen::Vector3f ( max_.x(), max_.y(), min_.z() ),
		Eigen::Vector3f ( min_.x(), max_.y(), min_.z() ),
		// Bottom Face
		Eigen::Vector3f ( max_.x(), min_.y(), max_.z() ),
		Eigen::Vector3f ( min_.x(), min_.y(), max_.z() ),
		Eigen::Vector3f ( max_.x(), min_.y(), min_.z() ),
		Eigen::Vector3f ( min_.x(), min_.y(), min_.z() ),
		// Front Face
		Eigen::Vector3f ( max_.x(), max_.y(), max_.z() ),
		Eigen::Vector3f ( min_.x(), max_.y(), max_.z() ),
		Eigen::Vector3f ( max_.x(), min_.y(), max_.z() ),
		Eigen::Vector3f ( min_.x(), min_.y(), max_.z() ),
		// Back Face
		Eigen::Vector3f ( max_.x(), max_.y(), min_.z() ),
		Eigen::Vector3f ( min_.x(), max_.y(), min_.z() ),
		Eigen::Vector3f ( max_.x(), min_.y(), min_.z() ),
		Eigen::Vector3f ( min_.x(), min_.y(), min_.z() ),
		// Left Face
		Eigen::Vector3f ( max_.x(), max_.y(), min_.z() ),
		Eigen::Vector3f ( max_.x(), max_.y(), max_.z() ),
		Eigen::Vector3f ( max_.x(), min_.y(), min_.z() ),
		Eigen::Vector3f ( max_.x(), min_.y(), max_.z() ),
		// Right Face
		Eigen::Vector3f ( min_.x(), max_.y(), max_.z() ),
		Eigen::Vector3f ( min_.x(), max_.y(), min_.z() ),
		Eigen::Vector3f ( min_.x(), min_.y(), max_.z() ),
		Eigen::Vector3f ( min_.x(), min_.y(), min_.z() ),
	};


	std::copy( vertex_data	 , vertex_data + 24	, std::back_inserter ( cube_ ) );


	Celer::BoundingBox3<float> b;

	b.fromPointCloud(cube_.begin(),cube_.end());

	std::cout << "Box Center Cube " << b.center() << std::endl;

	for ( std::size_t it = 0; it < cube_.size ( ); it++ )
	{
		cube_[it] = (cube_[it] - b.center())/b.diagonal();
	}

	std::cout << "Box Center Cube " << b.center() << std::endl;

	b.fromPointCloud(cube_.begin(),cube_.end());

	/// Requesting Vertex Buffers to the GPU
	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_cube_ );
	glBufferData ( GL_ARRAY_BUFFER , cube_.size ( ) * sizeof ( cube_[0] ) , &cube_[0] , GL_STATIC_DRAW );
	// Set up generic attributes pointers

}


/// Left to Right
void GLWidget::createPatch ( Eigen::Vector3f left , Eigen::Vector3f right , float step )
{
	float size = right.x ( ) - left.x ( );

	float dx = size / 10;
	float dz = dx;

	patch_.clear ( );

	float bx = 0.0f;
	float by = 0.0f;
	float bz = 0.0f;

	for ( float i = 0; i < 10; i++ )
	{
		for ( float j = 0; j < 10; j++ )
		{
			for ( float z = 0; z < 10; z++ )
			{
				patch_.push_back ( Eigen::Vector3f ( left.x ( ) + ( dx * i )      , dz * z , left.z ( ) + ( dz * j ) ) );
				patch_.push_back ( Eigen::Vector3f ( left.x ( ) + ( dx * i ) + dx , dz * z , left.z ( ) + ( dz * j ) ) );
				patch_.push_back ( Eigen::Vector3f ( left.x ( ) + ( dx * i )      , dz * z , left.z ( ) + ( dz * j ) + dz ) );
				patch_.push_back ( Eigen::Vector3f ( left.x ( ) + ( dx * i ) + dx , dz * z , left.z ( ) + ( dz * j ) + dz ) );
			}
		}
	}

	for ( std::size_t it = 0; it < patch_.size ( ); it++ )
	{
		bx += patch_[it].x ( );
		by += patch_[it].y ( );
		bz += patch_[it].z ( );
	}

	float mean = (float) patch_.size ( );

	box.fromPointCloud(patch_.begin(),patch_.end());

	Eigen::Vector3f t = Eigen::Vector3f ( bx / mean , by / mean , bz / mean );

	for ( std::size_t it = 0; it < patch_.size ( ); it++ )
	{
		patch_[it] = patch_[it] - box.center();
	}

//	patch_.push_back( Eigen::Vector3f(left) );
//	patch_.push_back( Eigen::Vector3f(right) );
//	patch_.push_back( Eigen::Vector3f(left.x(),left.y(),left.z()+0.5) );
//	patch_.push_back( Eigen::Vector3f(right.x(),right.y(),right.z()+0.5) );


	/// Requesting Vertex Buffers to the GPU
	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_patch_ );
	glBufferData ( GL_ARRAY_BUFFER , patch_.size ( ) * sizeof ( patch_[0] ) , &patch_[0] , GL_STATIC_DRAW );
	// Set up generic attributes pointers

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
	/// What is you viewport size my friend ... =(
	glViewport ( 0 , 0 , width , height );

	camera.setViewport ( Eigen::Vector2f ( (float) width , (float) height ) );
	camera.setPerspectiveMatrix ( camera.getFovy ( ) , (float) width / (float) height , 0.1f , 100.0f );
}
/// Real Looping
void GLWidget::paintGL ( )
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	backGround();

	patch_shader_->bind ( );
	/// 3rd attribute buffer : vertices
	patch_shader_->setUniform ( "ModelMatrix" , camera.getViewMatrix ( ) );
	patch_shader_->setUniform ( "ViewMatrix" , camera.getViewMatrix ( ) );
	patch_shader_->setUniform ( "ProjectionMatrix" , camera.getProjectionMatrix ( ) );
	patch_shader_->setUniform ( "WIN_SCALE" , (float) width ( ) , (float) height ( ) );
	glBindVertexArray ( vertexArray_patch_ );
	/// Draw the triangle !
	glDrawArrays ( GL_LINES_ADJACENCY , 0 , patch_.size ( ) );

	glBindVertexArray ( 0 );
	patch_shader_->unbind ( );

	///

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
//			sketch_.clear();
//			sketch_.push_back(Eigen::Vector3f(screen_pos.x(),screen_pos.y(),0.0f));
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
//			sketch_.push_back(Eigen::Vector3f(screen_pos.x(),screen_pos.y(),0.0f));
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

//		p.push_back(sketch_);
//
//		createSurfacePatchies(p,10.0f,100.0f, 500.0f);
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
/// Drag and Drop
void GLWidget::dragEnterEvent ( QDragEnterEvent *event )
{

	setBackgroundRole ( QPalette::Highlight );

	event->acceptProposedAction ( );

	emit changed ( event->mimeData ( ) );

	qDebug ( ) << "Enter";
}
/// Then, we invoke acceptProposedAction() on event, setting the drop action to the one proposed.
/// Lastly, we emit the changed() signal, with the data that was dropped and its MIME type information as a parameter.
/// For dragMoveEvent(), we just accept the proposed QDragMoveEvent object, event, with acceptProposedAction().
void GLWidget::dragMoveEvent ( QDragMoveEvent *event )
{
	event->acceptProposedAction ( );
}
///The DropArea class's implementation of dropEvent() extracts the event's mime data and displays it accordingly.
void GLWidget::dropEvent ( QDropEvent *event )
{
	const QMimeData *mimeData = event->mimeData ( );
	/// The mimeData object can contain one of the following objects: an image, HTML text, plain text, or a list of URLs.

	if ( event->mimeData ( )->hasUrls ( ) )
	{

		QList<QUrl> urlList = mimeData->urls ( );
		QString text;
		for ( int i = 0; i < urlList.size ( ) && i < 32; ++i )
		{
			QString url = urlList.at ( i ).path ( );
			text += url; // + QString("\n");
		}

	}

}

void GLWidget::dragLeaveEvent ( QDragLeaveEvent *event )
{
	event->accept ( );

	qDebug ( ) << "Exit";
}
/// For DropArea's implementation, we clear invoke clear() and then accept the proposed event.
/// The clear() function sets the text in DropArea to "<drop content>" and sets the backgroundRole to
/// QPalette::Dark. Lastly, it emits the changed() signal.

