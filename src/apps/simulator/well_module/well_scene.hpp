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

#include "well_item.hpp"

namespace RRM
{
        class WellScene: public QGraphicsScene
        {
                Q_OBJECT
                public:

                        using Natural = unsigned int;

                        WellScene ( QObject * parent = 0 );
                        void setTranformationMatrix ( const QTransform& _tranform );
                        QRectF fitView() const;
						void setDimension(const Eigen::Vector2f& _dim, bool _is_the_same = false);
                public slots:
						void createWells(int _number_of_wells, const std::map<int, int>& _types);
                signals:
                        void updatedWellsPosition(const std::map< int, Eigen::Vector4d >&);

                protected:
                        void mousePressEvent ( QGraphicsSceneMouseEvent* event ) override;
                        void mouseMoveEvent ( QGraphicsSceneMouseEvent* event ) override;
                        void mouseReleaseEvent ( QGraphicsSceneMouseEvent* event ) override;
                       
                private:

                        /// It is needed to transform any widget from the QGraphicsView Coordinate System to ours
                        ///        Default Qt      Ours
                        ///        *----->         ^
                        ///  from  |         to    |
                        ///        |               *----->
                        ///
                        QTransform tranformation_matrix_;

                        /// It needs a proper cleanup procedure
                        std::map<int, RRM::WellItem *> wells_item_;
                        std::size_t number_of_well_items_;

						void createBoard(double  _widht, double _height);

						// Where the item resides
                        QGraphicsRectItem * board_;
						// Helps in the centralization of the scene
						QGraphicsRectItem * boundary_;
                        //QGraphicsRectItem *proxyControl;	

                        void wellItemVisibility(bool _is_visible);

        };

}

#endif /* _WELLSCENE_HPP_ */
