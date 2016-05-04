// Data created August 6, 2015

#ifndef _SURFACE_HPP_
#define _SURFACE_HPP_

#include <QtWidgets/QGraphicsView>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QSurfaceFormat>

#include "Modeller/SketchBoardWidget/SketchSession/SketchSessionTesting.hpp"

#include "Modeller/SketchController.hpp"

/**
 * @brief Contains the scene (class SketchSession).
 *
 * */

class SketchBoard : public QGraphicsView
{
	Q_OBJECT

public:
	typedef SketchController::CrossSection  CrossSection;

	SketchBoard( RRM::CrossSection<qreal>& _cross_section,QWidget *parent = 0 );
	virtual ~SketchBoard();


	void wheelEvent ( QWheelEvent *event );
	void keyPressEvent ( QKeyEvent *event );

public slots:
	void setCrossSection( CrossSection& _cross_section, const std::vector<unsigned char>& _overlay_image);

signals:
	void currentCrossSection( CrossSection& _cross_section );

public:

	int scale_in_;
	int scale_out_;

	SketchSessionTesting   *sketchSession_;
	SketchController       *sketch_controller;
};

#endif /* _SURFACE_HPP_ */
