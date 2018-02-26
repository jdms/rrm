#ifndef SKETCHWINDOW_H
#define SKETCHWINDOW_H


#include <QMainWindow>
#include <QDockWidget>
#include <QGraphicsView>D

#include "./src/core/widgets/canvasstack.h"
#include "sketchscene.h"

//TODO: remove dependence from mainwindow
#include "./core/definitions/constants.hpp"
#include "./src/core/widgets/color_picker.h"


//class QGraphicsView;
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


        void addFixedCrossSectionCanvas( CrossSection* const& cs_, QColor c_ );
        bool removeFixedCrossSectionCanvas( double depth_ );
        void setFixedCrossSectionsVisible( bool status_ );


        void setCurrentCrossSection( double depth_ );

        void setTopViewImage( const std::string& image_ );
        void setCrossSectionImage( double depth_, const QString& file_, double ox_, double oy_, double x_, double y_ );

        void screenshot();



    signals:


        void updateVolume( Settings::CrossSection::CrossSectionDirections dir_, double w_, double h_ );
        void acceptCurve( const PolyCurve& curve_, double depth_ );
        void defineColorCurrent( const QColor& color_ );
        void setAsCurrent( double depth_ );

        void objectSelected( std::size_t index_ );
        void commitObject();
        void removeCurveFromObject( double depth_, std::size_t index_ );

        void setImageCrossSection( double depth_, const QString& file_, double ox_, double oy_, double x_, double y_ );
        void removeImageFromCrossSection( double depth_ );

        void setImageToTopView( const QString& file_, double ox_, double oy_, double x_, double y_ );
        void removeImageFromTopView();


        void addFixedCrossSection( double depth_ );
        void removeFixedCrossSection( double depth_ );

        void getHeightMap();
        void enablePreview( bool status_ );








    protected:

        void createWindow();
        void createToolBar();


        void setupScene(SketchScene *scene_ );






    private:

        QHBoxLayout* hb_central_widget;

        QDockWidget* dw_canvas_stack = nullptr;
        CanvasStack* cs = nullptr;

        QGraphicsView* main = nullptr;
        QGraphicsView* tv_main = nullptr;


        ColorPicker *cp_color;

        QAction* ac_discard;
        QAction* ac_commit;
        QAction* ac_create;
        QAction* ac_edit_boundary;
        QAction* ac_edit_scene;
        QAction* ac_screenshot;
        QAction* ac_axes;
        QAction* ac_height_map;
        QAction* ac_fixed_csections;
        QAction* ac_enable_preview;

        SketchScene* main_scene = nullptr;
        SketchScene* tv_scene = nullptr;

        std::map< double, SketchScene* > scenes;

        const int WIDTH_APP = 450;
        const int HEIGHT_APP = 320;

};

#endif // SKETCHWINDOW_H
