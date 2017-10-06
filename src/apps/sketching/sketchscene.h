#ifndef SKETCHSCENE_H
#define SKETCHSCENE_H

#include <QGraphicsScene>

#include "./item_wrappers/volume_item_wrapper.h"
#include "./core/base/models/scene.h"

class SketchScene: public QGraphicsScene, public Scene
{
    Q_OBJECT

    enum class UserInteraction { SKETCHING, EDITING_BOUNDARY };

    public:

        SketchScene();
        SketchScene( CrossSection* const& raw_ );

        void readCrossSection( CrossSection* const& raw_ );


        virtual void addVolume( Volume* const& raw_ );
        virtual void clearVolume();


        void addCrossSection( CrossSection* const& raw_ ){}
        void removeCrossSection( CrossSection* const& raw_ ){}



    signals:


        void acceptVolumeDimensions( double w_, double h_ );


    protected:


        void mousePressEvent( QGraphicsSceneMouseEvent *event );
        void mouseMoveEvent ( QGraphicsSceneMouseEvent* event );
        void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );


    protected:


        UserInteraction current_interaction = UserInteraction::EDITING_BOUNDARY;

        VolumeItemWrapper* volume;

};

#endif // SKETCHSCENE_H
