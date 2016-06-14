#include "BoundaryItem.h"

BoundaryItem::BoundaryItem ( int width , int height , const QColor& _color ) :	QGraphicsPathItem ( )
{

//	this->prepareGeometryChange ( );

//	QPainterPath path0;
//	path0.moveTo ( -width * 0.5 , height * 0.5 );
//	path0.lineTo ( width * 0.5 , height * 0.5 );
//	path0.lineTo ( width * 0.5 , height * 0.5 );
//	path0.lineTo ( width * 0.5 , -height * 0.5 );
//
//	QPainterPath path2;
//	path2.moveTo ( width * 0.5 , -height * 0.5 );
//	path2.lineTo ( -width * 0.5 , -height * 0.5 );
//	path2.lineTo ( -width * 0.5 , -height * 0.5 );
//	path2.lineTo ( -width * 0.5 , height * 0.5 );
//
//	boundary.push_back ( path0 );
//	boundary.push_back ( path2 );

	this->prepareGeometryChange ( );

	boundary.clear ( );

	QPainterPath path0;

	path0.addRect ( 0 , 0 , width , height );

	boundary.push_back ( path0 );

	bb2D_.setRect(0,0,width,height);

	this->color = _color;
}

BoundaryItem::~BoundaryItem ( )
{
	boundary.clear ( );
}

void BoundaryItem::setNewBoundary ( qreal x , qreal y , qreal width , qreal height )
{

	this->prepareGeometryChange ( );

	boundary.clear ( );

	QPainterPath path0;

	path0.addRect ( x , y , width , height );

	bb2D_.setRect(x,y,width,height);

	boundary.push_back ( path0 );

}

void BoundaryItem::paint ( QPainter *painter , const QStyleOptionGraphicsItem *option , QWidget *w )
{

	painter->setRenderHint ( QPainter::Antialiasing );

	QPen pen_color ( QColor ( 0 , 200 , 0 ) );
	pen_color.setWidth ( 1 );
	QBrush brush;
	brush.setColor ( this->color );
	//brush.setColor ( QColor ( 240 , 0 , 0 , 100 ) );
	brush.setStyle ( Qt::SolidPattern );

	painter->setPen ( pen_color );
	painter->setBrush ( brush );
//
//	for ( auto crv : boundary )
//		painter->drawPath ( crv );

	painter->drawRect(bb2D_);

}

QRectF BoundaryItem::boundingRect ( ) const
{
	QPainterPath path;

	int ncurves = (int) boundary.size ( );
	for ( int i = 0; i < ncurves; ++i )
	{
		QPainterPath curve = boundary.at ( i );
		path.addPath ( curve );
	}

	return path.boundingRect ( );
}
