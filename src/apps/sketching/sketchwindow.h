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


       /**
       * Method to create the canvas associated to the main cross-section, i. e., it exhibts the width and depth directions.
       * The creation of the lateral bar only is called here since, it is available only to main cross-sections.
       * @see Javadoc_Test()
       * @see ~Javadoc_Test()
       * @see testMeToo()
       * @see publicVar()
       * @return std::shared_ptr< SketchScene > A shared pointer to the scene associated to the main cross-section.
       */
        std::shared_ptr< SketchScene > createMainCanvas();


       /**
       * Method to add fixed cross-sections. Fixed cross-sections should have the same direction per time.
       * @param depth_ The depth of the cross-section in the direction dir_ ( WIDTH, LENGHT or DEPTH ) to be visualized.
       * @param dir_ The direction of the cross-section ( WIDTH, LENGHT or DEPTH ) to be visualized.
       * @param color_ The color of the marker that was used in the depth slider ( see slider in the 3d widget ).
       * @see Javadoc_Test()
       * @see ~Javadoc_Test()
       * @see testMeToo()
       * @see publicVar()
       * @return std::shared_ptr< SketchScene > A shared pointer to the scene associated to the cross-section added.
       */
        std::shared_ptr< SketchScene > addCanvas( double depth_ = 0, const Settings::CrossSection::CrossSectionDirections& dir_ = Settings::CrossSection::CrossSectionDirections::Z , QColor color_ = Qt::red );


       /**
       * Method to remove a fixed cross-section.
       * @param depth_ The depth of the cross-section to be removed.
       * @see Javadoc_Test()
       * @see ~Javadoc_Test()
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void removeCanvas( double depth_ );


       /**
       * Method to create the canvas associated to the top view cross-section, i. e., it exhibts only the height direction.
       * The creation of the lateral bar not is called here, since, it is available only to main cross-sections.
       * @see Javadoc_Test()
       * @see ~Javadoc_Test()
       * @see testMeToo()
       * @see publicVar()
       * @return std::shared_ptr< SketchScene > A shared pointer to the scene associated to the top view cross-section.
       */
        std::shared_ptr< SketchScene > createTopViewCanvas();



    public slots:



        void updateColorWidget( int red_, int green_, int blue_ );
        void disableResizeVolume( bool status_ );

        void setModeSelecting( bool status_ );
        void setModeSelectingStratigraphies( bool status_ );
        void setModeRegionSelecting( bool status_ );

        void applyVerticalExaggeration();
        void resetVerticalExaggeration();


        void showDipAngle( bool status_ );
        void setDipAnglePictureMovable( bool status_ );

        void screenshot();
        void reset();

        void removeAllCanvas();


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
       * Protected method responsibles for the interface creation. It calls the createToolbar and creates the fixed cross-sections container.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void createInterface();

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
       * Protected method to handle the keyboard events.
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

        QWidget* bar_ = nullptr;                                    /**< Dip angle and vertical exaggeration widget. To be moved to a new class. */

        const bool SHOW_VERTICAL_EXAGGERATION = false;              /**< Default status to the show lateral bar action. True means the action is selected, otherwise, unselected. */


        QToolBar* tb_region = nullptr;
        QAction* ac_select_regions = nullptr;
        const bool SELECT_REGION_DEFAULT_STATUS = false;

};

#endif // SKETCHWINDOW_H
