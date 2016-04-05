/*
 * SketchController.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: felipe
 */

#include "SketchController.hpp"

SketchController::SketchController ( CrossSection*   _cross_section,
			             SketchSessionTesting* _sketch_view ,
				     QObject*        parent) : QObject(parent)
{
	this->cross_section_ = _cross_section;
	this->sketch_view_   = _sketch_view;

	arrangment.initialize( );

	arrangment.log();

	connect ( this->sketch_view_ , SIGNAL( smoothCurve(QPolygonF) ) , this , SLOT( insertCurve(QPolygonF) ) );

	// Sketching

}

void SketchController::newSession ( qreal x , qreal y , qreal width , qreal height )
{
	this->sketch_view_->initialization ( x , y , width , height );// THE VIEW
	this->cross_section_->initialization ( x , y , width , height ); // THE MODEL
}

void SketchController::newSession ( QPixmap pixmap )
{
	this->sketch_view_->initializationWithImage ( pixmap );  // THE VIEW
	this->cross_section_->initialization ( pixmap.rect ( ).x ( ) ,   // THE MODEL
                                               pixmap.rect ( ).y ( ) ,
					       pixmap.rect ( ).width ( ) ,
					       pixmap.rect ( ).height ( ) );
}

void SketchController::insertCurve ( QPolygonF _polygon )
{
	Curve2D curve = convertCurves(_polygon);

	unsigned int id = arrangment.insertCurve(curve);

	sketch_view_->insertCurve(id,_polygon);

	arrangment.log();

}

QPolygonF SketchController::convertCurves ( Curve2D& _curve )
{
	return QPolygonF();
}

SketchController::Curve2D SketchController::convertCurves ( QPolygonF _polygon )
{
	return Curve2D();
}

SketchController::~SketchController ( )
{
	// TODO Auto-generated destructor stub
}

