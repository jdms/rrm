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

/**
 *  @file    WellItem.hpp
 *  @author  Felipe Moura de Carvalho (fmourade)
 *  @date    13/12/2016
 */

#include "well_item.hpp"

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QStyleOption>

#include <iostream>

namespace RRM
{

        WellItem::WellItem(QGraphicsItem *parent) : QGraphicsItem(parent)
        {
            this->setup();
        }

        WellItem::WellItem(QGraphicsItem *parent, int _id) : QGraphicsItem(parent)
        {
            this->setup();
            this->id_ = _id;
            color_ = Qt::gray;
        }


        void WellItem::setup ( )
        {
                /// QGraphicsItem setup
                setFlag ( ItemIsSelectable );
                setFlag ( ItemIsMovable );
                setFlag ( ItemSendsGeometryChanges );
                setCacheMode ( NoCache );
                setZValue ( 1 );
                /// Region setup
                this->id_ = -1;
				scale_factor = 3;
        }

        QRectF WellItem::boundingRect ( ) const
        {
                QPolygon poly;

				poly << QPoint(0, -30 * scale_factor);
				poly << QPoint(15 * scale_factor, 0);
				poly << QPoint(0, 30 * scale_factor);
				poly << QPoint(-15 * scale_factor, 0);

                return poly.boundingRect ( );
        }

        QPainterPath WellItem::shape ( ) const
        {
                QPainterPath path;

                QPolygon poly;
				poly << QPoint(0, -12 * scale_factor);
				poly << QPoint(10 * scale_factor, 0);
				poly << QPoint(0, 12 * scale_factor);
				poly << QPoint(-10 * scale_factor, 0);

                path.addPolygon ( poly );
                return path;
        }

        void WellItem::paint ( QPainter *painter , const QStyleOptionGraphicsItem *option , QWidget * widget )
        {
                painter->setRenderHints ( QPainter::SmoothPixmapTransform | QPainter::Antialiasing , true );
                painter->setRenderHints ( QPainter::TextAntialiasing , true );
                //@XXX Revert QGraphicsView Scale to the default coordinate System.
                painter->scale ( 1 , -1 );

                QRadialGradient gradient ( -3 , -3 , 10 );
                if ( option->state & QStyle::State_Sunken )
                {
                    gradient.setColorAt(0, this->color_);
                    gradient.setColorAt(1, QColor(Qt::white).light(80));
                }
                else
                {
                        gradient.setCenter(3, 3);
                        gradient.setFocalPoint(3, 3);
                        gradient.setColorAt(1, QColor(this->color_).light(80));
                        gradient.setColorAt(0, QColor(this->color_).light(120));
                }

                painter->setBrush ( gradient );
                painter->setPen ( QPen ( Qt::black , 2 , Qt::SolidLine , Qt::RoundCap , Qt::RoundJoin ) );

                QPolygon poly;
				poly << QPoint(0, -12 * scale_factor);
				poly << QPoint(10 * scale_factor, 0);
				poly << QPoint(0, 12 * scale_factor);
				poly << QPoint(-10 * scale_factor, 0);

				int factor = 3;
                //painter->drawPolygon ( poly );
				painter->drawEllipse(-6 * scale_factor, -6 * scale_factor, 12 * scale_factor, 12 * scale_factor);

                /// FIMEX maybe be useful in the future
                //painter->drawEllipse(QPoint(0, -18), 3, 3);

                painter->setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                painter->setFont ( QFont ( "Arial" , 8 , QFont::Bold ) );
                // I have used the .adjusted() to make the text centralized on the diamond.
                painter->drawText ( poly.boundingRect ( ).adjusted ( poly.boundingRect ( ).width ( ) * ( -0.05 ) , poly.boundingRect ( ).height ( ) * ( -0.1 ) , 0 , 0 ) , Qt::AlignCenter , QString::number ( this->id_ ) );

        }

        QVariant WellItem::itemChange ( GraphicsItemChange change , const QVariant &value )
        {
                switch ( change )
                {
                        case QGraphicsItem::ItemPositionChange:
                        {
                            qreal adjust = 0.5;

                            const QRectF parentRect(parentItem()->sceneBoundingRect());

                            //std::cout << "parent " << parentRect.right() - 0.5 << " " << parentRect.left() + 0.5 << " " << parentRect.top() + 0.5 << " " << parentRect.bottom() - 0.5 << std::endl;

                            qreal right  = parentRect.right() - adjust;
                            qreal left   = parentRect.left() + adjust;
                            qreal top    = parentRect.top() + adjust;
                            qreal bottom = parentRect.bottom() - adjust;

                            QPointF newPos(value.toPointF());

                            if (newPos.x() > right)
                            {
                                newPos.setX(right);
                            }
                            if (newPos.x() < left)
                            {
                                newPos.setX(left);
                            }
                            if (newPos.y() < top)
                            {
                                newPos.setY(top);
                            }
                            if (newPos.y() > bottom)
                            {
                                newPos.setY(bottom);
                            }

                            return newPos;
                            break;
                        }
                        default:
                        {
                                break;
                        }
                };

                return QGraphicsItem::itemChange ( change , value );
        }

        void WellItem::mousePressEvent ( QGraphicsSceneMouseEvent *event )
        {
                if (event->buttons( ) & Qt::LeftButton)
                {
                        QGraphicsItem::mousePressEvent ( event );
                }

        }

        void WellItem::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
        {

                if (event->buttons( ) & Qt::LeftButton)
                {
                        QGraphicsItem::mouseMoveEvent ( event );
                }
        }


        void WellItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent *event )
        {

                QGraphicsItem::mouseReleaseEvent ( event );
        }

        // Region setup
        void WellItem::setID ( const int _id )
        {
                this->id_ = _id;
        }

        int WellItem::getID ( ) const
        {
                return this->id_;
        }

        void WellItem::setColor(const QColor _color)
        {
            this->color_ = _color;
        }

        QColor WellItem::getColor() const
        {
            return this->color_;
        }
}

