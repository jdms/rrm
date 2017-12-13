/*
 * Canvas.cpp
 *
 *  Created on: Sep 13, 2016
 *      Author: felipe
 */

#include "well_canvas.hpp"

namespace RRM
{
        WellCanvas::WellCanvas ( QWidget *parent )
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

                setMinimumSize ( 300 , 200 );
                setWindowTitle ( tr ( "RRM Well Plug-in" ) );

        }

        void WellCanvas::itemMoved ( )
        {
                if ( !timerId )
                        timerId = startTimer ( 1000 / 25 );
        }

        void WellCanvas::fitInView()
        {
            QGraphicsView::fitInView(this->sceneRect(), Qt::KeepAspectRatio);
        }

        void WellCanvas::keyPressEvent ( QKeyEvent *event )
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
                                QGraphicsView::fitInView( this->sceneRect(), Qt::KeepAspectRatio );
                                break;
                        }

                        default:
                        {
                                QGraphicsView::keyPressEvent ( event );
                                break;
                        }

                }
        }

        void WellCanvas::timerEvent ( QTimerEvent *event )
        {
                Q_UNUSED( event );

        }

        void WellCanvas::wheelEvent ( QWheelEvent *event )
        {
                scaleView ( std::pow ( (double) 2 , -event->delta ( ) / 240.0 ) );
        }

        void WellCanvas::drawBackground ( QPainter *painter , const QRectF &rect )
        {
                Q_UNUSED( rect );

    //            // Shadow
                //QRectF sceneRect = this->sceneRect ( );

                //QRectF rightShadow ( sceneRect.right ( ) , sceneRect.top ( ) + 5 , 5 , sceneRect.height ( ) );
                //QRectF bottomShadow ( sceneRect.left ( ) + 5 , sceneRect.bottom ( ) , sceneRect.width ( ) , 5 );
                //if ( rightShadow.intersects ( rect ) || rightShadow.contains ( rect ) )
                //		painter->fillRect ( rightShadow , Qt::darkGray );
                //if ( bottomShadow.intersects ( rect ) || bottomShadow.contains ( rect ) )
                //		painter->fillRect ( bottomShadow , Qt::darkGray );

                //// Fill
                //QLinearGradient gradient ( sceneRect.topLeft ( ) , sceneRect.bottomRight ( ) );
                //gradient.setColorAt ( 0 , Qt::white );
                //gradient.setColorAt ( 1 , Qt::lightGray );
                //painter->fillRect ( rect.intersected ( sceneRect ) , gradient );
                //painter->setBrush ( Qt::NoBrush );
                //painter->drawRect ( sceneRect );

                //// Text
                //QRectF textRect ( sceneRect.left ( ) + 4 , sceneRect.top ( ) + 4 , sceneRect.width ( ) - 4 , sceneRect.height ( ) - 4 );
                //QString message ( tr ( "Click and drag the nodes around, and zoom with the mouse " "wheel or the '+' and '-' keys" ) );

                //// Combine the transformation, thus it can be applied when zoom in/out
                ////painter->setTransform ( transformation_matrix_ , true );
                //QFont font = painter->font ( );
                //font.setBold ( true );
                //font.setPointSize ( 14 );
                //painter->setFont ( font );
                //painter->setPen ( Qt::lightGray );
                //painter->drawText ( textRect.translated ( 2 , 2 ) , message );
                //painter->setPen ( Qt::black );
                //painter->drawText ( textRect , message );
        }

        void WellCanvas::scaleView ( qreal scaleFactor )
        {
                qreal factor = transform ( ).scale ( scaleFactor , scaleFactor ).mapRect ( QRectF ( 0 , 0 , 1 , 1 ) ).width ( );
                if ( factor < 0.07 || factor > 100 )
                        return;

                scale ( scaleFactor , scaleFactor );
        }

        void WellCanvas::zoomIn ( )
        {
                scaleView ( qreal ( 1.2 ) );
        }

        void WellCanvas::zoomOut ( )
        {
                scaleView ( 1 / qreal ( 1.2 ) );
        }

        QTransform WellCanvas::getTransformationMatrix ( ) const
        {
                return transformation_matrix_;
        }
}
