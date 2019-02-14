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
 * @file coordinateaxes_2d.h
 * @author Clarissa C. Marques
 * @date 9 Sep 2012
 * @brief File containing the class CoordinateAxes2d
 */

#ifndef COORDINATEAXES2D_H
#define COORDINATEAXES2D_H

#include <QGraphicsLineItem>
#include <QPainter>
#include <QColor>

/**
 *  A graphical representation to the axes in the scene 2d.
 *  It is represent by a lines.
 *  It inherits from QGraphicsLineItem.
 */


class CoordinateAxes2d: public QGraphicsLineItem
{
    public:

        enum class Plane{ XY, XZ, YZ };

        /**
        * Constructor.
        * @param pl indicates to which coordinate axes it will represent
        */
        CoordinateAxes2d( const Plane& pl = Plane::XY );


        /**
        * Method to set the coordinate axes it will represent
        * @param pl indicates to which coordinate axes it will represent
        * @see Plane
        * @return void.
        */
        void setPlane( const Plane& pl );


        /**
        * Method to set the lenght of the x axes it will have
        * @param l the lenght of the x axes
        * @return void.
        */
        void setAxisXLenght( double l );


        /**
        * Method to set the lenght of the y axes it will have
        * @param l the lenght of the y axes
        * @return void.
        */
        void setAxisYLenght( double l );


        /**
        * Method to get the bounding box of the axes
        * @see QRectF
        * @return QRectF the bounding box of the image
        */
        QRectF boundingRect() const;


        /**
        * Update the length of the y coordinate considering the vertical exaggeration.
        * This method was created to simulate the effect of the vertical exaggeration without apply it directly,
        * which was causing undesired deformation on the text.
        * @param scale_ double. The scale to be applied.
        * @param height_ the real size of the y coordinate.
        * @return void.
        */
        void updateVerticalExaggeration( double scale_, double height_ );



        /**
        * Stop the vertical exaggeration effect turning the lenght of the y coordinate back to its real lenght
        * This method was created to simulate the effect of the vertical exaggeration without apply it directly,
        * which was causing undesired deformation on the text.
        * @param height_ the real size of the y coordinate.
        * @return void.
        */
        void stopVerticalExaggeration(  double height_ );


        /**
        * Stop the vertical exaggeration effect turning the lenght of the y coordinate back to its real lenght, which is stored into the variable 'height
        * This method was created to simulate the effect of the vertical exaggeration without apply it directly,
        * which was causing undesired deformation on the text.
        * @return void.
        */
        void resetVerticalExaggeration();


    protected:


        /**
        * Method to repaint the axes
        * @see QPainter performs ainting on the widget
        * @see QStyleOptionGraphicsItem provides style options for the item
        * @see QWidget  it is an optional parameter. If provided, it points to the widget that is being painted on; otherwise, it is nullptr
        * @return void.
        */
         void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);


         /**
         * Method to draw x coordinate
         * @see QPainter performs painting on the widget
         * @return void.
         */
         void drawAxisX( QPainter* p );


          /**
          * Method to draw y coordinate
          * @see QPainter performs painting on the widget
          * @return void.
          */
         void drawAxisY( QPainter* p );


          /**
          * Method to draw y coordinate with scale
          * @see QPainter performs ainting on the widget
          * @return void.
          */
         void drawAxisYWithScale( QPainter* p );


    protected:

        double axisx_length;                                                /**< The length of the x coordinate.  */

        double axisy_length;                                                /**< The length of the y coordinate.  */

        const QColor width_color = Qt::red;                                 /**< The default color to the width direction.  */

        const QColor height_color = Qt::green;                              /**< The default color to the height direction.  */

        const QColor depth_color = Qt::blue;                                /**< The default color to the depth direction.  */

        QColor current_x;                                                   /**< The color which the x coordinate will be painted.  */

        QColor current_y;                                                   /**< The color which the y coordinate will be painted.  */

        Plane plane;                                                        /**< The plane which coordinate axes it will be represented.  */

        int scale = 1;                                                      /**< An auxiliary integer to scale the axes simulating the vertical exaggeration. */

        bool scale_on = false;                                              /**< A boolean to define if the axes should scale due the vertical exaggeration or not */

        double height = 0.0;                                                /**< The size of the y coordinate without scale */
};

#endif // COORDINATEAXES2D_H
