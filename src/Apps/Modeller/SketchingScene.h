#ifndef SKETCHINGSCENE_H
#define SKETCHINGSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>


#include "InputSketch.h"
#include "HorizonController.h"
#include "BoundaryController.h"

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

        QColor getColor() const;


    public slots:

        void applyRemoveAbove();
        void applyRemoveBelow();

        void setColor( int R, int G, int B );
        void getColor( int R, int G, int B );

        void setModeSelect();
        void setModeSketching();


    protected:

        void createBoundary();
        void updateAllElements();



    private:

        InteractionMode mode;

        InputSketch *sketch;
        HorizonController *horizonc;
        BoundaryController *boundaryc;

        QColor currentColor;


};

#endif // SKETCHINGSCENE_H
