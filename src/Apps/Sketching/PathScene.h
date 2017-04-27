#ifndef PATHSCENE_H
#define PATHSCENE_H

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QKeyEvent>

#include "Core/Geology/Models/Volume.h"
#include "ItemWrappers/VolumeItemWrapper.h"
#include "Core/Geology/Models/CrossSection.h"
#include "ItemWrappers/CrossSectionItemWrapper.h"
#include "./Inputsketch.h"



class PathScene: public QGraphicsScene
{
    Q_OBJECT

    public:

        enum class UserInteraction { SKETCHING, EDITING_BOUNDARY, SELECTING };

        PathScene( QObject* parent = 0 );

        inline void setVolume( Volume* const &vol_ )
        {
            volume.setVolumeRaw( vol_ );
            setSceneRect( volume.boundingRect() );

            addItem( csection_image );
            addItem( &volume );
            addItem( sketch );
        }

        void setImagetoCrossSection( const QString& url_ );

        void addCrossSection( double depth_ );

        inline void drawCrossSections( bool status_ )
        {
            draw_cross_sections = status_;
            for( auto &it: csections )
            {
                (it.second)->setVisible( status_ );
            }
            update();
        }

        void finishSketch();
        inline bool acceptSketch( Curve2D& curve_ );

        void screenshot();

        void resetData();
        inline void init(){}

        void mousePressEvent( QGraphicsSceneMouseEvent *event );
        void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
        void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );


        void dragEnterEvent( QGraphicsSceneDragDropEvent* event );
        void dragMoveEvent( QGraphicsSceneDragDropEvent* event );
        void dragLeaveEvent( QGraphicsSceneDragDropEvent* event );
        void dropEvent( QGraphicsSceneDragDropEvent* event );




    public slots:

        void clearSketch();
        void updateScene();


        inline void setModeSketching(){ current_interaction = UserInteraction::SKETCHING; }
        inline void setModeBoundaryEditing(){ current_interaction = UserInteraction::EDITING_BOUNDARY; }
        inline void setCurrentColor( const QColor& c_ ){ current_color = c_; sketch->setColor( c_ ); }


    signals:

        void curveAccepted( Curve2D c_ );
        void updateVolumeWidthDepth( double w_, double d_ );


    protected:


        void savetoRasterImage( const QString& filename );
        void savetoVectorImage( const QString& filename );


        UserInteraction current_interaction = UserInteraction::SKETCHING;
        InputSketch *sketch;
        QPointF boundary_anchor;

        std::map< double, CrossSectionItemWrapper* > csections;
        VolumeItemWrapper volume;
        QGraphicsPixmapItem* csection_image;

        QColor current_color;

        bool draw_cross_sections = true;

};

#endif // PATHSCENE_H
