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
        void stopVerticalExaggeration();
        double getVerticalExaggeration() const;

    protected:

        void setupScene();



    private:

        std::shared_ptr< SketchScene > scene = nullptr;
        double v_exag = 1.0;
};

#endif // SKETCHINGCANVAS_H
