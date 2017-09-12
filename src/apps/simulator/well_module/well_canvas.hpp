/*
 * Canvas.hpp
 *
 *  Created on: Sep 13, 2016
 *      Author: felipe
 */

#ifndef _CANVAS_HPP_
#define _CANVAS_HPP_

#include <QtWidgets/QGraphicsView>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>


#include <cmath>

#include "well_scene.hpp"
namespace RRM
{
        class WellCanvas: public QGraphicsView
        {
                Q_OBJECT

                public:
                        WellCanvas ( QWidget *parent = 0 );

                        void itemMoved ( );
                        QTransform getTransformationMatrix ( ) const;
                        QGraphicsRectItem * canvasArea();

                public slots:
                        void zoomIn ( );
                        void zoomOut ( );
                        void fitInView();

                protected:
                        void keyPressEvent ( QKeyEvent *event ) override;

                        void timerEvent ( QTimerEvent *event ) override;

                        void wheelEvent ( QWheelEvent *event ) override;

                        void drawBackground ( QPainter *painter , const QRectF &rect ) override;

                        void scaleView ( qreal scaleFactor );

                private:
                        int timerId;
                        QTransform transformation_matrix_;

        };
}
#endif /* _CANVAS_HPP_ */
