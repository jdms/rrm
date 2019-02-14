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
 * @file pages_stack.h
 * @author Clarissa C. Marques
 * @date 9 Sep 2012
 * @brief File containing the class PagesStack
 */


#ifndef PAGESSTACK_H
#define PAGESSTACK_H

#include <QStackedWidget>

#include "./core/definitions/constants.hpp"

#include "ui_volume_resize_widget.h"
#include "ui_object_properties.h"

/**
 *  A customized QStackedWidget to hold a volume resizing, objects log, and mesh resolution widgets
 */

class PagesStack: public QStackedWidget
{
    Q_OBJECT

    public:

        /**
        * Constructor.
        */
        PagesStack();


        /**
        * Method to create and define the actions and connects of the volume resizing widget
        * @return void.
        */
        void createVolumeActions();


        /**
        * Method to reset the options for all widgets
        * @return void.
        */
        void clear();


        /**
        * Method to return the current value in the width slider
        * @return double current value of the width slider
        */
        double getWidth();


        /**
        * Method to return the current value in the height slider
        * @return double current value of the height slider
        */
        double getHeight();


        /**
        * Method to return the current value in the lenght slider
        * @return double current value of the lenght slider
        */
        double getLenght();


    public slots:


        /**
        * Method (slots) called when the volume dimensions has changed and the sliders need to be updated
        * @param width new width of the volume
        * @param height new height of the volume
        * @param depth new depth of the volume
        * @return void.
        */
        void changeVolumeSize( double width, double height, double depth );


        /**
        * Method (slots) called when the range of the volume dimensions has changed and the sliders need to be updated
        * @param width new width maximum of the volume
        * @param height new height maximum of the volume
        * @param depth new depth maximum of the volume
        * @return void.
        */
        void changeRangeSize( double width, double height, double depth );


        /**
        * Method (slots) called when the volume resizing widget needs to be enabled/disabled
        * @param status if status value is true, the volume resizing should be enabled and disabled otherwise
        * @return void.
        */
        void setEnabledVolumeResize( bool status );


        /**
        * Method (slots) called when the volume width has changed and the width slider need to be updated
        * @param w new width of the volume
        * @return void.
        */

        void setVolumeWidth( double w );


        /**
        * Method (slots) called when the volume height has changed and the height slider need to be updated
        * @param h new height of the volume
        * @return void.
        */
        void setVolumeHeight( double h );


        /**
        * Method (slots) called when the volume depth has changed and the depth slider need to be updated
        * @param d new depth of the volume
        * @return void.
        */
        void setVolumeDepth( double d );


        /**
        * Method (slots) called to change the current page to show other widgets
        * @param type_ the type of object which was selected
        * @see Settings::Objects::ObjectType
        * @return void.
        */
        void selectObjectPage( const Settings::Objects::ObjectType& type_ );


        /**
        * Method (slots) called to update the object log widget with the name and the log of the stratigraphy/structural.
        * @param name_ the name of the stratigraphy/structural which was selected
        * @param text_ the log of the stratigraphy/structural
        * @return void.
        */
        void loadObjectInformation( const std::string& name_, const std::string& text_ );


        /**
        * Method (slots) called to set the current resolution as low
        * @return void.
        */
        void checkLowResolution();


        /**
        * Method (slots) called to set the current resolution as medium
        * @return void.
        */
        void checkMediumResolution();


        /**
        * Method (slots) called to set the current resolution as high
        * @return void.
        */
        void checkHighResolution();


    signals:


        /**
        * Signal sent to notify that the width volume was changed
        * @param value the new volume width
        * @return void.
        */
        void widthVolumeChanged( int value );


        /**
        * Signal sent to notify that the height volume was changed
        * @param value the new volume height
        * @return void.
        */
        void heightVolumeChanged( int value );


        /**
        * Signal sent to notify that the depth volume was changed
        * @param value the new volume depth
        * @return void.
        */
        void depthVolumeChanged( int value );


        /**
        * Signal sent to notify that the log information of the selected stratigraphy/structural was changed and need to be save
        * @param text the log information of the selected stratigraphy/structural
        * @see QString
        * @return void.
        */
        void saveText( const QString& text_ );


        /**
        * Signal sent to notify that the current resolution is low
        * @return void.
        */
        void setLowResolution();


        /**
        * Signal sent to notify that the current resolution is medium
        * @return void.
        */
        void setMediumResolution();


        /**
        * Signal sent to notify that the current resolution is high
        * @return void.
        */
        void setHighResolution();


    protected:


        /**
        * Method to create the interface of the volume resizing widget
        * @return void.
        */
        void createVolumePropertiesPage();


        /**
        * Method to create the interface of the object log information widget
        * @return void.
        */
        void createObjectPropertiesPage();


        /**
        * Method to create connects related to the mesh resolution widget
        * @return void.
        */
        void createResolutionActions();


    private:


        Ui::fm_volume wd_volume_resize;                                     /**< the volume resizing widget  */

        Ui::Form wd_object_properties;                                      /**< the object log information widget  */

        const int MAX_WIDTH = 10000;                                        /**< the maximum value default to the width range */

        const int MAX_HEIGHT = 10000;                                       /**< the maximum value default to the height range */

        const int MAX_DEPTH = 10000;                                        /**< the maximum value default to the depth range */

        const int SINGLE_STEP = 10;                                         /**< the default value of the single step for the sliders */

        const bool VOLUME_RESIZING = true;                                  /**< a boolean to represent if the resizing volume is enabled or disabled*/

};

#endif // PAGESSTACK_H
