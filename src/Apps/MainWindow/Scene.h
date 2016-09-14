#ifndef SKETCHINGSCENE_H
#define SKETCHINGSCENE_H



#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsView>
#include <QMimeData>
#include <QString>
#include <QSvgGenerator>



#include "Mainwindow/Controller.hpp"

#include "Sketching/StratigraphicItem.hpp"
#include "Sketching/InputSketch.h"
#include "Sketching/BoundaryItem2D.hpp"

#include "3dView/Surface.h"
#include "3dView/BoundingBox3D.h"

#include "Core/Geometry/PolygonalCurve/CurveN.hpp"


class Scene: public QGraphicsScene
{
	Q_OBJECT
	
	
	public:



        typedef qreal			Real;
        typedef PolygonalCurve<Real, 2, PointN<Real, 2>, VectorN<Real, 2> > Curve2D;
        typedef PointN<Real, 2> 					    Point2D;

        enum class InteractionMode { OVERSKETCHING, INSERTING, BOUNDARY };
	


        Scene( QObject* parent = 0 );
        void init();

        void drawScene3D( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& width, const int& height );

        inline void setController( Controller* const& c ){ controller = c; connect( controller, SIGNAL( updateScene() ), this, SLOT( updateScene() ) ); }



    public slots:


        inline void setCurrentMode( const Scene::InteractionMode& mode ){ current_mode = mode; }
        inline const InteractionMode currentMode(){ return current_mode; }


        void createCrossSection( const float& d = 0.0f );
        void createBoundary();
        void addCurve();


        void updateColor( const QColor& color );
        void updateScene();


        void savetoRasterImage( const QString& filename );
        void savetoVectorImage( const QString& filename );


        void undoLastSketch();



    signals:

        void sendCoordinates( float, float );
        void insertSkecthing( InputSketch* );
        void updatedScene();





    protected:


        void createVolume3D();
        void editBoundary( int x, int y, int w, int h );


        void addCrossSectionToScene();
        void addBoundaryToScene();
        void addStratigraphyToScene();


        void setBackGround();
        void newSketch();


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
        StratigraphicItem *sketch;

        BoundaryItem2D *boundary;
        BoundingBox3D *boundary3D;
        QPointF boundary_anchor;

        QColor current_color;


        std::vector< StratigraphicItem* > stratigraphics_list;
        std::vector< int/*CrossSections3D**/ > crosssections3d_list;
        std::vector< Surface* > surfaces_list;
	
};





#endif // SKETCHINGSCENE_H
