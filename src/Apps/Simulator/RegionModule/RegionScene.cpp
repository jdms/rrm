/*
 * RegionScene.cpp
 *
 *  Created on: Sep 14, 2016
 *      Author: felipe
 */

#include "RegionScene.hpp"

namespace RRM
{
        RegionScene::RegionScene ( QGraphicsRectItem* _board, QObject * parent  ) : QGraphicsScene ( parent )
        {

//        QVBoxLayout * layout = new QVBoxLayout ( );
//        QPushButton *  p = new QPushButton ( "Hello" );
//        QWidget *        w = new QWidget ( );
//
//        layout->addWidget ( p );
//
//        w->setLayout ( layout );
//
//        w->resize ( layout->sizeHint ( ) );
//
//        proxyControl = this->addRect ( w->rect().x(), w->rect().y() + w->rect().height(), w->rect().width(), w->rect().height(), QPen ( Qt::black ) , QBrush ( Qt::darkGreen ) ); // widget->width() works properly here because of the resize(layout->sizeHint()) that we have used inside it
//        proxyControl->setFlag ( QGraphicsItem::ItemIsMovable , true );
//        proxyControl->setFlag ( QGraphicsItem::ItemIsSelectable , true );
//        // Create the proxy by adding the widget to the scene
//        QGraphicsProxyWidget * const proxy = this->addWidget ( w );
//        // In my case the rectangular graphics item is supposed to be above my widget so the position of the widget is shifted along the Y axis based on the height of the rectangle of that graphics item
//        proxy->setPos ( 0 , 0 + proxyControl->rect ( ).height ( ) );
//        proxy->setParentItem ( proxyControl );
//
//        proxyControl->setPos ( 800 , 0 );
//
//        //proxyControl->setTransform ( tranformation_matrix_ );
//
//        this->regions_.resize ( 10 );
//
//        this->addItem ( new RRM::RegionItem ( ) );

                this->board_ = _board;
                this->addItem ( this->board_ );
                number_of_region_items_ = 0;

        }

        void RegionScene::setTranformationMatrix ( const QTransform& _transform )
        {
                this->tranformation_matrix_ = _transform;

                //proxyControl->setTransform(_transform);

//        for ( auto item : this->items())
//        {
//                item->setTransform(_transform);
//        }

                update ( );
        }

        QRectF RegionScene::fitView() const
        {
                return board_->rect();
        }

        void RegionScene::mousePressEvent ( QGraphicsSceneMouseEvent* event )
        {
                // The shift modifier triggered QGraphicsScene internal selection
                // Feature
                if ( event->modifiers ( ) & Qt::ShiftModifier )
                {

                }
                else
                {
                        if ( event->buttons ( ) & Qt::LeftButton )
                        {
                                QGraphicsScene::mousePressEvent ( event );
                        }
                        else if ( event->buttons ( ) & Qt::RightButton )
                        {

                        }
                        else if ( event->buttons ( ) & Qt::MiddleButton )
                        {

                        }
                }

                update ( );
        }

        void RegionScene::mouseMoveEvent ( QGraphicsSceneMouseEvent* event )
        {
                emit this->mousePosition(event->scenePos());

                if ( event->modifiers ( ) & Qt::ShiftModifier )
                {
                        event->accept ( );
                }
                else
                {
                        if ( event->buttons ( ) & Qt::LeftButton )
                        {
                                QGraphicsScene::mouseMoveEvent ( event );
                        }
                }
                update ( );
        }

        void RegionScene::mouseReleaseEvent ( QGraphicsSceneMouseEvent* event )
        {
                std::map<std::size_t, Eigen::Vector4d> region_points;

                for (auto index : regions_item_)
                {
                    region_points[index.first] = Eigen::Vector4d(index.second->pos().x(), index.second->pos().y(),0.0,0.0);
                }

                emit updatedRegionsPosition(region_points);

                QGraphicsScene::mouseReleaseEvent ( event );
        }

        void RegionScene::setDimension ( const Eigen::Vector2f& _dim )
        {
                if ( this->board_ != nullptr )
                {
                        board_->setRect ( QRectF ( 0.0f , 0.0f , _dim.x ( ) , _dim.y ( ) ) );
                }
        }

        void RegionScene::createRegions ( std::size_t _number_of_regions )
        {

                regionItemVisibility(false);

                std::cout << " _number_of_Wells " << _number_of_regions << std::endl;

                number_of_region_items_ = _number_of_regions;

                for ( auto index = 0; index < _number_of_regions; index++ )
                {
                        if ( !regions_item_.count ( index ) )
                        {
                                regions_item_[index] = new RRM::RegionItem ( index + 1 );
                                this->addItem ( regions_item_[index] );
                                regions_item_[index]->setPos ( ( regions_item_[index]->boundingRect ( ).width ( ) * index ) , board_->boundingRect ( ).height ( ) );
                        }
                        else
                        {
                                regions_item_[index]->setVisible ( true );
                        }
                }

                regionItemVisibility(true);

                this->update();
        }

        /// Flow regions releated functions
        void RegionScene::regionItemVisibility(bool _is_visible)
        {
            for (auto index = 0; index < number_of_region_items_; index++)
            {
                if (regions_item_.count(index))
                {
                        regions_item_[index]->setVisible(_is_visible);
                }
            }
        }
}
