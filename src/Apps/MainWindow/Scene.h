#ifndef SKETCHINGSCENE_H
#define SKETCHINGSCENE_H



#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsView>
#include <QMimeData>
#include <QString>
#include <QGraphicsPixmapItem>
#include <QSvgGenerator>
#include <QRegion>
#include <QTransform>



#include "MainWindow/Controller.hpp"

#include "Sketching/StratigraphicItem.hpp"
#include "Sketching/InputSketch.h"
#include "Sketching/BoundaryItem2D.hpp"

#include "3dView/Surface.h"
#include "3dView/BoundingBox3D.h"

#include "3dView/Model3DUtils.hpp"

#include "Core/Geometry/PolygonalCurve/PolygonalCurve2D.hpp"
#include "./libs/Arrangement2D/Arrangement_2D.hpp"



class Scene: public QGraphicsScene
{
	Q_OBJECT
	
	
	public:



        enum class InteractionMode { OVERSKETCHING, INSERTING, BOUNDARY, SELECTING_ABOVE, SELECTING_BELOW, SELECTING_REGION };
	


        Scene( QObject* parent = 0 );

        void init();
        void defineVolumeQtCoordinates( int origin_x, int origin_y, int origin_z, int width, int height, int depth );
        void drawScene3D( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& width, const int& height );

        void setController( Controller* const& c );
        inline void setCurrentDirectory( const std::string& directory ){ shader_directory = QString( directory.c_str() ); }


    public slots:


        inline void setCurrentMode( const Scene::InteractionMode& mode ){ current_mode = mode; }
        inline const InteractionMode currentMode(){ return current_mode; }


        void enableSketchingAboveRegion( bool );
        void enableSketchingBelowRegion( bool );


        void createCrossSection( const float& d = 0.0f );
        void createSketchingBoundary();
        bool addCurve();


        void updateColor( const QColor& color );
        void clearScene();
        void updateScene();


        void savetoRasterImage( const QString& filename );
        void savetoVectorImage( const QString& filename );


        void undoLastSketch();
        void initGLContext();
        void updateGLContext();

        inline void setRandomColor( bool status ){ random_color = status; }

        std::vector< size_t > getAllSelectedItems();


        void setBackGroundImage( const QString& url );

        void disallowCurves( const std::vector< size_t >& curves_id );
        void setSelectionMode( const bool status );

        void defineSketchingAboveRegion();
        void stopSketchingAboveRegion();
        void defineSketchingBelowRegion();
        void stopSketchingBelowRegion();

        inline void enablePickingRegion( bool status )
        {
            if( status ) current_mode = InteractionMode::SELECTING_REGION;
            else current_mode = InteractionMode::OVERSKETCHING;
        }


        void getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps );


    signals:

        void sendCoordinates( float, float );
        void insertSkecthing( InputSketch* );
        void updatedScene();
        void initContext();
        void updateContext();
        void enableSketching( bool );
        void updateBoundGeometry( const int w, const int h, const int d );
        void sendRegionPoint( const int x, const int y, const int z );







    protected:


        void initData();

        void createVolume3D();
        void editBoundary( const int& x, const int& y, const int& w, const int& h );


        void addCrossSectionToScene();
        void addBoundaryToScene();
        void addStratigraphyToScene();


        void setBackGround();
        void newSketch();

        void updateTransformationsMatrices();


        Eigen::Vector3f scene2Dto3D( const Point2D& p );
        Eigen::Vector3f scene2Dto3D( const Eigen::Vector3f& p );

        Point2D scene3Dto2D( const Eigen::Vector3f& p );
        Eigen::Vector3f scene3DtoPlane( const Eigen::Vector3f& p );

        Curve2D scene2Dto3D( const Curve2D& c );
        Curve2D scene3Dto2D( const Curve2D& c );


        void mousePressEvent( QGraphicsSceneMouseEvent* event );
        void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
        void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );
        void dragEnterEvent( QGraphicsSceneDragDropEvent *event );
        void dropEvent( QGraphicsSceneDragDropEvent *event );
        void dragMoveEvent( QGraphicsSceneDragDropEvent * event );
        void dragLeaveEvent( QGraphicsSceneDragDropEvent * event );




    protected:


        InteractionMode current_mode;
        Controller *controller;

        InputSketch *temp_sketch;

        BoundaryItem2D *sketching_boundary;
        BoundingBox3D *boundary3D;
        QPointF boundary_anchor;

        bool random_color;
        QColor current_color;

        Eigen::Affine3f m_2dto3d;
        Eigen::Affine3f m_3dto2d;


        int qtscene_origin_x;
        int qtscene_origin_y;
        int qtscene_origin_z;
        int qtscene_width;
        int qtscene_height;
        int qtscene_depth;


        bool defining_above;
        bool defining_below;


        std::vector< size_t > allowed_above_surfaces;
        std::vector< size_t > allowed_below_surfaces;
        std::vector< size_t > selected_above_surfaces;
        std::vector< size_t > selected_below_surfaces;


        std::vector< int/*CrossSections3D**/ > crosssections3d_list;

        std::map< size_t, StratigraphicItem* > stratigraphics_list;
        std::map< size_t, Surface* > surfaces_list;

        QString shader_directory;

        RRM::Arrangement_2D arrangement;
        QGraphicsPixmapItem *background_image;

        QTransform mA;
	
};





#endif // SKETCHINGSCENE_H
