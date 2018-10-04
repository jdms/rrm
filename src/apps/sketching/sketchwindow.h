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
#include <QSlider>
#include <QStatusBar>
#include <QDial>
#include <QLabel>
#include <QLCDNumber>
#include <QKeyEvent>
#include <QPushButton>

#include "./src/core/widgets/canvasstack.h"
#include "./core/definitions/constants.hpp"
#include "./src/core/widgets/color_picker.h"
#include "./src/core/widgets/realfeaturedslider.h"

#include "sketchingcanvas.h"
#include "sketchscene.h"

#include "models/anglepicture.h "


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

        void createLateralBar();

        std::shared_ptr< SketchScene > createTopViewCanvas();

        void usingVerticalExaggeration(int v_exagg_);

        void keyPressEvent( QKeyEvent *event );



    public slots:

        void updateColorWidget( int red_, int green_, int blue_ );
        void disableResizeVolume( bool status_ );

        void setModeSelecting( bool status_ );
        void setModeSelectingStratigraphies( bool status_ );
        void setModeRegionSelecting( bool status_ );
        void setDipAngle( double angle_ );

        void reset();


    signals:


        void setImageToCrossSection( const std::string&, const Settings::CrossSection::CrossSectionDirections&, double, double, double, double, double );

        void removeImageFromCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );

        void updateVolumeDimensions( const  Settings::CrossSection::CrossSectionDirections& dir_, double width_, double height_ );

        void addCurve( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );
        void addTrajectory( const PolyCurve& curve_ );
        void removeLastCurve( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );

        void createObject();

        void useLastTrajectory();

        void defineColorCurrent( int red_, int green_, int blue_ );

        void objectSelected( const std::size_t& id_ );

        void getRegionByPoint( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );

        void sendSketchOfSelection( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );
        void stopSketchesOfSelection();

        void regionSelected( const std::size_t& id_, bool status_ );

        void sendPointGuidedExtrusion( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );

        void setAreaChoosed();

        void setVerticalExaggeration( double );

        void updatePointGuidedExtrusion( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );

        void sketchDoneGuidedExtrusion( const PolyCurve& curve_ );

    protected:

//        void createWindow();
        void createToolBar();




    private:

        ///================================================================================

        SketchingCanvas* sketchingcanvas = nullptr;
        CanvasStack* fixed_csections_canvas = nullptr;

        SketchingCanvas* topviewcanvas = nullptr;

        ColorPicker *cp_color = nullptr;

        QToolBar* tb_sketch = nullptr;
        QAction* ac_sketch_color = nullptr;
        QAction* ac_cancel_sketch = nullptr;
        QAction* ac_submit_sketch = nullptr;
        QAction* ac_end_object = nullptr;

        QToolBar* tb_boundary = nullptr;
        QAction* ac_resize_boundary = nullptr;
        const bool RESIZE_BOUNDARY_DEFAULT_STATUS = false;

        QToolBar* tb_image = nullptr;
        QAction* ac_resize_image = nullptr;
        QAction* ac_remove_image = nullptr;

        QToolBar* tb_region = nullptr;
        QAction* ac_select_regions = nullptr;
        const bool SELECT_REGION_DEFAULT_STATUS = false;

        QToolBar* tb_well = nullptr;
        QAction* ac_select_wells = nullptr;
        const bool SELECT_WELLS_DEFAULT_STATUS = false;

        QToolBar* tb_trajectory = nullptr;
        QAction* ac_use_last_trajectory = nullptr;
        const bool USE_TRAJECTORY_DEFAULT_STATUS = false;

        QWidget* bar_ = nullptr;
        QSlider* sl_vertical_exagg_ = nullptr;
        int nsteps_exagg = 100;
        double min_exagg = -1;
        double max_exagg = 3;
        double steps_exagg = 0;


        QDial* dl_input_angle_ = nullptr;
        QHBoxLayout* vb_angles = nullptr;
        QVBoxLayout* hb_central = nullptr;
        QHBoxLayout* hb_central1 = nullptr;
        QLCDNumber* lb_input_angle_  = nullptr;
        QLCDNumber* lb_output_angle_ = nullptr;
        QLabel* lb_exagger_value_ = nullptr;

        AnglePicture* lb_input_dpangle = nullptr;
        AnglePicture* lb_output_dpangle = nullptr;

        QPushButton* btn_show_oangle  = nullptr;


        QToolBar* tb_lateral_bar = nullptr;
        QAction* ac_show_bar = nullptr;
        const bool SHOW_VERTICAL_EXAGGERATION = false;



        ///================================================================================


//        QHBoxLayout* hb_central_widget;

//        QDockWidget* dw_canvas_stack = nullptr;
//        CanvasStack* cs = nullptr;

//        QGraphicsView* main = nullptr;
//        QGraphicsView* tv_main = nullptr;


//

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
