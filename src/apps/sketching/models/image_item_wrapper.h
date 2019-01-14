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



#ifndef ITEM_H
#define ITEM_H

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QString>


/**
 *  Auxiliary wrapper to hold and handle operations on the image, as scaling and translating.
 *  It inherits from QGraphicsPixmapItem.
 */


class ImageItemWrapper: public QGraphicsPixmapItem
{

    /**
    * \enum Interaction
    * \brief Enum to represent the set of interactions with the image.
    */

    enum class Interaction{ MOVING, RESIZING, NONE };

    public:


        /**
        * Constructor.
        */
        ImageItemWrapper();


        /**
        * Method to set all the properties required to represend the image
        * @param im_ the image
        * @param origin_ the origin of the image
        * @param top_right_ the point at the right-top corner of the image. It helps to compute the image dimensions.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setImage( const QPixmap& im, QPointF origin_, QPointF top_right_ );


        /**
        * Method to set the path of the file
        * @param file_ image path_
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setImagePath( const QString& file_ );


        /**
        * Method to get the path of the image file
        * @see testMeToo()
        * @see publicVar()
        * @return QString the path of the image.
        */
        QString getImagePath() const;


        /**
        * Method to get the origin of the image
        * @see testMeToo()
        * @see publicVar()
        * @return QPointF origin of the image.
        */
        QPointF getOrigin() const;


        /**
        * Method to get the top-right corner of the image
        * @see testMeToo()
        * @see publicVar()
        * @return QPointF the top-right corner of the image.
        */
        QPointF getTopRight() const;


        /**
        * Method to move the origin of the image to the new point origin_
        * @param origin_ new origin of the image
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void moveRectangle( QPointF origin_ );


        /**
        * Method to move the top-right of the image to the new point p_
        * @param origin_ new top-right corner of the image
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void resizeRectangle(const QPointF &p );


        /**
        * Method to get the bounding box of the image
        * @see testMeToo()
        * @see publicVar()
        * @return QRectF the bounding box of the image
        */
        QRectF boundingRect() const override;


    protected:

        //TODO: finish to document the parameters.
        /**
        * Method to repaint the image
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
                        QWidget* widget = nullptr ) override;


    private:

        QRectF m_imageRect;                                                             /**< The QRectF where the image will be drawn.  */

        QPixmap m_image;                                                                /**< A QPixmap to hold the image */

        QString path;                                                                   /**< A QString to define the path of the image file  */

        Interaction default_action = Interaction::NONE;                                 /**< Default status of the user interaction */

};

#endif // ITEM_H
