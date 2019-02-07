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



#include "canvasstack.h"
#include <QDockWidget>
#include <QBitmap>
#include <QStyleOptionDockWidget>
#include <QLabel>
#include <QAction>

CanvasStack::CanvasStack( QWidget* parent ): QWidget( parent )
{
    initialize();
}


void CanvasStack::initialize()
{

    setWindowFlags(Qt::Widget);

    hb_mainlayout = new QSplitter( this );
    hb_mainlayout->setOrientation( Qt::Vertical );

    QHBoxLayout* hb_main = new QHBoxLayout();
    hb_main->addWidget( hb_mainlayout );

    setLayout( hb_main );

    // the index of the current canvas, and it is mainly used as the cross-section depth
    current = 0.0;
}


void CanvasStack::addElement( double id_, QGraphicsView* canvas_ )
{
    // id_ corresponds to index of the canvas, and it is mainly used as the cross-section depth
    DockWidget* dc = new DockWidget( QString( "Canvas ").append( QString::number( id_, 'f', 1 ) ), this );
    dc->setAllowedAreas(Qt::AllDockWidgetAreas);
    dc->setGeometry( 100, 100, canvas_->rect().width(), canvas_->rect().height() );
    dc->setWidget( canvas_ );
    dc->updateGeometry();

    bool status = Container::addElement( id_, dc );
    if( status == false ) return;

    hb_mainlayout->addWidget( dc );

    connect( dc, &DockWidget::closeDockWidget, [=]() { emit closeSubWindow( id_ ); } );


}

QGraphicsView* CanvasStack::getElement( double id_ )
{
    bool status = Container::findElement( id_ );
    if( status == false ) return nullptr;

    return (QGraphicsView*)(Container::data[ id_ ]->widget());
}


void CanvasStack::removeElement( double id_ )
{

    if( findElement( id_ ) == false ) return;

    QGraphicsView* canvas_ = (QGraphicsView*)(Container::data[ id_ ]->widget() );
    if( canvas_ == nullptr ) return;

    delete canvas_;
    canvas_ = nullptr;

    deleteElement( id_ );

    bool status = Container::removeElement( id_ );
    if( status == false ) return;

    update();

}


void CanvasStack::setCurrent( double id_ )
{
    if( findElement( id_ ) == false ) return;

    Container::Iterator it =  Container::data.begin();
    while( it != Container::data.end() )
    {
        QGraphicsView* gv_ = (QGraphicsView*)(Container::data[ it->first ]->widget());
        gv_->setStyleSheet( "" );
        it++;
    }

    // changing the canvas style when it is the current
    QGraphicsView* gv_ = (QGraphicsView*)(Container::data[ id_ ]->widget());
    gv_->setStyleSheet( "border: 2px solid navy" );

    update();
}


QGraphicsView* CanvasStack::getCurrent()
{
    if( findElement( current ) == false ) return ( new QGraphicsView() );
    return (QGraphicsView*)(Container::data[ current ]->widget());
}


void CanvasStack::clear()
{

    while ( Container::data.begin()  != Container::data.end() )
    {
        Container::Iterator it =  Container::data.begin();
        removeElement( it->first );
    }

}


void CanvasStack::closeEvent(QCloseEvent *event)
{
      emit windowClosed();
      event->accept();
}
