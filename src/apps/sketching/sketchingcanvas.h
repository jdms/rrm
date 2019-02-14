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
 * @file sketchingcanvas.h
 * @author Clarissa C. Marques
 * @brief File containing the class SketchingCanvas
 */


#ifndef SKETCHINGCANVAS_H
#define SKETCHINGCANVAS_H

#include <QGraphicsView>
#include <QKeyEvent>

#include "sketchscene.h"


/**
 *   The holder to the 2d scene.
 *   A canvas to show the scene. It inherits from QGraphicsView.
 */
class SketchingCanvas: public QGraphicsView
{
    public:

        /**
        * Constructor. parent_ is passed to QGraphicsView's constructor.
        * @param parent the parent item
        */
        SketchingCanvas( QWidget *parent_ = nullptr );

        /**
       * Destructor.
       */
        ~SketchingCanvas() override;


       /**
       * Method to retrieves the scene.
       * @see SketchScene
       * @return std::shared_ptr< SketchScene > A shared pointer to the scene associated to this canvas.
       */
        const std::shared_ptr< SketchScene >& getScene() const;


       /**
       * Method to apply the vertical exaggeration in the scene.
       * @param scale The vertical exaggeration value (double).
       * @return Void.
       */
        void setVerticalExaggeration( double scale_ );


       /**
       * Method to stop applying the vertical exaggeration in the axes of the scene.
       * @return Void.
       */
        void stopVerticalExaggeration();


       /**
       * Method to retrieves the vertical exaggeration value used in the scene.
       * @return double return the value of v_exag (double), which corresponds to the vertical exaggeration value used in the scene.
       */
        double getVerticalExaggeration() const;



    protected:

       /**
       * Method to create the scene and its actions.
       * @return Void.
       */
        void setupScene();



    private:

        std::shared_ptr< SketchScene > scene = nullptr;                 /**< Represents the scene 2d. */

        double v_exag = 1.0;                                            /**< Represents the vertical exaggeration value. */
};

#endif // SKETCHINGCANVAS_H
