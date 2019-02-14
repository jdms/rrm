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
 * @file sketchingcanvas.cpp
 * @author Clarissa C. Marques
 * @date 9 Sep 2012
 * @brief File containing the class SketchingCanvas
 */

#include "sketchingcanvas.h"

SketchingCanvas::SketchingCanvas( QWidget *parent_ ): QGraphicsView( parent_ )
{
    setInteractive( true );
    setRenderHint( QPainter::Antialiasing );
    setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOn );
    setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOn );
    setSizeAdjustPolicy( QAbstractScrollArea::AdjustToContents );
    setupScene();
}


void SketchingCanvas::setupScene()
{
    scene = std::make_shared< SketchScene >();
    setScene( scene.get() );

    connect( scene.get(), &SketchScene::ensureObjectsVisibility, [=]()
    {
        QRectF rect_ = scene->itemsBoundingRect();
        // the bounding box rectangle that contains all elements of the scene plus an additional margin
        scene->setSceneRect( rect_.x() - 50, rect_.y() - 50, rect_.width() + 100, rect_.height() + 100 );
        scene->update();

        // be sure that all the rectangle fits in the view
        ensureVisible( scene->sceneRect() );

    } );

}


const std::shared_ptr< SketchScene >& SketchingCanvas::getScene() const
{
    return scene;
}



void SketchingCanvas::setVerticalExaggeration( double scale_ )
{
    v_exag = scale_;

    QMatrix matrix_;

    // an unitary matrix scaled by the vertical exaggeration scale. The minus signal is necessary to keep the
    // objects in the right orientation, not upside down
    matrix_.scale( 1.0, -1*scale_ );
    setMatrix( matrix_ );

    // this method applies the vertical exaggeration in a specific way to the axes.
    scene->revertVerticalExaggerationInAxes( matrix_, scale_ );
}


void SketchingCanvas::stopVerticalExaggeration()
{
    // resetting the vertical exaggeration applying the inverse matrix
    QMatrix matrix_ = matrix().inverted().scale( 1, -1 );
    setMatrix( matrix_, true);

    // resetting the vertical exaggeration in the axes
    scene->resetVerticalExaggerationInAxes();

}


double SketchingCanvas::getVerticalExaggeration() const
{
   return v_exag;
}



SketchingCanvas::~SketchingCanvas()
{

//    scene.reset();
//    scene = nullptr;
}
