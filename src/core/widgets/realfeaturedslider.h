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



#ifndef REALFEATUREDSLIDER_H
#define REALFEATUREDSLIDER_H


#include <QSlider>
#include <QColor>

#include "./slider.h"

/**
 *  A customized Slider with double values.
 */


class RealFeaturedSlider: public Slider
{
    Q_OBJECT

    public:

        /**
        * Constructor.
        * @param o_ the slider orientation
        */
        RealFeaturedSlider( Qt::Orientation o_ );


        /**
        * Constructor.
        * @param o_ the slider orientation
        * @param disc_ the slider discretization
        */
        RealFeaturedSlider( Qt::Orientation o_, int disc_ );


        /**
        * Method to get the mapping between the double and the QSliders values of the markers
        * @param markers map between the double and the QSliders values of the markers
        * @return void.
        */
        void getDoubleMarkers( std::map< double, int >& markers_ );


        /**
        * Method to return the double maximum value of the slider
        * @return double double maximum value of the slider
        */
        double maximumValue() const;


        /**
        * Method to return the double minimum value of the slider
        * @return double double minimum value of the slider
        */
        double minimumValue() const;


    public slots:


        /**
        * Methods (slots) called when the user uses a right-click to add a marker in some slider position
        * @param value the position where the user wants to add a marker
        * @return void.
        */
        void addMarker( double value );


        /**
        * Methods (slots) called when the user uses a medium-button-click to remove a marker of some slider position
        * @param value the position where the user wants to remove a marker
        * @return void.
        */
        void removeMarker( double value );


        /**
        * Methods (slots) called when the range of the slider is changed
        * @param min_ minimum value for the slider range
        * @param max_ maximum value for the slider range
        * @return void.
        */
        void setRange( double min_, double max_ );


        /**
        * Methods (slots) called when the discretization of the slider is changed
        * @param disc_ new discretization of the slider
        * @return void.
        */
        void setDiscretization( int disc_ );


        /**
        * Methods (slots) called when the current value of the slider is changed
        * @param value_ current value of the slider in double
        * @return void.
        */
        void setValue( double value_ );


        /**
        * Methods (slots) called to return the double value of the slider equivalent to the integer given
        * @return double double value equivalent to the given integer
        */
        double getDoubleValue( int index_ ) const;


        /**
        * Methods (slots) called to return the current double value of the slider
        * @return double current double value of the slider
        */
        double getValue() const ;


        /**
        * Methods (slots) called to return the integer value of the slider equivalent to the given double
        * @return int the integer value of the slider equivalent to the given double
        */
        int getIndex( double value_ ) const ;


        /**
        * Methods (slots) called to increase the value of the double slider, i.e,  give a single step
        * @return Void
        */
        void increaseValue();


        /**
        * Methods (slots) called to decrease the value of the double slider, i.e,  return a single step
        * @return void.
        */
        void decreaseValue();


        /**
        * Methods (slots) called to reset the slider to default value, and remove all markers
        * @return void.
        */
        void clear();


    protected slots:


        /**
        * Methods (slots) called to move the slider position to the given value
        * @param index_ the integer value
        * @return void.
        */
        void moveInDouble( int index_ );



    signals:


        /**
        * Signal emmited when the double slider has changed its current value
        * @param value_ the new double value
        * @return void.
        */
        void sliderMoved( const double& value_ );


        /**
        * Signal emmited when a marker is added in the slider
        * @param value_ the double value where the marker was added
        * @param color_ the color of the marker
        * @see QColor
        * @return void.
        */
        void markValue( const double& value, QColor color_ );


        /**
        * Signal emmited when a marker is removed from the slider
        * @param value_ the double value where the marker was removed
        * @return void.
        */
        void unmarkValue( double value );


        /**
        * Signal emmited when the slider is over a marker
        * @param value the double value where the marker is placed
        * @return void.
        */
        void hightlightValue( double value );



    protected:


        /**
        * Method (slots) to update the step value when the range of the slider is changed
        * @return void.
        */
        void updateStep();


        /**
        * Method (slots) called when the mouse button is pressed over an existent marker
        * @param pos_ the position of the marker on the slider
        * @return void.
        */
        void hightlightMarker( int pos_ );


        /**
        * Method (slots) to return if a marker exists or not in a given position
        * @param pos_ a position in the slider to know if exists a marker
        * @return boolean returns true if there is a marker in this value, anf false otherwise
        */
        bool isValidMarker( int value );


        /**
        * Method (slots) to update the markers positions after resizing the slider
        * @return void.
        */
        void updateMarkerPositions();


        /**
        * Method (slots) called to save a marker position
        * @param the integer value of the marker
        * @param slider_lenght the lenght of the slider
        * @param slider_min the minimum value of the slider range
        * @param slider_max the maximum value of the slider range
        * @return void.
        */
        void saveMarkerPosition( int value, int slider_length, int slider_min, int slider_max );


        /**
        * Method (slots) auxiliary to retrieve the slider properties
        * @param slider_lenght the lenght of the slider
        * @param slider_min the minimum positions of the slider range
        * @param slider_max the maximum positions of the slider range
        * @return void.
        */
        void getSubControlDimensions( int& slider_length, int& slider_min, int& slider_max ) const ;


        /**
        * Method (slots) to reset the slider values to the default value, and clear all markers
        * @return void.
        */
        void setDefaultValues();



    protected:

        double maximum;                                                                     /**< The maximum value of the slider range */

        double minimum;                                                                     /**< The minimum value of the slider range */

        double step;                                                                        /**< The single step of the slider */

        int discretization;                                                                 /**< The slider discretization */


};

#endif // REALFEATUREDSLIDER_H
