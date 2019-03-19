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
 * @file curveitem.cpp
 * @author Clarissa C. Marques
 * @brief File containing the classes CurveItem and SketchLibraryWrapper
 */


#include <QPainter>
#include <iostream>
#include "curveitem.h"

CurveItem::CurveItem( QGraphicsItem *parent_ ): QGraphicsPathItem( parent_ )
{
    setZValue( std::numeric_limits<float>::max() );
    setPath( curve );
}



QRectF CurveItem::boundingRect() const
{
    return curve.boundingRect();
}


void CurveItem::paint( QPainter * painter_, const QStyleOptionGraphicsItem * option_, QWidget * widget_ )
{

    if( isVisible() == false )
    {
        return;
    }

    // changing the curve style
    QPainterPathStroker stroker_;
    stroker_.setCurveThreshold( 0.4 );
    stroker_.setCapStyle( Qt::RoundCap );
    stroker_.setJoinStyle( Qt::RoundJoin );
    stroker_.createStroke( curve );

    QPen pen_;
    pen_.setColor( getColor() );
    pen_.setWidth( getWidth() );
    pen_.setStyle( getStyle() );

    // the curve has a constant width regardless of any transformations applied to the item
    pen_.setCosmetic( true );


    if( this->isSelected() == true )
    {
        pen_.setColor( Qt::blue );
    }


    painter_->setRenderHint( QPainter::Antialiasing );
    painter_->setPen( pen_ );
    painter_->drawPath( curve );

}


bool CurveItem::create( const QPointF& p_ )
{
    if( isDone() == false ) return false;

    // this notifies the item that its geometry is going to be changed. This is important to update its
    // bounding box.
    prepareGeometryChange();
    curve.moveTo( p_ );
    is_done = false;
    return true;
}


bool CurveItem::add( const QPointF& p_ )
{
    if( isDone() == true ) return false;

    prepareGeometryChange();
    curve.lineTo( p_ );
    return true;
}


bool CurveItem::hasSubpaths()
{
    // if the curve has more than one subpath so, return true. returns false otherwise
    return ( curve.toSubpathPolygons().size() > 1/*MIN_SUBPATHS_OVERSKETCH*/ );
}


void CurveItem::connect()
{
    setDone();


    bool status_ = !isEmpty();
    status_ &= hasSubpaths();

    // if the curve is not empty and has more than one segment (subpath) so this method connects the segments
    // using oversketch
    if( status_ == false ) return;

    prepareGeometryChange();
    QList< QPolygonF > subpaths_ = curve.toSubpathPolygons();
    curve = SketchLibraryWrapper::overSketchQt( subpaths_.at( 0 ), subpaths_.at( 1 ) );
    QGraphicsPathItem::update();
}


void CurveItem::setDone()
{
    setPath( curve );
    is_done = true;
}


bool CurveItem::isDone() const
{
    return is_done;
}


void CurveItem::setCurve( const PolyCurve& curve_ )
{
    // cleaning the curve
    curve = QPainterPath();

    std::size_t number_of_segments = curve_.getNumberOfSegments();
    for( std::size_t i = 0; i < number_of_segments; ++i )
    {
/* <<<<<<< HEAD */
        /* QPolygonF pol_ = std::move(SketchLibraryWrapper::fromCurve2DToQt( curve_.getSubcurve( i ) )); */
/* ======= */
        // get all subpaths of the curve_ and append to the curve.
        QPolygonF pol_ = SketchLibraryWrapper::fromCurve2DToQt( curve_.getSubcurve( i ) );
/* >>>>>>> devel */
        curve.addPolygon( pol_ );
    }

    setDone();

}


void CurveItem::setCurves( const std::map< double, PolyCurve >& curves_, bool swapped_ )
{
    curve = QPainterPath();

    // if swapped is true, each coordinate should be swapped before being added to the curve
    if( swapped_ == true )
    {

        // for each curve in the map append all of them in curve

        for( auto it_: curves_ )
        {
            PolyCurve curve_ = PolyCurve( ( it_.second ).getPointsSwapped() );

            std::size_t number_of_segments = curve_.getNumberOfSegments();
            for( std::size_t i = 0; i < number_of_segments; ++i )
            {
/* <<<<<<< HEAD */
                /* QPolygonF pol_ = std::move(SketchLibraryWrapper::fromCurve2DToQt( curve_.getSubcurve( i ) )); */
/* ======= */
                // get all subpaths of the curve_ and append to the curve.
                QPolygonF pol_ = SketchLibraryWrapper::fromCurve2DToQt( curve_.getSubcurve( i ) );
/* >>>>>>> devel */
                curve.addPolygon( pol_ );
            }

        }
        setDone();

    }

    else
    {
        for( auto it_: curves_ )
        {
            PolyCurve& curve_ = it_.second;

            std::size_t number_of_segments = curve_.getNumberOfSegments();
            for( std::size_t i = 0; i < number_of_segments; ++i )
            {
/* <<<<<<< HEAD */
                /* QPolygonF pol_ = std::move(SketchLibraryWrapper::fromCurve2DToQt( curve_.getSubcurve( i ) )); */
/* ======= */
                QPolygonF pol_ = SketchLibraryWrapper::fromCurve2DToQt( curve_.getSubcurve( i ) );
/* >>>>>>> devel */
                curve.addPolygon( pol_ );
            }

        }
        setDone();
    }

}


PolyCurve CurveItem::getCurve() const
{
    if( curve.isEmpty() == true ) return PolyCurve();
    Curve2D curve_ = SketchLibraryWrapper::fromQtToCurve2D( curve.toSubpathPolygons()[0] );
    return PolyCurve( curve_ );
}


void CurveItem::getMonotonicX()
{
    if( isEmpty() == true ) return;

    Curve2D curve_ = getCurve().getCurves2D()[ 0 ];

    // turning the first subpath into a monotonic curve in the X direction.
    // it is supposed that the curve has only one subpath, i.e., this is done after any connect
    curve_ = SketchLibrary1::monotonicInX( curve_ );
//    curve_ = SketchLibrary1::smooth( curve_ );
    setCurve( PolyCurve( curve_ ) );

}


void CurveItem::getMonotonicY()
{

    if( isEmpty() == true ) return;

    // turning the first subpath into a monotonic curve in the Y direction.
    // it is supposed that the curve has only one subpath, i.e., this is done after any connect
    Curve2D curve_ = getCurve().getCurves2D()[ 0 ];
    curve_ = SketchLibrary1::monotonicInY( curve_ );
//    curve_ = SketchLibrary1::smooth( curve_ );
    setCurve( PolyCurve( curve_ ) );

}


void CurveItem::clear()
{
    prepareGeometryChange();

    curve = QPainterPath();
    setPath( curve );

    is_done = true;
    QGraphicsPathItem::update();
}


bool CurveItem::isEmpty() const
{
    return curve.isEmpty();
}


void CurveItem::setColor( int r_, int g_, int b_ )
{
    line_color.red = r_;
    line_color.green = g_;
    line_color.blue = b_;
    QGraphicsPathItem::update();
}


void CurveItem::getColor( int& r_, int& g_, int& b_ )
{
    r_ = line_color.red;
    g_ = line_color.green;
    b_ = line_color.blue;
}


void CurveItem::setColor( const QColor& color_ )
{
    line_color.red = color_.red();
    line_color.green = color_.green();
    line_color.blue = color_.blue();
    QGraphicsPathItem::update();
}


QColor CurveItem::getColor() const
{
    return QColor( line_color.red, line_color.green, line_color.blue );
}


void CurveItem::setWidth( int width_ )
{
    line_width = width_;
    update();
}


int CurveItem::getWidth() const
{
    return line_width;
}


void CurveItem::setStyle( const Qt::PenStyle& style_ )
{
    line_style = style_;
    QGraphicsPathItem::update();
}


Qt::PenStyle CurveItem::getStyle() const
{
    return line_style;
}


void CurveItem::resetToDefaultColor()
{
    line_color = DEFAULT_LINE_COLOR;
}


void CurveItem::resetToDefaultWidth()
{
    line_width = DEFAULT_LINE_WIDTH;
}


void CurveItem::resetToDefaultStyle()
{
    line_style = DEFAULT_LINE_STYLE;
}


CurveItem::~CurveItem()
{
    clear();
}
