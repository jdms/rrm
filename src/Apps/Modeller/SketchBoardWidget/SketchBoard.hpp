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
	typedef qreal		 	 	Real;
	typedef RRM::CrossSection<Real>  	CrossSection;

	SketchBoard( QWidget *parent = 0 );
	virtual ~SketchBoard();

	void wheelEvent ( QWheelEvent *event );
	void keyPressEvent ( QKeyEvent *event );

public slots:
	void setCrossSection( const CrossSection& _cross_section );
	/// default Scene
	void newSession ( Real x , Real y , Real width , Real height );
	/// defaul Scene with Image
	void newSession ( const QPixmap& _image );

	void setBoundary ( Real x , Real y , Real width , Real height );

signals:
	void currentCrossSection( const CrossSection& _cross_section );

public:

	int scale_in_;
	int scale_out_;

	SketchSessionTesting   *sketchSession_;
	SketchController       *sketch_controller;
};

#endif /* _SURFACE_HPP_ */
