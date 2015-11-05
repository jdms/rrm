#ifndef MYGLFRAME_H
#define MYGLFRAME_H

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
#include <QtWidgets/QOpenGLWidget>


class GLWidget: public QOpenGLWidget
{
		Q_OBJECT

	public:

		// From QGLWidget
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

	public slots:

	 	void backGround();
		void loadShaderByResources();
		void loadShaders();
		void reloadShaders();

		/// Left to Right
		void createPatch           ( Eigen::Vector3f left, Eigen::Vector3f right, float step);
		void createSurfacePatch    ( );
		void createSurfacePatchies ( const std::vector<std::vector<Eigen::Vector3f> >& patchies, float stepx, float stepz, float volume_width );


	protected:
		void dragEnterEvent ( QDragEnterEvent *event );
		void dragMoveEvent ( QDragMoveEvent *event );
		void dragLeaveEvent ( QDragLeaveEvent *event );
		void dropEvent ( QDropEvent *event );

	signals:

        void changed(const QMimeData *mimeData = 0);

private:
	
        Tucano::Trackball camera;

        Tucano::Shader*   cube_shader_;
        Tucano::Shader*   background_;

        GLuint vertexArray_cube_;
        	GLuint vertexBuffer_cube_;
        	GLuint vertexCube_slot_;

	GLuint vertexArray_patch_;
		GLuint vertexBuffer_patch_;
		GLuint vertexPatch_slot_;

        std::vector<Eigen::Vector3f> vertices;


        std::vector<Eigen::Vector3f> patch_;
        std::vector<Eigen::Vector3f> sketch_;

        std::vector<std::vector<Eigen::Vector3f> > p;

        Celer::BoundingBox3<float> box;

};

#endif
