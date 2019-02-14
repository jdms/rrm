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
 * @file curveitem.h
 * @author Clarissa C. Marques
 * @date 9 Sep 2012
 * @brief File containing the classes CurveItem and SketchLibraryWrapper
 */


#ifndef CURVEITEM_H
#define CURVEITEM_H

#include <limits>
#include <QGraphicsPathItem>

#include "./libs/sketchlibrary.h"


/**
 *  Auxiliary wrapper to the SketchLibrary .
 *  This wrapper converts data structures from the curve library to Qt.
 */


class SketchLibraryWrapper
{
    public:

    /**
    * Static method to apply the oversketch from Qt curves.
    * @param seg0_ the first segment of curve using qt data structure
    * @param seg1_ the second segment of curve using qt data structure
    * @see QPolygonF
    * @see QPainterPath
    * @see SketchLibrary1::overSketch
    * @return QPainterPath the resulting curve from the oversketch.
    */
    static QPainterPath overSketchQt( const QPolygonF& seg0_, const QPolygonF& seg1_ )
    {
        Curve2D sketch_ = fromQtToCurve2D( seg0_ );
        Curve2D segment_ = fromQtToCurve2D( seg1_ );

        SketchLibrary1::overSketch( sketch_, segment_ );

        QPainterPath curve;
        curve.addPolygon( fromCurve2DToQt( sketch_ ) );
        return curve;

    }


    /**
    * Static method to convert a curve from Qt to Curve2D
    * @param pol_ curve from Qt data structure
    * @see Curve2D
    * @see QPolygonF
    * @return Curve2D
    */
    static Curve2D fromQtToCurve2D( const QPolygonF& pol_ )
    {
        Curve2D curve_;
        int start = ( pol_.isClosed() ) ? 1 : 0 ;

        for( int i = start ; i < pol_.size() ; ++i )
            curve_.add( QPointFToPoint2D( pol_[ i ] ) );

        curve_.close( pol_.isClosed() );
        return curve_;

    }


    /**
    * Static method to convert a curve from Curve2D to a curve in Qt
    * @param pol_ curve from Curve2D
    * @see Curve2D
    * @see QPolygonF
    * @return QPolygonF
    */
    static QPolygonF fromCurve2DToQt( const Curve2D& curve_ )
    {

        QPolygonF pol_;

        for( unsigned int i = 0 ; i < curve_.size() ; ++i )
            pol_ << Point2DToQPointF( curve_[ i ] ) ;

        if( curve_.isClosed() && curve_.size() > 0 )
            pol_ << Point2DToQPointF( curve_[ 0 ] ) ;

        return pol_;
    }


    /**
    * Static method to convert a Point2D to a point in Qt
    * @param p a point Point2D
    * @see Point2D
    * @see QPointF
    * @return QPointF
    */
    static QPointF Point2DToQPointF( const Point2D & p )
    {
        return QPointF( p.x() , p.y() ) ;
    }


    /**
    * Static method to convert a point in Qt to a point Point2D
    * @param p a point QPointF
    * @see Point2D
    * @see QPointF
    * @return Point2D
    */
    static Point2D QPointFToPoint2D( const QPointF &p )
    {
        return Point2D( p.x() , p.y() ) ;
    }


};


/**
 *  A data structure to help to represent the user input sketch
 *  It inherits from QGraphicsPathItem.
 */

class CurveItem : public QGraphicsPathItem
{
    public:

        /**
        * Constructor. parent_ is passed to QGraphicsPathItem's constructor.
        * @param parent_ the parent item
        */
        CurveItem( QGraphicsItem *parent_ = nullptr );


        /**
        * Destructor.
        */
        ~CurveItem() override;


        /**
        * Method to get the bounding box of the user input curve
        * @see QRectF()
        * @return QRectF the bounding box of the curve
        */
        QRectF boundingRect() const override ;


        /**
        * Method to start a new segment of curve
        * @param p_ the starting point of a new segment of curve
        * @see QPointF()
        * @return void.
        */
        bool create( const QPointF& p_ );


        /**
        * Method to connect the last point in the curve to the new point p_
        * @param p_ a new point to be connected to the curve
        * @see QPointF()
        * @return Boolean. Return true if the point can be added to the curve, and false otherwise
        * A point cannot be added if the curve is already done.
        */
        bool add( const QPointF& p_ );


        /**
        * Method to connect two segments, if there are more than one segment, of curve using oversketch.
        * @see SketchLibraryWrapper::overSketchQt()
        * @return void
        */
        void connect();


        /**
        * Set the curve as done. This not allow add any point more to the curve.
        * @return void
        */
        void setDone();


        /**
        * Check if the curve is done.
        * @return Boolean. Return true if the curve is done, and false if not.
        */
        bool isDone() const;


        /**
        * Method to clear the curve and set it as done.
        * Add a new point is only allowed using the 'Create' method.
        * @return void
        */
        void clear();


        /**
        * Method to check if the curve is empty
        * @return Boolean. It is true if the curve is empty and false if there is at least a point.
        */
        bool isEmpty() const;


        /**
        * Method to set the color of the curve
        * @param red the red component of the color (integer)
        * @param green the green component of the color (integer)
        * @param blue the blue component of the color (integer)
        * @return void
        */
        void setColor( int r_, int g_, int b_ );


        /**
        * Method to get the color of the curve
        * @param red reference to the red component of the color (integer)
        * @param green reference to the green component of the color (integer)
        * @param blue reference to the blue component of the color (integer)
        * @return void
        */
        void getColor( int& r_, int& g_, int& b_ );


        /**
        * Method to set the color of the curve using a QColor
        * @param color the new color of the curve
        * @return void
        */
        void setColor( const QColor& color_ );


        /**
        * Method to get the color of the curve by QColor
        * @see QColor()
        * @return QColor
        */
        QColor getColor() const;


        /**
        * Method to set the width of the curve line
        * @param width the width of the curve line
        * @return void
        */
        void setWidth( int width_ );


        /**
        * Method to get the width of the curve line
        * @return int the width of the curve
        */
        int getWidth() const;


        /**
        * Method to set the style of the curve line.
        * The style of the curve can be dotted, dashed, full, etc...
        * @see Qt::PenStyle
        * @return void
        */
        void setStyle( const Qt::PenStyle& style_ );


        /**
        * Method to get the style of the curve line.
        * The style of the curve can be dotted, dashed, full, etc...
        * @see Qt::PenStyle
        * @return Qt::PenStyle the style of the curve
        */
        Qt::PenStyle getStyle() const;


        /**
        * Method to set the curve directly
        * @param curve_ the curve to be set
        * @see PolyCurve
        * @return void
        */
        void setCurve( const PolyCurve& curve_ );


        /**
        * Method to set many segments of curves
        * @param curves_ the curves to be set
        * @param swapped_ boolean to indicate if the coordinate of points should be swapped
        * @see PolyCurve
        * @return void
        */
        void setCurves( const std::map< double, PolyCurve >& curves_, bool swapped_ = false );


        /**
        * Method to get the curve
        * @see PolyCurve
        * @return PolyCurve
        */
        PolyCurve getCurve() const;


        /**
        * Method to turn the curve in a X-monotonic curve
        * The transformed curve will replace the existent in the member curve.
        * @return void
        */
        void getMonotonicX();


        /**
        * Method to turn the curve in a Y-monotonic curve
        * The transformed curve will replace the existent in the member curve.
        * @return void
        */
        void getMonotonicY();


        /**
        * This method checks if the curve should be visible or not
        * @see QGraphicsPathItem::isVisible()
        * @return boolean. Returns true if the curve is visible, and false otherwise.
        */
        virtual bool isVisible() const { return QGraphicsPathItem::isVisible(); }


        /**
        * This method checks if the curve should be selected or not
        * @see QGraphicsPathItem::isSelected()
        * @return boolean. Returns true if the curve is selected, and false otherwise.
        */
        virtual bool isSelected() const { return QGraphicsPathItem::isSelected(); }


        /**
        * A pure virtual method to update
        * @return void
        */
        virtual void update(){}


        /**
        * Method to reset the color of the curve to default.
        * @see DEFAULT_LINE_COLOR
        * @return void
        */
        void resetToDefaultColor();


        /**
        * Method to reset the width of the curve to default.
        * @see DEFAULT_LINE_WIDTH
        * @return Void
        */
        void resetToDefaultWidth();


        /**
        * Method to reset the style of the curve to default.
        * @see DEFAULT_LINE_STYLE
        * @return void
        */
        void resetToDefaultStyle();



    protected:


        /**
        * Method to repaint the curve
        * @see QPainter performs painting on the widget
        * @see QStyleOptionGraphicsItem provides style options for the item
        * @see QWidget  it is an optional parameter. If provided, it points to the widget that is being painted on; otherwise, it is nullptr
        * @return void
        */
        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
                            QWidget* widget ) override;



        /**
        * This method checks if the curve has more than one segment
        * @return boolean. Returns true if the curve has more than one segment and false otherwise
        */
        bool hasSubpaths();


    protected:


        /**
        * @struct Color
        * @brief It is a data structure to represent color
        * @var red red component of a color
        * @var green green component of a color
        * @var blue blue component of a color
        */
        struct Color{ int red = 255, green = 0, blue = 0; };

        QPainterPath curve;                                                         /**< A data structure to hold the points of the curve  */

        const Color DEFAULT_LINE_COLOR;                                             /**< A Color to define the default color of the curve  */

        Color line_color = DEFAULT_LINE_COLOR;                                      /**< A Color to define the current color of the curve  */

        const int DEFAULT_LINE_WIDTH = 2;                                           /**< An integer to define the default width of the curve  */

        int line_width = DEFAULT_LINE_WIDTH;                                        /**< An integer to define the current width of the curve  */

        const Qt::PenStyle DEFAULT_LINE_STYLE = Qt::SolidLine;                      /**< A Qt::PenStyle to define the default style of the curve  */

        Qt::PenStyle line_style = DEFAULT_LINE_STYLE;                               /**< A Qt::PenStyle to define the current style of the curve  */

        bool is_done = true;                                                        /**< A boolean to define is the curve is done or not  */


};


#endif // CURVEITEM_H
