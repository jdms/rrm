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


class RealFeaturedSlider: public Slider
{
    Q_OBJECT

    public:

        RealFeaturedSlider( Qt::Orientation o_ );
        RealFeaturedSlider( Qt::Orientation o_, int disc_ );

        void getDoubleMarkers( std::map< double, int >& markers_ );
        double maximumValue() const;
        double minimumValue() const;


    signals:

        void sliderMoved( const double& value_ );
        void markValue( const double& value, QColor color_ );
        void unmarkValue( double value );
        void hightlightValue( double value );


    public slots:


        void addMarker( double value );
        void removeMarker( double value );


        void setRange( double min_, double max_ );
        void setDiscretization( int disc_ );

        void setValue( double value_ );
        double getDoubleValue( int index_ ) const;
        double getValue() const ;


        int getIndex( double value_ ) const ;

        void increaseValue();
        void decreaseValue();

        void clear();


    protected slots:

        void moveInDouble( int index_ );



    protected:

        void updateStep();
        void hightlightMarker( int pos_ );
        bool isValidMarker( int value );
        void updateMarkerPositions();
        void saveMarkerPosition( int value, int slider_length, int slider_min, int slider_max );
        void getSubControlDimensions( int& slider_length, int& slider_min, int& slider_max ) const ;
        void setDefaultValues();



    protected:

        double maximum;
        double minimum;
        double step;
        int discretization;




};

#endif // REALFEATUREDSLIDER_H
