/*
 * SketchController.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: felipe
 */

#include "SketchController.hpp"

SketchController::SketchController ( RRM::CrossSection<qreal>&   _cross_section,
				     QObject*        parent) : QObject(parent)
{
	this->cross_section_ = _cross_section;

	cross_section_.initialize( );

	cross_section_.log();

	// Sketching
}

void SketchController::newSession ( qreal x , qreal y , qreal width , qreal height )
{
//	this->cross_section_->initialization ( x , y , width , height ); // THE MODEL
}

void SketchController::newSession ( QPixmap pixmap )
{
//	this->cross_section_->initialization ( pixmap.rect ( ).x ( ) ,   // THE MODEL
//                                               pixmap.rect ( ).y ( ) ,
//					       pixmap.rect ( ).width ( ) ,
//					       pixmap.rect ( ).height ( ) );
}

void SketchController::insertCurve ( QPolygonF _polygon )
{
	Curve2D curve = convertCurves(_polygon);

	unsigned int id = cross_section_.insertCurve(curve);

	cross_section_.log();

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

