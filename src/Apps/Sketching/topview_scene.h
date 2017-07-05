#ifndef TOPVIEWSCENE_H
#define TOPVIEWSCENE_H

#include "sketch_scene.h"
#include "ItemWrappers/trajectory_item_wrapper.h"

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

class CrossSectionItemWrapper;



class TopViewScene: public SketchScene
{
    Q_OBJECT

    public:

        TopViewScene();

        virtual void addVolume( Volume* const& vol );
        virtual void updateVolume();

        void addCrossSection( double depth );
        bool isAddedCrossSection( double depth );
        void createCurrentCrossSection( double depth );

        void moveCurrentCrossSection( double depth );
        void showCrossSections( bool status );

        void addObject( Object * const &obj );
        bool isAddedObject( std::size_t id );
        virtual void updateObject( std::size_t id );


        virtual void savetoRasterImage( const QString& filename );
        virtual void savetoVectorImage( const QString& filename );

        virtual bool acceptSketch( Curve2D& curve ) override;
        virtual void removeCurve() override;
//        virtual void finishSketch() override;

        void setupPen();
        void showPopUp();

        virtual void clear();


    signals:

        void removeTrajectory();


    protected:

        virtual void clearData();

        void keyPressEvent( QKeyEvent *event );


    protected:

        bool draw_csections;
        CrossSectionItemWrapper* csection;
        std::map< double, CrossSectionItemWrapper* > csections;

        QPen trajectory_pen;

        std::map< std::size_t, TrajectoryItemWrapper*/*QPainterPath*/ > objects;


};

#endif // TOPVIEWSCENE_H
