#ifndef CANVASCOMPUTATION_H
#define CANVASCOMPUTATION_H

#include <GL/glew.h>


#include <iostream>
#include <fstream>
#include <vector>
#include <string>


#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QApplication>
#include <QMatrix4x4>
#include <QVector3D>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QWidgetAction>
#include <QCheckBox>
#include <QActionGroup>
#include <QFileDialog>

#include "Simulator/FlowVisualizationController.h"

//#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"
#include "Tucano/BoundingBox3.hpp"


using namespace std;


inline std::string read_shader_file( const char *file_path )
{
    std::string content;
    std::ifstream fileStream( file_path, std::ios::in );

    if( !fileStream.is_open() ) {
        std::cerr << "Could not read file " << file_path << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while( !fileStream.eof() ) {
        std::getline( fileStream, line );
        content.append( line + "\n" );
    }

    fileStream.close();
    return content;

}


class CanvasComputation: public QOpenGLWidget/*, public QOpenGLFunctions_3_3_Core*/
{
    Q_OBJECT


    public:

        CanvasComputation( QWidget *parent = 0 );



        void resetData();
        void resetCamera();
        void resetVisualization();


        void showVolumetricGrid();
        void selectProperty( int id, bool option, int option_color = 0 );
        void showSurface();

        void fillMenuProperties();


    protected:

        void initializeGL();
        void resizeGL( int width, int height );
        void paintGL();

        void initializeShaders();
        void deleteBuffers();

        void sendMeshGPU();
        void sendSurfaceGPU();


        void createActions();
        void createPopupMenu();
        void createMenuProperties();
        void createMenuColorMaps();
        void createMenuRendering();
        void createRenderingVectors(int id, std::string title , string type);


        void fillMenuPointProperties();
        void fillMenuCellProperties();


        void mousePressEvent( QMouseEvent *m );
        void mouseMoveEvent( QMouseEvent *m );
        void mouseReleaseEvent ( QMouseEvent *event );
        void wheelEvent( QWheelEvent *m );

        void keyPressEvent ( QKeyEvent * event );


    public slots:

        void resetSetup();
        void setColorMap();
        void setColorMapConstant();

    protected slots:

        void sendColorsGPU( std::string property, std::string type, int option = 0 );
        void showPoints( bool option );
        void showVolume( bool option );
        void showWireframe( bool option );
        void exportFile();


        void resetTransformations();


        /// Tucano
        void loadShaderByResources();
        void reloadShaders ( );


    signals:

        void sendProperty( std::string property, std::string type );
        void sendVectorProperty( std::string property, std::string type, int option );

    private:

        GLuint program;

        GLuint vao_mesh;
        GLuint *bf_mesh;
        GLuint bf_vertices;
        GLuint bf_colors;

        GLuint bf_faces;
        GLuint bf_lines;
        GLuint number_of_faces;
        GLuint number_of_vertices;
        GLuint number_of_lines;

        bool show_vertices;
        bool show_faces;
        bool show_lines;


        QMenu *mn_options;
        QMenu *mn_properties;
        QMenu *mn_vectors_rendering;
        QMenu *mn_colormap;
        QMenu *mn_options_vector;


        float alpha;
        float beta;
        float zoom;
        float speed_mouse;
        float speed_rotation;

        QAction *ac_reset_transformations;
        QAction *ac_clear_all;
        QAction *ac_export;

        QWidgetAction *wa_properties;
        QWidgetAction *wa_colormaps;
        QWidgetAction *wa_rendering;
        QWidgetAction *wa_options_vector;


        vector< QRadioButton* > rd_properties;
        vector< QRadioButton* > rd_colormaps;
        vector< QRadioButton* > rd_options_vector;
        QCheckBox *chk_show_points;
        QCheckBox *chk_show_wireframe;
        QCheckBox *chk_show_volume;

        vector< QAction *> ac_properties;
        vector< QMenu *> mn_vector_properties_points;

        FlowVisualizationController *flowvisualizationc;

        /// Tucano

        GLuint vertexArray_cube_;
        	GLuint vertexBuffer_cube_;
        	GLuint vertexCube_slot_;

	GLuint vertexArray_MESH_;
		GLuint vertexBuffer_MESH_;
		GLuint vertexMESH_Slot_;

		GLuint vertexBuffer_face_ID_;
		GLuint vertexBuffer_Lines_ID_;
		GLuint vertexBuffer_colors_;
		GLuint vertexColor_slot_;

        std::vector<Eigen::Vector3f> cube_;
        std::vector<Eigen::Vector3f> vertices_;

        Tucano::Shader*   cube_shader_;

        Tucano::Shader*   vtk_visualization_;

        Tucano::Trackball camera;
        Celer::BoundingBox3<float> box;

};

#endif // CANVASCOMPUTATION_H