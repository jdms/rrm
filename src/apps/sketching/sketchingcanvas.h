#ifndef SKETCHINGCANVAS_H
#define SKETCHINGCANVAS_H

#include <QGraphicsView>
#include <QKeyEvent>

#include "sketchscene.h"

class SketchingCanvas: public QGraphicsView
{
    public:

        SketchingCanvas( QWidget *parent_ = nullptr );
        ~SketchingCanvas() override;

        const std::shared_ptr< SketchScene >& getScene() const;

        void setVerticalExaggeration( double scale_ );
        double getVerticalExaggeration() const;

        void stopVerticalExaggeration();

    protected:

        void setupScene();
//        virtual void keyPressEvent( QKeyEvent *event );



    private:

        std::shared_ptr< SketchScene > scene = nullptr;
        double v_exag = 1.0;
};

#endif // SKETCHINGCANVAS_H
