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
 * @file view3dinterface.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class View3dInterface
 */

#include <QDir>

#include "view3dinterface.h"

#include "mainwindow.h"

View3dInterface::View3dInterface( MainWindow* const& window_ )
{
    window = window_;
}


void View3dInterface::createInterface()
{
    if( window == nullptr ) return;

    createView3dWindow();
    createView3dActions();
}


void View3dInterface::createView3dWindow()
{

    controller3d = new View3dController();

    canvas3d = new Canvas3d();
    canvas3d->setToolTip( "Canvas 3D" );
//    canvas3d->show();

    sl_depth_csection = new RealFeaturedSlider( Qt::Vertical );
    sl_depth_csection->setToolTip( "Move Cross-section" );

    QHBoxLayout* hb_central_widget = new QHBoxLayout();
    hb_central_widget->addWidget( canvas3d );
    hb_central_widget->addWidget( sl_depth_csection );


    QFrame* frame = new QFrame();
    frame->setFrameStyle(QFrame::Box | QFrame::Raised );
    frame->setLineWidth( 0 );
    frame->setMidLineWidth( 1 );
    frame->setLayout( hb_central_widget );

    QHBoxLayout* hb_layout = new QHBoxLayout();
    hb_layout->addWidget( frame );

    QWidget* central_widget = new QWidget();
    central_widget->setLayout( hb_layout );

    window->setCentralWidget( central_widget );
    window->show();


    controller3d->setMainWindow( std::shared_ptr< Canvas3d > ( canvas3d ) );
    controller3d->setController( std::shared_ptr< Controller > ( window->controller ) );



}


void View3dInterface::createView3dActions()
{

    connect( sl_depth_csection, &RealFeaturedSlider::sliderMoved, [=]( double depth_ )
    { window->app->moveMainCrossSection( depth_ ); } );

    connect( sl_depth_csection, &RealFeaturedSlider::sliderMoved, [=]()
    { window->app->updateRegionBoundary(); } );

    connect( sl_depth_csection, &RealFeaturedSlider::markValue, [=]( const double& depth_, QColor color_ )
    { window->app->addFixedCrossSection( depth_, color_ );  } );

    connect( sl_depth_csection, &RealFeaturedSlider::unmarkValue, [=]( const double& depth_ )
    { window->app->removeFixedCrossSection( depth_ );  } );

    connect( window->app, &RRMApplication::updateVolume, [=]()
    { controller3d->updateVolume(); } );

    connect( window->app, &RRMApplication::updateRange, [=]( double min_, double max_, bool inverted_  )
    {
        sl_depth_csection->setRange( min_, max_ );
        sl_depth_csection->setInvertedControls( !inverted_ );
        sl_depth_csection->setInvertedAppearance( !inverted_ );
    } );

    connect( window->app, &RRMApplication::updateDiscretization, [=]( const std::size_t& disc_ )
    { sl_depth_csection->setDiscretization( static_cast< int >(disc_) ); } );

    connect( window->app, &RRMApplication::addObject, [=]( const ObjectPtr& obj_ )
    { controller3d->addStratigraphy( obj_ ); } );

    connect( window->app, &RRMApplication::updateObjects, [=]()
    { controller3d->updateObjects(); } );

    connect( window->app, &RRMApplication::updateTrajectories, [=]()
    { controller3d->updateObjects(); } );

    connect( window->app, &RRMApplication::changeToTopViewDirection, [=]()
    { sl_depth_csection->clear(); controller3d->setTopViewCrossSection(); controller3d->updateVolume(); } );


    connect( window->app, &RRMApplication::changeToCrossSectionDirection, [=]()
    { sl_depth_csection->clear(); controller3d->setMainViewCrossSection(); controller3d->updateVolume(); } );


    connect( window->app, &RRMApplication::updateMainCrossSection, [=]()
    { controller3d->updateMainCrossSection(); } );

    connect( window->app, &RRMApplication::updateTopViewCrossSection, [=]()
    { controller3d->updateMainCrossSection(); } );


    connect( window->app, &RRMApplication::resetApplication, [=]()
    { controller3d->clear(); sl_depth_csection->clear(); } );

    connect( window->app, &RRMApplication::startApplication, [=]{ controller3d->init(); }
    );

    connect( window->app, &RRMApplication::addRegions, [=]{ controller3d->addRegions(); }
    );

    connect( window->app, &RRMApplication::updateRegions, [=]{ controller3d->updateRegions(); } );

    connect( window->app, &RRMApplication::clearRegions, [=]{ controller3d->clearRegions(); } );

    connect( window->app, &RRMApplication::setVerticalExaggerationScale, [=]( double scale_ ){ controller3d->setVerticalExaggeration( scale_ ); } );

    connect( window->app, &RRMApplication::removeMarker, [=]( double id_ ){ sl_depth_csection->removeMarker( id_ ); } );

    connect( window, &MainWindow::takeScreenshot, [=](){ canvas3d->screenshot(); } );

}


void View3dInterface::init()
{
    controller3d->init();
}
