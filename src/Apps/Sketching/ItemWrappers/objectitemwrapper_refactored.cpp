#include "objectitemwrapper_refactored.h"

#include "Sketching/PolyQtUtils.hpp"
#include "Core/Geology/Models/object_refactored.h"

ObjectItemWrapper_Refactored::ObjectItemWrapper_Refactored()
{
    object = nullptr;
    setFlag( QGraphicsItem::ItemIsSelectable, false );

}




QRectF ObjectItemWrapper_Refactored::boundingRect() const
{
    return curve.boundingRect();
}


void ObjectItemWrapper_Refactored::paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
                                          QWidget* w )
{
    if( isVisible() == false ) return;

    if( ( isSelected() == true ) && ( object->getSelected() == false ) )
    {
        QColor c = visible_subpaths_pen.color().darker();
        visible_subpaths_pen.setColor( visible_subpaths_pen.color().darker() );
    }

    painter->setRenderHint( QPainter::Antialiasing );
    painter->setPen( visible_subpaths_pen );
    painter->drawPath( curve );
}


void ObjectItemWrapper_Refactored::setRawObject( Object_Refactored* const& obj, double depth )
{
    object = obj;
    csection_depth = depth;

    setupPen();
    updateObject();
}


void ObjectItemWrapper_Refactored::updateDepth( double depth )
{
    csection_depth = depth;
    updateObject();
}


std::size_t ObjectItemWrapper_Refactored::getId() const
{
    return object->getId();
}


void ObjectItemWrapper_Refactored::updateState()
{
    bool selectable = false;

    if( object->getSelectable() == true )
    {
        visible_subpaths_pen.setColor( selectable_color );
        selectable = true;
    }
    else if( object->getSelected() == true )
    {
        int r, g, b;
        object->getColor( r, g, b );
        visible_subpaths_pen.setColor( QColor( r, g, b ).lighter() );
        visible_subpaths_pen.setStyle( Qt::DotLine );
    }
    else
    {
        int r, g, b;
        object->getColor( r, g, b );
        visible_subpaths_pen.setColor( QColor( r, g, b ) );
        visible_subpaths_pen.setStyle( Qt::SolidLine );
    }

    setFlag( QGraphicsItem::ItemIsSelectable, selectable );
    update();
}


void ObjectItemWrapper_Refactored::enableEditing()
{
    setFlag( QGraphicsItem::ItemIsSelectable, object->getEditable() );
}


void ObjectItemWrapper_Refactored::updateCurve()
{

    clearCurve();

    Curve2D c = object->getCrossSectionCurve( csection_depth );
    std::vector< std::size_t > edges = object->getCrossSectionCurveEdges( csection_depth );


    QPolygonF points = PolyQtUtils::curve2DToQPolyginF( c );

    if( edges.empty() == true )
    {
        curve.addPolygon( points );
        setPath( curve );
        update();
        return;
    }

    std::size_t nedges = edges.size()/2;
    std::size_t last_id = 10000;

    for( size_t i = 0; i < nedges; ++i )
    {

        std::size_t id0 = edges[ 2*i ];
        std::size_t id1 = edges[ 2*i + 1 ];

        if( last_id != id0 )
            curve.moveTo( QPointF( points[ id0 ].x(),
                                   points[ id0 ].y() ) );
        else
            curve.lineTo( QPointF( points[ id1 ].x(),
                                   points[ id1 ].y() ) );


        last_id = id1;
    }

    setPath( curve );
    update();

}


void ObjectItemWrapper_Refactored::updateObject()
{
    updateCurve();
    updateState();
}


bool ObjectItemWrapper_Refactored::isVisible() const
{
    return object->getVisibility();
}


void ObjectItemWrapper_Refactored::setupPen()
{
    visible_subpaths_pen.setStyle( Qt::SolidLine );
    visible_subpaths_pen.setCapStyle( Qt::RoundCap );
    visible_subpaths_pen.setJoinStyle( Qt::RoundJoin );
    visible_subpaths_pen.setWidth( 3 );


    hidden_subpaths_pen.setStyle( Qt::DotLine );
    hidden_subpaths_pen.setCapStyle( Qt::RoundCap );
    hidden_subpaths_pen.setJoinStyle( Qt::RoundJoin );
    hidden_subpaths_pen.setWidth( 3 );
}


void ObjectItemWrapper_Refactored::clear()
{
    clearData();
    csection_depth = 0.0;
}


void ObjectItemWrapper_Refactored::clearData()
{
    if( object != nullptr )
        object->clear();
    object = nullptr;

    clearCurve();
}


void ObjectItemWrapper_Refactored::clearCurve()
{
    prepareGeometryChange();
    curve = QPainterPath();

    intersection_points.clear();
    hidden_subpaths.clear();
}



