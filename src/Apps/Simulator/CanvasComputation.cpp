#include "CanvasComputation.h"

CanvasComputation::CanvasComputation ( QWidget* parent ) :
																QOpenGLWidget ( parent )
{

	flowvisualizationc = new FlowVisualizationController ( );
	mn_options = NULL;

	resetSetup ( );
	createActions ( );
	createPopupMenu ( );

	connect ( parent , SIGNAL( sendInputUser( std::string, std::string, float, float ) ) , flowvisualizationc , SLOT( getUserInput( std::string, std::string, float, float ) ) );
//    connect( parent, SIGNAL( sendSurfaceFile( std::string ) ), flowvisualizationc, SLOT( openSurfaceFile( std::string ) ) );
//    connect( parent, SIGNAL( sendInputUserFile( std::string ) ), flowvisualizationc, SLOT( openUserInputFile( std::string ) ) );
	connect ( parent , SIGNAL( computeVolume() ) , flowvisualizationc , SLOT( computeVolumetricMesh() ) );
//    connect( parent, SIGNAL( computeFlowProperties() ), flowvisualizationc, SLOT( computeFlowProperties() ) );
	connect ( parent , SIGNAL( computePressureProperty() ) , flowvisualizationc , SLOT( computePressure() ) );
	connect ( parent , SIGNAL( computeVelocityProperty() ) , flowvisualizationc , SLOT( computeVelocity() ) );
	connect ( parent , SIGNAL( computeTOFProperty() ) , flowvisualizationc , SLOT( computeTOF() ) );

	connect ( flowvisualizationc , SIGNAL( updateComboBox( std::vector< std::string >, std::vector< std::string > ) ) , parent , SLOT( updateComboBox( std::vector< std::string >, std::vector< std::string > ) ) );
	connect ( parent , SIGNAL( selectFlowProperty( int, bool& ) ) , flowvisualizationc , SLOT( selectFlowProperty( int, bool& ) ) );

	bf_vertices = 0;
	bf_colors = 0;

	/// Tucano

	cube_shader_ = 0;
	vertexArray_cube_ = 0;
	vertexBuffer_cube_ = 0;
	/// PORRA ERA SO O SLOT QUE TAVA ERRADO !!!! Q MERDA !!!
	vertexCube_slot_ = 0;

	vtk_visualization_ = 0;
	vertexArray_MESH_ = 0;
	// Vertices
	vertexBuffer_MESH_ = 0;
	vertexMESH_Slot_ = 0;
	vertexBuffer_face_ID_ = 0;
	vertexBuffer_Lines_ID_ = 0;
	// Color
	vertexBuffer_colors_ = 0;
	vertexColor_slot_ = 1;

}

void CanvasComputation::createActions ( )
{
	ac_reset_transformations = new QAction ( tr ( "Reset View" ) , this );
	ac_clear_all = new QAction ( tr ( "Clear All" ) , this );
	ac_export = new QAction ( tr ( "Export" ) , this );

	wa_properties = new QWidgetAction ( this );
	wa_colormaps = new QWidgetAction ( this );
	wa_rendering = new QWidgetAction ( this );

	chk_show_points = new QCheckBox ( tr ( "Show Points" ) );
	chk_show_points->setChecked ( show_vertices );

	chk_show_wireframe = new QCheckBox ( tr ( "Show WireFrame" ) );
	chk_show_wireframe->setChecked ( show_lines );

	chk_show_volume = new QCheckBox ( tr ( "Show Volume" ) );
	chk_show_volume->setChecked ( show_faces );

	connect ( this , SIGNAL( sendProperty( std::string, std::string ) ) , this , SLOT( sendColorsGPU( std::string, std::string ) ) );
	connect ( chk_show_points , SIGNAL( clicked( bool ) ) , this , SLOT( showPoints( bool ) ) );
	connect ( chk_show_volume , SIGNAL( clicked( bool ) ) , this , SLOT( showVolume( bool ) ) );
	connect ( chk_show_wireframe , SIGNAL( clicked( bool ) ) , this , SLOT( showWireframe( bool ) ) );
	connect ( this , SIGNAL( sendVectorProperty( std::string, std::string, int ) ) , this , SLOT( sendColorsGPU( std::string, std::string, int ) ) );
	connect ( ac_reset_transformations , SIGNAL( triggered() ) , this , SLOT( resetTransformations() ) );
	connect ( ac_clear_all , SIGNAL( triggered() ) , this , SLOT( resetSetup() ) );
	connect ( ac_export , SIGNAL( triggered() ) , this , SLOT( exportFile() ) );

}

void CanvasComputation::fillMenuProperties ( )
{

	mn_properties->clear ( );
	fillMenuPointProperties ( );
	fillMenuCellProperties ( );

}

void CanvasComputation::fillMenuPointProperties ( )
{

	std::string name;
	std::string format;
	std::string type;
	int ncoords = 0;

	mn_properties->addSection ( "POINTS ATTRIBUTES" );
	QActionGroup* ag_properties = new QActionGroup ( this );

	int nproperties = flowvisualizationc->getNumberofPointsProperties ( );
	for ( int i = 0, ids = 0, idv = 0; i < nproperties; ++i )
	{

		flowvisualizationc->getPointProperty ( i , name , format , type , ncoords );
		if ( ncoords == 1 )
		{
			ac_properties.push_back ( new QAction ( tr ( name.c_str ( ) ) , this ) );
			cout << name << endl;
			mn_properties->addAction ( ac_properties[ids] );
			ag_properties->addAction ( ac_properties[ids] );

			connect ( ac_properties[ids] , &QAction::triggered ,
																			[=]()
																			{	emit sendProperty( name , type.c_str() );} );
			ids++;

		}
		else if ( ncoords == 3 )
		{
			createRenderingVectors ( idv , name , type.c_str ( ) );
			idv++;

		}

	}
}

void CanvasComputation::fillMenuCellProperties ( )
{
	std::string name;
	std::string format;
	std::string type;
	int ncoords = 0;

	mn_properties->addSection ( "CELLS ATTRIBUTES" );
	QActionGroup* ag_properties_cells = new QActionGroup ( this );

	int nactions = (int) ac_properties.size ( );
	int nproperties = flowvisualizationc->getNumberofCellsProperties ( );
	for ( int i = 0, ids = 0, idv = 0; i < nproperties; ++i )
	{

		flowvisualizationc->getCellProperty ( i , name , format , type , ncoords );
		if ( ncoords == 1 )
		{
			ac_properties.push_back ( new QAction ( tr ( name.c_str ( ) ) , this ) );
			mn_properties->addAction ( ac_properties[ids + nactions] );
			ag_properties_cells->addAction ( ac_properties[ids + nactions] );

			connect ( ac_properties[ids + nactions] , &QAction::triggered ,
																			[=]()
																			{	emit sendProperty( name , type.c_str() );} );
			ids++;

		}
		else if ( ncoords == 3 )
		{
			createRenderingVectors ( idv , name , type.c_str ( ) );
			idv++;

		}

	}
}

void CanvasComputation::createMenuProperties ( )
{
	mn_properties = new QMenu ( tr ( "Properties" ) );
	fillMenuProperties ( );

}

void CanvasComputation::createMenuColorMaps ( )
{
	rd_colormaps.push_back ( new QRadioButton ( tr ( "Jet" ) ) );
	rd_colormaps.push_back ( new QRadioButton ( tr ( "Constant" ) ) );

	QVBoxLayout *vb_layout = new QVBoxLayout;
	vb_layout->addWidget ( rd_colormaps[0] );
	vb_layout->addWidget ( rd_colormaps[1] );

	QGroupBox *gb_colormaps = new QGroupBox ( );
	gb_colormaps->setFlat ( true );
	gb_colormaps->setLayout ( vb_layout );

	wa_colormaps->setDefaultWidget ( gb_colormaps );

	mn_colormap = new QMenu ( tr ( "ColorMaps" ) );
	mn_colormap->addAction ( wa_colormaps );

	connect ( rd_colormaps[0] , SIGNAL( clicked( bool) ) , this , SLOT( setColorMap() ) );
	connect ( rd_colormaps[1] , SIGNAL( clicked( bool) ) , this , SLOT( setColorMapConstant() ) );

}

void CanvasComputation::createMenuRendering ( )
{

	QVBoxLayout *vb_layout = new QVBoxLayout;
	vb_layout->addWidget ( chk_show_points );
	vb_layout->addWidget ( chk_show_wireframe );
	vb_layout->addWidget ( chk_show_volume );

	QGroupBox *gb_rendering = new QGroupBox ( );
	gb_rendering->setFlat ( true );
	gb_rendering->setLayout ( vb_layout );

	wa_rendering->setDefaultWidget ( gb_rendering );

}

void CanvasComputation::createRenderingVectors ( int id , std::string title , std::string type )
{

	rd_options_vector.push_back ( new QRadioButton ( tr ( "Magnitude" ) ) );
	rd_options_vector.push_back ( new QRadioButton ( tr ( "X" ) ) );
	rd_options_vector.push_back ( new QRadioButton ( tr ( "Y" ) ) );
	rd_options_vector.push_back ( new QRadioButton ( tr ( "Z" ) ) );

	id = (int) mn_vector_properties_points.size ( );

	QVBoxLayout *vb_layout = new QVBoxLayout ( this );
	vb_layout->addWidget ( rd_options_vector[4 * id + 0] );
	vb_layout->addWidget ( rd_options_vector[4 * id + 1] );
	vb_layout->addWidget ( rd_options_vector[4 * id + 2] );
	vb_layout->addWidget ( rd_options_vector[4 * id + 3] );

	QGroupBox *gb_options_vector = new QGroupBox ( );
	gb_options_vector->setFlat ( true );
	gb_options_vector->setLayout ( vb_layout );

	wa_options_vector = new QWidgetAction ( this );
	wa_options_vector->setDefaultWidget ( gb_options_vector );

	mn_vector_properties_points.push_back ( new QMenu ( ) );
	mn_vector_properties_points[id]->addAction ( wa_options_vector );
	mn_vector_properties_points[id]->setTitle ( title.c_str ( ) );

	mn_properties->addMenu ( mn_vector_properties_points[id] );

	connect ( rd_options_vector[4 * id + 0] , &QRadioButton::clicked , this ,
																	[=]()
																	{	emit sendVectorProperty( title.c_str() , type, 0 );} );
	connect ( rd_options_vector[4 * id + 1] , &QRadioButton::clicked , this ,
																	[=]()
																	{	emit sendVectorProperty( title.c_str() , type, 1 );} );
	connect ( rd_options_vector[4 * id + 2] , &QRadioButton::clicked , this ,
																	[=]()
																	{	emit sendVectorProperty( title.c_str() , type, 2 );} );
	connect ( rd_options_vector[4 * id + 3] , &QRadioButton::clicked , this ,
																	[=]()
																	{	emit sendVectorProperty( title.c_str() , type, 3 );} );

}

void CanvasComputation::createPopupMenu ( )
{

	createMenuProperties ( );
	createMenuColorMaps ( );
	createMenuRendering ( );

	mn_options = new QMenu ( );
	mn_options->addMenu ( mn_properties );
	mn_options->addMenu ( mn_colormap );
	mn_options->addSeparator ( );
	mn_options->addAction ( wa_rendering );
	mn_options->addSeparator ( );
	mn_options->addAction ( ac_reset_transformations );
	mn_options->addAction ( ac_clear_all );
	mn_options->addSeparator ( );
	mn_options->addAction ( ac_export );

}

void CanvasComputation::initializeGL ( )
{

	glewExperimental = GL_TRUE;
	GLenum err = glewInit ( );
	if ( GLEW_OK != err )
	{
		fprintf ( stderr , "Error: %s\n" , glewGetErrorString ( err ) );
	}

	glClearColor ( 1.0f , 1.0f , 1.0f , 1.0f );
	glEnable ( GL_DEPTH_TEST );

	bf_mesh = new GLuint[2];
	initializeShaders ( );

	/// Tucano

	cube_.clear ( );

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

	std::copy ( vertex_data_strip , vertex_data_strip + 24 , std::back_inserter ( cube_ ) );

//	for( auto c : vertices )
//	{
//		std::cout << "Vertices : " << c << std::endl;
//	}

	glGenVertexArrays ( 1 , &vertexArray_cube_ );
	glBindVertexArray ( vertexArray_cube_ );

	/// Requesting Vertex Buffers to the GPU
	glGenBuffers ( 1 , &vertexBuffer_cube_ );
	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_cube_ );
	glBufferData ( GL_ARRAY_BUFFER , cube_.size ( ) * sizeof ( cube_[0] ) , &cube_[0] , GL_STATIC_DRAW );
	// Set up generic attributes pointers
	glEnableVertexAttribArray ( vertexCube_slot_ );
	glVertexAttribPointer ( vertexCube_slot_ , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

	glBindVertexArray ( 0 );

	camera.setPerspectiveMatrix ( 60.0 , (float) this->width ( ) / (float) this->height ( ) , 0.1f , 100.0f );

	loadShaderByResources ( );

	glPointSize ( 10 );

	vertices_.clear ( );

	for ( auto c : vertices_ )
	{
		std::cout << "Vertices : " << c << std::endl;
	}

	glGenVertexArrays ( 1 , &vertexArray_MESH_ );
	glBindVertexArray ( vertexArray_MESH_ );

	/// Requesting Vertex Buffers to the GPU
	glGenBuffers ( 1 , &vertexBuffer_MESH_ );
	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_MESH_ );
	glBufferData ( GL_ARRAY_BUFFER , vertices_.size ( ) * sizeof ( vertices_[0] ) , &vertices_[0] , GL_STATIC_DRAW );
	// Set up generic attributes pointers
	glEnableVertexAttribArray ( vertexMESH_Slot_ );
	glVertexAttribPointer ( vertexMESH_Slot_ , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

	/// Requesting Vertex Buffers to the GPU
	glGenBuffers ( 1 , &vertexBuffer_colors_ );
	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_colors_ );
	glBufferData ( GL_ARRAY_BUFFER , 0 , 0 , GL_STATIC_DRAW );
	// Set up generic attributes pointers
	glEnableVertexAttribArray ( vertexColor_slot_ );
	glVertexAttribPointer ( vertexColor_slot_ , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

	/// Requesting Vertex Buffers to the GPU
	glGenBuffers ( 1 , &vertexBuffer_face_ID_ );
	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_face_ID_ );
	glBufferData ( GL_ARRAY_BUFFER , 0 , 0 , GL_STATIC_DRAW );

	/// Requesting Vertex Buffers to the GPU
	glGenBuffers ( 1 , &vertexBuffer_Lines_ID_ );
	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_Lines_ID_ );
	glBufferData ( GL_ARRAY_BUFFER , 0 , 0 , GL_STATIC_DRAW );

	glBindVertexArray ( 0 );

}

void CanvasComputation::keyPressEvent ( QKeyEvent * event )
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

void CanvasComputation::backGround()
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

void CanvasComputation::reloadShaders ( )
{
	if ( cube_shader_ )
	{
		cube_shader_->reloadShaders ( );
	}
	if ( vtk_visualization_ )
	{
		vtk_visualization_->reloadShaders ( );
	}
	if ( background_ )
	{
		background_->reloadShaders ( );
	}
}

void CanvasComputation::loadShaderByResources ( )
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
	cube_shader_ = new Tucano::Shader ( "Cube" , ( shaderDirectory + "Shaders/CubeSinglePassWireframe.vert" ).toStdString ( ) , ( shaderDirectory + "Shaders/CubeSinglePassWireframe.frag" ).toStdString ( ) ,
																	( shaderDirectory + "Shaders/CubeSinglePassWireframe.geom" ).toStdString ( ) , "" , "" );
	cube_shader_->initialize ( );

	vtk_visualization_ = new Tucano::Shader ( "vtk_visualization_" , ( shaderDirectory + "Shaders/HWU/vtk.vert" ).toStdString ( ) , ( shaderDirectory + "Shaders/HWU/vtk.frag" ).toStdString ( ) , "" , "" , "" );
	vtk_visualization_->initialize ( );

	background_ = new Tucano::Shader ( "BackGround" , ( shaderDirectory + "Shaders/DummyQuad.vert" ).toStdString ( ),
					                  ( shaderDirectory + "Shaders/DummyQuad.frag" ).toStdString ( ),
						          ( shaderDirectory + "Shaders/DummyQuad.geom" ).toStdString ( ) , "" , "" );
        background_->initialize ( );

	std::cout << " cube_shader_ Clarissa" << cube_shader_->getShaderProgram ( ) << std::endl;
	std::cout << " vtk_visualization_ Clarissa" << vtk_visualization_->getShaderProgram ( ) << std::endl;

}

void CanvasComputation::resizeGL ( int width , int height )
{
	this->makeCurrent ( );

	height = height ? height : 1;
	glViewport ( 0 , 0 , (GLint) width , (GLint) height );

	glViewport ( 0 , 0 , width , height );

	camera.setViewport ( Eigen::Vector2f ( (float) width , (float) height ) );
	camera.setPerspectiveMatrix ( camera.getFovy ( ) , (float) width / (float) height , 0.1f , 100.0f );

}

void CanvasComputation::paintGL ( )
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	backGround();

	vtk_visualization_->bind ( );
	/// 3rd attribute buffer : vertices
	vtk_visualization_->setUniform ( "ModelMatrix" , camera.getViewMatrix ( ) );
	vtk_visualization_->setUniform ( "ViewMatrix" , camera.getViewMatrix ( ) );
	vtk_visualization_->setUniform ( "ProjectionMatrix" , camera.getProjectionMatrix ( ) );
	vtk_visualization_->setUniform ( "WIN_SCALE" , (float) width ( ) , (float) height ( ) );
	glBindVertexArray ( vertexArray_MESH_ );
	/// Draw the triangle !

	if ( show_vertices )
	{
		glDrawArrays ( GL_POINTS , 0 , vertices_.size ( ) );
	}

	else if ( show_faces )
	{
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vertexBuffer_face_ID_ );
		glDrawElements ( GL_TRIANGLES , number_of_faces , GL_UNSIGNED_INT , NULL );
	}
	else if ( show_lines )
	{
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vertexBuffer_Lines_ID_ );
		glDrawElements ( GL_LINES , number_of_lines , GL_UNSIGNED_INT , NULL );
	}
	else
	{

	}

	glBindVertexArray ( 0 );
	vtk_visualization_->unbind ( );

//    	vtk_visualization_->bind ( );
//    	/// 3rd attribute buffer : vertices
//    	vtk_visualization_->setUniform ( "ModelMatrix" , camera.getViewMatrix ( ) );
//    	vtk_visualization_->setUniform ( "ViewMatrix" , camera.getViewMatrix ( ) );
//    	vtk_visualization_->setUniform ( "ProjectionMatrix" , camera.getProjectionMatrix ( ) );
//    	vtk_visualization_->setUniform ( "WIN_SCALE" , (float) width ( ) , (float) height ( ) );
//    	glBindVertexArray ( vertexArray_MESH_ );
//    	/// Draw the triangle !
////        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vertexBuffer_face_ID_ );
////        glDrawElements(GL_TRIANGLES, number_of_faces, GL_UNSIGNED_INT, NULL );
//
//    	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vertexBuffer_Lines_ID_ );
//    	glDrawElements(GL_LINES, number_of_lines, GL_UNSIGNED_INT, NULL );
//
//
//    	glBindVertexArray ( 0 );
//    	vtk_visualization_->unbind ( );

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

}

void CanvasComputation::initializeShaders ( )
{

#if defined(_WIN32) || defined(_WIN64) // Windows Directory Style
	/* Do windows stuff */
	std::string vertex_shader_string = read_shader_file( "D:/Workspace/RRM/build-mscv2013_x32/build/bin/Shaders/HWU/vertex_shader.vert" );
	std::string fragment_shader_string = read_shader_file( "D:/Workspace/RRM/build-mscv2013_x32/build/bin/Shaders/HWU/fragment_shader.frag" );
#elif defined(__linux__)               // Linux Directory Style
	/* Do linux stuff */
	std::string vertex_shader_string = read_shader_file ( "/media/d/Workspace/RRM/build-Linux_GCC-4.9_x64/build/bin/Shaders/vertex_shader.vert" );
	std::string fragment_shader_string = read_shader_file ( "/media/d/Workspace/RRM/build-Linux_GCC-4.9_x64/build/bin/Shaders/fragment_shader.frag" );
#else
	/* Error, both can't be defined or undefined same time */
	std::cerr << "Operate System not supported !"
	halt();
#endif

	const char *vertex_shader_source = vertex_shader_string.c_str ( );
	const char *fragment_shader_source = fragment_shader_string.c_str ( );

	GLuint vertex_shader = glCreateShader ( GL_VERTEX_SHADER );
	glShaderSource ( vertex_shader , 1 , &vertex_shader_source , NULL );
	glCompileShader ( vertex_shader );

	// verifying if the vertex compilation was ok

	GLint params_vertex;
	glGetShaderiv ( vertex_shader , GL_COMPILE_STATUS , &params_vertex );
	if ( params_vertex == GL_FALSE )
		cout << "Compile vertex failed!" << endl;

	GLint fragment_shader = glCreateShader ( GL_FRAGMENT_SHADER );
	glShaderSource ( fragment_shader , 1 , &fragment_shader_source , NULL );
	glCompileShader ( fragment_shader );

	// verifying if the fragment compilation was ok

	GLint params_fragment;
	glGetShaderiv ( fragment_shader , GL_COMPILE_STATUS , &params_fragment );
	if ( params_fragment == GL_FALSE )
		cout << "Compile fragment failed!" << endl;

	program = glCreateProgram ( );
	glAttachShader ( program , vertex_shader );
	glAttachShader ( program , fragment_shader );

	glLinkProgram ( program );
	glUseProgram ( program );

	glDeleteShader ( vertex_shader );
	glDeleteShader ( fragment_shader );
}

void CanvasComputation::deleteBuffers ( )
{

	delete[] bf_mesh;

	if ( glIsBuffer ( bf_faces ) == GL_TRUE )
		glDeleteBuffers ( 1 , &bf_faces );

	if ( glIsVertexArray ( vao_mesh ) == GL_TRUE )
		glDeleteVertexArrays ( 1 , &vao_mesh );

}

void CanvasComputation::sendMeshGPU ( )
{

	vector<GLfloat> vertices;
	//vector< GLuint > lines;
//        flowvisualizationc->getPointsSurface( vertices );
	flowvisualizationc->getVertices ( vertices );

	number_of_vertices = (GLuint) vertices.size ( );

	vertices_.clear ( );

	for ( std::size_t it = 0; it < vertices.size ( ) - 3; it += 3 )
	{
		vertices_.push_back ( Eigen::Vector3f ( vertices[it] , vertices[it + 1] , vertices[it + 2] ) );
	}

	box.fromPointCloud ( vertices_.begin ( ) , vertices_.end ( ) );

	for ( std::size_t it = 0; it < vertices_.size ( ); it++ )
	{
		std::cout << " Points " << vertices_[it] << std::endl;
	}

	std::cout << "Box Center Clarissa " << box.center ( ) << std::endl;

	for ( std::size_t it = 0; it < vertices_.size ( ); it++ )
	{
		vertices_[it] = ( vertices_[it] - box.center ( ) ) / box.diagonal ( );
	}

	box.fromPointCloud ( vertices_.begin ( ) , vertices_.end ( ) );

	for ( std::size_t it = 0; it < vertices_.size ( ); it++ )
	{
		std::cout << " Center " << vertices_[it] << std::endl;
	}
	/// Requesting Vertex Buffers to the GPU
	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_MESH_ );
	glBufferData ( GL_ARRAY_BUFFER , vertices_.size ( ) * sizeof ( vertices_[0] ) , &vertices_[0] , GL_STATIC_DRAW );
	// Set up generic attributes pointers

	vector<GLfloat> colors;
//	flowvisualizationc->getSurfaceColors( colors );
	flowvisualizationc->getColors ( colors );
	GLint ncolors = (int) colors.size ( );

	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_colors_ );
	glBufferData ( GL_ARRAY_BUFFER , ncolors * sizeof ( GL_FLOAT ) , colors.data ( ) , GL_STATIC_DRAW );

	vector<GLuint> faces;
//	flowvisualizationc->getTrianglesSurface( faces );
	flowvisualizationc->getTriangles ( faces );

	if ( faces.empty ( ) == false )
	{
		number_of_faces = (GLint) faces.size ( );

		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vertexBuffer_face_ID_ );
		glBufferData ( GL_ELEMENT_ARRAY_BUFFER , faces.size ( ) * sizeof ( faces[0] ) , &faces[0] , GL_STATIC_DRAW );
	}

	vector<GLuint> lines;
	flowvisualizationc->getWireframe ( lines );

	if ( lines.empty ( ) == false )
	{
		number_of_lines = (GLint) lines.size ( );
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vertexBuffer_Lines_ID_ );
		glBufferData ( GL_ELEMENT_ARRAY_BUFFER , number_of_lines * sizeof ( lines[0] ) , &lines[0] , GL_STATIC_DRAW );

	}

//
//
//
//    glPointSize( 3.0f );
//    glGenVertexArrays( 1, &vao_mesh );
//    glBindVertexArray( vao_mesh );
//
//
//
//    glGenBuffers( 2, bf_mesh );
//
//
//        vector< GLfloat > vertices;
//        flowvisualizationc->getVertices( vertices );
//        number_of_vertices = (int) vertices.size();
//
//        std::vector<Eigen::Vector3f> v;
//
//        for ( std::size_t it = 0; it < vertices.size() - 3; it+=3 )
//        {
//        	v.push_back( Eigen::Vector3f( vertices[it], vertices[it+1], vertices[it+2] ) );
//        }
//
//
//	box.fromPointCloud(v.begin(),v.end());
//
//	std::cout << "Box Center " << box.center() << std::endl;
//
//	for ( std::size_t it = 0; it < v.size ( ); it++ )
//	{
//		v[it] = (v[it] - box.center())/box.diagonal();
//	}
//
//
//        glBindBuffer( GL_ARRAY_BUFFER, bf_mesh[ 0 ] );
//        glBufferData( GL_ARRAY_BUFFER, number_of_vertices*sizeof( GL_FLOAT ), &v[0], GL_STATIC_DRAW );
//        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
//        glEnableVertexAttribArray( 0 );
//
//        vector< GLfloat > colors;
//        flowvisualizationc->getColors( colors );
//        GLint ncolors = (int) colors.size();
//
//        glBindBuffer( GL_ARRAY_BUFFER, bf_mesh[ 1 ] );
//        glBufferData( GL_ARRAY_BUFFER, ncolors*sizeof( GL_FLOAT ), colors.data(), GL_STATIC_DRAW );
//        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
//        glEnableVertexAttribArray( 1 );
//
////
////        vector< GLuint > lines;
////        flowvisualizationc->getWireframe( lines );
////
////        if( lines.empty() == false )
////        {
////            number_of_lines = (GLint) lines.size();
////            glGenBuffers( 1, &bf_lines );
////            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_lines );
////            glBufferData( GL_ELEMENT_ARRAY_BUFFER, number_of_lines*sizeof( GLuint ) , lines.data(), GL_STATIC_DRAW );
////
////        }
////
////
////        vector< GLuint > faces;
////        flowvisualizationc->getTriangles( faces );
////
////        if( faces.empty() == false )
////        {
////            number_of_faces = (GLint) faces.size();
////
////            glGenBuffers( 1, &bf_faces );
////            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces );
////            glBufferData( GL_ELEMENT_ARRAY_BUFFER, number_of_faces*sizeof( GLuint ) , faces.data(), GL_STATIC_DRAW );
////
////        }
//
//
//        glBindBuffer( GL_ARRAY_BUFFER, 0 );
//        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
//
//
//    glBindVertexArray( 0 );

}

void CanvasComputation::sendColorsGPU ( std::string property , std::string type , int option )
{
//    if( property.empty() == false && type.empty() == false )
//        flowvisualizationc->setCurrentProperty( property, type );
//
//    glBindVertexArray( vao_mesh );
//
//        vector< GLfloat > colors;
//
//        if( option == 0 )
//            flowvisualizationc->getColors( colors );
//        else
//            flowvisualizationc->getColors( colors, option );
//
//        GLint ncolors = (GLint) colors.size();
//
//        glBindBuffer( GL_ARRAY_BUFFER, bf_mesh[ 1 ] );
//        glBufferData( GL_ARRAY_BUFFER, ncolors*sizeof( GL_FLOAT ), colors.data(), GL_STATIC_DRAW );
//        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
//        glEnableVertexAttribArray( 1 );
//
//    glBindBuffer( GL_ARRAY_BUFFER, 0 );

	if ( property.empty ( ) == false && type.empty ( ) == false )
		flowvisualizationc->setCurrentProperty ( property , type );

	vector<GLfloat> colors;

	if ( option == 0 )
		flowvisualizationc->getColors ( colors );
	else
		flowvisualizationc->getColors ( colors , option );

	GLint ncolors = (GLint) colors.size ( );

	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_colors_ );
	glBufferData ( GL_ARRAY_BUFFER , ncolors * sizeof ( GL_FLOAT ) , colors.data ( ) , GL_STATIC_DRAW );

	update ( );

}

void CanvasComputation::showVolumetricGrid ( )
{
//    flowvisualizationc->readData();
//    fillMenuProperties();
	sendMeshGPU ( );

	update ( );
}

void CanvasComputation::resetSetup ( )
{
	resetVisualization ( );
	resetCamera ( );
	resetData ( );
}

void CanvasComputation::resetData ( )
{

	// empty controller

	number_of_vertices = 0;
	number_of_faces = 0;
	number_of_lines = 0;

	rd_properties.clear ( );
	rd_options_vector.clear ( );
	ac_properties.clear ( );
	mn_vector_properties_points.clear ( );

	flowvisualizationc->clear ( );

}

void CanvasComputation::resetCamera ( )
{

	camera.reset ( );
}

void CanvasComputation::resetVisualization ( )
{
	speed_mouse = 0.55f;
	speed_rotation = 0.5f;

	show_vertices = true;
	show_faces = true;
	show_lines = true;

}

void CanvasComputation::mousePressEvent ( QMouseEvent *event )
{

	if ( ( event->buttons ( ) & Qt::RightButton ) && ( event->modifiers ( ) == Qt::ControlModifier ) )
	{
		mn_options->exec ( event->globalPos ( ) );
	}

	/// Tucano
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

}

void CanvasComputation::mouseMoveEvent ( QMouseEvent *event )
{

	/// Tucano
	Eigen::Vector2f screen_pos ( event->x ( ) , event->y ( ) );
	if ( ( event->modifiers ( ) & Qt::ShiftModifier ) )
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

void CanvasComputation::mouseReleaseEvent ( QMouseEvent *event )
{

	if ( event->button ( ) == Qt::LeftButton )
	{
		camera.endTranslation ( );
		camera.endRotation ( );
	}
	if ( ( event->button ( ) == Qt::RightButton ) )
	{
		//light_trackball.endRotation();

//		p.push_back(sketch_);
//
//		createSurfacePatchies(p,10.0f,100.0f, 500.0f);
	}

	update ( );
}

void CanvasComputation::wheelEvent ( QWheelEvent *event )
{

//    if( m->orientation() == Qt::Vertical )
//    {
//
//        int delta = m->delta();
//
//        if( delta < 0 )
//            zoom *= 1.1f;
//        else
//            zoom *= 0.9f;
//
//        update();
//
//    }
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

void CanvasComputation::showPoints ( bool option )
{
	show_vertices = option;
	update ( );
}

void CanvasComputation::showVolume ( bool option )
{
	show_faces = option;
	update ( );
}

void CanvasComputation::showWireframe ( bool option )
{
	show_lines = option;
	update ( );
}

void CanvasComputation::resetTransformations ( )
{
	resetCamera ( );
	flowvisualizationc->setCurrentProperty ( 0 );
}

void CanvasComputation::setColorMap ( )
{
	flowvisualizationc->setColorMap ( "JET" );
	sendColorsGPU ( "" , "" );
	update ( );
}

void CanvasComputation::setColorMapConstant ( )
{
	flowvisualizationc->setColorMap ( "CONSTANT" );
	sendColorsGPU ( "" , "" );
	update ( );
}

void CanvasComputation::sendSurfaceGPU ( )
{

	vector<GLfloat> vertices;
	vector<GLuint> lines;
	flowvisualizationc->getPointsSurface ( vertices );

	number_of_vertices = (GLuint) vertices.size ( );

	vertices_.clear ( );

	for ( std::size_t it = 0; it < vertices.size ( ) - 3; it += 3 )
	{
		vertices_.push_back ( Eigen::Vector3f ( vertices[it] , vertices[it + 1] , vertices[it + 2] ) );
	}

	box.fromPointCloud ( vertices_.begin ( ) , vertices_.end ( ) );

	for ( std::size_t it = 0; it < vertices_.size ( ); it++ )
	{
		std::cout << " Points " << vertices_[it] << std::endl;
	}

	std::cout << "Box Center Clarissa " << box.center ( ) << std::endl;

	for ( std::size_t it = 0; it < vertices_.size ( ); it++ )
	{
		vertices_[it] = ( vertices_[it] - box.center ( ) ) / box.diagonal ( );
	}

	box.fromPointCloud ( vertices_.begin ( ) , vertices_.end ( ) );

	for ( std::size_t it = 0; it < vertices_.size ( ); it++ )
	{
		std::cout << " Center " << vertices_[it] << std::endl;
	}
	/// Requesting Vertex Buffers to the GPU
	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_MESH_ );
	glBufferData ( GL_ARRAY_BUFFER , vertices_.size ( ) * sizeof ( vertices_[0] ) , &vertices_[0] , GL_STATIC_DRAW );
	// Set up generic attributes pointers

	vector<GLfloat> colors;
	flowvisualizationc->getSurfaceColors ( colors );
	GLint ncolors = (int) colors.size ( );

	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_colors_ );
	glBufferData ( GL_ARRAY_BUFFER , ncolors * sizeof ( GL_FLOAT ) , colors.data ( ) , GL_STATIC_DRAW );

	vector<GLuint> faces;
	flowvisualizationc->getTrianglesSurface ( faces );

	if ( faces.empty ( ) == false )
	{
		number_of_faces = (GLint) faces.size ( );

		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vertexBuffer_face_ID_ );
		glBufferData ( GL_ELEMENT_ARRAY_BUFFER , faces.size ( ) * sizeof ( faces[0] ) , &faces[0] , GL_STATIC_DRAW );
	}

//
//    glPointSize( 3.0f );
//    glGenVertexArrays( 1, &vao_mesh );
//
//
//    glGenBuffers( 1, &bf_vertices );
//    glBindVertexArray( vao_mesh );
//
//
//        vertices_.clear();
//
//	std::cout << "Box Center Clarissa " << box.center() << std::endl;
//
//        glBindBuffer( GL_ARRAY_BUFFER, bf_vertices );
//        glBufferData( GL_ARRAY_BUFFER, vertices_.size()*sizeof( GL_FLOAT ), &vertices_[0], GL_STATIC_DRAW );
//        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
//        glEnableVertexAttribArray( 0 );
//
//        vector< GLfloat > colors;
//        flowvisualizationc->getSurfaceColors( colors );
//        GLint ncolors = (int) colors.size();
//
//
//        glGenBuffers( 1, &bf_colors );
//        glBindBuffer( GL_ARRAY_BUFFER, bf_colors);
//        glBufferData( GL_ARRAY_BUFFER, ncolors*sizeof( GL_FLOAT ), colors.data(), GL_STATIC_DRAW );
//        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
//        glEnableVertexAttribArray( 1 );
//
//        flowvisualizationc->getWireframeSurface( lines );
//
//        if( lines.empty() == false )
//        {
//            number_of_lines = (GLint) lines.size();
//            glGenBuffers( 1, &bf_lines );
//            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_lines );
//            glBufferData( GL_ELEMENT_ARRAY_BUFFER, number_of_lines*sizeof( GLuint ) , lines.data(), GL_STATIC_DRAW );
//
//        }
//
//        vector< GLuint > faces;
//        flowvisualizationc->getTrianglesSurface( faces );
//
//        if( faces.empty() == false )
//        {
//            number_of_faces = (GLint) faces.size();
//
//            glGenBuffers( 1, &bf_faces );
//            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces );
//            glBufferData( GL_ELEMENT_ARRAY_BUFFER, number_of_faces*sizeof( GLuint ) , faces.data(), GL_STATIC_DRAW );
//
//        }
//
//
//
//        glBindBuffer( GL_ARRAY_BUFFER, 0 );
//        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
//
//
////    glBindVertexArray( 0 );

}

void CanvasComputation::selectProperty ( int id , bool option , int option_color )
{
	FlowProperty& p = flowvisualizationc->getPropertyfromMap ( id );
	std::string name;
	std::string type;

	p.getName ( name );
	p.getType ( type );
	if ( option == true )
		sendColorsGPU ( name , type , option_color );
	else
		sendColorsGPU ( name , type );

}

void CanvasComputation::showSurface ( )
{
	sendSurfaceGPU ( );
	update ( );
}

void CanvasComputation::exportFile ( )
{
	QString selected_format = "";
	QString fname_string = QFileDialog::getSaveFileName ( this , tr ( "Save File" ) , "/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/inputs" , "VTK files (*.vtk)" , &selected_format );

	flowvisualizationc->exportFile ( fname_string.toStdString ( ) );
}

