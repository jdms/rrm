/*
 * SketchController.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: felipe
 */

#include "SketchController.hpp"

SketchController::SketchController ( CrossSection*   _cross_section,
			             SketchSession* _sketch_view ,
				     QObject*        parent) : QObject(parent)
{
	this->cross_section_ = _cross_section;
	this->sketch_view_   = _sketch_view;
}


void SketchController::smoothCurveSlot ( QPolygonF raw_sketch )
{
}

void SketchController::newSessionSlot ( QPixmap pixmap )
{
}

void SketchController::newSessionSlot ( qreal x , qreal y , qreal width , qreal height )
{
}

void SketchController::insertCurve ( QPolygonF _polygon )
{
}

QPolygonF SketchController::covertCurveModel_to_CurveView ( Curve2D& _curve )
{
}

SketchController::Curve2D SketchController::covertCurveView_to_CurveModel ( QPolygonF _polygon )
{
}

SketchController::~SketchController ( )
{
	// TODO Auto-generated destructor stub
}

