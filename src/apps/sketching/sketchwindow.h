/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */



#ifndef SKETCHWINDOW_H
#define SKETCHWINDOW_H


#include <QMainWindow>
#include <QDockWidget>
#include <QGraphicsView>

#include "./src/core/widgets/canvasstack.h"
#include "sketchscene.h"

//TODO: remove dependence from mainwindow
#include "./core/definitions/constants.hpp"
#include "./src/core/widgets/color_picker.h"

#include "sketchingcanvas.h"


//class QGraphicsView;
class QWheelEvent;



class SketchWindow: public QMainWindow
{
    Q_OBJECT


    public:

        SketchWindow( QWidget* parent = nullptr );
        ~SketchWindow();

        std::shared_ptr< SketchScene > createMainCanvas();
        std::shared_ptr< SketchScene > addCanvas( double depth_ = 0 );
        void removeCanvas( double depth_ );


        ///================================================================================


//    public slots:


//        void addMainCanvas( CrossSection* const& cs_ );
//        void addTopViewCanvas( CrossSection* const& cs_ );
//        void updateCanvas();


//        void addCrossSection( CrossSection* const& cs_ );



//        void addObject( Object* const& obj_ );
//        void updateObject( const std::size_t& index_ );


//        void addTrajectory( Object* const& obj_ );
//        void updateTrajectory( const std::size_t& index_ );


//        void setModeSelecting();
//        void setModeSketching();


//        void setCurrentColor( int r_, int g_, int b_ );


//        void clear();


//        void addFixedCrossSectionCanvas( CrossSection* const& cs_, QColor c_ );
//        bool removeFixedCrossSectionCanvas( double depth_ );
//        void setFixedCrossSectionsVisible( bool status_ );


//        void setCurrentCrossSection( double depth_ );

//        void setTopViewImage( const std::string& image_ );
//        void setCrossSectionImage( double depth_, const QString& file_, double ox_, double oy_, double x_, double y_ );

//        void screenshot();



    signals:

        void addCurve( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );

//        void updateVolume( Settings::CrossSection::CrossSectionDirections dir_, double w_, double h_ );
//        void acceptCurve( const PolyCurve& curve_, double depth_ );
//        void defineColorCurrent( const QColor& color_ );
//        void setAsCurrent( double depth_ );

//        void objectSelected( std::size_t index_ );
//        void commitObject();
//        void removeCurveFromObject( double depth_, std::size_t index_ );

//        void setImageCrossSection( double depth_, const QString& file_, double ox_, double oy_, double x_, double y_ );
//        void removeImageFromCrossSection( double depth_ );

//        void setImageToTopView( const QString& file_, double ox_, double oy_, double x_, double y_ );
//        void removeImageFromTopView();


//        void addFixedCrossSection( double depth_ );
//        void removeFixedCrossSection( double depth_ );

//        void getHeightMap();
//        void enablePreview( bool status_ );








    protected:

//        void createWindow();
        void createToolBar();




    private:

        ///================================================================================

        SketchingCanvas* sketchingcanvas = nullptr;
        CanvasStack* fixed_csections_canvas = nullptr;

        QToolBar* tb_sketch = nullptr;
        QAction* ac_sketch_color = nullptr;
        QAction* ac_cancel_sketch = nullptr;
        QAction* ac_submit_sketch = nullptr;
        QAction* ac_end_object = nullptr;

        QToolBar* tb_boundary = nullptr;
        QAction* ac_resize_boundary = nullptr;
        const bool RESIZE_BOUNDARY_DEFAULT_STATUS = false;

        QToolBar* tb_region = nullptr;
        QAction* ac_select_regions = nullptr;
        const bool SELECT_REGION_DEFAULT_STATUS = false;

        QToolBar* tb_well = nullptr;
        QAction* ac_select_wells = nullptr;
        const bool SELECT_WELLS_DEFAULT_STATUS = false;

        ///================================================================================


//        QHBoxLayout* hb_central_widget;

//        QDockWidget* dw_canvas_stack = nullptr;
//        CanvasStack* cs = nullptr;

//        QGraphicsView* main = nullptr;
//        QGraphicsView* tv_main = nullptr;


//        ColorPicker *cp_color;

//        QAction* ac_discard;
//        QAction* ac_commit;
//        QAction* ac_create;
//        QAction* ac_edit_boundary;
//        QAction* ac_edit_scene;
//        QAction* ac_screenshot;
//        QAction* ac_axes;
//        QAction* ac_height_map;
//        QAction* ac_fixed_csections;
//        QAction* ac_enable_preview;

//        SketchScene* main_scene = nullptr;
//        SketchScene* tv_scene = nullptr;

//        std::map< double, SketchScene* > scenes;

//        const int WIDTH_APP = 450;
//        const int HEIGHT_APP = 320;

};

#endif // SKETCHWINDOW_H
