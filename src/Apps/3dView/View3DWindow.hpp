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


#ifndef VIEW3DMODULE_H
#define VIEW3DMODULE_H


#include <QMainWindow>
#include <QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSlider>
#include <QtWidgets/QGridLayout>
#include <QLabel>



#include "3dView/View3DCanvas.hpp"

/**!
 * @brief  brief  Embeds the sketching area into the main window.
 *
 */

class View3DWindow: public QMainWindow
{
    Q_OBJECT

    public:



        View3DWindow( QWidget* parent = 0 );

        inline void setScene( Scene* scene )
        {
            canvas3d->setScene( scene );
            connect( scene, SIGNAL( updatedScene() ), canvas3d, SLOT( update() ) );
        }


    protected:

        void createWindow();


    signals:

        void initializeScene();
        void changeResolution( const int, const int );
        void exportToCPS3();


    public:

        View3DCanvas *canvas3d;
        QSlider* sl_resolution_numI;
        QSlider* sl_resolution_numJ;



};

#endif // VIEW3DMODULE_H
