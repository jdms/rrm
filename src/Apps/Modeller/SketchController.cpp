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

	std::map<unsigned int, QPolygonF> view_curves_;

	QPolygonF view_curve;

	for ( auto& curves_iterator: cross_section_.edges_ )
	{
		view_curve = this->convertCurves(curves_iterator.second.segment.curve);

		view_curves_[curves_iterator.first] = view_curve;
	}

	emit updateSBIM(view_curves_);

}

QPolygonF SketchController::convertCurves ( Curve2D& _curve )
{
	QPolygonF polygonal_curve;

	polygonal_curve.resize(_curve.size());

	for (std::size_t it = 0; it < _curve.size(); it++)
	{
		polygonal_curve[it] = QPointF(_curve[it].x(),_curve[it].y());
	}

	return polygonal_curve;
}

SketchController::Curve2D SketchController::convertCurves ( QPolygonF _polygon )
{
	Curve2D polygonal_curve;

	for (std::size_t it = 0; it < _polygon.size(); it++)
	{
		// \fixme curvendemisional doenst have a clear interface over the its polyline ...
		polygonal_curve.push_back( SketchController::Point2D(_polygon[it].x(),_polygon[it].y()) );
	}

	return polygonal_curve;
}

SketchController::~SketchController ( )
{
	// TODO Auto-generated destructor stub
}

