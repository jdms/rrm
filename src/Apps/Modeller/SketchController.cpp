/*
 * SketchController.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: felipe
 */

#include "SketchController.hpp"

SketchController::SketchController ( QObject* parent) : QObject(parent)
{
	next = 0;
	// Sketching
}

SketchController::~SketchController ( )
{
	// TODO Auto-generated destructor stub
}

void SketchController::clear ( )
{
	//this->cross_section_.clear();
	this->cross_section_.initialize(this->cross_section_.viewPort_.first.x(),
					this->cross_section_.viewPort_.first.y(),
					this->cross_section_.viewPort_.second.x(),
					this->cross_section_.viewPort_.second.y());

	this->cross_section_redo_ = this->cross_section_;
	this->cross_section_undo_ = this->cross_section_;

	updateSBIM();
}

void SketchController::setRule( RRM::GeologicRules _update_rule)
{
	this->cross_section_.setRule(_update_rule);
}

void SketchController::newSession ( qreal x , qreal y , qreal width , qreal height )
{
	cross_section_.initialize ( x , y , width , height ); // THE MODEL
	this->cross_section_redo_ = this->cross_section_;
	this->cross_section_undo_ = this->cross_section_;
	updateSBIM();
}

void SketchController::newSession ( QPixmap pixmap )
{
//	this->cross_section_->initialization ( pixmap.rect ( ).x ( ) ,   // THE MODEL
//                                             pixmap.rect ( ).y ( ) ,
//					       pixmap.rect ( ).width ( ) ,
//					       pixmap.rect ( ).height ( ) );
}

void SketchController::insertCurve(QPolygonF _raw_sketch_curve, QColor _color)
{

// insert the curve on the current crossSection and notify the view

	Curve2D curve = convertCurves(_raw_sketch_curve);
//
	CrossSection tmp = this->cross_section_;

	unsigned int result = cross_section_.insertCurve(curve, _color.redF(), _color.greenF(), _color.blueF());

	if (result == 1)
	{
		this->cross_section_redo_ = cross_section_;
		this->cross_section_undo_ = tmp;

		std::cout << "insert" << std::endl;

		this->updateSBIM();
	}
	

//	cross_section_.log();
//
//	if ( next == 0)
//	{
//		crossSection_1(this->cross_section_,1000);
//	}
//	if ( next == 1)
//	{
//		crossSection_2(this->cross_section_,1000);
//	}
//	if ( next == 2)
//	{
//		crossSection_3(this->cross_section_,1000);
//	}
//
//	next++;
//	cross_section_.log();

}
// updateSBIM with the new crossSection. Emit a Signal updateSBIM at the end to notify the view

void SketchController::updateSBIM (  )
{

	std::map<unsigned int, std::pair<QColor, QPolygonF> > view_curves_;
	std::map<unsigned int, QPointF>   view_vertices_;

	std::pair<QColor,QPolygonF> view_curve;

	for ( auto& curves_iterator: cross_section_.edges_ )
	{
		if (curves_iterator.second.is_visible_)
		{
			view_curve.first = QColor::fromRgbF(curves_iterator.second.r, curves_iterator.second.g, curves_iterator.second.b);
			view_curve.second = this->convertCurves(curves_iterator.second.segment.curve);
			view_curves_[curves_iterator.first].first = curves_iterator.second.segment.curve_index;
			view_curves_[curves_iterator.first] = view_curve;
		}
	}

	for ( auto& vertex_iterator: cross_section_.vertices_ )
	{

		view_vertices_[vertex_iterator.first] = QPointF(vertex_iterator.second.location_.x(),vertex_iterator.second.location_.y());
	}

	emit updateSBIM(view_curves_,view_vertices_);
}

/// @todo Create the appropriate boundary
void SketchController::setCrossSection( const RRM::CrossSection<qreal>& _cross_section)
{
	this->cross_section_ = _cross_section;

	this->cross_section_redo_ = this->cross_section_;
	this->cross_section_undo_ = this->cross_section_;

	this->updateSBIM();
}

SketchController::CrossSection SketchController::getCrossSection ( ) const
{
	return this->cross_section_;
}

void SketchController::undo()
{
	this->cross_section_ = cross_section_undo_;
	this->updateSBIM();
}

void SketchController::redo()
{
	this->cross_section_ = this->cross_section_redo_;
	this->updateSBIM();
}

QPolygonF SketchController::convertCurves ( Curve2D& _curve )
{
	QPolygonF polygonal_curve;

	polygonal_curve.resize(_curve.size());

	for (std::size_t it = 0; it < _curve.size(); it++)
	{
		// RRM coordinate System to Qt
		polygonal_curve[it] = QPointF(_curve[it].x(),-1*_curve[it].y());
	}

	return polygonal_curve;
}

SketchController::Curve2D SketchController::convertCurves ( QPolygonF _polygon )
{
	Curve2D polygonal_curve;

	for ( int it = 0; it < _polygon.size(); it++)
	{
		// \fixme curvendemisional doenst have a clear interface over the its polyline ...
		// RRM coordinate System to Qt
		polygonal_curve.push_back( SketchController::Point2D(_polygon[it].x(),-1*_polygon[it].y()) );
	}

	return polygonal_curve;
}


