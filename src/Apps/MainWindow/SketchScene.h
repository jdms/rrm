#ifndef SKETCHSCENE_H
#define SKETCHSCENE_H

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneMouseEvent>

#include "Volume.h"
#include "VolumeItemWrapper.h"
#include "ObjectItemWrap.h"
#include "CrossSection.h"
#include "Sketching/Inputsketch.h"

class SketchScene: public QGraphicsScene
{
    Q_OBJECT

    public:

        enum class UserInteraction { SKETCHING, EDITING_BOUNDARY, SELECTING };


        SketchScene( QObject* parent = 0 );


        inline void setVolume( Volume* const &vol_ )
        {
            volume.setVolumeRaw( vol_ );
            addItem( csection_image );
            addItem( &volume );
            addItem( sketch );
        }
        void setImagetoCrossSection( const QString& url_ );
        void addObject( Object *obj_ );


        void resetData();
        inline void init(){}


        inline void setCrossSection( const CrossSection1& csection_ )
        {
            csection = csection_;
//            drawCrossSection();
            drawCrossSectionObjects();
            drawCrossSectionRegions();
            update();
        }
        inline void drawCrossSection()
        {
//            drawCrossSectionObjects();
//            drawCrossSectionRegions();
//            update();
        }
        inline void drawCrossSectionObjects()
        {
            std::set< Object* > objects_ = csection.getObjectsReferenced();
            for( Object* o: objects_ )
            {
                addObject( o );
            }
        }
        inline void drawCrossSectionRegions()
        {
        }


        void finishSketch();
        bool acceptSketch( Curve2D& curve_ );


        void mousePressEvent( QGraphicsSceneMouseEvent *event );
        void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
        void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );
        void mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event );


        void dragEnterEvent( QGraphicsSceneDragDropEvent* event );
        void dragMoveEvent( QGraphicsSceneDragDropEvent* event );
        void dragLeaveEvent( QGraphicsSceneDragDropEvent* event );
        void dropEvent( QGraphicsSceneDragDropEvent* event );

        void sketchingInteractions( QGraphicsSceneMouseEvent* event );



    public slots:


        void clearSketch();

        void updateScene();

        inline void setModeSketching(){ current_interaction = UserInteraction::SKETCHING; }
        inline void setModeBoundaryEditing(){ current_interaction = UserInteraction::EDITING_BOUNDARY; }
        inline void setModeSelecting(){ current_interaction = UserInteraction::SELECTING; }


    signals:

        void curveAccepted( Curve2D c_ );
        void interpolateObject();
        void updateVolumeRawGeometry( double w_, double h_ );


    protected:





    protected:

        UserInteraction current_interaction = UserInteraction::EDITING_BOUNDARY;
        InputSketch *sketch;
        QPointF boundary_anchor;

        CrossSection1 csection;
        VolumeItemWrapper volume;
        QGraphicsPixmapItem* csection_image;

        std::map< std::size_t, ObjectItemWrap* > object_list;





};

#endif // SKETCHSCENE_H
