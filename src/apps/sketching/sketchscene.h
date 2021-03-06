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
 * @file sketchscene.h
 * @author Clarissa C. Marques
 * @brief File containing the class SketchScene
 */


#ifndef SKETCHSCENE_H
#define SKETCHSCENE_H

#include <memory>


#include <QGraphicsScene>

#include "./models/inputsketch.h"
#include "./models/coordinateaxes_2d.h"
#include "./models/image_item_wrapper.h"
#include "./core/models/scene.h"


#include "./models/qtgeometries/curveitem.h"
#include "./models/volumeitem.h"
#include "./models/stratigraphyitem.h"
#include "./models/regionitem.h"
#include "./models/crosssectionitem.h"
#include "./models/dipanglepicture.h"


/**
 *   The 2d scene.
 *   This scene is responsible for viewing the elements present on a given cross-section.
 */

class SketchScene: public QGraphicsScene
{
    Q_OBJECT


     /**
     * \enum UserInteraction
     * \brief Enum to represent the set of interactions modes.
     */
    enum class UserInteraction { NONE, SKETCHING, RESIZING_BOUNDARY, RESIZING_IMAGE, CREATE_REGION, SELECTING_STRATIGRAPHY_OLD, SELECTING_STRATIGRAPHY, SELECTING_REGION, SELECTING_REGIONS, SELECTING_WELLS, GUIDED_EXTRUSION, TRAJECTORY_GUIDED };


    public:

       /**
       * Constructor.
       */
        SketchScene();


        /**
       * Destructor.
       */
        ~SketchScene();


       /**
       * Method to initialize the scene.
       * This method creates and adds all the auxiliary items to the scene, for example, the input sketch and items to handle images.
       * @return void.
       */
        void init();


       /**
       * Method to define cross-section information.
       * This method sets the direction and depth of the cross-section to be viewed in the scene. It is used to allow or restrict some interactions.
       * @param dir direction of the cross-section to be viewed in this scene.
       * @param depth depth of the cross-section to be viewed in this scene.
       * @see Settings::CrossSection::CrossSectionDirections
       * @return void.
       */
        void setCrossSectionInformation( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


       /**
       * Method to get the cross-section information.
       * @param dir reference to the direction of the cross-section to be viewed in this scene.
       * @param depth reference to the depth of the cross-section to be viewed in this scene.
       * @see Settings::CrossSection::CrossSectionDirections
       * @return void.
       */
        void getCrossSectionInformation( Settings::CrossSection::CrossSectionDirections& dir_, double& depth_ );


        /**
        * Method to create a volume in the scene.
        * This volume will hold a const shared pointer to a volume data structure, so that it can retrieve the required information
        * to draw it on scene.
        * @param volume data structure that contains the geometry and others required information to draw it on scene.
        * @see Volume
        * @return void.
        */
        void createVolume( const std::shared_ptr< Volume >& volume_ );

        /**
        * Method to update a volume in the scene.
        * This update can be since updating the dimensions as its visibility.
        * @return void.
        */
        void updateVolume();


        /**
        * Method to add cross-section in the scene.
        * This cross-section will hold a const shared pointer to a cross-section data structure, so that it can retrieve the required information
        * to draw it on scene.
        * @param csection data structure that contains the geometry and others required information to draw it on scene.
        * @see CrossSection
        * @return void.
        */
        void addCrossSection( const std::shared_ptr< CrossSection >& csection_ );

        /**
        * Method to update a cross-section in the scene.
        * This update can be since updating the dimensions to its visibility.
        * @param dir direction which cross-section is done.
        * @param depth depth of the cross-section in that direction.
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void updateCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Method to remove a cross-section in the scene.
        * @param dir direction which cross-section is done.
        * @param depth depth of the cross-section in that direction.
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void removeCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Method to update all cross-sections in the scene.
        * @return void.
        */
        void updateCrossSections();


        /**
        * Method to add stratigraphies and structurals in the scene.
        * This stratigraphy will hold a const shared pointer to a stratigraphy data structure, so that it can retrieve the required information
        * to draw it on scene. Note that currently a structural has the same data structure as the stratigraphy.
        * @param strat data structure that contains the geometry and others required information to draw it on scene.
        * @see Stratigraphy
        * @return void.
        */
        void addStratigraphy( const std::shared_ptr< Stratigraphy >& strat_ );


        /**
        * Method to update the stratigraphy or structural, which index is id_.
        * This update can be since updating the dimensions, color to its visibility.
        * @param id index of the stratigraphy to be updated.
        * @return void.
        */
        void updateStratigraphy( const std::size_t& id_ );


        /**
        * Method to update all stratigraphies and structurals in the scene.
        * @return void.
        */
        void updateStratigraphies();


        /**
        * Method to update all trajectories from stratigraphies or structurals in the scene.
        * @return void.
        */
        void updateStratigraphiesTrajectories();


        /**
        * Method to get all the selected stratigraphies in the scene, mainly when using the actions to preserve above/below and region.
        * @return void.
        */
        void getSelectedStratigraphies();

        /**
        * Method to add regions in the scene.
        * This region will hold a const shared pointer to a region data structure, so that it can retrieve the required information
        * to draw it on scene.
        * @param region data structure that contains the geometry and others required information to draw it on scene.
        * @see Regions
        * @return void.
        */
        void addRegion( const std::shared_ptr< Regions >& region_ );


        /**
        * Method to update the region which index is id_.
        * This update can be since updating the dimensions, color to its visibility.
        * @param id index of the region to be updated.
        * @return void.
        */
        void updateRegion( const std::size_t& id_ );


        /**
        * Method to update all regions in the scene.
        * @return void.
        */
        void updateRegions();


        /**
        * Method to remove all regions in the scene.
        * @return void.
        */
        void clearRegions();


        /**
        * Method to get all the selected regions in the scene, mainly when using the action to preserve region.
        * @return void.
        */
        void getSelectedRegions();


        /**
        * Method to enable or disable the action of sketching.
        * @param status boolean to indicate if the sketch should be enabled or disabled. Its value is true if the sketch should be enabled
        * and false otherwise
        * @return void.
        */
        void enableSketch( bool status_ );


        /**
        * Method to check if action of sketching is enabled/disabled.
        * @return bool return true if the sketch is enabled and false if not.
        */
        bool isSketchEnabled() const;


        /**
        * Method to remove and delete all objects from the scene.
        * @return void.
        */
        void clearScene();


    public slots:


        /**
        * Method(slot) to cancel the sketch the user has done before submit it.
        * This method only erases the curve inside of the input sketch.
        * @return void.
        */
        void cancelSketch();


        /**
        * Method(slot) to submit the sketch the user has done as a curve of the current object.
        * @return void.
        */
        void submitSketch();


        /**
        * Method(slot) to change the current color of the input sketch to color_.
        * @param color color to be the current one in the input sketch.
        * @see QColor
        * @return void.
        */
        void setSketchColor( const QColor& color_ );


//        inline void setCurrentColor( const QColor& color_ )
//        {
//            std::cout << "Changing sketch color..." << std::endl << std::flush;

//            if( sketch == nullptr ) return;
//            sketch->setColor( color_ );
//            QGraphicsScene::update();
//        }


        /**
        * Method(slot) to notify the controller to create the surface with the curves stored in the current object.
        * @return void.
        */
        void endObject();


        /**
        * Method(slot) to set the interaction mode 'sketching' to active or desactivated.
        * @param status boolean to represent the status of the 'sketching' mode. The value is true if it is active or, false otherwise.
        * If the value is false the interaction mode changes to 'NONE'.
        * @param UserInteraction
        * @return void.
        */
        void setSketchingMode( bool status_ = true );


        /**
        * Method(slot) to set the interaction mode 'resizing boundary' to active or desactivated.
        * @param status boolean to represent the status of the 'resizing boundary' mode. The value is true if it is active or, false otherwise.
        * If the value is false the interaction mode changes to 'SKETCHING'.
        * @param UserInteraction
        * @return void.
        */
        void setResizingBoundaryMode( bool status_ );


        /**
        * Method(slot) to set the interaction mode 'old stratigraphy selection' to active or desactivated.
        * This method is used when the preserve above and below is active and the user select stratigraphies by picking the curves to be the new boundary.
        * @param status boolean to represent the status of the 'old stratigraphy selection' mode. The value is true if it is active or, false otherwise.
        * If the value is false the interaction mode changes to 'SKETCHING'.
        * @param UserInteraction
        * @return void.
        */
        void setOldSelectingStratigraphyMode( bool status_ );


        /**
        * Method(slot) to set the interaction mode 'new stratigraphy selection' to active or desactivated.
        * This method is used when the preserve above and below is active and the user select stratigraphies by making sketches to mark the curves to be the new boundary.
        * @param status boolean to represent the status of the 'new stratigraphy selection' mode. The value is true if it is active or, false otherwise.
        * If the value is false the interaction mode changes to 'SKETCHING'.
        * @param UserInteraction
        * @return void.
        */
        void setSelectingStratigraphyMode( bool status_ );


        /**
        * Method(slot) to set the interaction mode 'region selection' to active or inactive.
        * This method is not being used properly.
        * @param status boolean to represent the status of the 'region selection' mode. The value is true if it is active or, false otherwise.
        * If the value is false the interaction mode changes to 'SKETCHING'.
        * @param UserInteraction
        * @return void.
        */
        void setSelectingRegionsMode( bool status_ );


        /**
        * Method(slot) to set the interaction mode 'resize image' to active or desactivated.
        * @param status boolean to represent the status of the 'resize image' mode. The value is true if it is active or, false otherwise.
        * If the value is false the interaction mode changes to 'SKETCHING'.
        * @param UserInteraction
        * @return void.
        */
        void setResizingImageMode( bool status_ );


        /**
        * Method(slot) to set the interaction mode 'selecting region' to activate or desactivated.
        * This method is used when the preserve region is active and the user select regions choosing a point to know which regions it belongs to.
        * @param status boolean to represent the status of the 'selecting region' mode. The value is true if it is active or, false otherwise.
        * If the value is false the interaction mode changes to 'SKETCHING'.
        * @param UserInteraction
        * @return void.
        */
        void setSelectingRegionMode( bool status_ );


        /**
        * Method(slot) to add images to the cross-section associated to this scene.
        * This method emit a signal in order the controller save the image in the apropriate cross-section.
        * @param file path to the image file.
        * @see QString
        * @return void.
        */
        void addImageToCrossSection( const QString& file_ );


        /**
        * Method(slot) to show the image to the cross-section associated to this scene.
        * @param file_ path to the image file.
        * @param ox x coordinate of the origin of the image.
        * @param oy y coordinate of the origin of the image.
        * @param w width of the image.
        * @param h height of the image.
        * @return void.
        */
        void setImageInCrossSection( const std::string& file_, double ox_, double oy_, double w_, double y_ );


        /**
        * Method(slot) to remove the image from the cross-section associated to this scene.
        * This method does not send a signal to update the controller. For this, please check the method 'removeImageInCrossSectionAndUpdate';
        * @see removeImageInCrossSectionAndUpdate()
        * @return void.
        */
        void removeImageInCrossSection();


        /**
        * Method(slot) to remove the image from the cross-section associated to this scene and update the cross-section in the controller.
        * @return void.
        */
        void removeImageInCrossSectionAndUpdate();


        /**
        * Method(slot) auxiliary to define the a region given a lower and/or upper boundary.
        * @return void.
        */
        void defineBoundaryArea();


        /**
        * Method(slot) to set a curve as the lower boundary and calls the method 'defineBoundaryArea' to define a region.
        * @param boundary curve selected to be the lower boundary.
        * @see defineBoundaryArea()
        * @see PolyCurve
        * @return void.
        */
        void defineLowerBoundaryCurve( const PolyCurve& boundary_ );


        /**
        * Method(slot)to set a curve as the upper boundary and calls the method 'defineBoundaryArea' to define a region.
        * @param boundary curve selected to be the upper boundary.
        * @see defineBoundaryArea()
        * @see PolyCurve
        * @return void.
        */
        void defineUpperBoundaryCurve( const PolyCurve& boundary_ );


        /**
        * Method(slot) to remove any lower boundary.
        * @return void.
        */
        void clearLowerBoundaryCurve();


        /**
        * Method(slot) to remove any upper boundary.
        * @return void.
        */
        void clearUpperBoundaryCurve();


        /**
        * Method(slot) to update the point used to guided extrusion.
        * @param p new point to the guided extrusion
        * @return void.
        */
        void updatePointGuidedExtrusion( const QPointF& p_ );


        /**
        * Method(slot) to stop the guided extrusion.
        * @return void.
        */
        void stopPointGuidedExtrusion();


        /**
        * Method(slot) to submit the sketch done to be the trajectory in the guided extrusion.
        * @return void.
        */
        void submitSketchGuidedExtrusion();



        /**
        * Method(slot) to show or hide the output dip angle picture.
        * @param status a boolean to define if the output dip angle should be shown or not.
        * If status is true the picture will be shown, and hidding if the value is false
        * @param pix a new output dip angle picture
        * @see QPixmap
        * @return void.
        */
        void showDipAnglePicture( bool status_, const QPixmap& pix_ );


        /**
        * Method(slot) to update the output dip angle picture for a new one.
        * @param pix a new output dip angle picture
        * If status is true the picture will be shown, and hidding if the value is false
        * @see QPixmap
        * @return void.
        */
        void updateDipAnglePicture( const QPixmap& pix_ );


        /**
        * Method(slot) to turn the output dip angle picture movable or not.
        * @param status boolean to set if the picture will be movable or not.
        * It is true if the picture shoudl be movable, and false otherwise.
        * @return void.
        */
        void setDipAnglePictureMovable( bool status_ );


        /**
        * Method(slot) to set the axes visible or not.
        * @param status boolean to set if axes picture will be visible or not.
        * It is true if the axes is visible, and false otherwise.
        * @return void.
        */
        void setAxesVisible( bool status_ );


        /**
        * Method(slot) to save the screenshot as a raster image.
        * @param filename name of the image file to be saved
        * @see QString
        * @return void.
        */
        void savetoRasterImage( const QString& filename );


        /**
        * Method(slot) to save the screenshot as a vector image.
        * @param filename name of the image file to be saved
        * @see QString
        * @return void.
        */
        void savetoVectorImage( const QString& filename );


        /**
        * Inline method(slot) to invert or not the current image in the cross-section.
        * @param status boolean to indicate if the image should be inverted or not.
        * It is true if the image should be inverted, and false otherwise.
        * @return void.
        */
        inline void invertImage( bool status_ ) {
            if(status_ == true ) factor = -1;
            else factor = 1;
        }


        /**
        * Inline method(slot) to add or remove the axes from the scene.
        * @param status boolean to indicate if the axes should be removed or added in the scene.
        * It is true if the axes should be added, and false otherwise.
        * @return void.
        */
        inline void axesInScene( bool status_ )
        {
            if( status_ == false )
                removeItem( &axes );
            else
                addItem( &axes );
            update();
        }


        /**
        * Method(slot) to update the axes dimensions.
        * @return void.
        */
        void updateAxes();


        /**
        * Method(slot) to undo the vertical exaggeration in the axes.
        * This method was implement as part of the solution to undo the undesired deformation in the axes resulting from the vertical exaggeration.
        * @param matrix the canvas matrix where the vertical exaggeration was applied. It will be used to revert the transformation on the axes.
        * @param scale the value of the vertical exaggeration. It will be used to apply the scale on the axes in a different way.
        * @return void.
        */
        void revertVerticalExaggerationInAxes( QMatrix matrix_, double scale_ );


        /**
        * Method(slot) to reset the vertical exaggeration in the scene.
        * @return void.
        */
        void resetVerticalExaggerationInAxes();


        /**
        * Method(slot) to set the interaction mode 'guided extrusion' to activate or desactivated.
        * This method is not being used properly.
        * @param status boolean to represent the status of the 'guided extrusion' mode. The value is true if it is active or, false otherwise.
        * If the value is false the interaction mode changes to 'SKETCHING'.
        * @return void.
        */
        void setGuidedExtrusionMode( bool status_ );


   signals:


        /**
        * Signal to notify the others parts of the system that the volume dimension was changed.
        * @param dir direction of the current cross-section in which the dimensions change was done.
        * @param width new width dimension. Note that, depending on which cross-section was made the change, this value can be the volume width or the depth.
        * @param height new height dimension. Note that, depending on which cross-section was made the change, this value can be the volume width or the depth.
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void resizeVolumeDimensions( const Settings::CrossSection::CrossSectionDirections& dir_, double width_, double height_ );


        /**
        * Signal to notify the view to adjust the focus so that all elements on the scene should appear.
        * @return void.
        */
        void ensureObjectsVisibility();


        /**
        * Signal to send the sketch as a curve of the current object.
        * @param curve sketch done by the user to be a curve of the current object.
        * @param dir direction of the cross-section which was done the sketch
        * @param depth depth of the cross-section which was done the sketch
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void sketchDone( const PolyCurve& curve_,
                         const Settings::CrossSection::CrossSectionDirections& dir_ = Settings::CrossSection::CrossSectionDirections::Y, double depth_ = 0 );


        /**
        * Signal to send to controller to create the surface from the curves stored on the current object.
        * @return void.
        */
        void createObject();

        /**
        * Signal emmited to controller saves the image data in the cross-section associated to the scene.
        * @param string path to the image file.
        * @param CrossSectionDirections cross-section direction of the current scene.
        * @param double cross-section depth of the current scene.
        * @param double x coordinate of the origin of the image.
        * @param double y coordinate of the origin of the image.
        * @param double width of the image.
        * @param double height of the image.
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void setImageToCrossSection( const std::string&, const Settings::CrossSection::CrossSectionDirections&, double, double, double, double, double );


        /**
        * Signal emmited to controller removes the image data in the cross-section associated to the scene.
        * @param CrossSectionDirections cross-section direction of the current scene.
        * @param double cross-section depth of the current scene.
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void removeImageFromCrossSection(  const Settings::CrossSection::CrossSectionDirections&, double );


        /**
        * Signal emmited to specifies which object was selected.
        * @param id index of the selected object
        * @return void.
        */
        void objectSelected( const std::size_t& id_ );


        /**
        * Signal to send the sketch made as a selection marker.
        * This signal is used when the new method to select stratigraphies and structurals is enabled.
        * @param curve the sketch done to select a stratigraphy/structural
        * @param dir direction of the cross-section associated to the current scene.
        * @param depth depth of the cross-section associated to the current scene.
        * @see PolyCurve
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void sendSketchOfSelection( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Signal to send the controller to stop the process of the stratigraphies and structural selection.
        * @return void.
        */
        void stopSketchesOfSelection();


        /**
        * Signal emitted when a point in a region was selected.
        * @param px x coordinate of the selected point
        * @param py y coordinate of the selected point
        * @param depth depth of the cross-section associated to the current scene
        * @param dir direction of the cross-section associated to the current scene
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void getRegionByPoint( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * Signal emitted when a region, which index is id_, is selected or deselected.
        * This method is not being used properly.
        * @param id index of the selected region.
        * @param status status of selection of the region. It is true if it is selected and false otherwise.
        * @return void.
        */
        void regionSelected( const std::size_t& id_, bool status_ );


        /**
        * Signal emitted to controller remove the last curve inserted in the current object.
        * This method is not being used properly.
        * @param dir direction of the cross-section associated to this current scene.
        * @param depth depth of the cross-section associated to this current scene.
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void removeLastCurve( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Signal emitted to send the point to be used in the guided extrusion.
        * This method is not being used properly.
        * @param px x coordinate of the point selected
        * @param py y coordinate of the point selected
        * @param depth depth of the cross-section associated to this current scene.
        * @param dir direction of the cross-section associated to this current scene.
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void sendPointGuidedExtrusion( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * Signal emitted to send the sketch as the trajectory in guided extrusion process.
        * This method is not being used properly.
        * @param curve the curve used as the trajectory to the guided extrusion
        * @see PolyCurve
        * @return void.
        */
        void sketchDoneGuidedExtrusion( const PolyCurve& curve_ );


    protected:


        /**
        * Method to process the sketch curve to be smoothed and monotonic.
        * @return void.
        */
        void processSketch();


        /**
        * Method to add a selection marker in the scene. This method emit the signal so send the marker to controller, see the signal 'sendPointGuidedExtrusion'.
        * @see sendPointGuidedExtrusion()
        * @return void.
        */
        void addToSketchesOfSelection();


        /**
        * Method to remove all the selection markers from the scene.
        * @return void.
        */
        void removeSketchesOfSelection();


        /**
        * Method to update the image and its dimensions in the scene.
        * @return void.
        */
        void updateImageinCrossSection();


        /**
        * Virtual method to handle the events when a mouse button is pressed.
        * @param event captures the mouse event to identify which operation was performed and if and which button is pressed
        * @see QGraphicsSceneMouseEvent
        * @return void.
        */
        virtual void mousePressEvent( QGraphicsSceneMouseEvent *event_ ) ;


        /**
        * Virtual method to handle the events when a mouse button is double clicked.
        * @param event captures the mouse event to identify which operation was performed and if and which button is pressed
        * @see QGraphicsSceneMouseEvent
        * @return void.
        */
        virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event_ );


        /**
        * Virtual method to handle the events when the mouse is moved.
        * @param event captures the mouse event to identify which operation was performed and if and which button is pressed
        * @see QGraphicsSceneMouseEvent
        * @return void.
        */
        virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent* event_ );


        /**
        * Virtual method to handle the events when a mouse button is released.
        * @param event captures the mouse event to identify which operation was performed and if and which button is pressed
        * @see QGraphicsSceneMouseEvent
        * @return void.
        */
        virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* event_ );


        /**
        * Virtual method to handle when the mouse enter on the scene dragging something.
        * @param event captures the mouse event to identify which operation was performed and if and which button is pressed
        * @see QGraphicsSceneDragDropEvent
        * @return void.
        */
        virtual void dragEnterEvent( QGraphicsSceneDragDropEvent* event_ );


        /**
        * Virtual method to handle when the mouse is over the scene and drop something.
        * @param event captures the mouse event to identify which operation was performed and if and which button is pressed
        * @see QGraphicsSceneDragDropEvent
        * @return void.
        */
        virtual void dropEvent( QGraphicsSceneDragDropEvent* event_ );


        /**
        * Virtual method to handle when the mouse is moving over the scene dragging something.
        * @param event captures the mouse event to identify which operation was performed and if and which button is pressed
        * @see QGraphicsSceneDragDropEvent
        * @return void.
        */
        virtual void dragMoveEvent( QGraphicsSceneDragDropEvent* event_ );


        /**
        * Virtual method to handle the wheel movement of the mouse.
        * It is tipically used when a zoom is performed.
        * @param event captures the mouse event to identify which operation was performed and how much and which direction the
        * wheel was moved
        * @see QGraphicsSceneWheelEvent
        * @return void.
        */
        virtual void wheelEvent( QGraphicsSceneWheelEvent *event_ );


    protected:



        bool sketch_enabled = true;                                                     /**< Boolean flag to indicates if sketch is enabled or disabled */

        std::shared_ptr< CurveItem > sketch = nullptr;                                  /**< Object used to draw the input sketches */

        double csection_depth = 0.0;                                                    /**< Double to represent the depth of the croos-section associated to this scene. */

        Settings::CrossSection::CrossSectionDirections csection_direction =             /**< CrossSectionDirection to represent the direction of the croos-section associated to this scene. */
                Settings::CrossSection::DEFAULT_CSECTION_DIRECTION;

        std::shared_ptr< VolumeItem > volume1 = nullptr;                                /**< Wrapper instance used to graphically represent the volume in the 2d scene. */

        std::map< std::size_t, std::shared_ptr< CrossSectionItem > > cross_sections;   /**< A map of wrapper instances used to graphically represent the cross-sections in the 2d scene.
                                                                                             The first map value corresponds the index of the cross-section. */

        std::map< std::size_t, std::shared_ptr< StratigraphyItem > > stratigraphies;    /**< A map of wrapper instances used to graphically represent the stratigraphies and structurals in the 2d scene.
                                                                                             The first map values corresponds to the index of the stratigraphy/structural */

        std::map< std::size_t, std::shared_ptr< RegionItem > > regions;                 /**< A map of wrapper instances used to graphically represent the regions in the 2d scene.
                                                                                             The first map values corresponds to the index of the region */

        ImageItemWrapper* image = nullptr;                                              /**< Wrapper instance used to handle the image and perform operations on it. */

        DipAnglePicture* dipangle = nullptr;                                            /**< Wrapper instance used to store the output dip angle picture. */

        std::vector< std::shared_ptr< CurveItem > > sketches_of_selection;              /**< A vector to store the selection markers used in the new method of stratigraphies and structurals selection */

        UserInteraction current_interaction = UserInteraction::SKETCHING;               /**< A variable corresponding to the current mode of user interaction. See the enum 'UserInteraction'. */

        QGraphicsEllipseItem* resize_marker = nullptr;                                  /**< An auxiliary instance of a QGraphicsEllipseItem to be a marker where the user should drag to scale the image. */

        QGraphicsEllipseItem* move_marker = nullptr;                                    /**< An auxiliary instance of a QGraphicsEllipseItem to be a marker where the user should drag to move the image. */

        QGraphicsEllipseItem* trajectory_point = nullptr;                               /**< An auxiliary instance of a QGraphicsEllipseItem to be a marker where the user should drag to position the guided extrusion point.
                                                                                             It is not being used properly. */

        PolygonItem* boundary_area = nullptr;                                          /**< An auxiliary instance of a PolygonItem to be a colorful region when using preserve above/below/region. */

        PolyCurve lower;                                                                /**< An auxiliary instance of a PolyCurve to represent the lower boundary curve. */

        PolyCurve upper;                                                                /**< An auxiliary instance of a PolyCurve to represent the upper boundary curve. */

        CoordinateAxes2d axes;                                                          /**< Wrapper instance used to represent the axes in the 2d scene.  */

        int factor = 1;                                                                 /**< An auxiliary integer used to invert or not the image on the scene.  */
};

#endif // SKETCHSCENE_H
