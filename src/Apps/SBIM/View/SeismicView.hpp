/*
 * SeismicView.hpp
 *
 *  Created on: Apr 14, 2016
 *      Author: felipe
 */

#ifndef _SBIM_SEISMICVIEW_HPP_
#define _SBIM_SEISMICVIEW_HPP_


#include <QtWidgets/QGraphicsView>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>

#include "ui_SeismicView.h"

namespace RRM
{

	class SeismicView : public QGraphicsView
	{
		public:
			SeismicView ( QWidget* parent = 0);
			~SeismicView ( );

			void wheelEvent ( QWheelEvent *event );
			void keyPressEvent ( QKeyEvent *event );
		private:

			Ui::SeismicViewerWidget * ui;


	};

} /* namespace RRM */

#endif /* _SBIM_SEISMICVIEW_HPP_ */
