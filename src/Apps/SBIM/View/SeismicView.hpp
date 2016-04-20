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

#include <QtWidgets/QLabel>

#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>


namespace RRM
{

	class SeismicView : public QGraphicsView
	{
		Q_OBJECT

		public:
			SeismicView ( QWidget* parent = 0);
			~SeismicView ( );

		public:
			QGraphicsScene* scene_;

	};

} /* namespace RRM */

#endif /* _SBIM_SEISMICVIEW_HPP_ */
