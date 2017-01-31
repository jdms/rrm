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

#include "SketchingCanvas.h"

SketchingCanvas::SketchingCanvas( QWidget* parent ): QGraphicsView( parent )
{
    setup();
}


void SketchingCanvas::setup()
{


    setRenderHint( QPainter::Antialiasing, true );
    setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
    setViewportUpdateMode( QGraphicsView::FullViewportUpdate );


    setBackGround();
    scale( 1, -1 );

    lb_coordinates = new QLabel();

}






void SketchingCanvas::setBackGround()
{

    int step = 20;

    QRadialGradient gradient( (qreal)width()*0.5f, (qreal)height()*0.5f, width() );

    for ( qreal radius = 0.0f; radius <= 1.0f; radius += 0.1f )
    {
        gradient.setColorAt( radius, QColor::fromRgb( 154.28f + ( radius*step ), 176.20f + ( radius*step ), 199.16f + ( radius*step ) ) );
    }

    setBackgroundBrush( QBrush( gradient ) );

}


void SketchingCanvas::updateCoordinates( float posx, float posy )
{
    lb_coordinates->setText( QString( "%1, %2" ).arg( posx ).arg( posy ) );

}


void SketchingCanvas::screenshot()
{

    QString selectedFilter;
    QString name_of_file = QFileDialog::getSaveFileName( this, tr( "Save Image" ), "./screenshots/", tr( "PNG (*.png);;SVG (*.svg)" ), &selectedFilter );


    if( selectedFilter == "PNG (*.png)" )
    {
        scene->savetoRasterImage( name_of_file );
    }
    else if ( selectedFilter == "SVG (*.svg)" )
    {
        scene->savetoVectorImage( name_of_file );
    }

}



void SketchingCanvas::wheelEvent( QWheelEvent *event )
{

    // Scale the view
    double scaleFactor = 1.5;

    if( event->delta() > 0 )
    {
        scale( scaleFactor, scaleFactor );
    }

    else
    {
        scale( 1.0/scaleFactor, 1.0/scaleFactor );
    }

}
