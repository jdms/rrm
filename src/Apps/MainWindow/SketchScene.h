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


        inline void setVolume( const Volume* vol_ )
        {
            volume.setVolumeRaw( vol_ );
            addItem( &volume );
        }

        void resetData();

        inline void init()
        {

//            drawCrossSection();

        }


        inline void setCrossSection( const CrossSection1& csection_ )
        {
            csection = csection_;
            drawCrossSection();
        }


        inline void drawCrossSection()
        {
            drawCrossSectionObjects();
            drawCrossSectionRegions();
            update();
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

        void clearSketch();


        void addObject( Object *obj_ );



        void mousePressEvent( QGraphicsSceneMouseEvent *event );

        void mouseMoveEvent( QGraphicsSceneMouseEvent* event );

        void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );

        void mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event );



        void sketchingInteractions( QGraphicsSceneMouseEvent* event );



    signals:

        void curveAccepted( Curve2D c_ );
        void createNewObject();



    protected:

        UserInteraction current_interaction = UserInteraction::SKETCHING;
        InputSketch *sketch;

        CrossSection1 csection;
        VolumeItemWrapper volume;

        std::map< std::size_t, ObjectItemWrap* > object_list;





};

#endif // SKETCHSCENE_H
