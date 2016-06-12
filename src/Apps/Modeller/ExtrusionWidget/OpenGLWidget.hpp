#ifndef _OPENGLWIDGET_H
#define _OPENGLWIDGET_H

#include <GL/glew.h>

#include<Tucano/Trackball.hpp>
#include<Tucano/Shader.hpp>
#include<Tucano/BoundingBox3.hpp>

#include <iostream>

#include <QtCore/QTimer>
#include <QtCore/QTimeLine>
#include <QtCore/QFile>
#include <QtCore/QSet>
#include <QtCore/QMimeData>

#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QMimeData>

#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>

#include <QtWidgets/QApplication>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QOpenGLWidget>

#include "ExtrusionController.hpp"

/**!
 * @brief OpenGL window for 3D data visualization. Currently implements the extrusion.
 */

class GLWidget: public QOpenGLWidget
{
		Q_OBJECT
	public:
		// From QGLWidget
		typedef RRM::ExtrusionController::SeismicSlices SeismicSlices;
		typedef RRM::ExtrusionController::CrossSection  CrossSection;


		explicit GLWidget ( QWidget* parent = 0 );
		void initializeGL ( );
		void resizeGL ( int width , int height );

		void paintGL ( );
		void processMultiKeys ( );
		void mousePressEvent ( QMouseEvent *event );
		void mouseMoveEvent ( QMouseEvent *event );
		void mouseReleaseEvent ( QMouseEvent *event );
		void wheelEvent ( QWheelEvent *e );
		void keyPressEvent ( QKeyEvent * event );
		void keyReleaseEvent ( QKeyEvent * event );

		void create8VerticesIndices ();

	public slots:

		void black_screen_stepx(int x);
		void black_screen_stepz(int z);
		void black_screen_volumeWidth(int w);
		void setBlackScreenModule();

	 	void backGround();
	 	// Deployed
		void loadShaderByResources();
		// Development
		void loadShaders();
		void reloadShaders();
		// Seismic
		void setSeismicModule();
		void updateRendering();
		void updateSeismicSlices ( const SeismicSlices& _seismic_slices);
		bool extrusionInitialize ( float _x_min,
					   float _y_min,
					   float _z_min,
					   float _x_max,
					   float _y_max,
					   float _z_max );
		void setPlanePosition( int _index );

		// Black Screen
		void updateBlackScreen(const CrossSection& _cross_section);

	signals:

        void changed(const QMimeData *mimeData = 0);

private:
	
        QMenu* menu_module_type_;
        QAction* action_seismic_module_;
        QAction* action_blankSceen_module_;

        /// BLACK SCREEN
        float stepx = 1;
        float stepz = 20;
        float volume_width = 400;
        // Scene related attributes
        Tucano::Trackball camera;
        Tucano::Shader*   background_;
        // Entity related attributes
        // Cube, representing the boudingBox of the Geological Model
        GLuint vertexArray_BlackScreen_cube_;
        	GLuint vertexBuffer_BlackScreen_cube_;
        	/// layout ( location = 0) vec3 position
        	GLuint position_BlackScreen_slot_;
        std::vector<Eigen::Vector3f> blackScreen_cube_;
        Tucano::Shader*   blackScreen_cube_shader_;

        // The interpolated surface
	GLuint vertexArray_patch_;
		GLuint vertexBuffer_patch_;
		GLuint vertexPatch_slot_;
	std::vector<Eigen::Vector3f> patch_;
	Tucano::Shader*   patch_shader_;

	Celer::BoundingBox3<float> box;

	/// SEISMIC MODULE
        // The sketch lines
	GLuint lines_vertexArray_;
		GLuint lines_vertexBuffer_;
		GLuint lines_vertexSlot_;
	std::vector<Eigen::Vector4f> lines_;
	Tucano::Shader*   lines_shader_;

        GLuint vertexArray_Seismic_cube_;
        	GLuint vertexBuffer_Seismic_cube_;
        	GLuint position_seismic_cube_;
        std::vector<float> seismic_cube_;
        Tucano::Shader*   seismic_cube_shader_;


	GLuint vertexArray_Seismic_plane_;
		GLuint vertexBuffer_Seismic_plane_;
		/// layout (location = 0) vec4 position
		GLuint position_seismic_plane_;
	std::vector<float> seismic_plane_;
	Tucano::Shader*    seismic_plane_shader_;
	float		   seismic_slice_plane_position;
	int		   seismic_slice_plane_index;

	Tucano::Shader*   mesh_shader_;
	GLuint vertexArray_MESH_;
		GLuint positionBuffer_MESH_;
			GLuint position_MESH_Slot_;
		GLuint normalBuffer_MESH_;
			GLuint normal_MESH_Slot_;
		GLuint colorBuffer_MESH_;
			GLuint color_MESH_Slot_;
		GLuint vertexBuffer_MESH_face_ID_;
		GLuint number_of_faces;

		std::vector<float> 		vertex_;
		std::vector<float> 		normal_;
		std::vector<std::size_t>  	faces_;
		std::vector<GLuint>  		facesGL_;

	std::vector<GLfloat > 		vertices;
	std::vector<GLuint > 		indices;

	/// Test
	GLuint vertexArray_for_the_Cube_;
		GLuint vertexBuffer_cube_8vertices_;
		GLuint vertices_8slot_;
		GLuint vertexBuffer_cube_8verticesIndices_;

public:
	RRM::ExtrusionController extrusion_controller_;


};

#endif

