#ifndef SKETCHING2DMODULE_H
#define SKETCHING2DMODULE_H

#include <QDockWidget>
#include "Modeller/Canvas2D.h"

class Sketching2DModule: public QDockWidget
{
    Q_OBJECT

    public:

        Sketching2DModule( QWidget* parent );

        void clearCanvas2D();

    protected:

        void createWindow();
        void createActions( QWidget* parent );

    public slots:

        void applyRemoveAbove();
        void applyRemoveBelow();
        void pointerSelection( bool flag );

        void emitColor( int R, int G, int B );
        void emitModeSelect();
        void emitModeSketching();


    signals:

        void applyremoveabove();
        void applyremovebelow();
        void setColor( int R, int G, int B );
        void selectMode();
        void sketchingMode();


    private:

        Canvas2D *canvas2D;

};

#endif // SKETCHING2DMODULE_H
