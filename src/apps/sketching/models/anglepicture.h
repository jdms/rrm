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
 * @file anglepicture.h
 * @author Clarissa C. Marques
 * @brief File containing the class AnglePicture
 */


#ifndef ANGLEPICTURE_H
#define ANGLEPICTURE_H

#include <QLabel>
#include <QPixmap>

/**
 *  Auxiliary wrapper to hold and handle operations on the output dip angle picture.
 *  It inherits from QLabel.
 */

class AnglePicture: public QLabel
{

    public:

        /**
        * Constructor.
        * @param size size of the image
        * @param angle the output dip angle
        */
        AnglePicture(  const QSize& size_, double angle_ );


    public slots:


        /**
        * Method to update the output dip angle
        * @param angle the output dip angle
        * @return void.
        */
        void updateAngle( double angle_ );

    private slots:


        /**
        * Method to repaint the image
        * @param QPaintEvent contains event parameters for painting events
        * @return void.
        */
        void paintEvent(QPaintEvent* e);


    private:


        QSize size;                                     /**< The size of the image  */

        double angle = 0;                               /**< The output dip angle

*/
};

#endif // ANGLEPICTURE_H
