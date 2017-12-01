#ifndef SKETCHWINDOW_H
#define SKETCHWINDOW_H


#include <QMainWindow>

#include "canvasstack.h"
#include "sketchscene.h"

//TODO: remove dependence from mainwindow
#include "./core/base/constants/constants.hpp"
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
        void addTopViewCanvas( CrossSection* const& cs_ );
        void updateCanvas();


        void addCrossSection( CrossSection* const& cs_ );



        void addObject( Object* const& obj_ );
        void updateObject( const std::size_t& index_ );


        void addTrajectory( Object* const& obj_ );
        void updateTrajectory( const std::size_t& index_ );


        void setModeSelecting();
        void setModeSketching();


        void setCurrentColor( int r_, int g_, int b_ );


        void clear();


        void addFixedCrossSectionCanvas( CrossSection* const& cs_ );
        bool removeFixedCrossSectionCanvas( double depth_ );


        void setCurrentCrossSection( double depth_ );

        void setTopViewImage( const std::string& image_ );



    signals:


        void updateVolume( Settings::CrossSection::CrossSectionDirections dir_, double w_, double h_ );
        void acceptCurve( const PolyCurve& curve_, double depth_ );
        void defineColorCurrent( const QColor& color_ );
        void setAsCurrent( double depth_ );

        void objectSelected( std::size_t index_ );
        void commitObject();

        void setImageCrossSection( double depth_, const QString& file_, double ox_, double oy_, double scale_ );
        void getHeightMap();



    protected:

        void createWindow();
        void createToolBar();


        void setupScene( SketchScene* const& scene_ );






    private:

        QHBoxLayout* hb_central_widget;

        CanvasStack* cs = nullptr;

        QGraphicsView* main = nullptr;
        QGraphicsView* tv_main = nullptr;


        ColorPicker *cp_color;

        QAction* ac_discard;
        QAction* ac_commit;
        QAction* ac_create;
        QAction* ac_edit_scene;
        QAction* ac_axes;
        QAction* ac_height_map;

};

#endif // SKETCHWINDOW_H
