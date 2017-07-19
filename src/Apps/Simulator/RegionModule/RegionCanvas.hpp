/*
 * RegionCanvas.hpp
 *
 *  Created on: Sep 13, 2016
 *      Author: felipe
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
