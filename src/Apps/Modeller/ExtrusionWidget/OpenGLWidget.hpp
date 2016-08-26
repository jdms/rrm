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
#include <QtGui/QOpenGLContext>

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

        typedef RRM::ExtrusionController::SeismicSlices SeismicSlices;
        typedef RRM::ExtrusionController::CrossSection  CrossSection;


        explicit GLWidget ( QWidget* parent = 0 );
        ~GLWidget();

        virtual void initializeGL ( );
        void resizeGL ( int width , int height );
        void paintGL ( );

        void updateSeismicResolution( int res );


    public slots:


        void black_screen_stepx( int x );
        void black_screen_stepz( int z );
        void black_screen_volumeWidth( int w );


        void background();
        void clear();


        inline void setBlackScreenModule()
        {
            extrusion_controller_.module_ = RRM::ExtrusionController::BlankScreen;
        }

        inline void setSeismicModule()
        {
            extrusion_controller_.module_ = RRM::ExtrusionController::Seismic;
        }



        void updateRendering();
        void updateBlackScreen( const CrossSection& _cross_section );
        void updateSeismicSlices ( const SeismicSlices& _seismic_slices );
        bool extrusionInitialize ( float _x_min, float _y_min, float _z_min, float _x_max, float _y_max, float _z_max );
        void setPlanePosition( int _index );



    signals:

        void emitBlackScreenVisiblity( bool );


    protected:


        void createMenu();
        void init();

        void renderBlankScreen();
        void renderSeismic();



        void loadShaderByResources();
        void loadShaders( const QString& shaderDirectory );
        void reloadShaders();
        void deleteShaders();


        void createBlankScreenBuffers();
        void createSeismicBuffers();
        void create8VerticesIndices ();
        void resetBuffers();



        void mousePressEvent ( QMouseEvent *event );
        void mouseMoveEvent ( QMouseEvent *event );
        void mouseReleaseEvent ( QMouseEvent *event );
        void wheelEvent ( QWheelEvent *e );

        void keyPressEvent ( QKeyEvent * event );



    public:

        RRM::ExtrusionController extrusion_controller_;



    private:

        QMenu* module_menu;
        QAction* ac_seismic_module;
        QAction* ac_blankscreen_module;

        /// BLACK SCREEN
        float stepx;
        float stepz;
        float volume_width;

        CrossSection cross_section_;


        Tucano::Trackball camera;
        Tucano::Shader*   shader_background;



        // Represent the boudingBox of the Geological Model
        Tucano::Shader*   shader_boundingbox_blankscreen;

        GLuint va_boundingbox_blankscreen;
        GLuint vb_boundingbox_blankscreen;
        GLuint slot_boundingbox_vertex;

        std::vector<Eigen::Vector3f> boundingbox_blankscreen;



        // The interpolated surface
        Tucano::Shader*   shader_interpolated_surface;

        GLuint va_interpolated_surface;
        GLuint vb_surface_vertices;
        GLuint vb_surface_color;
        GLuint slot_surface_vertex;
        GLuint slot_surface_color;

        std::vector<Eigen::Vector3f> interpolated_surfaces_colors;
        std::vector<Eigen::Vector3f> interpolated_surfaces;
        Celer::BoundingBox3<float> box;



        /// SEISMIC MODULE


        // The sketches seismic
        Tucano::Shader*   shader_seimisc_sketches;

        GLuint va_seismic_sketches;
        GLuint vb_seismic_sketches_vertex;
        GLuint slot_seismic_sketches_vertex;

        std::vector<Eigen::Vector4f> seismic_sketches;



        // The boundingbox seismic
        Tucano::Shader*   shader_boundingbox_seismic;

        GLuint va_boundingbox_seismic;
        GLuint vb_boundingbox_seismic_vertex;
        GLuint slot_boundingbox_seismic_vertex;

        std::vector<float> boundingbox_seismic;



        // The plane slice seismic
        Tucano::Shader*    shader_plane_seismic;

        GLuint va_plane_seismic;
        GLuint vb_plane_seismic_vertex;
        GLuint slot_plane_seismic_vertex;

        std::vector<float> seismic_plane;

        float seismic_plane_slice_position;
        int	seismic_plane_sliceid;




        // The mesh seismic
        Tucano::Shader*   shader_seismic_mesh;

        GLuint va_seismic_mesh;
        GLuint vb_seismic_mesh_vertex;
        GLuint vb_seismic_mesh_normal;
        GLuint vb_seismic_mesh_color;
        GLuint vb_seismic_mesh_idfaces;
        GLuint slot_seismic_mesh_vertex;
        GLuint slot_seismic_mesh_normal;
        GLuint slot_seismic_mesh_color;


        std::vector<float> seismic_mesh_vertices;
        std::vector<float> seismic_mesh_normals;
        std::vector<float> seismic_mesh_colors;
        std::vector<GLuint> seismic_mesh_faces;
        std::vector<std::size_t> list_of_faces;

        GLuint number_of_faces;



        std::vector<GLfloat > vertices;
        std::vector<GLuint > indices;

        /// Test
        GLuint vertexArray_for_the_Cube_;
        GLuint vertexBuffer_cube_8vertices_;
        GLuint vertices_8slot_;
        GLuint vertexBuffer_cube_8verticesIndices_;




};

#endif

