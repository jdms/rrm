#ifndef SKETCHWINDOW_H
#define SKETCHWINDOW_H


#include <QMainWindow>

#include "canvasstack.h"
#include "sketchscene.h"

class QGraphicsView;
class QWheelEvent;



class SketchWindow: public QMainWindow
{
    Q_OBJECT


    public:

        SketchWindow( QWidget* parent = 0 );



    public slots:

        void addCanvas( CrossSection* const& cs_ );
        void removeCanvas( CrossSection* const& cs_ );
        void highlightCanvas( CrossSection* const& cs_ );
        void updateCanvas();



    signals:


        void updateVolume( CrossSection::Direction dir_, double w_, double h_ );
        void acceptCurve( const PolyCurve& curve_ );



    protected:

        void createWindow();
        void setCurrentScene( CrossSection* const& cs_ );





    private:

        CanvasStack* cs;

};

#endif // SKETCHWINDOW_H
