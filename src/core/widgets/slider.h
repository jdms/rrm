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



#ifndef SLIDER_H
#define SLIDER_H

#include <QtWidgets/QSlider>
#include <QString>
#include <QMouseEvent>

#include "./core/definitions/constants.hpp"


class Slider: public QSlider
{
    Q_OBJECT

    public:

        Slider( QWidget* parent = 0 );

        int getNumberofMarkers() const;
        void getMarkers( std::map< int, int >& markers_ ) const;


    public slots:

        void addMarker( int value );
        void removeMarker( int value );


    signals:

        void markValue( int value, QColor color_ );
        void unmarkValue( int value );
        void hightlightValue( int value );



    protected:

        void hightlightMarker( int pos_ );
        bool isValidMarker( int value );
        void updateMarkerPositions();
        void saveMarkerPosition( int value, int slider_length, int slider_min, int slider_max );
        void getSubControlDimensions( int& slider_length, int& slider_min, int& slider_max ) const ;


        void paintEvent( QPaintEvent *ev );
        void mousePressEvent( QMouseEvent *event );
        void resizeEvent( QResizeEvent *event );




    protected:


        std::map< int, int > markers;
        std::map< int, QColor > color_markers;

};

#endif // SLIDER_H
