/*
 * SeismicWindow.hpp
 *
 *  Created on: Apr 14, 2016
 *      Author: felipe
 */

#ifndef _SBIM_SEISMICWINDOW_HPP_
#define _SBIM_SEISMICWINDOW_HPP_
// Qt5 includes
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsItem>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>

#include "ui_SeismicWindow.h"

// RRM Includes
#include "SBIM/SeismicSlice.hpp"
#include "SBIM/SBIMSeismicController.hpp"
#include "SBIM/View/SeismicView.hpp"


namespace RRM
{
	class SeismicWindow: public QWidget
	{
		Q_OBJECT

		public:
			SeismicWindow ( QWidget* parent = 0 );
			~SeismicWindow ( );

			void wheelEvent ( QWheelEvent *event );
			void keyPressEvent ( QKeyEvent *event );
			void fitInView ( );

		public slots:
			bool addSeismicSlice ( );

		public:
			/// View
			Ui::SeismicWindowWidget* ui;
			SeismicView* seismic_viewer_;

			std::map<unsigned int , QGraphicsItem*> lines_checkpoint_;
			std::map<unsigned int , QLabel*> sketch_images_;

			/// RRM Seismic Module
			SBIMSeismicController sketch_seismic_controller_;

	};

} /* namespace RRM */

#endif /* _SBIM_SEISMICWINDOW_HPP_ */
