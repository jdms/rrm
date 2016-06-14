#ifndef FLOWVISUALIZATIONCANVAS_H
#define FLOWVISUALIZATIONCANVAS_H


#include <GL/glew.h>


#include <QOpenGLWidget>
#include <QMouseEvent>


#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"
//#include "Tucano/BoundingBox3.hpp"

#include "FlowVisualizationController.h"
#include "ColorMap.h"
#include "FlowRenderingOptionsMenu.h"
#include "CoordinateAxes.h"
#include "FlowCrossSection.h"


class FlowVisualizationCanvas: public QOpenGLWidget
{
    Q_OBJECT

    public:

        FlowVisualizationCanvas( QWidget *parent = 0 );


        void setOpenGLFormat();
        void setController( FlowVisualizationController *c );
        void createRenderingMenu();

        void clear();




    public slots:


        void updateMesh();
        void updateVolumetricMesh();

        void showVertices( bool status );
        void showEdges( bool status );
        void showFaces( bool status );


        void setVerticesColorbyProperty( std::string name, std::string method = "" );
        void setFacesColorbyProperty( std::string name, std::string method = "" );


        void setConstantColor();
        void setJETColor();

        void disableCrossSection();
        void setCrossSectionNormalCoordinates( float X, float Y, float Z );

        inline void setCurrentDirectory( std::string current_dir ){ current_directory.clear(); current_directory = current_dir; }


    protected:

        void createActions();

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
        const float speed_zoom = 0.1f;

        Mesh mesh;
        FlowCrossSection crosssection;
        ColorMap colormap;

        FlowRenderingOptionsMenu* rendering_menu;

        std::string coloring_property_name;
        std::string coloring_property_method;
        std::string coloring_property_type;

        CoordinateAxes* axes;

        bool apply_crosssection;


};

#endif // FLOWVISUALIZATIONCANVAS_H
