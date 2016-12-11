#ifndef FLOWVISUALIZATIONCANVAS_H
#define FLOWVISUALIZATIONCANVAS_H


#include <GL/glew.h>

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QFileDialog>


#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"
//#include "Tucano/BoundingBox3.hpp"

#include "FlowVisualizationController.h"
#include "ColorMap.h"
#include "FlowRenderingOptionsMenu.h"
#include "CoordinateAxes.h"
#include "FlowCrossSection.h"
#include "Colorbar.h"
//#include "PointMarkers.h"


class FlowVisualizationCanvas: public QOpenGLWidget
{
    Q_OBJECT

    public:

		FlowVisualizationCanvas(QWidget *parent, QString _current_dir);
        ~FlowVisualizationCanvas();

        void setOpenGLFormat();
        void setController( FlowVisualizationController *c );
        void createRenderingMenu();



        void clear();



    public slots:

//        void updateMesh(  const Mesh::TYPE& type, std::vector< double > positions, std::vector< unsigned int > faces );

        void updateMesh();
        void updateVolumetricMesh();
        void updateCornerPoint();

        void showVertices( bool status );
        void showEdges( bool status );
        void showFaces( bool status );
        void showBoundingBox( bool status );


        void setVerticesColorbyProperty( std::string name, std::string method = "" );
        void setFacesColorbyProperty( std::string name, std::string method = "" );

        void setConstantColor();
        void setColorMap();


        void disableCrossSection( bool status );
        void setCrossSectionNormalCoordinates( float X, float Y, float Z );


        inline void setCurrentDirectory( std::string current_dir ){ current_directory.clear(); current_directory = current_dir; }

//        void exportSurface();
//        void exportVolume();
//        void exportCornerPoint();
//        void exportResults();

        void setColors( const std::vector< float >& colors );
        void setCurrentColormap( const ColorMap::COLORMAP& cm );

        inline void setColorBar( ColorBar* cb ){ colorbar = cb; }

        void showRegions();


        void setAleatoryColorMap();


		/// Debug Purpose
		void reloadShader();



    signals:

        void getSurfaceCrossSection();
        void buildcornerpoint();
        void buildunstructured();
        void editParameters();
        void readSurfacefromFile();
        void applyCrossSection();
        void clearAll();


    protected:

        void createActions();
        void initializeShader();
        void loadBackGround();

        void initializeGL() Q_DECL_OVERRIDE;
        void paintGL() Q_DECL_OVERRIDE;
        void resizeGL( int width, int height );

        void mouseMoveEvent( QMouseEvent *event );
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent( QMouseEvent *event );
        void wheelEvent( QWheelEvent *event );

        void keyPressEvent( QKeyEvent *event );


    private:

        FlowVisualizationController* controller ;

        QSurfaceFormat format;
        std::string current_directory;

        Tucano::Trackball camera;
		Tucano::Trackball coordinate_axis_;
		GLfloat aspect_ratio_;
		GLfloat width_;
		GLfloat height_;

        const float speed_zoom = 0.1f;

        Mesh mesh;
        ColorMap colormap;

        FlowRenderingOptionsMenu* rendering_menu;

        std::string coloring_property_name;
        std::string coloring_property_method;
        std::string coloring_property_type;

        bool show_axis;
        CoordinateAxes axes;

        float scale;

        bool apply_crosssection;
        FlowCrossSection crosssection;

        Tucano::Shader*   background;
        GLuint va_background;
        GLuint vb_background;


        ColorMap::COLORMAP current_colormap;
        bool show_colorbar;
        ColorBar* colorbar;



};

#endif // FLOWVISUALIZATIONCANVAS_H
