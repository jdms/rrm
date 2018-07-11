#ifndef SKETCHINGCANVAS_H
#define SKETCHINGCANVAS_H

#include <QGraphicsView>

#include "sketchscene.h"

class SketchingCanvas: public QGraphicsView
{
    public:

        SketchingCanvas( QWidget *parent_ = nullptr );
        ~SketchingCanvas() override;

        const std::shared_ptr< SketchScene >& getScene() const;

    protected:

        void setupScene();


    private:

        std::shared_ptr< SketchScene > scene = nullptr;
};

#endif // SKETCHINGCANVAS_H
