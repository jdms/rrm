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
 * @file lineitem.h
 * @author Clarissa C. Marques
 * @brief File containing the class LineItem
 */


#ifndef LINEITEM_H
#define LINEITEM_H

#include <QGraphicsLineItem>


/**
 *  A graphical representation to a line in the QGraphicsScene.
 *  It inherits from QGraphicsLineItem.
 */


class LineItem: public QGraphicsLineItem
{
    public:


        /**
        * Constructor. parent is passed to QGraphicsLineItem's constructor.
        * @param parent the parent item
        */
        LineItem( QGraphicsItem *parent_ = nullptr);


        /**
        * Destructor.
        */
        ~LineItem() override;


        /**
        * Method to get the bounding box of the line
        * @see QRectF
        * @return QRectF the bounding box of the line
        */
        QRectF boundingRect() const override ;


        /**
        * This method clears the line
        * @return void.
        */
        void clear();


        /**
        * Method to check if the line is empty
        * @return Boolean. It is true if the QLineF is empty and false if it is not.
        */
        bool isEmpty() const;


        /**
        * Method to set the color of the line
        * @param r the red component of the color (integer)
        * @param g the green component of the color (integer)
        * @param b the blue component of the color (integer)
        * @return void.
        */
        void setLineColor( int r_, int g_, int b_ );


        /**
        * Method to get the color of the line
        * @param r reference to the red component of the color (integer)
        * @param g reference to the green component of the color (integer)
        * @param b reference to the blue component of the color (integer)
        * @return void.
        */
        void getLineColor( int& r_, int& g_, int& b_ ) const;


        /**
        * Method to set the color of the line using a QColor
        * @param color the new color of the line
        * @see QColor
        * @return void.
        */
        void setLineColor( QColor color_ );


        /**
        * Method to get the color of the line by QColor
        * @see QColor
        * @return QColor
        */
        QColor getLineColor() const;


        /**
        * Method to set the width of the line
        * @param width the width of the line
        * @return void.
        */
        void setLineWidth( int width_ );


        /**
        * Method to get the width of the line
        * @return int the width of the line
        */
        int getLineWidth() const;


        /**
        * This method checks if the curve should be visible or not
        * @return boolean. Returns true if the line is visible, and false otherwise.
        */
        virtual bool isVisible() const { return QGraphicsLineItem::isVisible(); }


        /**
        * Method to reset the color of the line to default.
        * @see DEFAULT_LINE_COLOR
        * @return void.
        */
        void resetToDefaultLineColor();


    protected:


        /**
        * Method to repaint the curve
        * @param QPainter performs painting on the widget
        * @param QStyleOptionGraphicsItem provides style options for the item
        * @param QWidget  it is an optional parameter. If provided, it points to the widget that is being painted on; otherwise, it is nullptr
        * @return void.
        */
        void paint( QPainter* painter_, const QStyleOptionGraphicsItem* option_,
                    QWidget* widget_ ) override;


        /**
        * @struct Color
        * @brief It is a data structure to represent color
        * @var red red component of a color
        * @var green green component of a color
        * @var blue blue component of a color
        */
        struct Color{ int red = 255, green = 0, blue = 0;
                      Color( int r_, int g_, int b_ ): red( r_ ), green( g_ ),
                                                       blue( b_ ){} };

        QLineF line;                                                            /**< A data structure to represent the line  */

        Color DEFAULT_LINE_COLOR = { 0, 255, 255 };                             /**< A Color to define the default color of the line */

        Color line_color = DEFAULT_LINE_COLOR;                                  /**< A Color to define the current color of the line */

        const int DEFAULT_LINE_WIDTH = 2;                                       /**< An integer to define the default width of the line  */

        int line_width = DEFAULT_LINE_WIDTH;                                    /**< An integer to define the current width of the line  */


};

#endif // LINEITEM_H
