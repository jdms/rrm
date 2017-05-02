#ifndef SKETCHSCENE_H
#define SKETCHSCENE_H

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtAlgorithms>
#include <QColor>

#include "Core/Geology/Models/Volume.h"
#include "ItemWrappers/VolumeItemWrapper.h"
#include "ItemWrappers/ObjectItemWrap.h"
#include "Core/Geology/Models/CrossSection.h"
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
            setSceneRect( volume.boundingRect() );          

            addItem( csection_image );
            addItem( &volume );


//            QGraphicsView* view_ = views()[ 0 ];
////            int margin_ = ( view_->width() - volume.getWidth() )*0.5;
////            view_->ensureVisible( volume.boundingRect(), margin_, 50 );
//            view_->fitInView( volume.boundingRect(), Qt::KeepAspectRatio );

        }
        void setImagetoCrossSection( const QString& url_ );
        void addObject( Object *obj_ );


        void resetData();
        inline void init(){}


        inline void setCrossSection( const CrossSection& csection_ )
        {
            csection = csection_;
//            drawCrossSectionObjects();
//            drawCrossSectionRegions();
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

        inline void setAllowedObjects( const std::vector< std::size_t >& allowed_ )
        {
            allowed_objects = allowed_;

            for( std::size_t id_: allowed_ )
            {
                object_list[ id_ ]->setState( ObjectItemWrap::State::ALLOWED );
            }

            setModeSelecting();
            update();
        }


        inline void disallowObjects()
        {
            for( std::size_t id_: allowed_objects )
            {
                if( id_ == surface_selected ) continue;
                object_list[ id_ ]->setState( ObjectItemWrap::State::NONE );
            }

            allowed_objects.clear();
            update();
        }


        inline void unselectObject( const std::size_t id_ )
        {
            object_list[ id_ ]->setState( ObjectItemWrap::State::NONE );
            update();
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
        void clearScene();

        void updateScene();
        void screenshot();

        inline void setModeSketching(){ current_interaction = UserInteraction::SKETCHING; }
        inline void setModeBoundaryEditing(){ current_interaction = UserInteraction::EDITING_BOUNDARY; }
        inline void setModeSelecting(){ current_interaction = UserInteraction::SELECTING; }
        inline void setCurrentColor( const QColor& c_ ){ current_color = c_;  }


    signals:

        void curveAccepted( Curve2D c_ );
        void interpolateObject();
        void updateVolumeWidthHeight( double w_, double h_ );
        void selectedSurface( const std::size_t& id_ );


    protected:


        void savetoRasterImage( const QString& filename );
        void savetoVectorImage( const QString& filename );



    protected:

        UserInteraction current_interaction = UserInteraction::SKETCHING;
        InputSketch *sketch;
        QPointF boundary_anchor;

        CrossSection csection;
        VolumeItemWrapper volume;
        QGraphicsPixmapItem* csection_image;

        std::map< std::size_t, ObjectItemWrap* > object_list;
        std::vector< std::size_t > allowed_objects;
        std::size_t surface_selected = 10000;

        QColor current_color;





};

#endif // SKETCHSCENE_H
