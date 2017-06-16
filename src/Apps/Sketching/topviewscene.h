#ifndef TOPVIEWSCENE_H
#define TOPVIEWSCENE_H

#include "sketchscene_refactored.h"

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

class CrossSectionItemWrapper_Refactored;



class TopViewScene: public SketchScene_Refactored
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

        void addObject( Object_Refactored * const &obj );
        bool isAddedObject( std::size_t id );


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
        CrossSectionItemWrapper_Refactored* csection;
        std::map< double, CrossSectionItemWrapper_Refactored* > csections;

        QPen trajectory_pen;

        std::map< std::size_t, QPainterPath > objects;


};

#endif // TOPVIEWSCENE_H
