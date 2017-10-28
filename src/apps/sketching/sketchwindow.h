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
        void setMainCanvas( CrossSection* const& cs_ );
        void updateCanvas();


        void updateVolumes();


        void addObject( Object* const& obj_ );
        void updateObject( const std::size_t& index_ );



        void setCurrentCrossSection( const double& value_ );





    protected slots:

        void discardSketch();
        void commitSketch();
        void createSurface();



    signals:


        void updateVolume( CrossSection::Direction dir_, double w_, double h_ );
        void acceptCurve( const PolyCurve& curve_ );



    protected:

        void createWindow();
        void createToolBar();
        void setCurrentScene( CrossSection* const& cs_ );





    private:

        CanvasStack* cs;
        QGraphicsView* main = nullptr;

        QAction* ac_discard;
        QAction* ac_commit;
        QAction* ac_create;
        QAction* ac_edit_scene;

};

#endif // SKETCHWINDOW_H
