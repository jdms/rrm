/*
 * SeismicView.hpp
 *
 *  Created on: Apr 14, 2016
 *      Author: felipe
 */

#ifndef _SBIM_SEISMICVIEW_HPP_
#define _SBIM_SEISMICVIEW_HPP_


#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsItem>

#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>

#include "ui_SeismicView.h"

namespace RRM
{

	class SeismicView : public QWidget
	{
		Q_OBJECT

		public:
			SeismicView ( QWidget* parent = 0);
			~SeismicView ( );

			void wheelEvent ( QWheelEvent *event );
			void keyPressEvent ( QKeyEvent *event );
			void fitInView ( );
		public:

			Ui::SeismicViewerWidget * ui;

			QGraphicsView  *image_viewer;

			QGraphicsScene * scene;


	};

} /* namespace RRM */

#endif /* _SBIM_SEISMICVIEW_HPP_ */
