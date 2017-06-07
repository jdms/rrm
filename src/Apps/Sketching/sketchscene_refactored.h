#ifndef SKETCHSCENE_REFACTORED_H
#define SKETCHSCENE_REFACTORED_H

#include <QGraphicsScene>
#include <QColor>
#include <QString>

#include "Sketching/InputSketch.h"
#include "ItemWrappers/volumeitemwrapper_refactored.h"
#include "ItemWrappers/objectitemwrapper_refactored.h"



class Volume;
class Object_Refactored;
class QGraphicsSceneMouseEvent;
class QGraphicsPixmapItem;
class QGraphicsSceneDragDropEvent;
class QKeyEvent;


class SketchScene_Refactored: public QGraphicsScene
{
    Q_OBJECT

    public:


        SketchScene_Refactored();

        void addVolume( Volume* const& vol );
        void resizingVolume( const QPointF& point, bool done = false );

        void addObject( Object_Refactored* const& obj );

        void setModeEditable( bool status );

        void setCurrentColor( const QColor& color );
        void setCurrentColor( int r, int g, int b );

        void updateVolume();
        void updateObject( std::size_t id );

        void reActiveObject( std::size_t id );
        void removeObjectsFromScene();


        void startSketch( const QPointF& p );
        void clearSketch();

        void finishSketch();
        bool acceptSketch( Curve2D& curve );
        bool isValidSketch();
        void enableSketch( bool status );

        void processCurve( Curve2D& curve );
        void removeCurve();

        bool hasImageInCrossSection();
        void setCurrentCrossSection( double depth );
        void removeImageFromCrossSection();

        void setModeSketching();
        void setModeSelecting();
        void setModeEditingBoundary();
        void setModeMovingImage();


        void enableDeletingCurves( bool status );
        void selectBounderingRegion();

        void clear();
        void setDefaultValues();



    signals:

        void addCurveToObject( const Curve2D& curve );
        void removeCurveFromObject( double depth );
        void createSurface();
        void updateVolumeDimensions( double w, double h );
        void enableDeleting( bool status );
        void selectedObject( std::size_t id );
        void updateColor( const QColor& c );



    protected:

        void createCrossSectionImageItem();
        void setImageToCrossSection( const QString& file );


        bool isAddedObject( std::size_t id ) const ;
        void disableMovingImage();

        void clearData();

        void mousePressEvent( QGraphicsSceneMouseEvent *event );
        void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
        void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );

        void dragEnterEvent( QGraphicsSceneDragDropEvent* event );
        void dragMoveEvent( QGraphicsSceneDragDropEvent* event );
        void dropEvent( QGraphicsSceneDragDropEvent* event );

        void keyPressEvent( QKeyEvent *event );



    protected:

        enum class UserInteraction { SKETCHING, SELECTING, MOVING_IMAGE, EDITING_BOUNDARY,
                                     EDITING_SCENE };


        double current_csection;
        QColor current_color;
        UserInteraction current_interaction;
        const UserInteraction DEFAULT_INTERACTION = UserInteraction::SKETCHING;

        QPointF boundary_anchor;


        QGraphicsPixmapItem* csection_image;
        VolumeItemWrapper_Refactored volume;
        InputSketch* sketch;

        std::map< std::size_t, ObjectItemWrapper_Refactored* > objects;
        std::map< double, QString > backgrounds;

        bool is_sketch_enabled;
        bool is_delete_enabled;
};

#endif // SKETCHSCENE_REFACTORED_H
