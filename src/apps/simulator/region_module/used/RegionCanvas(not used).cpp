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



#include "RegionCanvas.hpp"

namespace RRM
{
        RegionCanvas::RegionCanvas ( QWidget *parent )
                                                                                                                                        : QGraphicsView ( parent ), timerId ( 0 )
        {
                this->setRenderHint ( QPainter::Antialiasing , true );

                this->setFocusPolicy ( Qt::StrongFocus );
                this->setMouseTracking ( true );

                setCacheMode ( CacheBackground );
                setViewportUpdateMode ( BoundingRectViewportUpdate );
                setRenderHint ( QPainter::Antialiasing );
                setTransformationAnchor ( AnchorUnderMouse );
                // XXX The QGraphicsView Orientation =(
                this->transformation_matrix_.scale ( 1 , -1 );
                this->setTransform ( this->transformation_matrix_ );

                setMinimumSize ( 600 , 400 );
                setWindowTitle ( tr ( "RRM Well Plug-in" ) );

                board_ = new QGraphicsRectItem ( 0.0f , 0.0f , 600.0f , 400.0f );

        }

        QGraphicsRectItem * RegionCanvas::canvasArea()
        {
                return board_;
        }

        void RegionCanvas::itemMoved ( )
        {
                if ( !timerId )
                        timerId = startTimer ( 1000 / 25 );
        }

        void RegionCanvas::keyPressEvent ( QKeyEvent *event )
        {
                switch ( event->key ( ) )
                {
                        case Qt::Key_Plus:
                        {
                                zoomIn ( );
                                break;
                        }

                        case Qt::Key_Minus:
                        {
                                zoomOut ( );
                                break;
                        }

                        case Qt::Key_Escape:
                        {
                                QGraphicsView::fitInView( board_->boundingRect(), Qt::KeepAspectRatio );
                                break;
                        }

                        default:
                        {
                                QGraphicsView::keyPressEvent ( event );
                                break;
                        }

                }
        }

        void RegionCanvas::timerEvent ( QTimerEvent *event )
        {
                Q_UNUSED( event );

        }

        void RegionCanvas::wheelEvent ( QWheelEvent *event )
        {
                scaleView ( std::pow ( (double) 2 , -event->delta ( ) / 240.0 ) );
        }

        void RegionCanvas::drawBackground ( QPainter *painter , const QRectF &rect )
        {
                Q_UNUSED( rect );

                // Shadow
//        QRectF sceneRect = this->sceneRect ( );
//
//        QRectF rightShadow ( sceneRect.right ( ) , sceneRect.top ( ) + 5 , 5 , sceneRect.height ( ) );
//        QRectF bottomShadow ( sceneRect.left ( ) + 5 , sceneRect.bottom ( ) , sceneRect.width ( ) , 5 );
//        if ( rightShadow.intersects ( rect ) || rightShadow.contains ( rect ) )
//                painter->fillRect ( rightShadow , Qt::darkGray );
//        if ( bottomShadow.intersects ( rect ) || bottomShadow.contains ( rect ) )
//                painter->fillRect ( bottomShadow , Qt::darkGray );
//
//        // Fill
//        QLinearGradient gradient ( sceneRect.topLeft ( ) , sceneRect.bottomRight ( ) );
//        gradient.setColorAt ( 0 , Qt::white );
//        gradient.setColorAt ( 1 , Qt::lightGray );
//        painter->fillRect ( rect.intersected ( sceneRect ) , gradient );
//        painter->setBrush ( Qt::NoBrush );
//        painter->drawRect ( sceneRect );
//
//        // Text
//        QRectF textRect ( sceneRect.left ( ) + 4 , sceneRect.top ( ) + 4 , sceneRect.width ( ) - 4 , sceneRect.height ( ) - 4 );
//        QString message ( tr ( "Click and drag the nodes around, and zoom with the mouse " "wheel or the '+' and '-' keys" ) );
//
//        // Combine the transformation, thus it can be applied when zoom in/out
//        painter->setTransform ( transformation_matrix , true );
//        QFont font = painter->font ( );
//        font.setBold ( true );
//        font.setPointSize ( 14 );
//        painter->setFont ( font );
//        painter->setPen ( Qt::lightGray );
//        painter->drawText ( textRect.translated ( 2 , 2 ) , message );
//        painter->setPen ( Qt::black );
//        painter->drawText ( textRect , message );
        }

        void RegionCanvas::scaleView ( qreal scaleFactor )
        {
                qreal factor = transform ( ).scale ( scaleFactor , scaleFactor ).mapRect ( QRectF ( 0 , 0 , 1 , 1 ) ).width ( );
                if ( factor < 0.07 || factor > 100 )
                        return;

                scale ( scaleFactor , scaleFactor );
        }

        void RegionCanvas::zoomIn ( )
        {
                scaleView ( qreal ( 1.2 ) );
        }

        void RegionCanvas::zoomOut ( )
        {
                scaleView ( 1 / qreal ( 1.2 ) );
        }

        QTransform RegionCanvas::getTransformationMatrix ( ) const
        {
                return transformation_matrix_;
        }
}
