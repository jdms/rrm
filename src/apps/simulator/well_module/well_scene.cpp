/*
 * WellScene.cpp
 *
 *  Created on: Sep 14, 2016
 *      Author: felipe
 */

#include "well_scene.hpp"

namespace RRM
{
        WellScene::WellScene ( QObject * parent  ) : QGraphicsScene ( parent )
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
//        this->wells_.resize ( 10 );
//
//        this->addItem ( new RRM::WellItem ( ) );
                this->board_ = nullptr;
                this->boundary_ = nullptr;

                this->createBoard(100, 100);

                number_of_well_items_ = 2;

                int index = 0;
                wells_item_[index] = new RRM::WellItem(board_, index + 1);
                /// @See http://lists.qt-project.org/pipermail/qt-interest-old/2010-June/024612.html
                /// Regarding the warning  QGraphicsScene::addItem item has already been	added to this scene Error
                //The usual way this happens is that when you create your QGraphicsItem you
                //specify a parent for the item that is already in the scene.In this case there
                //is no need to explicitly add it to the scene afterwards as the QGraphicsItem
                //constructor does it for you if the parent is already in a scene.
                //this->addItem(wells_item_[index]);
                wells_item_[index]->setPos(0,0);
                wells_item_[index]->setColor(Qt::blue);

                index = 1;
                wells_item_[index] = new RRM::WellItem(board_, index + 1);
                /// @See http://lists.qt-project.org/pipermail/qt-interest-old/2010-June/024612.html
                //this->addItem(wells_item_[index]);
                wells_item_[index]->setPos(board_->boundingRect().width(), board_->boundingRect().height());
                wells_item_[index]->setColor(Qt::red);

        }

        void WellScene::createBoard(double  _widht, double _height)
        {

            double offset = std::max(_height, _widht);
            double aspect_ratio = _height/_widht;
            double w_ratio = 1.0;
            double h_ratio = 1.0;

            if (aspect_ratio > 1.0)
            {
                h_ratio = _widht / _height;
            }
            else
            {
                w_ratio = aspect_ratio;
            }

            if (this->board_ == nullptr)
            {
                this->board_ = new QGraphicsRectItem(0, 0, _widht, _height);
                this->boundary_ = new QGraphicsRectItem(board_->boundingRect().x() - _widht*0.1*w_ratio,
                                                        board_->boundingRect().y() - _height*0.1*h_ratio,
                                                        board_->boundingRect().width() + _widht*0.2*w_ratio,
                                                        board_->boundingRect().height() + _height*0.2*h_ratio);

                this->addItem(this->board_);
                this->addItem(this->boundary_);
            }
            else
            {
                this->board_->setRect(0, 0, _widht, _height);
                this->boundary_->setRect(board_->boundingRect().x() - _widht*0.1*w_ratio,
                                         board_->boundingRect().y() - _height*0.1*h_ratio,
                                         board_->boundingRect().width() + _widht*0.2*w_ratio,
                                         board_->boundingRect().height() + _height*0.2*h_ratio);
            }


            /// this is needed to centralize the board in the middle of the widget
            this->setSceneRect(this->boundary_->rect());
        }

        void WellScene::setTranformationMatrix ( const QTransform& _transform )
        {
                this->tranformation_matrix_ = _transform;

                //proxyControl->setTransform(_transform);

//        for ( auto item : this->items())
//        {
//                item->setTransform(_transform);
//        }

                update ( );
        }

        QRectF WellScene::fitView() const
        {
                return board_->rect();
        }

        void WellScene::mousePressEvent ( QGraphicsSceneMouseEvent* event )
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

        void WellScene::mouseMoveEvent ( QGraphicsSceneMouseEvent* event )
        {
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

        void WellScene::mouseReleaseEvent ( QGraphicsSceneMouseEvent* event )
        {
                std::map<int, Eigen::Vector4d> region_points;



                for (auto index : wells_item_)
                {
                    /// @TODO , still confused

                    region_points[index.first] = Eigen::Vector4d(index.second->pos().x(), index.second->pos().y(), 0.0, 0.0);

                }

                emit updatedWellsPosition(region_points);

                QGraphicsScene::mouseReleaseEvent ( event );
        }

        void WellScene::setDimension ( const Eigen::Vector2f& _dim, bool _bounding_box_changed  )
        {
                std::map<int, Eigen::Vector4d> region_points;

                this->createBoard(_dim.x(), _dim.y());

                if (_bounding_box_changed)
                {

                    //int index = 0;
                    wells_item_[0]->setPos(0, 0);
                    //index = 1;
                    wells_item_[1]->setPos(board_->boundingRect().width(), board_->boundingRect().height());

                    for (auto index : wells_item_)
                    {

                        if (index.first > 1)
                        {
                            wells_item_[index.first]->setPos((wells_item_[index.first]->boundingRect().width() * index.first), board_->boundingRect().height());
                        }

                        /// @TODO , still confused
                        region_points[index.first] = Eigen::Vector4d(index.second->pos().x(), index.second->pos().y(), 0.0, 0.0);

                    }

                    emit updatedWellsPosition(region_points);
                }
                else
                {
                    for (auto index : wells_item_)
                    {

                        /// @TODO , still confused
                        region_points[index.first] = Eigen::Vector4d(wells_item_[index.first]->pos().x(), wells_item_[index.first]->pos().y(), 0.0, 0.0);

                    }
                }

                update();
        }

        void WellScene::createWells(int _number_of_wells, const std::map<int, int>& _types)
        {
                std::cout << " _number_of_Wells " << _types.size() << std::endl;

                number_of_well_items_ = _number_of_wells;

                wellItemVisibility(false);

                for ( auto index : _types )
                {
                        if ( !wells_item_.count ( index.first ) )
                        {
                            wells_item_[index.first] = new RRM::WellItem(board_, index.first + 1);
                            //this->addItem(wells_item_[index.first]);
                            wells_item_[index.first]->setPos((wells_item_[index.first]->boundingRect().width() * index.first), board_->boundingRect().height());

                            /// FIXME June 2017
                            if (index.second == -1) /// injector
                            {
                                wells_item_[index.first]->setColor(Qt::red);
                            }
                            else if (index.second == 1) /// producer
                            {
                                wells_item_[index.first]->setColor(Qt::blue);
                            }
                            else
                            {

                            }

                        }
                        else
                        {
                            /// FIXME June 2017
                            if (index.second == -1) /// injector
                            {
                                wells_item_[index.first]->setColor(Qt::red);
                            }
                            else if (index.second == 1) /// producer
                            {
                                wells_item_[index.first]->setColor(Qt::blue);
                            }
                            else
                            {

                            }

                            wells_item_[index.first]->setVisible(true);
                        }
                }

                std::map<int, Eigen::Vector4d> region_points;

                for (auto index : wells_item_)
                {
                    /// @TODO , still confused
                    region_points[index.first] = Eigen::Vector4d(index.second->pos().x(), index.second->pos().y(), 0.0, 0.0);
                }

                emit updatedWellsPosition(region_points);

                wellItemVisibility(true);

                this->update();
        }

        /// Flow regions releated functions
        void WellScene::wellItemVisibility(bool _is_visible)
        {

            for (auto &item : wells_item_)
            {
                if (item.first >= number_of_well_items_)
                {
                    item.second->setVisible(false);
                }
                else
                {
                    item.second->setVisible(true);
                }
            }

            for (auto index = 0; index < number_of_well_items_; index++)
            {
                if (wells_item_.count(index))
                {
                        wells_item_[index]->setVisible(_is_visible);
                }
            }
        }
}

