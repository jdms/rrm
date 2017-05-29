#ifndef SKETCHSCENE_REFACTORED_H
#define SKETCHSCENE_REFACTORED_H

#include <QGraphicsScene>
#include <QColor>

#include "Sketching/InputSketch.h"
#include "ItemWrappers/volumeitemwrapper_refactored.h"



class Volume;
class QGraphicsSceneMouseEvent;
class QGraphicsPixmapItem;
class QGraphicsSceneDragDropEvent;


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

        void finishSketch();
        bool acceptSketch( Curve2D& curve );


        bool isValidSketch();

         void processCurve( Curve2D& curve );


    signals:

        void addCurveToObject( const Curve2D& curve );



    protected:

        void createCrossSectionImageItem();
        void setImageToCrossSection( const QString& file );
        void resetImageToCrossSection();

        void mousePressEvent( QGraphicsSceneMouseEvent *event );
        void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
        void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );

        void dragEnterEvent( QGraphicsSceneDragDropEvent* event );
        void dragMoveEvent( QGraphicsSceneDragDropEvent* event );
        void dropEvent( QGraphicsSceneDragDropEvent* event );





    private:

        enum class UserInteraction { SKETCHING, SELECTING, MOVING_IMAGE, EDITING_BOUNDARY };

        QColor current_color;
        UserInteraction current_interaction;

        QGraphicsPixmapItem* csection_image;
        VolumeItemWrapper_Refactored volume;
        InputSketch* sketch;

};

#endif // SKETCHSCENE_REFACTORED_H
