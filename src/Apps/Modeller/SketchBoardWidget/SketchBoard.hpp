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
	SketchBoard( RRM::CrossSection<qreal>& _cross_section,QWidget *parent = 0 );
	virtual ~SketchBoard();


	void wheelEvent ( QWheelEvent *event );
	void keyPressEvent ( QKeyEvent *event );
private:

	int scale_in_;
	int scale_out_;

	SketchSessionTesting   *sketchSession_;
	SketchController       *sketch_controller;
};

#endif /* _SURFACE_HPP_ */
