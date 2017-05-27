#ifndef SKETCHSCENE_REFACTORED_H
#define SKETCHSCENE_REFACTORED_H

#include <QGraphicsScene>
#include <QColor>

#include "ItemWrappers/volumeitemwrapper_refactored.h"

class Volume;
class InputSketch;

class SketchScene_Refactored: public QGraphicsScene
{
    Q_OBJECT

    public:

        SketchScene_Refactored();

        void addVolume( Volume* const& vol );
        inline void addObject(){}

        inline void setObjectSelected( std::size_t id ){}
        inline void setObjectSelectable( std::size_t id ){}

        void setCurrentColor( const QColor& color );

        inline void upadteVolume(){}
        inline void updateObject( std::size_t id ){}

        inline void updateScene(){}
        inline void clear(){}

        inline void setDefaultValues(){}


        void startSketch( const QPointF& p );
        void clearSketch();


    private:

        QColor current_color;

        VolumeItemWrapper_Refactored volume;
        InputSketch* sketch;

};

#endif // SKETCHSCENE_REFACTORED_H
