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
 * @file slider.h
 * @author Clarissa C. Marques
 * @brief File containing the class Slider
 */



#ifndef SLIDER_H
#define SLIDER_H

#include <QtWidgets/QSlider>
#include <QString>
#include <QMouseEvent>

#include "./core/definitions/constants.hpp"

/**
 *  A customized QSlider that allows the user add markers in desired positions
 */

class Slider: public QSlider
{
    Q_OBJECT

    public:


        /**
        * Constructor. parent is passed to QSlider's constructor.
        * @param parent the parent item
        * @see QWidget
        */
        Slider( QWidget* parent = nullptr );


        /**
        * Method to return the number of markers existents on slider
        * @return void.
        */
        int getNumberofMarkers() const;


        /**
        * Method to get the mapping between the position of the marker and the value of the slider
        * @param markers map between the position and the QSliders values of the markers
        * @return void.
        */
        void getMarkers( std::map< int, int >& markers_ ) const;


    public slots:


        /**
        * Methods (slots) called when the user uses a right-click to add a marker in some slider position
        * @param value the position where the user wants to add a marker
        * @return void.
        */
        void addMarker( int value );


        /**
        * Methods (slots) called when the user uses a medium-button-click to remove a marker of some slider position
        * @param value the position where the user wants to remove a marker
        * @return void.
        */
        void removeMarker( int value );


        /**
        * Inline method called to remove all markers
        * @return void.
        */
        inline void clearMarkers(){  markers.clear(); color_markers.clear(); }


    signals:


        /**
        * Signal emmited when a marker is added in the slider
        * @param value the value where the marker was added
        * @param color the color of the marker
        * @return void.
        */
        void markValue( int value, QColor color_ );


        /**
        * Signal emmited when a marker is removed from the slider
        * @param value the value where the marker was removed
        * @return void.
        */
        void unmarkValue( int value );


        /**
        * Signal emmited when the slider is over a marker
        * @param value the value where the marker is placed
        * @return void.
        */
        void hightlightValue( int value );


    protected:


        /**
        * Method called when the mouse button is pressed over an existent marker
        * @param pos the position of the marker on the slider
        * @return void.
        */
        void hightlightMarker( int pos_ );


        /**
        * Method (slots) to return if a marker exists or not in a given position
        * @param pos a position in the slider to know if exists a marker
        * @return void.
        */
        bool isValidMarker( int value );


        /**
        * Method to update the markers positions after resizing the slider
        * @return void.
        */
        void updateMarkerPositions();


        /**
        * Method called to save a marker position
        * @param the integer value of the marker
        * @param slider_lenght the lenght of the slider
        * @param slider_min the minimum value of the slider range
        * @param slider_max the maximum value of the slider range
        * @return void.
        */
        void saveMarkerPosition( int value, int slider_length, int slider_min, int slider_max );


        /**
        * Method auxiliary to retrieve the slider properties
        * @param slider_lenght the lenght of the slider
        * @param slider_min the minimum value of the slider range
        * @param slider_max the maximum value of the slider range
        * @return void.
        */
        void getSubControlDimensions( int& slider_length, int& slider_min, int& slider_max ) const ;


        /**
        * Method to redraw the sliders.
        * This is an override method and was implemented to draw the markers
        * @param ev the event of repaint
        * @see QPaintEvent
        * @return void.
        */
        void paintEvent( QPaintEvent *ev );


        /**
        * Virtual method to handle the events when a mouse button is pressed.
        * @see QMouseEvent
        * @return void.
        */
        void mousePressEvent( QMouseEvent *event );


        /**
        * Virtual method to handle the events when the slider is resized
        * @see QResizeEvent
        * @return void.
        */
        void resizeEvent( QResizeEvent *event );


    protected:


        std::map< int, int > markers;                                       /**< map between the position of the marker and the value of the slider */

        std::map< int, QColor > color_markers;                              /**< map between the value of the slider and the colors of the markers */

};

#endif // SLIDER_H
