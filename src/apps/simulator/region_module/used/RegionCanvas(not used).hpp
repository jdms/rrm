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


#ifndef _REGIONCANVAS_HPP_
#define _REGIONCANVAS_HPP_

#include <QtWidgets/QGraphicsView>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>


#include <cmath>

#include "RegionScene.hpp"
namespace RRM
{
        class RegionCanvas: public QGraphicsView
        {
                Q_OBJECT

                public:
                        RegionCanvas ( QWidget *parent = 0 );

                        void itemMoved ( );
                        QTransform getTransformationMatrix ( ) const;
                        QGraphicsRectItem * canvasArea();

                public slots:
                        void zoomIn ( );
                        void zoomOut ( );

                protected:
                        void keyPressEvent ( QKeyEvent *event ) override;

                        void timerEvent ( QTimerEvent *event ) override;

                        void wheelEvent ( QWheelEvent *event ) override;

                        void drawBackground ( QPainter *painter , const QRectF &rect ) override;

                        void scaleView ( qreal scaleFactor );

                private:

                        int timerId;
                        QGraphicsRectItem * board_;
                        QTransform transformation_matrix_;

        };
}
#endif /* _REGIONCANVAS_HPP_ */
