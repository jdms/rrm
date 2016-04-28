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
#include "SBIM/View/SeismicThumbnailItem.hpp"

#include <iostream>


namespace RRM
{
	class SeismicWindow: public QWidget
	{
		Q_OBJECT

		public:
			typedef SBIMSeismicController::CrossSection  CrossSection;

			SeismicWindow ( QWidget* parent = 0 );
			~SeismicWindow ( );

			void wheelEvent ( QWheelEvent *event );
			void keyPressEvent ( QKeyEvent *event );
			void showEvent( QShowEvent *);

		public slots:
			bool addSeismicSlice ( );
			void setCurrentSeismicSlice( QListWidgetItem * item );
			void updateCrossSection (const CrossSection& _cross_section);

		signals:
			void currentCrossSection (const CrossSection& _seismic_slice, const QPixmap& _overlay_image );
		public:
			/// View
			Ui::SeismicWindowWidget* ui;
			SeismicView* seismic_viewer_;

			std::map<unsigned int , QGraphicsItem*> lines_checkpoint_;
			std::map<unsigned int , QPixmap*> sketch_images_;

			/// RRM Seismic Module
			SBIMSeismicController sketch_seismic_controller_;

	};

} /* namespace RRM */

#endif /* _SBIM_SEISMICWINDOW_HPP_ */
