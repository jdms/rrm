/*
 * SketchController.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: felipe
 */

#include "SketchController.hpp"

SketchController::SketchController ( QObject*        parent) : QObject(parent)
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
	this->cross_section_.clear();
	this->cross_section_.initialize(0.0,0.0,700,400);
	updateSBIM();
}

void SketchController::newSession ( qreal x , qreal y , qreal width , qreal height )
{
	cross_section_.initialize ( x , y , width , height ); // THE MODEL
	updateSBIM();
}

void SketchController::newSession ( QPixmap pixmap )
{
//	this->cross_section_->initialization ( pixmap.rect ( ).x ( ) ,   // THE MODEL
//                                             pixmap.rect ( ).y ( ) ,
//					       pixmap.rect ( ).width ( ) ,
//					       pixmap.rect ( ).height ( ) );
}

void SketchController::insertCurve ( QPolygonF _polygon )
{

// insert the curve on the current crossSection and notify the view

	Curve2D curve = convertCurves(_polygon);
//
	unsigned int id = cross_section_.insertCurve(curve);

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
	std::cout << "insert" << std::endl;

	this->updateSBIM();
}

// updateSBIM with the new crossSection. Emit a Signal updateSBIM at the end to notify the view
void SketchController::updateSBIM (  )
{
	std::map<unsigned int, QPolygonF> view_curves_;
	std::map<unsigned int, QPointF>   view_vertices_;

	QPolygonF view_curve;

	for ( auto& curves_iterator: cross_section_.edges_ )
	{
		if (curves_iterator.second.is_visible_)
		{
			view_curve = this->convertCurves(curves_iterator.second.segment.curve);
			view_curves_[curves_iterator.first] = view_curve;
		}
	}

	for ( auto& vertex_iterator: cross_section_.vertices_ )
	{

		view_vertices_[vertex_iterator.first] = QPointF(vertex_iterator.second.location_.x(),vertex_iterator.second.location_.y());
	}

	emit updateSBIM(view_curves_,view_vertices_);
}
// Just Boundary
void SketchController::crossSection_1 (RRM::CrossSection<double>& _cross_section,double _scale)
{
	// Reset the ID manager
	_cross_section.initialize();

	// create tow vertices
	RRM::Vertex<double> v1;
	RRM::Vertex<double> v2;

	v1.id_ = _cross_section.vertex_index_.getID();
	v1.location_ = RRM::CrossSection<double>::Point2D(_scale*-1.0,_scale*-1.0);

	v2.id_ = _cross_section.vertex_index_.getID();
	v2.location_ = RRM::CrossSection<double>::Point2D(_scale*1.0,_scale*1.0);

	// create tow edges
	RRM::Edge<double> e1;
	RRM::Edge<double> e2;

	e1.id_ = _cross_section.edge_index_.getID();
	e2.id_ = _cross_section.edge_index_.getID();

	e1.source_id_ = v1.id_;
	e1.target_id_ = v2.id_;

	e2.source_id_ = v2.id_;
	e2.target_id_ = v1.id_;

	e1.is_boundary_ = true;
	e2.is_boundary_ = true;

	e1.segment.curve.add(RRM::CrossSection<double>::Point2D(v1.location_));
	e1.segment.curve.add(RRM::CrossSection<double>::Point2D(_scale*-1.0,_scale*1.0 ));
	e1.segment.curve.add(RRM::CrossSection<double>::Point2D(v2.location_));

	e2.segment.curve.add(RRM::CrossSection<double>::Point2D(v2.location_));
	e2.segment.curve.add(RRM::CrossSection<double>::Point2D(_scale*1.0,_scale*-1.0 ));
	e2.segment.curve.add(RRM::CrossSection<double>::Point2D(v1.location_));

	_cross_section.edges_[e1.id_] = e1;
	_cross_section.edges_[e2.id_] = e2;

	_cross_section.vertices_[v1.id_] = v1;
	_cross_section.vertices_[v2.id_] = v2;

	_cross_section.viewPort_.first  = Point2D(v1.location_);
	_cross_section.viewPort_.second = Point2D(v2.location_);

}
// Boundary + 1 Curve
void SketchController::crossSection_2 (RRM::CrossSection<double>& _cross_section, double scale)
{
	// Reset the ID manager
	_cross_section.initialize();

	// create tow vertices
	RRM::Vertex<double> v1;
	RRM::Vertex<double> v2;
	RRM::Vertex<double> v3;
	RRM::Vertex<double> v4;

	v1.id_ = _cross_section.vertex_index_.getID();
	v1.location_ = RRM::CrossSection<double>::Point2D(scale*-1.0,scale*-1.0);

	v2.id_ = _cross_section.vertex_index_.getID();
	v2.location_ = RRM::CrossSection<double>::Point2D(scale*1.0,scale*1.0);

	v3.id_ = _cross_section.vertex_index_.getID();
	v3.location_ = RRM::CrossSection<double>::Point2D(scale*-1.0,scale*0.0);

	v4.id_ = _cross_section.vertex_index_.getID();
	v4.location_ = RRM::CrossSection<double>::Point2D(scale*1.0,scale*0.0);


	// create tow edges
	RRM::Edge<double> e1;
	RRM::Edge<double> e2;
	RRM::Edge<double> e3;
	RRM::Edge<double> e4;
	RRM::Edge<double> e5;

	e1.id_ = _cross_section.edge_index_.getID();
	e2.id_ = _cross_section.edge_index_.getID();
	e3.id_ = _cross_section.edge_index_.getID();
	e4.id_ = _cross_section.edge_index_.getID();
	e5.id_ = _cross_section.edge_index_.getID();


	e1.is_boundary_ = true;
	e1.source_id_ = v3.id_;
	e1.target_id_ = v2.id_;

	e1.segment.curve.add(v3.location_);
	e1.segment.curve.add(RRM::CrossSection<double>::Point2D(scale*-1.0,scale*1.0));
	e1.segment.curve.add(v2.location_);


	e2.is_boundary_ = true;
	e2.source_id_ = v4.id_;
	e2.target_id_ = v1.id_;

	e2.segment.curve.add(v4.location_);
	e2.segment.curve.add(RRM::CrossSection<double>::Point2D(scale*1.0,scale*-1.0));
	e2.segment.curve.add(v1.location_);

	e3.is_boundary_ = true;
	e3.source_id_ = v2.id_;
	e3.target_id_ = v4.id_;

	e3.segment.curve.add(v2.location_);
	e3.segment.curve.add(v4.location_);

	e4.is_boundary_ = true;
	e4.source_id_ = v1.id_;
	e4.target_id_ = v3.id_;

	e4.segment.curve.add(v1.location_);
	e4.segment.curve.add(v3.location_);

	e5.is_boundary_ = false;
	e5.source_id_ = v3.id_;
	e5.target_id_ = v4.id_;

	e5.segment.curve.add(v3.location_);
	e5.segment.curve.add(RRM::CrossSection<double>::Point2D(scale*-0.5,scale*0.25));
	e5.segment.curve.add(RRM::CrossSection<double>::Point2D(scale*0.5,scale*-0.25));
	e5.segment.curve.add(v4.location_);

	e5.segment.curve.chaikinSubDivide(2);


	_cross_section.edges_[e1.id_] = e1;
	_cross_section.edges_[e2.id_] = e2;
	_cross_section.edges_[e3.id_] = e3;
	_cross_section.edges_[e4.id_] = e4;
	_cross_section.edges_[e5.id_] = e5;

	_cross_section.vertices_[v1.id_] = v1;
	_cross_section.vertices_[v2.id_] = v2;
	_cross_section.vertices_[v3.id_] = v3;
	_cross_section.vertices_[v4.id_] = v4;

	_cross_section.viewPort_.first  = Point2D(v1.location_);
	_cross_section.viewPort_.second = Point2D(v2.location_);

}

void SketchController::crossSection_3(RRM::CrossSection<double>& _cross_section, double scale)
{
	// Reset the ID manager
	_cross_section.initialize();

	// create tow vertices
	RRM::Vertex<double> v1;
	RRM::Vertex<double> v2;
	RRM::Vertex<double> v3;
	RRM::Vertex<double> v4;
	RRM::Vertex<double> v5;
	RRM::Vertex<double> v6;

	v1.id_ = _cross_section.vertex_index_.getID();
	v1.location_ = RRM::CrossSection<double>::Point2D(scale*-1.0,scale*-1.0);

	v2.id_ = _cross_section.vertex_index_.getID();
	v2.location_ = RRM::CrossSection<double>::Point2D(scale*1.0,scale*1.0);

	v3.id_ = _cross_section.vertex_index_.getID();
	v3.location_ = RRM::CrossSection<double>::Point2D(scale*-1.0,scale*0.0);

	v4.id_ = _cross_section.vertex_index_.getID();
	v4.location_ = RRM::CrossSection<double>::Point2D(scale*1.0,scale*0.0);

	v5.id_ = _cross_section.vertex_index_.getID();
	v5.location_ = RRM::CrossSection<double>::Point2D(scale*-0.25,scale*0.0);

	v6.id_ = _cross_section.vertex_index_.getID();
	v6.location_ = RRM::CrossSection<double>::Point2D(scale*0.25,scale*0.0);

	// create tow edges
	RRM::Edge<double> e1;
	RRM::Edge<double> e2;
	RRM::Edge<double> e3;
	RRM::Edge<double> e4;
	RRM::Edge<double> e5;
	RRM::Edge<double> e6;
	RRM::Edge<double> e7;
	RRM::Edge<double> e8;

	e1.id_ = _cross_section.edge_index_.getID();
	e2.id_ = _cross_section.edge_index_.getID();
	e3.id_ = _cross_section.edge_index_.getID();
	e4.id_ = _cross_section.edge_index_.getID();
	e5.id_ = _cross_section.edge_index_.getID();
	e6.id_ = _cross_section.edge_index_.getID();
	e7.id_ = _cross_section.edge_index_.getID();
	e8.id_ = _cross_section.edge_index_.getID();


	e1.is_boundary_ = true;
	e1.source_id_ = v3.id_;
	e1.target_id_ = v2.id_;

	v3.edges_.insert(e1.id_);
	v2.edges_.insert(e1.id_);

	e1.segment.curve.add(v3.location_);
	e1.segment.curve.add(RRM::CrossSection<double>::Point2D(scale*-1.0,scale*1.0));
	e1.segment.curve.add(v2.location_);


	e2.is_boundary_ = true;
	e2.source_id_ = v4.id_;
	e2.target_id_ = v1.id_;

	v1.edges_.insert(e2.id_);
	v4.edges_.insert(e2.id_);

	e2.segment.curve.add(v4.location_);
	e2.segment.curve.add(RRM::CrossSection<double>::Point2D(scale*1.0,scale*-1.0));
	e2.segment.curve.add(v1.location_);

	e3.is_boundary_ = true;
	e3.source_id_ = v2.id_;
	e3.target_id_ = v4.id_;

	v2.edges_.insert(e3.id_);
	v4.edges_.insert(e3.id_);

	e3.segment.curve.add(v2.location_);
	e3.segment.curve.add(v4.location_);

	e4.is_boundary_ = true;
	e4.source_id_ = v1.id_;
	e4.target_id_ = v3.id_;

	v1.edges_.insert(e4.id_);
	v3.edges_.insert(e4.id_);

	e4.segment.curve.add(v1.location_);
	e4.segment.curve.add(v3.location_);

	e5.is_boundary_ = false;
	e5.source_id_ = v3.id_;
	e5.target_id_ = v5.id_;

	v3.edges_.insert(e5.id_);
	v5.edges_.insert(e5.id_);

	e5.segment.curve.add(v3.location_);
	e5.segment.curve.add(v5.location_);

	e6.is_boundary_ = false;
	e6.source_id_ = v5.id_;
	e6.target_id_ = v6.id_;

	v5.edges_.insert(e6.id_);
	v6.edges_.insert(e6.id_);

	e6.segment.curve.add(v5.location_);
	e6.segment.curve.add(RRM::CrossSection<double>::Point2D(scale*0.0,scale*-0.25));
	e6.segment.curve.add(v6.location_);

	e7.is_boundary_ = false;
	e7.source_id_ = v5.id_;
	e7.target_id_ = v6.id_;

	v5.edges_.insert(e7.id_);
	v6.edges_.insert(e7.id_);

	e7.segment.curve.add(v5.location_);
	e7.segment.curve.add(v6.location_);

	e8.is_boundary_ = false;
	e8.source_id_ = v6.id_;
	e8.target_id_ = v4.id_;

	v4.edges_.insert(e8.id_);
	v6.edges_.insert(e8.id_);

	e8.segment.curve.add(v6.location_);
	e8.segment.curve.add(v4.location_);


	e6.segment.curve.chaikinSubDivide(2);

	_cross_section.edges_[e1.id_] = e1;
	_cross_section.edges_[e2.id_] = e2;
	_cross_section.edges_[e3.id_] = e3;
	_cross_section.edges_[e4.id_] = e4;
	_cross_section.edges_[e5.id_] = e5;
	_cross_section.edges_[e6.id_] = e6;
	_cross_section.edges_[e7.id_] = e7;
	_cross_section.edges_[e8.id_] = e8;

	_cross_section.vertices_[v1.id_] = v1;
	_cross_section.vertices_[v2.id_] = v2;
	_cross_section.vertices_[v3.id_] = v3;
	_cross_section.vertices_[v4.id_] = v4;
	_cross_section.vertices_[v5.id_] = v5;
	_cross_section.vertices_[v6.id_] = v6;

	_cross_section.viewPort_.first  = Point2D(v1.location_);
	_cross_section.viewPort_.second = Point2D(v2.location_);


	std::cout << " Source e6 " << _cross_section.vertices_[e6.source_id_].location_ << std::endl;
	std::cout << " First  Point e6 " << _cross_section.edges_[e6.id_].segment.curve.front() << std::endl;

	std::cout << " Target Point e6 " << _cross_section.vertices_[e6.target_id_].location_ << std::endl;
	std::cout << " Last e6 " << _cross_section.edges_[e6.id_].segment.curve.back() << std::endl;

	std::cout << " v4 " << _cross_section.vertices_[v4.id_].edges_.size() << std::endl;

}
/// @todo Create the appropriate boundary
void SketchController::setCrossSection( const RRM::CrossSection<qreal>& _cross_section)
{
	this->cross_section_ = _cross_section;
	this->updateSBIM();
}

SketchController::CrossSection SketchController::getCrossSection ( ) const
{
	return this->cross_section_;
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

