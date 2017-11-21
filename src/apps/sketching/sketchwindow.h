#ifndef SKETCHWINDOW_H
#define SKETCHWINDOW_H


#include <QMainWindow>

#include "canvasstack.h"
#include "sketchscene.h"

//TODO: remove dependence from mainwindow
#include "mainwindow/widgets/color_picker.h"


class QGraphicsView;
class QWheelEvent;



class SketchWindow: public QMainWindow
{
    Q_OBJECT


    public:

        SketchWindow( QWidget* parent = 0 );



    public slots:


        void addMainCanvas( CrossSection* const& cs_ );

//        void setUpColor();

//        void addCanvas( CrossSection* const& cs_, bool main_ = false );
//        void removeCanvas( CrossSection* const& cs_ );
//        void removeCanvas( double depth_ );
//        void highlightCanvas( CrossSection* const& cs_ );
//        void highlightCanvas( double depth_ );
//        void setMainCanvas( CrossSection* const& cs_ );
//        void updateCanvas();


//        void updateVolumes();


//        void addObject( Object* const& obj_ );
//        void updateObject( const std::size_t& index_ );


//        void addTrajectory( Object* const& obj_ );
//        void updateTrajectory( const std::size_t& index_ );
//        void updateTrajectories();


//        void addCrossSection( CrossSection* const& cs_ );
//        void setCurrentCrossSection( const double& value_ );


//        void setModeSelecting();
//        void setModeSketching();

//        void clear();

//        void setupCrossSectionWindow();
//        void setupTopViewWindow();



    protected slots:

//        void discardSketch();
//        void commitSketch();
//        void createSurface();



    signals:


        void updateVolume( CrossSection::Direction dir_, double w_, double h_ );
        void acceptCurve( const PolyCurve& curve_ );
        void defineColorCurrent( const QColor& color_ );
        void setAsCurrent( double depth_ );

        void objectSelected( std::size_t index_ );
        void commitObject();



    protected:

        void createWindow();
        void createToolBar();


        void setupScene( SketchScene* const& scene_ );
//        void setCurrentScene( CrossSection* const& cs_ );
//        void setCurrentScene( double depth_ );





    private:

        CanvasStack* cs;
        QGraphicsView* main = nullptr;


        ColorPicker *cp_color;

        QAction* ac_discard;
        QAction* ac_commit;
        QAction* ac_create;
        QAction* ac_edit_scene;

};

#endif // SKETCHWINDOW_H
