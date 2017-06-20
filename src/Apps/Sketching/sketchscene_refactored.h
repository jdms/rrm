#ifndef SKETCHSCENE_REFACTORED_H
#define SKETCHSCENE_REFACTORED_H

#include <QGraphicsScene>
#include <QColor>
#include <QString>

#include "Sketching/InputSketch.h"
#include "ItemWrappers/coordinateaxes2d.h"
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

        CoordinateAxes2d axes;

        SketchScene_Refactored();

        void addVolume( Volume* const& vol );
        void resizingVolume( const QPointF& point, bool done = false );

        void addObject( Object_Refactored* const& obj );
        void addObjectTest( const std::vector< double >& vertices,
                            const std::vector< std::size_t >&edges );

        void removeObjectTest();


        void setModeEditable( bool status );

        void setCurrentColor( const QColor& color );
        void setCurrentColor( int r, int g, int b );

        virtual void updateVolume();
        void updateObject( std::size_t id );

        void reActiveObject( std::size_t id );
        void removeObjectsFromScene();


        void startSketch( const QPointF& p );
        void clearSketch();

        virtual void finishSketch();
        virtual bool acceptSketch( Curve2D& curve );
        bool isValidSketch();
        void enableSketch( bool status );

        void processCurve( Curve2D& curve );
        virtual void removeCurve();

        bool hasImageInCrossSection();
        void setCurrentCrossSection( double depth );
        void removeImageFromCrossSection();

        void setModeSketching();
        void setModeSelecting();
        void setModeEditingBoundary();
        void setModeMovingImage();


        void enableDeletingCurves( bool status );
        void selectBounderingRegion();

        virtual void savetoRasterImage( const QString& filename );
        virtual void savetoVectorImage( const QString& filename );

        void clear();
        void setDefaultValues();

        void enableAxes( bool status );



    signals:

        void addCurveToObject( const Curve2D& curve );
        void removeCurveFromObject( double depth );
        void createSurface();
        void updateVolumeDimensions( double w, double h );
        void enableDeleting( bool status );
        void selectedObject( std::size_t id );
        void updateColor( const QColor& c );
        void setUpColor();


    protected:

        void createCrossSectionImageItem();
        void setImageToCrossSection( const QString& file );
        void setImageToCrossSection();


        bool isAddedObject( std::size_t id ) const ;
        void disableMovingImage();

        void clearData();

        void mousePressEvent( QGraphicsSceneMouseEvent *event );
        void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
        void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );
        void mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event );

        void dragEnterEvent( QGraphicsSceneDragDropEvent* event );
        void dragMoveEvent( QGraphicsSceneDragDropEvent* event );
        void dropEvent( QGraphicsSceneDragDropEvent* event );

        void keyPressEvent( QKeyEvent *event );



    protected:

        struct ImageData
        {
            QString file;
            QPointF origin;
        };

        enum class UserInteraction { SKETCHING, SELECTING, MOVING_IMAGE, EDITING_BOUNDARY,
                                     EDITING_SCENE };


        double current_csection;

        QColor current_color;
        QPen pen_testing;

        UserInteraction current_interaction;
        const UserInteraction DEFAULT_INTERACTION = UserInteraction::SKETCHING;

        QPointF boundary_anchor;


        QGraphicsPixmapItem* csection_image;
        VolumeItemWrapper_Refactored volume;
        InputSketch* sketch;

        std::map< std::size_t, ObjectItemWrapper_Refactored* > objects;
        std::map< double, ImageData > backgrounds;

        bool is_sketch_enabled;
        bool is_delete_enabled;

        bool is_axes_enable;


        QGraphicsPathItem* object_test;
        QGraphicsPathItem object_test1;
};

#endif // SKETCHSCENE_REFACTORED_H
