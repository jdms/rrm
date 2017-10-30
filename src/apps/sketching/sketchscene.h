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

    enum class UserInteraction { SKETCHING, EDITING_BOUNDARY, EDITING_SCENE };


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
        void updateCrossSection();


        virtual void addObject( Object* const& raw_ );
        virtual void addObject( Object* const& raw_, double depth_ );
        virtual void updateObject(  const std::size_t& index_ );



        void setCurrent( bool status_ );
        bool isCurrent() const;






    signals:


        void acceptVolumeDimensions( double w_, double h_ );
        void acceptCurve( const PolyCurve& curve_ );

        void discard();
        void commit();
        void create();


    public slots:

        void edit( bool status_ );

        void setModeSketching();
        void setModeEditingBoundary();
        void setModeEditingScene();


    protected:


        virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
        virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent* event );
        virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );
        virtual void wheelEvent( QGraphicsSceneWheelEvent *event );


    protected:


        UserInteraction current_interaction = UserInteraction::SKETCHING;
        InputSketch* user_input;

        CrossSection* csection;

        VolumeItemWrapper* volume;
        ObjectsContainer objects;

        const double ZOOM_SCALE = 1.1;
        bool is_current = false;

};

#endif // SKETCHSCENE_H
