/*
 * SketchScene.hpp
 *
 *  Created on: Sep 14, 2016
 *      Author: felipe
 */

#ifndef _WELLSCENE_HPP_
#define _WELLSCENE_HPP_

#include <QtWidgets/QGraphicsProxyWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtCore/QtDebug>

#include <Eigen/Dense>
#include <map>

#include "RegionItem.hpp"

namespace RRM
{
        class RegionScene: public QGraphicsScene
        {
                Q_OBJECT
                public:

                        using Natural = unsigned int;

                        RegionScene ( QGraphicsRectItem* _board, QObject * parent = 0 );
                        void setTranformationMatrix ( const QTransform& _tranform );
                        QRectF fitView() const;
                public slots:
                        void createRegions ( std::size_t _number_of_Regions );
                signals:
                        void updatedRegionsPosition(const std::map< std::size_t, Eigen::Vector4d >&);
                        void mousePosition(const QPointF& _position);

                protected:
                        void mousePressEvent ( QGraphicsSceneMouseEvent* event ) override;
                        void mouseMoveEvent ( QGraphicsSceneMouseEvent* event ) override;
                        void mouseReleaseEvent ( QGraphicsSceneMouseEvent* event ) override;

                        void setDimension ( const Eigen::Vector2f& _dim );
                private:

                        /// It is needed to transform any widget from the QGraphicsView Coordinate System to ours
                        ///        Default Qt      Ours
                        ///        *----->         ^
                        ///  from  |         to    |
                        ///        |               *----->
                        ///
                        QTransform tranformation_matrix_;

                        /// It needs a proper cleanup procedure
                        std::map<std::size_t, RRM::RegionItem *> regions_item_;
                        std::size_t number_of_region_items_;

                        QGraphicsRectItem * board_;
                        //QGraphicsRectItem *proxyControl;

                        void regionItemVisibility(bool _is_visible);

        };

}

#endif /* _WELLSCENE_HPP_ */