#include "OpenGLWidget.hpp"

GLWidget::GLWidget ( QWidget* parent ) : QOpenGLWidget ( parent )
{

	/// Internface

        menu_module_type_ = new QMenu(this);
        action_seismic_module_ = new QAction(tr ("Seismic Module"),menu_module_type_);
        action_blankSceen_module_= new QAction( tr ("Black Screen Module"),menu_module_type_);

        menu_module_type_->addSection ( tr ( "Module" ) );
        menu_module_type_->addAction(action_seismic_module_);
        menu_module_type_->addAction(action_blankSceen_module_);

	connect ( action_seismic_module_ , SIGNAL( triggered()) ,this, SLOT(setSeismicModule()) );
	connect ( action_blankSceen_module_ , SIGNAL (triggered()) ,this, SLOT(setBlackScreenModule()) );

	// Mesh Layout:
	// - Geometry   vec4  slot = 0
	// - Normal     vec4  slot = 1
	// - Colour     vec4  slot = 2
	// - Attributes vec4  slot = 3

	// [v0,v1,v2,v3,n0,n1,n2,n3,c0,c1,c2,c4,att0,att1,att2,att3]

	// Scene
	background_ = 0;

	// Entity
	vertexArray_BlackScreen_cube_ = 0;
		vertexBuffer_BlackScreen_cube_ = 0;
		// Be careful on the assignment of each slot attributes
		position_BlackScreen_slot_ = 0;
		blackScreen_cube_shader_ = 0;

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

	mesh_shader_ = 0;
	vertexArray_MESH_ = 0;
		positionBuffer_MESH_ = 0;
			position_MESH_Slot_ = 0;
		normalBuffer_MESH_ = 0;
			normal_MESH_Slot_ = 1;
		colorBuffer_MESH_ = 0;
			color_MESH_Slot_ = 2;
		// Element Array
		vertexBuffer_MESH_face_ID_ = 0;

	// Entity
	vertexArray_Seismic_cube_ = 0;
		vertexBuffer_Seismic_cube_ = 0;
		// Be careful on the assignment of each slot attributes
		position_seismic_cube_ = 0;
		seismic_cube_shader_ = 0;

	// Entity
	vertexArray_Seismic_plane_ = 0;
		vertexBuffer_Seismic_plane_ = 0;
		// Be careful on the assignment of each slot attributes
		position_seismic_plane_ = 0;
		seismic_plane_shader_ = 0;
		seismic_slice_plane_index = 0;
		seismic_slice_plane_position = 0.0f;

	vertexArray_for_the_Cube_ = 0;
		vertexBuffer_cube_8vertices_ = 0;
		vertices_8slot_ = 0;
		vertexBuffer_cube_8verticesIndices_ = 0;


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
	glEnable(GL_MULTISAMPLE);
	glMinSampleShading(1.0f);

	/// BLACK SCREEN ----
	glGenVertexArrays ( 1 , &vertexArray_BlackScreen_cube_ );
	glBindVertexArray ( vertexArray_BlackScreen_cube_ );

		/// Requesting Vertex Buffers to the GPU
		glGenBuffers ( 1 , &vertexBuffer_BlackScreen_cube_);
		glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_BlackScreen_cube_ );
		glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
		// Set up generic attributes pointers
		glEnableVertexAttribArray ( position_BlackScreen_slot_ );
		glVertexAttribPointer ( position_BlackScreen_slot_ , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

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
		glVertexAttribPointer ( vertexPatch_slot_ , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

	glBindVertexArray ( 0 );


	/// SEISMIC ----
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
	loadShaders();

	//596x291x297

	glGenVertexArrays ( 1 , &vertexArray_MESH_ );
	glBindVertexArray ( vertexArray_MESH_ );

		/// Requesting Vertex Buffers to the GPU
		glGenBuffers ( 1 , &positionBuffer_MESH_ );
		glBindBuffer ( GL_ARRAY_BUFFER , positionBuffer_MESH_ );
		glBufferData ( GL_ARRAY_BUFFER , 0 , 0 , GL_STATIC_DRAW );

		// Set up generic attributes pointers
		glEnableVertexAttribArray ( position_MESH_Slot_ );
		glVertexAttribPointer ( position_MESH_Slot_ , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

		/// Requesting Vertex Buffers to the GPU
		glGenBuffers ( 1 , &normalBuffer_MESH_ );
		glBindBuffer ( GL_ARRAY_BUFFER , normalBuffer_MESH_ );
		glBufferData ( GL_ARRAY_BUFFER , 0 , 0 , GL_STATIC_DRAW );

		glEnableVertexAttribArray ( normal_MESH_Slot_ );
		glVertexAttribPointer ( normal_MESH_Slot_ , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

		/// Requesting Vertex Buffers to the GPU
		glGenBuffers ( 1 , &colorBuffer_MESH_ );
		glBindBuffer ( GL_ARRAY_BUFFER , colorBuffer_MESH_ );
		glBufferData ( GL_ARRAY_BUFFER , 0 , 0 , GL_STATIC_DRAW );

		glEnableVertexAttribArray ( color_MESH_Slot_ );
		glVertexAttribPointer ( color_MESH_Slot_ , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

		/// Requesting Vertex Buffers to the GPU
		glGenBuffers ( 1 , &vertexBuffer_MESH_face_ID_ );
		glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vertexBuffer_MESH_face_ID_ );
		glBufferData ( GL_ELEMENT_ARRAY_BUFFER , 0 , 0 , GL_STATIC_DRAW );

	glBindVertexArray ( 0 );

	glGenVertexArrays ( 1 , &vertexArray_Seismic_cube_ );
	glBindVertexArray ( vertexArray_Seismic_cube_ );

		/// Requesting Vertex Buffers to the GPU
		glGenBuffers ( 1 , &vertexBuffer_Seismic_cube_ );
		glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_Seismic_cube_ );
		glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
		// Set up generic attributes pointers
		glEnableVertexAttribArray ( position_seismic_cube_ );
		glVertexAttribPointer ( position_seismic_cube_ , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );

	glBindVertexArray ( 0 );

	glGenVertexArrays ( 1 , &vertexArray_Seismic_plane_ );
	glBindVertexArray ( vertexArray_Seismic_plane_ );

		/// Requesting Vertex Buffers to the GPU
		glGenBuffers ( 1 , &vertexBuffer_Seismic_plane_ );
		glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_Seismic_plane_ );
		glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
		// Set up generic attributes pointers
		glEnableVertexAttribArray ( position_seismic_plane_ );
		glVertexAttribPointer ( position_seismic_plane_ , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );

	glBindVertexArray ( 0 );


	create8VerticesIndices();


	extrusionInitialize(0.0,0.0,0.0,596.0,291.0,297.0);

	this->extrusion_controller_.module_ = RRM::ExtrusionController::Seismic;

	camera.setPerspectiveMatrix ( 60.0 , (float) this->width ( ) / (float) this->height ( ) , 0.1f , 100.0f );

	/// Black Screen


	std::vector<std::vector<Eigen::Vector3f> > v;

	std::vector<Eigen::Vector3f>  poly;
	std::vector<Eigen::Vector3f>  temp;

	poly.push_back( Eigen::Vector3f ( -1.0 , 0.25 , 0.0 ) );
	poly.push_back( Eigen::Vector3f ( 1.0 , 0.25 , 0.0 ) );

	temp.push_back( Eigen::Vector3f ( -1.0 , -1.0 , 0.0 ) );
	temp.push_back( Eigen::Vector3f ( 1.0 , -1.0 , 0.0 ) );

	v.push_back(poly);
	poly.clear();

	poly.push_back( Eigen::Vector3f ( -1.0 , 0.5 , 0.0 ) );
	poly.push_back( Eigen::Vector3f ( 1.0 , 0.5 , 0.0 ) );

	temp.push_back( Eigen::Vector3f ( -1.0 , 1.0 , 2.0 ) );
	temp.push_back( Eigen::Vector3f ( 1.0 , 1.0 , 2.0 ) );

	v.push_back(poly);

	// Create the bounding box in Image space
	Celer::BoundingBox3<float> bbox;
	bbox.fromPointCloud(temp.begin(),temp.end());

//	this->extrusion_controller_.createBlackScreenCube(bbox,cube_);
//
//	this->extrusion_controller_.createBlackScreenExtrusionMesh( v, 1,1,2, bbox.center(), bbox.diagonal(), this->patch_ );
//
//	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_cube_ );
//	glBufferData ( GL_ARRAY_BUFFER , cube_.size ( ) * sizeof ( cube_[0] ) , cube_.data() , GL_STATIC_DRAW );
//	glBindBuffer ( GL_ARRAY_BUFFER , 0);
//
//	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_patch_ );
//	glBufferData ( GL_ARRAY_BUFFER , patch_.size ( ) * sizeof ( patch_[0] ) , patch_.data() , GL_STATIC_DRAW );
//	glBindBuffer ( GL_ARRAY_BUFFER , 0);

}

void GLWidget::reloadShaders ( )
{
	if ( blackScreen_cube_shader_ )
	{
		blackScreen_cube_shader_->reloadShaders ( );
	}
	if ( lines_shader_ )
	{
		lines_shader_->reloadShaders ( );
	}
	if ( background_ )
	{
		background_->reloadShaders ( );
	}
	if ( mesh_shader_ )
	{
		mesh_shader_->reloadShaders ( );
	}
	if ( seismic_cube_shader_ )
	{
		seismic_cube_shader_->reloadShaders ( );
	}
	if ( seismic_plane_shader_ )
	{
		seismic_plane_shader_->reloadShaders ( );
	}
	if ( patch_shader_ )
	{
		patch_shader_->reloadShaders ( );
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

	//! Effects -_
	blackScreen_cube_shader_ = new Tucano::Shader ( "Cube" , ( shaderDirectory + "Shaders/BlankScreenCube.vert" ).toStdString ( ),
					             ( shaderDirectory + "Shaders/BlankScreenCube.frag" ).toStdString ( ),
						     ( shaderDirectory + "Shaders/BlankScreenCube.geom" ).toStdString ( ) , "" , "" );
	blackScreen_cube_shader_->initialize ( );
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

		mesh_shader_ = new Tucano::Shader("Seismic", (shaderDirectory + "Shaders/Seismic.vert").toStdString(),
													(shaderDirectory + "Shaders/Seismic.frag").toStdString(),
													(shaderDirectory + "Shaders/Seismic.geom").toStdString(), "", "");
		mesh_shader_->initialize();

	seismic_cube_shader_ = new Tucano::Shader ( "Seismic  Cube" , ( shaderDirectory + "Shaders/CubeSinglePassWireframe.vert" ).toStdString ( ),
					             	     	      ( shaderDirectory + "Shaders/CubeSinglePassWireframe.frag" ).toStdString ( ),
								      ( shaderDirectory + "Shaders/CubeSinglePassWireframe.geom" ).toStdString ( ) , "" , "" );
	seismic_cube_shader_->initialize ( );

	//! Effects --
	seismic_plane_shader_ = new Tucano::Shader ( "Seismic  Plane",( shaderDirectory + "Shaders/SeismicSlicePlane.vert" ).toStdString ( ),
					             	     	      ( shaderDirectory + "Shaders/SeismicSlicePlane.frag" ).toStdString ( ),
								      ( shaderDirectory + "Shaders/SeismicSlicePlane.geom" ).toStdString ( ) , "" , "" );
	seismic_plane_shader_->initialize ( );

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
	blackScreen_cube_shader_ = new Tucano::Shader ( "Cube" , ( shaderDirectory + "Shaders/BlankScreenCube.vert" ).toStdString ( ),
					             ( shaderDirectory + "Shaders/BlankScreenCube.frag" ).toStdString ( ),
						     ( shaderDirectory + "Shaders/BlankScreenCube.geom" ).toStdString ( ) , "" , "" );
	blackScreen_cube_shader_->initialize ( );
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


        mesh_shader_ = new Tucano::Shader ( "Seismic" , ( shaderDirectory + "Shaders/Seismic.vert" ).toStdString ( ),
					                ( shaderDirectory + "Shaders/Seismic.frag" ).toStdString ( ),
							( shaderDirectory + "Shaders/Seismic.geom" ).toStdString ( ), "" , "" );
        mesh_shader_->initialize ( );

	//! Effects --
	seismic_cube_shader_ = new Tucano::Shader ( "Seismic  Cube" , ( shaderDirectory + "Shaders/CubeSinglePassWireframe.vert" ).toStdString ( ),
					             	     	      ( shaderDirectory + "Shaders/CubeSinglePassWireframe.frag" ).toStdString ( ),
								      ( shaderDirectory + "Shaders/CubeSinglePassWireframe.geom" ).toStdString ( ) , "" , "" );
	seismic_cube_shader_->initialize ( );

	//! Effects --
	seismic_plane_shader_ = new Tucano::Shader ( "Seismic  Plane",( shaderDirectory + "Shaders/SeismicSlicePlane.vert" ).toStdString ( ),
					             	     	      ( shaderDirectory + "Shaders/SeismicSlicePlane.frag" ).toStdString ( ),
								      ( shaderDirectory + "Shaders/SeismicSlicePlane.geom" ).toStdString ( ) , "" , "" );
	seismic_plane_shader_->initialize ( );

}

void GLWidget::clear()
{

	lines_.clear();

	glBindBuffer ( GL_ARRAY_BUFFER , lines_vertexBuffer_ );
	glBufferData ( GL_ARRAY_BUFFER , lines_.size ( ) * sizeof ( lines_[0] ) , lines_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER , 0 );

	facesGL_.clear();
	vertex_.clear();
	normal_.clear();

	glBindBuffer ( GL_ARRAY_BUFFER , positionBuffer_MESH_ );
	glBufferData ( GL_ARRAY_BUFFER , this->vertex_.size() * sizeof ( this->vertex_[0] ) , this->vertex_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER , 0 );

	glBindBuffer ( GL_ARRAY_BUFFER , normalBuffer_MESH_ );
	glBufferData ( GL_ARRAY_BUFFER , this->normal_.size() * sizeof ( this->normal_[0] ) , this->normal_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER , 0 );

	glBindBuffer ( GL_ARRAY_BUFFER , colorBuffer_MESH_ );
	glBufferData ( GL_ARRAY_BUFFER , this->normal_.size() * sizeof ( this->normal_[0] ) , this->normal_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER , 0 );

	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vertexBuffer_MESH_face_ID_ );
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER , this->facesGL_.size() * sizeof ( this->facesGL_[0] )  , this->facesGL_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 );

}

void GLWidget::backGround()
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	background_->bind();

	background_->setUniform("viewportSize", width(), height() );

	glBindVertexArray ( vertexArray_BlackScreen_cube_ );
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


	if ( this->extrusion_controller_.module_ == RRM::ExtrusionController::Seismic )
	{
		glLineWidth(2.0);
		if ( lines_.size() > 0 )
		{
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

		// Triangles
		mesh_shader_->bind ( );
		/// 3rd attribute buffer : vertices
		mesh_shader_->setUniform ( "ModelMatrix" , camera.getViewMatrix ( ) );
		mesh_shader_->setUniform ( "ViewMatrix" , camera.getViewMatrix ( ) );
		mesh_shader_->setUniform ( "ProjectionMatrix" , camera.getProjectionMatrix ( ) );
		mesh_shader_->setUniform ( "WIN_SCALE" , (float) width ( ) , (float) height ( ) );

			glBindVertexArray ( vertexArray_MESH_ );
			/// Draw the triangle !
				glDrawElements ( GL_TRIANGLES , this->facesGL_.size() , GL_UNSIGNED_INT , 0 );
		//		mesh_shader_->setUniform ( "xcolor" , 0.0f,0.0f,0.0f );
				//glDrawArrays ( GL_POINTS , 0 , this->vertex_.size ( ) );
			glBindVertexArray ( 0 );
		mesh_shader_->unbind ( );

		seismic_plane_shader_->bind ( );
		/// 3rd attribute buffer : vertices
		seismic_plane_shader_->setUniform ( "ModelMatrix" , camera.getViewMatrix ( ) );
		seismic_plane_shader_->setUniform ( "ViewMatrix" , camera.getViewMatrix ( ) );
		seismic_plane_shader_->setUniform ( "ProjectionMatrix" , camera.getProjectionMatrix ( ) );
		seismic_plane_shader_->setUniform ( "WIN_SCALE" , (float) width ( ) , (float) height ( ) );
		seismic_plane_shader_->setUniform ( "color_plane" , 0.0f,1.0f,0.0f,1.0f );
		seismic_plane_shader_->setUniform ( "z" , this->seismic_slice_plane_position );
			glBindVertexArray ( vertexArray_Seismic_plane_ );
			/// Draw the triangle !
			glDrawArrays ( GL_LINES_ADJACENCY , 0 , seismic_plane_.size() );

			glBindVertexArray ( 0 );
		seismic_plane_shader_->unbind ( );

		seismic_cube_shader_->bind ( );
		/// 3rd attribute buffer : vertices
		seismic_cube_shader_->setUniform ( "ModelMatrix" , camera.getViewMatrix ( ) );
		seismic_cube_shader_->setUniform ( "ViewMatrix" , camera.getViewMatrix ( ) );
		seismic_cube_shader_->setUniform ( "ProjectionMatrix" , camera.getProjectionMatrix ( ) );
		seismic_cube_shader_->setUniform ( "WIN_SCALE" , (float) width ( ) , (float) height ( ) );
		seismic_cube_shader_->setUniform ( "color_plane" , 0.5f,0.5f,0.5f,0.2f );
			glBindVertexArray ( vertexArray_Seismic_cube_ );
			/// Draw the triangle !
			glDrawArrays ( GL_LINES_ADJACENCY , 0 , seismic_cube_.size() );

			glBindVertexArray ( 0 );
		seismic_cube_shader_->unbind ( );


	}else if ( this->extrusion_controller_.module_ == RRM::ExtrusionController::BlankScreen )
	{

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

		blackScreen_cube_shader_->bind ( );
		/// 3rd attribute buffer : vertices
		blackScreen_cube_shader_->setUniform ( "ModelMatrix" , camera.getViewMatrix ( ) );
		blackScreen_cube_shader_->setUniform ( "ViewMatrix" , camera.getViewMatrix ( ) );
		blackScreen_cube_shader_->setUniform ( "ProjectionMatrix" , camera.getProjectionMatrix ( ) );
		blackScreen_cube_shader_->setUniform ( "WIN_SCALE" , (float) width ( ) , (float) height ( ) );
		glBindVertexArray ( vertexArray_BlackScreen_cube_ );
		/// Draw the triangle !
		glDrawArrays ( GL_LINES_ADJACENCY , 0 , blackScreen_cube_.size() );

		glBindVertexArray ( 0 );
		blackScreen_cube_shader_->unbind ( );

	}

//	glPointSize(5.0);
//	glBindVertexArray(vertexArray_for_the_Cube_);
//		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer_cube_8verticesIndices_);
//	//	// Draw the triangle !
//		glDrawElements(GL_TRIANGLES, indices.size() , GL_UNSIGNED_INT, 0);
//
//	glBindVertexArray(0);


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

		case Qt::Key_Space:
		{
			this->updateRendering();
		}
			break;

		case Qt::Key_F2:
		{
			this->facesGL_.clear();

			this->extrusion_controller_.setResolution(8,this->vertex_,this->normal_,this->faces_);

			for (auto f: this->faces_ )
			{
				this->facesGL_.push_back(GLuint(f));
			}

			glBindBuffer ( GL_ARRAY_BUFFER , positionBuffer_MESH_ );
			glBufferData ( GL_ARRAY_BUFFER , this->vertex_.size() * sizeof ( this->vertex_[0] ) , this->vertex_.data() , GL_STATIC_DRAW );
			glBindBuffer ( GL_ARRAY_BUFFER , 0 );

			glBindBuffer ( GL_ARRAY_BUFFER , normalBuffer_MESH_ );
			glBufferData ( GL_ARRAY_BUFFER , this->normal_.size() * sizeof ( this->normal_[0] ) , this->normal_.data() , GL_STATIC_DRAW );
			glBindBuffer ( GL_ARRAY_BUFFER , 0 );

			glBindBuffer ( GL_ARRAY_BUFFER , colorBuffer_MESH_ );
			glBufferData ( GL_ARRAY_BUFFER , this->normal_.size() * sizeof ( this->normal_[0] ) , this->normal_.data() , GL_STATIC_DRAW );
			glBindBuffer ( GL_ARRAY_BUFFER , 0 );

			glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vertexBuffer_MESH_face_ID_ );
			glBufferData ( GL_ELEMENT_ARRAY_BUFFER , this->facesGL_.size() * sizeof ( this->facesGL_[0] )  , this->facesGL_.data() , GL_STATIC_DRAW );
			glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 );

			update();
		}
			break;
		case Qt::Key_F3:
		{
			this->facesGL_.clear();

			this->extrusion_controller_.setResolution(16,this->vertex_,this->normal_,this->faces_);

			for (auto f: this->faces_ )
			{
				this->facesGL_.push_back(GLuint(f));
			}

			glBindBuffer ( GL_ARRAY_BUFFER , positionBuffer_MESH_ );
			glBufferData ( GL_ARRAY_BUFFER , this->vertex_.size() * sizeof ( this->vertex_[0] ) , this->vertex_.data() , GL_STATIC_DRAW );
			glBindBuffer ( GL_ARRAY_BUFFER , 0 );

			glBindBuffer ( GL_ARRAY_BUFFER , normalBuffer_MESH_ );
			glBufferData ( GL_ARRAY_BUFFER , this->normal_.size() * sizeof ( this->normal_[0] ) , this->normal_.data() , GL_STATIC_DRAW );
			glBindBuffer ( GL_ARRAY_BUFFER , 0 );

			glBindBuffer ( GL_ARRAY_BUFFER , colorBuffer_MESH_ );
			glBufferData ( GL_ARRAY_BUFFER , this->normal_.size() * sizeof ( this->normal_[0] ) , this->normal_.data() , GL_STATIC_DRAW );
			glBindBuffer ( GL_ARRAY_BUFFER , 0 );

			glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vertexBuffer_MESH_face_ID_ );
			glBufferData ( GL_ELEMENT_ARRAY_BUFFER , this->facesGL_.size() * sizeof ( this->facesGL_[0] )  , this->facesGL_.data() , GL_STATIC_DRAW );
			glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 );

			update();
		}
			break;
		case Qt::Key_F4:
		{
			this->facesGL_.clear();

			this->extrusion_controller_.setResolution(32,this->vertex_,this->normal_,this->faces_);

			for (auto f: this->faces_ )
			{
				this->facesGL_.push_back(GLuint(f));
			}

			glBindBuffer ( GL_ARRAY_BUFFER , positionBuffer_MESH_ );
			glBufferData ( GL_ARRAY_BUFFER , this->vertex_.size() * sizeof ( this->vertex_[0] ) , this->vertex_.data() , GL_STATIC_DRAW );
			glBindBuffer ( GL_ARRAY_BUFFER , 0 );

			glBindBuffer ( GL_ARRAY_BUFFER , normalBuffer_MESH_ );
			glBufferData ( GL_ARRAY_BUFFER , this->normal_.size() * sizeof ( this->normal_[0] ) , this->normal_.data() , GL_STATIC_DRAW );
			glBindBuffer ( GL_ARRAY_BUFFER , 0 );

			glBindBuffer ( GL_ARRAY_BUFFER , colorBuffer_MESH_ );
			glBufferData ( GL_ARRAY_BUFFER , this->normal_.size() * sizeof ( this->normal_[0] ) , this->normal_.data() , GL_STATIC_DRAW );
			glBindBuffer ( GL_ARRAY_BUFFER , 0 );

			glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vertexBuffer_MESH_face_ID_ );
			glBufferData ( GL_ELEMENT_ARRAY_BUFFER , this->facesGL_.size() * sizeof ( this->facesGL_[0] )  , this->facesGL_.data() , GL_STATIC_DRAW );
			glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 );

			update();

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

	if ( ( event->buttons ( ) & Qt::RightButton ) && ( event->modifiers ( ) & Qt::AltModifier )  )
	{
		menu_module_type_->exec ( event->globalPos ( ) );
	}

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
void GLWidget::setSeismicModule()
{
	this->extrusion_controller_.module_ = RRM::ExtrusionController::Seismic;
}

void GLWidget::setPlanePosition( int _index )
{
	this->seismic_slice_plane_position = this->extrusion_controller_.slicePositon(_index);
	update();
}

void GLWidget::updateSeismicSlices ( const SeismicSlices& _seismic_slices )
{

	this->extrusion_controller_.setSeismicSlices(_seismic_slices);

}

void GLWidget::updateRendering()
{
	this->lines_.clear();
	this->facesGL_.clear();

	this->lines_ = this->extrusion_controller_.updateSeismicSlices(this->vertex_,this->normal_,this->faces_);

	/// Send the new meshes to the GPU
	glBindBuffer ( GL_ARRAY_BUFFER , lines_vertexBuffer_ );
	glBufferData ( GL_ARRAY_BUFFER , lines_.size ( ) * sizeof ( lines_[0] ) , lines_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER , 0 );

	//
	for (auto f: this->faces_ )
	{
		this->facesGL_.push_back(GLuint(f));
	}

	glBindBuffer ( GL_ARRAY_BUFFER , positionBuffer_MESH_ );
	glBufferData ( GL_ARRAY_BUFFER , this->vertex_.size() * sizeof ( this->vertex_[0] ) , this->vertex_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER , 0 );

	glBindBuffer ( GL_ARRAY_BUFFER , normalBuffer_MESH_ );
	glBufferData ( GL_ARRAY_BUFFER , this->normal_.size() * sizeof ( this->normal_[0] ) , this->normal_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER , 0 );

	glBindBuffer ( GL_ARRAY_BUFFER , colorBuffer_MESH_ );
	glBufferData ( GL_ARRAY_BUFFER , this->normal_.size() * sizeof ( this->normal_[0] ) , this->normal_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER , 0 );

	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , vertexBuffer_MESH_face_ID_ );
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER , this->facesGL_.size() * sizeof ( this->facesGL_[0] )  , this->facesGL_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 );

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


	this->seismic_cube_ = this->extrusion_controller_.getCubeMesh();

	this->seismic_plane_ = this->extrusion_controller_.getPlaneMesh(0.0f);
	this->seismic_slice_plane_position = this->extrusion_controller_.slicePositon(this->seismic_slice_plane_index);

	/// Send the new meshes to the GPU
	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_Seismic_plane_ );
	glBufferData ( GL_ARRAY_BUFFER , this->seismic_plane_.size ( ) * sizeof ( this->seismic_plane_[0] ) , this->seismic_plane_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER , 0 );

	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_Seismic_cube_ );
	glBufferData ( GL_ARRAY_BUFFER , this->seismic_cube_.size ( ) * sizeof ( this->seismic_cube_[0] ) , this->seismic_cube_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER , 0);

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

/// BLACK SCREEN
/// Seismic Module
void GLWidget::setBlackScreenModule()
{
	this->extrusion_controller_.module_ = RRM::ExtrusionController::BlankScreen;
}


void GLWidget::updateBlackScreen(const CrossSection& _cross_section)
{
	std::cout << "New Curve" << std::endl;
	_cross_section.log();

	this->extrusion_controller_.setBlackScreenCrossSection(_cross_section);
	this->extrusion_controller_.updateBlackScreenMesh(stepx,stepz,500,blackScreen_cube_,patch_);

	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_BlackScreen_cube_ );
	glBufferData ( GL_ARRAY_BUFFER , blackScreen_cube_.size ( ) * sizeof ( blackScreen_cube_[0] ) , blackScreen_cube_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER , 0);

	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_patch_ );
	glBufferData ( GL_ARRAY_BUFFER , patch_.size ( ) * sizeof ( patch_[0] ) , patch_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER , 0);

	update();
}

void GLWidget::black_screen_stepx ( int x )
{
	this->stepx = static_cast<float>(x);
	this->extrusion_controller_.updateBlackScreenMesh(stepx,stepz,volume_width,blackScreen_cube_,patch_);

	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_BlackScreen_cube_ );
	glBufferData ( GL_ARRAY_BUFFER , blackScreen_cube_.size ( ) * sizeof ( blackScreen_cube_[0] ) , blackScreen_cube_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER , 0);

	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_patch_ );
	glBufferData ( GL_ARRAY_BUFFER , patch_.size ( ) * sizeof ( patch_[0] ) , patch_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER , 0);

	update();
}
void GLWidget::black_screen_stepz ( int z )
{
	this->stepz = static_cast<float>(z);
	this->extrusion_controller_.updateBlackScreenMesh(stepx,stepz,volume_width,blackScreen_cube_,patch_);

	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_BlackScreen_cube_ );
	glBufferData ( GL_ARRAY_BUFFER , blackScreen_cube_.size ( ) * sizeof ( blackScreen_cube_[0] ) , blackScreen_cube_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER , 0);

	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_patch_ );
	glBufferData ( GL_ARRAY_BUFFER , patch_.size ( ) * sizeof ( patch_[0] ) , patch_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER , 0);

	update();
}
void GLWidget::black_screen_volumeWidth ( int w )
{
	this->volume_width = static_cast<float>(w);
	this->extrusion_controller_.updateBlackScreenMesh(stepx,stepz,volume_width,blackScreen_cube_,patch_);

	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_BlackScreen_cube_ );
	glBufferData ( GL_ARRAY_BUFFER , blackScreen_cube_.size ( ) * sizeof ( blackScreen_cube_[0] ) , blackScreen_cube_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER , 0);

	glBindBuffer ( GL_ARRAY_BUFFER , vertexBuffer_patch_ );
	glBufferData ( GL_ARRAY_BUFFER , patch_.size ( ) * sizeof ( patch_[0] ) , patch_.data() , GL_STATIC_DRAW );
	glBindBuffer ( GL_ARRAY_BUFFER , 0);

	update();
}
