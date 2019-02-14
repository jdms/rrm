/****************************************************************************
 * RRM - Rapid Reservoir Modeling Project                                   *
 * Copyright (C) 2015                                                       *
 * UofC - University of Calgary                                             *
 *                                                                          *
 * This file is part of RRM Software.                                       *
 *                                                                          *
 * RRM is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * RRM is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.             *
 ****************************************************************************/

/**
 * @file sketchwindow.h
 * @author Clarissa C. Marques
 * @brief File containing the class SketchWindow
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
 *   \brief The interface of the Sketch application.
 *   It contains the canvas and toolbar with the main actions related to sketch.
 */

class SketchWindow: public QMainWindow
{
    Q_OBJECT


    public:


        /**
        * Constructor. parent_ is passed to QMainWindow's constructor.
        * @param parent_ the parent item
        */
        SketchWindow( QWidget* parent = nullptr );

       /**
       * \brief Destructor.
       * Delete all elements created in the window.
       */
        ~SketchWindow();


       /**
       * \brief Method to create the canvas associated to the main cross-section, i. e., it exhibts the width and depth directions.
       * The creation of the lateral bar only is called here since, it is available only to main cross-sections.
       * @see SketchScene
       * @return std::shared_ptr< SketchScene > A shared pointer to the scene associated to the main cross-section.
       */
        std::shared_ptr< SketchScene > createMainCanvas();


       /**
       * \brief Method to add fixed cross-sections. Fixed cross-sections should have the same direction per time.
       * @param depth The depth of the cross-section in the direction dir ( WIDTH, LENGHT or DEPTH ) to be visualized.
       * @param dir The direction of the cross-section ( WIDTH, LENGHT or DEPTH ) to be visualized.
       * @param color The color of the marker that was used in the depth slider ( see slider in the 3d widget ).
       * @see SketchScene
       * @return std::shared_ptr< SketchScene > A shared pointer to the scene associated to the cross-section added.
       */
        std::shared_ptr< SketchScene > addCanvas( double depth_ = 0, const Settings::CrossSection::CrossSectionDirections& dir_ = Settings::CrossSection::CrossSectionDirections::Z , QColor color_ = Qt::red );


       /**
       * \brief Method to remove a fixed cross-section.
       * @param depth The depth of the cross-section to be removed.
       * @return void
       */
        void removeCanvas( double depth_ );


       /**
       * \brief Method to create the canvas associated to the top view cross-section, i. e., it exhibts only the height direction.
       * The creation of the lateral bar not is called here, since, it is available only to main cross-sections.
       * @see SketchScene
       * @return std::shared_ptr< SketchScene > A shared pointer to the scene associated to the top view cross-section.
       */
        std::shared_ptr< SketchScene > createTopViewCanvas();



    public slots:


       /**
       * \brief Method called to update the color selected in the picker color and also, update the current color in the cross-sections being used.
       * @param red red component of the new color.
       * @param green green component of the new color.
       * @param blue blue component of the new color.
       * @return void
       */
        void updateColorWidget( int red_, int green_, int blue_ );

       /**
       * \brief Method called to enable/disable the action "Resize boundary".
       * @param status boolean. It is true if the action "Resize boundary" should be enabled, or false otherwise.
       * @return void
       */
        void disableResizeVolume( bool status_ );


        //TODO: this method needs to be changed
       /**
       * \brief Method called to change the interaction mode of the user to selection. This mode disable the curves selection.
       * @param status boolean. It is true if the selection interaction should be enabled, or false otherwise.
       * @return void
       */
        void setModeSelecting( bool status_ );


       /**
       * \brief Method called to change the interaction mode of the user to selection of curves.
       * @param status boolean. It is true if the curves selection interaction should be enabled, or false otherwise.
       * @return void
       */
        void setModeSelectingStratigraphies( bool status_ );


       /**
       * \brief Method called to change the interaction mode of the user to selection of regions.
       * @param status boolean. It is true if the regions selection interaction should be enabled, or false otherwise.
       * @return void
       */
        void setModeRegionSelecting( bool status_ );


       /**
       * \brief Method called to reapply the vertical exaggeration after a volume dimensions change.
       * @return void
       */
        void applyVerticalExaggeration();


        /**
       * \brief Method called to reset the vertical exaggeration.
       * @return void
       */
        void resetVerticalExaggeration();


        /**
       * \brief Method called to show or hide the output dip angle picture in the scene.
       * @param status boolean. It is true if the output dip angle picture should be shown, or false otherwise.
       * @return void
       */
        void showDipAngle( bool status_ );


         /**
       * \brief Method called to enable/disable to move the output dip angle picture in the scene.
       * @param status boolean. It is true if the output dip angle picture is allowed to be moved, otherwise the value is false.
       * @return void
       */
        void setDipAnglePictureMovable( bool status_ );


       /**
       * \brief Method called to take a screenshot of the scene.
       * @return void
       */
        void screenshot();


       /**
       * \brief Method called to clear the scene.
       * @return void
       */
        void reset();


        /**
       * \brief Method called to remove all the fixed cross-sections canvas.
       * @return void
       */
        void removeAllCanvas();


    signals:


       /**
       * \brief When a fixed cross-section is closed this signal is emitted to warn the depth slider (see 3d window) to remove the associated marker.
       * @param id A double value corresponding the depth of the cross-section removed.
       * @see View3dInterface::sl_depth_csection
       * @return void
       */
        void removeMarkerFromSlider( double id_ );


       /**
       * \brief Signal to ask the controller to save the image in the cross-section specified.
       * @param const std::string& path of the image
       * @param const Settings::CrossSection::CrossSectionDirections& direction of the cross-section
       * @param double depth of the cross-section
       * @param double x coordinate of the origin of the image
       * @param double y coordinate of the origin of the image
       * @param double width of the image
       * @param double height of the image
       * @see Settings::CrossSection::CrossSectionDirections
       * @return void
       */
        void setImageToCrossSection( const std::string&, const Settings::CrossSection::CrossSectionDirections&, double, double, double, double, double );


       /**
       * \brief Signal to ask the controller to remove the image of the cross-section specified.
       * @param const Settings::CrossSection::CrossSectionDirections& direction of the cross-section
       * @param double depth of the cross-section
       * @see Settings::CrossSection::CrossSectionDirections
       * @return void
       */
        void removeImageFromCrossSection( const Settings::CrossSection::CrossSectionDirections& , double );


       /**
       * \brief Signal to update the current image dimensions.
       * @param const Settings::CrossSection::CrossSectionDirections& direction of the cross-section
       * @param double width of the image
       * @param double height of the image
       * @see Settings::CrossSection::CrossSectionDirections
       * @return void
       */
        void updateVolumeDimensions( const  Settings::CrossSection::CrossSectionDirections& , double , double );


       /**
       * \brief Signal to send the sketch and add it as a curve of the current object
       * @param curve the sketch drawn by the user (Polycurve)
       * @param dir the direction of the cross-section which was drawn the sketch
       * @param depth the depth of the cross-section which was drawn the sketch
       * @see Settings::CrossSection::CrossSectionDirections
       * @return void
       */
        void addCurve( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );

       /**
       * \brief Signal to send the sketch and add it as a trajectory of the current object. Currently, a trajectory only is drawn in the cross-section of the height direction.
       * @param curve the sketch drawn by the user (Polycurve)
       * @see PolyCurve
       * @return void
       */
        void addTrajectory( const PolyCurve& curve_ );


       /**
       * \brief Signal to send to controller so that it removes the last curve inserted in the cross-section of direction dir_ and depth depth_.
       * @param dir direction of the cross-section which the curve was drawn.
       * @param depth depth_ of the cross-section which the curve was drawn.
       * @see Settings::CrossSection::CrossSectionDirections
       * @return void
       */
        void removeLastCurve( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


       /**
       * \brief Signal to send to controller so that it creates the surface with the curves of the current object.
       * @return void
       */
        void createObject();


       /**
       * \brief Signal to send to controller so that it retrieves the last trajectory to be used in the current object.
       * @return void
       */
        void useLastTrajectory();


       /**
       * \brief Signal emitted when the user select a new color in the color picker.
       * @param red red component of the new color.
       * @param green green component of the new color.
       * @param blue blue component of the new color.
       * @return void
       */
        void defineColorCurrent( int red_, int green_, int blue_ );


        /**
       * \brief Signal emitted when the user select a curve.
       * @param id the id of the object which has the curve selected.
       * @return void
       */
        void objectSelected( const std::size_t& id_ );


       /**
       * \brief Signal emitted when the user pick a point inside a region.
       * @param px x coordinate of the point.
       * @param py y coordinate of the point.
       * @param depth depth of the cross-section in which the point was picked.
       * @param dir direction of the cross-section in which the point was picked.
       * @see Settings::CrossSection::CrossSectionDirections
       * @return void
       */
        void getRegionByPoint( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
       * \brief Signal emitted when the user drawn a sketch to select a curve.
       * @param curve sketch drawn used to select a curve
       * @param dir direction of the cross-section in which the sketch was done.
       * @param depth depth of the cross-section in which the sketch was done.
       * @see Settings::CrossSection::CrossSectionDirections
       * @return void
       */
        void sendSketchOfSelection( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
       * \brief This signal is emitted when the user is satisfied with the regions selected using the sketches of selection. This stop the process cycle and the region selected is considerate the final.
       * @return void
       */
        void stopSketchesOfSelection();


        /**
       * \brief Signal emitted to update the vertical exaggeration in the scene 2d and 3d.
       * @param double the current vertical exaggeration
       * @return void
       */
        void setVerticalExaggeration( double );


        /**
       * \brief Signal emitted to show or hide the output dip angle picture in the scene. It is associated to the button "Show" in the LateralBar.
       * @param status boolean. True if the scene should show the output dip angle picture, and false otherwise.
       * @return void
       */
        void showDipAnglePicture( bool status_ );


        /**
       * \brief Signal emitted when the user select a region clicking in the object region. Currently this method is bot being used properly.
       * @param id index of the region
       * @param status status of selection of the object region. If true the region is selected, false otherwise.
       * @return void
       */
        void regionSelected( const std::size_t& id_, bool status_ );


        //TODO: fix all comments about guided extrusion

        /**
       * \brief Signal emitted when the user select a point to be used in the guided extrusion. Currently this method is bot being used.
       * @param px x coordinate of the point.
       * @param py y coordinate of the point.
       * @param depth depth of the cross-section in which the point was picked.
       * @param dir direction of the cross-section in which the point was picked.
       * @see Settings::CrossSection::CrossSectionDirections
       * @return void
       */
        void sendPointGuidedExtrusion( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );


       /**
       * \brief Signal emitted to update point to be used in the guided extrusion. Currently this method is bot being used.
       * @param px x coordinate of the point.
       * @param py y coordinate of the point.
       * @param depth depth of the cross-section in which the point was picked.
       * @param dir direction of the cross-section in which the point was picked.
       * @see Settings::CrossSection::CrossSectionDirections
       * @return void
       */
        void updatePointGuidedExtrusion( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );


       /**
       * \brief This signal is emitted to send the sketch done in the guided extrusion.
       * @param curve sketch done by the user for the guided extrusion.
       * @see PolyCurve
       * @return void
       */
        void sketchDoneGuidedExtrusion( const PolyCurve& curve_ );


    protected:


        /**
       * \brief Protected method responsibles for the interface creation. It calls the createToolbar and creates the fixed cross-sections container.
       * @return void
       */
        void createInterface();


        /**
       * \brief Protected method responsibles for the toolbar creation.
       * @return void
       */
        void createToolBar();


        /**
       * \brief Protected method responsibles for the actions and connects creation related to the toolbar.
       * @param canvas an instance of the SketchingCanvas. It is passed as parameter so that the scenes inside it can be connected by the signals
       * @see SketchingCanvas
       * @return void
       */
        void createToolbarActions( const SketchingCanvas* canvas_ );


        /**
       * \brief Protected method responsibles for the actions and connects creation related to the main cross-section scene.
       * @param canvas the main canvas, which hold the 'WIDTH' and the 'DEPTH' cross-sections. It is passed as parameter so that the scenes inside
       * it can be connected by the signals
       * @see SketchingCanvas
       * @return void
       */
        void createMainSceneActions( const SketchingCanvas* canvas_ );


        /**
       * \brief Protected method responsibles for the actions and connects creation related to the top view cross-section scene.
       * @param canvas the top view canvas, which hold the 'HEIGHT' cross-sections. It is passed as parameter so that the scenes inside it can be
       * connected by the signals
       * @see SketchingCanvas
       * @return void
       */
        void createTopViewSceneActions( const SketchingCanvas* canvas_ );


        /**
       * \brief Protected method responsibles for the actions and connects creation
       * related to the lateral bar.
       * @return void
       */
        void createLateralBarActions();


        /**
       * \brief Protected method responsibles for the actions and connects creation
       * related to the lateral bar.
       * @return void
       */
        void createFixedCrossSectionsActions();


        /**
       * \brief Protected method responsibles for the lateral bar creation. The lateral bar contains the dip angle and vertical exaggeration widgets.
       * @return void
       */
        void createLateralBar();


        /**
       * \brief Protected method to handle the keyboard events.
       * @param event Corresponds to the keyboard event.
       * @see QKeyEvent
       * @return void
       */
        void keyPressEvent( QKeyEvent *event );


    private:


        std::shared_ptr< SketchingCanvas > sketchingcanvas;         /**< Canvas to draw the sketches, exhibiting the main cross-section. It inherits from QGraphicsView. */

        std::shared_ptr< SketchingCanvas > topviewcanvas;           /**< Canvas to draw the sketches, exhibiting the top-view cross-section. It inherits from QGraphicsView. */

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
