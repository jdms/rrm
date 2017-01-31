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

#include "View3DWindow.hpp"

View3DWindow::View3DWindow ( QWidget* parent ) : QMainWindow(parent)
{

    createWindow();

    connect( canvas3d, &View3DCanvas::openglInitialized, [=](){ emit initializeScene(); } );
    connect( sl_resolution_numI, &QSlider::valueChanged, [=]( int value ){ emit changeResolution( value, sl_resolution_numJ->value() ); } );
    connect( sl_resolution_numJ, &QSlider::valueChanged, [=]( int value ){ emit changeResolution( sl_resolution_numI->value(), value ); } );

}


void View3DWindow::createWindow()
{


    canvas3d = new View3DCanvas ( this );
    sl_resolution_numI = new QSlider( Qt::Horizontal );
    sl_resolution_numJ = new QSlider( Qt::Horizontal );


    QGridLayout* gl_resolution = new QGridLayout();
    gl_resolution->addWidget( new QLabel( "Width: " ), 0, 0 );
    gl_resolution->addWidget( sl_resolution_numI, 0, 1 );
    gl_resolution->addWidget( new QLabel( "Depth: " ), 1, 0 );
    gl_resolution->addWidget( sl_resolution_numJ, 1, 1 );



    QFrame *fr = new QFrame ( this );
    fr->setMaximumHeight( 50 );
    fr->setFrameStyle ( QFrame::Box | QFrame::Sunken );
    fr->setLayout ( gl_resolution );


    QVBoxLayout* vb_canvas3d = new QVBoxLayout();
    vb_canvas3d->setContentsMargins( 0, 0, 0, 0 );
    vb_canvas3d->setSpacing( 0 );
//    vb_canvas3d->addWidget( fr );
    vb_canvas3d->addWidget( canvas3d );


    QWidget* wg_3dview = new QWidget();
    wg_3dview->setLayout( vb_canvas3d );

    setCentralWidget( wg_3dview );


}


