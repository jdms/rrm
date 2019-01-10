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


       /**
       * Method called to update the color selected in the picker color and also, update the current color in the cross-sections being used.
       * @param red red component of the new color.
       * @param green green component of the new color.
       * @param blue blue component of the new color.
       * @see publicVar()
       * @return Void
       */
        void updateColorWidget( int red_, int green_, int blue_ );

       /**
       * Method called to enable/disable the action "Resize boundary".
       * @param status_ boolean. It is true if the action "Resize boundary" should be enabled, or false otherwise.
       * @see publicVar()
       * @return Void
       */
        void disableResizeVolume( bool status_ );


        //TODO: this method needs to be revisited
       /**
       * Method called to change the interaction mode of the user to selection. This mode disable the curves selection.
       * @param status_ boolean. It is true if the selection interaction should be enabled, or false otherwise.
       * @see publicVar()
       * @return Void
       */
        void setModeSelecting( bool status_ );


       /**
       * Method called to change the interaction mode of the user to selection of curves.
       * @param status_ boolean. It is true if the curves selection interaction should be enabled, or false otherwise.
       * @see publicVar()
       * @return Void
       */
        void setModeSelectingStratigraphies( bool status_ );

       /**
       * Method called to change the interaction mode of the user to selection of regions.
       * @param status_ boolean. It is true if the regions selection interaction should be enabled, or false otherwise.
       * @see publicVar()
       * @return Void
       */
        void setModeRegionSelecting( bool status_ );


       /**
       * Method called to reapply the vertical exaggeration after a volume dimensions change.
       * @see publicVar()
       * @return Void
       */
        void applyVerticalExaggeration();


        /**
       * Method called to reset the vertical exaggeration.
       * @see publicVar()
       * @return Void
       */
        void resetVerticalExaggeration();


        /**
       * Method called to show or hide the output dip angle picture in the scene.
       * @param status_ boolean. It is true if the output dip angle picture should be shown, or false otherwise.
       * @see publicVar()
       * @return Void
       */
        void showDipAngle( bool status_ );


         /**
       * Method called to enable/disable to move the output dip angle picture in the scene.
       * @param status_ boolean. It is true if the output dip angle picture is allowed to be moved, otherwise the value is false.
       * @see publicVar()
       * @return Void
       */
        void setDipAnglePictureMovable( bool status_ );


       /**
       * Method called to take a screenshot of the scene.
       * @see publicVar()
       * @return Void
       */
        void screenshot();


       /**
       * Method called to clear the scene.
       * @see publicVar()
       * @return Void
       */
        void reset();


        /**
       * Method called to remove all the fixed cross-sections canvas.
       * @see publicVar()
       * @return Void
       */
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



       /**
       * When a fixed cross-section is closed this signal is emitted to warn the depth slider (see 3d window) to remove the associated marker.
       * @param id_ A double value corresponding the depth of the cross-section removed.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void removeMarkerFromSlider( double id_ );


       /**
       * Signal to ask the controller to save the image in the cross-section specified.
       * @param const std::string& path of the image
       * @param const Settings::CrossSection::CrossSectionDirections& direction of the cross-section
       * @param double depth of the cross-section
       * @param double x coordinate of the origin of the image
       * @param double y coordinate of the origin of the image
       * @param double width of the image
       * @param double height of the image
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void setImageToCrossSection( const std::string&, const Settings::CrossSection::CrossSectionDirections&, double, double, double, double, double );


       /**
       * Signal to ask the controller to remove the image of the cross-section specified.
       * @param const Settings::CrossSection::CrossSectionDirections& direction of the cross-section
       * @param double depth of the cross-section
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void removeImageFromCrossSection( const Settings::CrossSection::CrossSectionDirections& , double );

       /**
       * Signal to update the current image dimensions.
       * @param const Settings::CrossSection::CrossSectionDirections& direction of the cross-section
       * @param double width of the image
       * @param double height of the image
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void updateVolumeDimensions( const  Settings::CrossSection::CrossSectionDirections& , double , double );

       /**
       * Signal to send the sketch and add it as a curve of the current object
       * @param curve_ the sketch drawn by the user (Polycurve)
       * @param dir_ the direction of the cross-section which was drawn the sketch
       * @param depth_ the depth of the cross-section which was drawn the sketch
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void addCurve( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );

       /**
       * Signal to send the sketch and add it as a trajectory of the current object. Currently, a trajectory only is drawn in the cross-section of the height direction.
       * @param curve_ the sketch drawn by the user (Polycurve)
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void addTrajectory( const PolyCurve& curve_ );


       /**
       * Signal to send to controller so that it removes the last curve inserted in the cross-section of direction dir_ and depth depth_.
       * @param dir_ direction of the cross-section which the curve was drawn.
       * @param depth_ depth_ of the cross-section which the curve was drawn.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void removeLastCurve( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


       /**
       * Signal to send to controller so that it creates the surface with the curves of the current object.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void createObject();


       /**
       * Signal to send to controller so that it retrieves the last trajectory to be used in the current object.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void useLastTrajectory();


       /**
       * Signal emitted when the user select a new color in the color picker.
       * @param red red component of the new color.
       * @param green green component of the new color.
       * @param blue blue component of the new color.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void defineColorCurrent( int red_, int green_, int blue_ );


        /**
       * Signal emitted when the user select a curve.
       * @param id_ the id_ of the object which has the curve selected.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void objectSelected( const std::size_t& id_ );


       /**
       * Signal emitted when the user pick a point inside a region.
       * @param px_ x coordinate of the point.
       * @param py_ y coordinate of the point.
       * @param depth_ depth of the cross-section in which the point was picked.
       * @param dir_ direction of the cross-section in which the point was picked.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void getRegionByPoint( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );

        /**
       * Signal emitted when the user drawn a sketch to select a curve.
       * @param curve_ sketch drawn used to select a curve
       * @param dir_ direction of the cross-section in which the sketch was done.
       * @param depth_ depth of the cross-section in which the sketch was done.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void sendSketchOfSelection( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
       * This signal is emitted when the user is satisfied with the regions selected using the sketches of selection. This stop the process cycle and the region selected is considerate the final.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void stopSketchesOfSelection();


        /**
       * Signal emitted to update the vertical exaggeration in the scene 2d and 3d.
       * @param double the current vertical exaggeration
       * @see publicVar()
       * @return Void
       */
        void setVerticalExaggeration( double );


        /**
       * Signal emitted to show or hide the output dip angle picture in the scene. It is associated to the button "Show" in the LateralBar.
       * @param status_ boolean. True if the scene should show the output dip angle picture, and false otherwise.
       * @see publicVar()
       * @return Void
       */
        void showDipAnglePicture( bool status_ );


        /**
       * Signal emitted when the user select a region clicking in the object region. Currently this method is bot being used properly.
       * @param id_ index of the region
       * @param status_ status of selection of the object region. If true the region is selected, false otherwise.
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void regionSelected( const std::size_t& id_, bool status_ );


        //TODO: fix all comments about guided extrusion

        /**
       * Signal emitted when the user select a point to be used in the guided extrusion. Currently this method is bot being used.
       * @param px_ x coordinate of the point.
       * @param py_ y coordinate of the point.
       * @param depth_ depth of the cross-section in which the point was picked.
       * @param dir_ direction of the cross-section in which the point was picked.
       * @see publicVar()
       * @return Void
       */
        void sendPointGuidedExtrusion( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );


       /**
       * Signal emitted to update point to be used in the guided extrusion. Currently this method is bot being used.
       * @param px_ x coordinate of the point.
       * @param py_ y coordinate of the point.
       * @param depth_ depth of the cross-section in which the point was picked.
       * @param dir_ direction of the cross-section in which the point was picked.
       * @see publicVar()
       * @return Void
       */
        void updatePointGuidedExtrusion( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );


       /**
       * This signal is emitted to send the sketch done in the guided extrusion.
       * @param curve_ sketch done by the user for the guided extrusion.
       * @see publicVar()
       * @return Void
       */
        void sketchDoneGuidedExtrusion( const PolyCurve& curve_ );


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
