// Data created August 6, 2015
#ifndef _SURFACE_HPP_
#define _SURFACE_HPP_

#include <QtCore/QtMath>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLabel>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QSurfaceFormat>
#include <QtGUI/QTransform>
#include <QtSVG/QSvgGenerator>

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
	// clear
	void clear();
	void setCrossSection( const CrossSection& _cross_section );
	/// default Scene
	void newSession ( Real x , Real y , Real width , Real height );
	/// defaul Scene with Image
	void newSession ( const QPixmap& _image );

	void setBoundary ( Real x , Real y , Real width , Real height );
	void screenShot();
	/// SVG Image
	void saveVector(const  QString& _filename);
	/// Raster Image
	void saveRaster(const  QString& _filename);
	void newBoundary();
	void newSketch();

	void undo();
	void redo();
	void zoomIn();
	void zoomOut();

	void setModeSketch();
	void setModeRegionPoint();
	void setModeRemoveAboveIntersection();
	void setModeRemoveBelowIntersection();
	void setModeEdition();
	
signals:
	void currentCrossSection( const CrossSection& _cross_section );

public:

	int scale_in_;
	int scale_out_;
	double scaleFactor = 1.15;

	QLabel * status_text_;

	SketchSessionTesting   *sketchSession_;
	SketchController       *sketch_controller;
};

#endif /* _SURFACE_HPP_ */
