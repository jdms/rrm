#ifndef SKETCHINGSCENE_H
#define SKETCHINGSCENE_H

#include <iostream>

#include <QtCore/QDebug>

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneMouseEvent>

#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QMimeData>
#include <QtCore/QUrl>


#include "Modeller/BoundaryController.h"
#include "Apps/Modeller/HorizonController.h"
#include "Apps/Modeller/InputSketch.h"

class SketchingScene: public QGraphicsScene
{
    Q_OBJECT

    public:

        enum class InteractionMode
        {
            SELECT,
            SKETCHING,
        };

        SketchingScene( QObject *parent = 0 );
        ~SketchingScene();

        // virtual mouse methods

        void mousePressEvent( QGraphicsSceneMouseEvent *event );
        void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
        void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );

	void dragEnterEvent ( QGraphicsSceneDragDropEvent *event );
	void dragMoveEvent 	( QGraphicsSceneDragDropEvent *event );
	void dragLeaveEvent ( QGraphicsSceneDragDropEvent *event );
	void dropEvent 		( QGraphicsSceneDragDropEvent *event );

        QColor getColor() const;


    public slots:

        void applyRemoveAbove();
        void applyRemoveBelow();


        void setColor( int R, int G, int B );
        void getColor( int R, int G, int B );

        void setModeSelect();
        void setModeSketching();

        // Skecting Felipe
        bool initialization_with_image( const QPixmap& pixmap  );



    protected:

        void createBoundary();
        void updateAllElements();

    private:

        InteractionMode mode;

        InputSketch *sketch;
        HorizonController *horizonc;
        BoundaryController *boundaryc;

        QColor currentColor;

        QGraphicsPixmapItem * ghost_image;


        /// Sketching new Boundary
        bool 	boundary_sketching_;
        QPointF boundary_anchor_point_;


};

#endif // SKETCHINGSCENE_H
