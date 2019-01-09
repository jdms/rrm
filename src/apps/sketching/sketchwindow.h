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
#include <QDoubleSpinBox>

#include "./src/core/widgets/canvasstack.h"
#include "./core/definitions/constants.hpp"
#include "./src/core/widgets/color_picker.h"
#include "./src/core/widgets/realfeaturedslider.h"

#include "sketchingcanvas.h"
#include "sketchscene.h"

#include "models/anglepicture.h"

#include "lateralbar.h"


class QWheelEvent;


/**
 *   The interface of the Sketch application.
 *   It contains the canvas and toolbar with the main actions related to sketch.
 */

class SketchWindow: public QMainWindow
{
    Q_OBJECT


    public:

       /**
       * Constructor.
       */
        SketchWindow( QWidget* parent = nullptr );

       /**
       * Destructor. Delete all elements created in the window.
       */
        ~SketchWindow();

        std::shared_ptr< SketchScene > createMainCanvas();
        std::shared_ptr< SketchScene > addCanvas( double depth_ = 0, const Settings::CrossSection::CrossSectionDirections& dir_ = Settings::CrossSection::CrossSectionDirections::Z , QColor color_ = Qt::red );
        void removeCanvas( double depth_ );


        std::shared_ptr< SketchScene > createTopViewCanvas();

//        void usingVerticalExaggeration( int v_exagg_);
        void applyVerticalExaggeration();






    public slots:

        void updateColorWidget( int red_, int green_, int blue_ );
        void disableResizeVolume( bool status_ );

        void setModeSelecting( bool status_ );
        void setModeSelectingStratigraphies( bool status_ );
        void setModeRegionSelecting( bool status_ );
//        void setDipAngle( double angle_ );
        void setDipAnglePictureMovable( bool status_ );

        void showDipAngle( bool status_ );
        void resetVerticalExaggeration();

        void screenshot();
        void reset();

//        void updateDipAngle();
        void removeAllCanvas();

//        void usingVerticalExaggerationSpinBox( double v_exagg_ );

    signals:


        /// New signals

       /**
       * A signal to change the current color, i.e., the color of the current object.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void sgn_change_current_color();

       /**
       * A signal to submit the sketch as an object curve.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void sgn_submit_curve();

       /**
       * A signal to resize the current boundary.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void sgn_resize_boundary();

       /**
       * A signal to resize the image on the scene.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void sgn_resize_image();

       /**
       * A signal to remove the image from the scene.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void sgn_remove_image();

       /**
       * A signal to take a screenshot from the scene.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void sgn_take_screenshot();

       /**
       * A signal to show and hide the axes from the scene.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void sgn_show_axes();

        /// End -- New signals



        void removeMarkerFromSlider( double id_ );

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

        void showDipAnglePicture( bool status_ );


    protected:

       /**
       * Protected method responsibles for the toolbar creation.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void createToolBar();


       /**
       * Protected method responsibles for the lateral bar creation. The lateral bar contains the dip angle and vertical exaggeration widgets.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void createLateralBar();


       /**
       * Protected method to handle the keyboard interaction.
       * @param event Corresponds to the mouse event.
       * @see check QKeyEvent on Qt manual for more details.
       * @return Void
       */
        void keyPressEvent( QKeyEvent *event );

    private:


        SketchingCanvas* sketchingcanvas = nullptr;                 /**< Canvas to draw the sketches, exhibiting the main cross-section. It inherits from QGraphicsView. */
        SketchingCanvas* topviewcanvas = nullptr;                   /**< Canvas to draw the sketches, exhibiting the top-view cross-section. It inherits from QGraphicsView. */

        CanvasStack* fixed_csections_canvas = nullptr;              /**< Container of canvas. Stores the fixed canvas. */

        ColorPicker *cp_color = nullptr;                            /**< Widget to pick a color. Stores the fixed canvas. */

        QToolBar* tb_sketch = nullptr;                              /**< Toolbar with sketching related actions. */
        QAction* ac_cancel_sketch = nullptr;                        /**< Action to cancel a sketch before submit it. */
        QAction* ac_submit_sketch = nullptr;                        /**< Action to submit a sketch to the current object. */
        QAction* ac_end_object = nullptr;                           /**< Action to create the surface from the curves submited to the current object. */

        QToolBar* tb_boundary = nullptr;                            /**< Toolbar with boundary related actions. */
        QAction* ac_resize_boundary = nullptr;                      /**< Action to resize boundary. It is a selectable action. */
        const bool RESIZE_BOUNDARY_DEFAULT_STATUS = false;          /**< Default status to the resize boundary action. True means the action is selected, otherwise, unselected.  */

        QToolBar* tb_image = nullptr;                               /**< Toolbar with image related actions. */
        QAction* ac_resize_image = nullptr;                         /**< Action to resize image. It is a selectable action. */
        QAction* ac_remove_image = nullptr;                         /**< Action to remove image from scene. */

        QToolBar* tb_region = nullptr;
        QAction* ac_select_regions = nullptr;
        const bool SELECT_REGION_DEFAULT_STATUS = false;


        // only in top-view cross-section
        QToolBar* tb_trajectory = nullptr;                          /**< Toolbar with trajectory related actions. */
        QAction* ac_use_last_trajectory = nullptr;                  /**< Action to reuse the last trajectory submitted. It is a selectable action. */
        const bool USE_TRAJECTORY_DEFAULT_STATUS = false;           /**< Default status to the resize boundary action. True means the action is selected, otherwise, unselected. */

        QAction* ac_fixed_csections = nullptr;                      /**< Action to show/hide the fixed cross-sections window. It is a selectable action. */

        QToolBar* tb_misc = nullptr;                                /**< Toolbar with other related actions. */
        QAction* ac_screenshot = nullptr;                           /**< Action to take a screenshot from the scene. */
        QAction* ac_axes = nullptr;                                 /**< Action to show/hide the axes. */


        // only in main cross-section
        QToolBar* tb_lateral_bar = nullptr;                         /**< Toolbar with dip angle and vertical exaggeration related actions. */
        QAction* ac_show_bar = nullptr;                             /**< Action to show/hide the lateral bar. It is a selectable action. Lateral bar contains dip angle and vertical exaggeration widgets. */
        LateralBar* latBar = nullptr;
        const bool SHOW_VERTICAL_EXAGGERATION = false;              /**< Default status to the show lateral bar action. True means the action is selected, otherwise, unselected. */


        QWidget* bar_ = nullptr;                                    /**< Dip angle and vertical exaggeration widget. To be moved to a new class. */
        QSlider* sl_vertical_exagg_ = nullptr;
        int nsteps_exagg = 100;
        double min_exagg = -1;
        double max_exagg = 4;
        double steps_exagg = 0;


        QDial* dl_input_angle_ = nullptr;
        QHBoxLayout* vb_angles = nullptr;
        QVBoxLayout* hb_lateral_bar = nullptr;
        QHBoxLayout* hb_central = nullptr;
        QLCDNumber* lb_input_angle_  = nullptr;
        QLCDNumber* lb_output_angle_ = nullptr;
        QLabel* lb_exagger_value_ = nullptr;
        QDoubleSpinBox* sp_exagger_value = nullptr;
        QPushButton* btn_reset_exaggeration = nullptr;

        AnglePicture* lb_input_dpangle = nullptr;
        AnglePicture* lb_output_dpangle = nullptr;

        QPushButton* btn_show_oangle  = nullptr;
        QPushButton* btn_move_oangle = nullptr;

        QToolBar* tb_well = nullptr;                      // not being used
        QAction* ac_select_wells = nullptr;               // not being used
        const bool SELECT_WELLS_DEFAULT_STATUS = false;   // not being used



        int count = 0;

};

#endif // SKETCHWINDOW_H
