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


#ifndef SKETCHINGCANVAS_H
#define SKETCHINGCANVAS_H

#include <QtWidgets/QGraphicsView>
#include <QLabel>
#include <QWheelEvent>
#include <QFileDialog>
#include <QMouseEvent>

#include "MainWindow/Scene.h"

class SketchingCanvas: public QGraphicsView
{
	Q_OBJECT
	
	
	public:
	
        SketchingCanvas( QWidget* parent = 0 );
		void setup();



    protected:

        void setBackGround();

        void wheelEvent( QWheelEvent *event );


    public slots:

        void updateCoordinates( float posx, float posy );
        void screenshot();

        inline void insertSketch(){ scene->addCurve(); }
        inline void undoLastSketch(){ scene->undoLastSketch(); }
        inline void updateColor( const QColor& color ){ scene->updateColor( color ); }
        inline void addScene( Scene* const& sc ){ scene = sc; setScene( scene ); 
		connect(this->scene, &Scene::sendCoordinates, this, &SketchingCanvas::updateCoordinates);
		}



    signals:

        void newBoundary();





    public:

        QLabel* lb_coordinates;

		
	protected:
	
		Scene* scene;

	
	
	
};





#endif // SKETCHINGCANVAS_H
