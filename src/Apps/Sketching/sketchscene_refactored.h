#ifndef SKETCHSCENE_REFACTORED_H
#define SKETCHSCENE_REFACTORED_H

#include <QGraphicsScene>
#include <QColor>

class SketchScene_Refactored: public QGraphicsScene
{
    Q_OBJECT

    public:

        SketchScene_Refactored();

        inline void addVolume(){}
        inline void addObject(){}

        inline void setObjectSelected( std::size_t id ){}
        inline void setObjectSelectable( std::size_t id ){}

        void setCurrentColor( const QColor& color );

        inline void upadteVolume(){}
        inline void updateObject( std::size_t id ){}

        inline void updateScene(){}
        inline void clear(){}

        inline void setDefaultValues(){}

    private:

        QColor current_color;

};

#endif // SKETCHSCENE_REFACTORED_H
