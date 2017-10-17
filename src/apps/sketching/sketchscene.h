#ifndef SKETCHSCENE_H
#define SKETCHSCENE_H

#include <QGraphicsScene>


#include "inputsketch.h"
#include "./item_wrappers/volume_item_wrapper.h"
#include "./item_wrappers/object_item_wrapper.h"
#include "./core/base/models/scene.h"


class SketchScene: public QGraphicsScene, public Scene
{
    Q_OBJECT

    enum class UserInteraction { SKETCHING, EDITING_BOUNDARY };


    public:


        using ObjectsContainer = Container< std::size_t, ObjectItemWrapper* >;


        SketchScene();
        SketchScene( CrossSection* const& raw_ );


        void readCrossSection( CrossSection* const& raw_ );


        virtual void addVolume( Volume* const& raw_ );
        virtual void updateVolume();
        virtual void clearVolume();


        void addCrossSection( CrossSection* const& raw_ ){}
        void removeCrossSection( CrossSection* const& raw_ ){}


        void addObject( Object* const& raw_ );





    signals:


        void acceptVolumeDimensions( double w_, double h_ );
        void acceptCurve( const PolyCurve& curve_ );


    protected:


        virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
        virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent* event );
        virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );


    protected:


        UserInteraction current_interaction = UserInteraction::SKETCHING;
        InputSketch* user_input;


        VolumeItemWrapper* volume;
        ObjectsContainer objects;

};

#endif // SKETCHSCENE_H
