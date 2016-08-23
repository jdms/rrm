#ifndef SKETCHINGSCENE_H
#define SKETCHINGSCENE_H

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsView>
#include <QMimeData>
#include <QString>
#include <QSvgGenerator>


#include "Modeller/InputSketch.h"
#include "Modeller/BoundaryItem.h"
#include "Core/Geometry/PolygonalCurve/CurveN.hpp"


class SketchingScene: public QGraphicsScene
{
	Q_OBJECT
	
	
	public:


        typedef qreal			Real;
        typedef PolygonalCurve<Real, 2, PointN<Real, 2>, VectorN<Real, 2> > Curve2D;
        typedef PointN<Real, 2> 					    Point2D;

        enum class InteractionMode { OVERSKETCHING, INSERTING, BOUNDARY };
	


        SketchingScene( QObject* parent = 0 );
        void init();



    public slots:


        inline void setCurrentMode( const InteractionMode& mode ){ current_mode = mode; }
        inline const InteractionMode currentMode(){ return current_mode; }

        inline void modeBoundary(){ current_mode = InteractionMode::BOUNDARY; }

        void insertLastSketch();
        void undoLastSketch();

        void updateColor( const QColor& color );

        void savetoRasterImage( const QString& filename );
        void savetoVectorImage( const QString& filename );




    signals:

        void sendCoordinates( float, float );
        void insertSkecthing( InputSketch* );



    protected:


        void setBackGround();
        void createConnections();
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

        InputSketch *temp_sketch;
        InputSketch *sketch;

        BoundaryItem *boundary;
        QPointF boundary_anchor;

        QColor current_color;
	
};





#endif // SKETCHINGSCENE_H
