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
 * @file view3dinterface.h
 * @author Clarissa C. Marques
 * @brief File containing the class View3dInterface
 */


#ifndef VIEW3DINTERFACE_H
#define VIEW3DINTERFACE_H


#include <QDockWidget>

#include "3dview/canvas3d.h"
#include "3dview/view3dcontroller.h"
#include "./core/widgets/realfeaturedslider.h"


class MainWindow;



/**
 *   Class responsible for initializing the 3d application
 */


class View3dInterface: public QObject
{
    Q_OBJECT

    public:


        /**
        * Constructor.
        * @param window a const reference to the MainWindow
        * @see MainWindow
        */
        View3dInterface( MainWindow* const& window_ );


        /**
        * Method to creates the interface of the 3d application
        * @return void.
        */
        void createInterface();


    public slots:


        /**
        * Method to initialize the controller.
        * @return void.
        */
        void init();

    protected:


        /**
        * Method to create the 3d window interface
        * @return void.
        */
        void createView3dWindow();


        /**
        * Method to initialize and create the connects related to the actions of the 3d application
        * @return void.
        */
        void createView3dActions();


    protected:

        MainWindow* window = nullptr;                                   /**< Main Window */

        View3dController* controller3d = nullptr;                       /**< 3d Controller */

        Canvas3d* canvas3d = nullptr;                                   /**< 3d Canvas */

        RealFeaturedSlider* sl_depth_csection = nullptr;                /**< Cross-section slider */

};

#endif // VIEW3DINTERFACE_H
