#include "trajectoryitemwrapper.h"

#include "Sketching/PolyQtUtils.hpp"
#include "Core/Geology/Models/object_refactored.h"


TrajectoryItemWrapper::TrajectoryItemWrapper()
{
    object = nullptr;
    setFlag( QGraphicsItem::ItemIsSelectable, false );

}




QRectF TrajectoryItemWrapper::boundingRect() const
{
    return curve.boundingRect();
}


void TrajectoryItemWrapper::paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
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


void TrajectoryItemWrapper::setRawObject( Object_Refactored* const& obj )
{
    object = obj;
    setupPen();
    updateObject();
}



std::size_t TrajectoryItemWrapper::getId() const
{
    return object->getId();
}


void TrajectoryItemWrapper::updateState()
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


void TrajectoryItemWrapper::enableEditing()
{
    setFlag( QGraphicsItem::ItemIsSelectable, object->getEditable() );
}


void TrajectoryItemWrapper::updateCurve()
{

    clearCurve();

    Curve2D c = object->getTrajectoryCurve();
    QPolygonF points = PolyQtUtils::curve2DToQPolyginF( c );

    curve.addPolygon( points );
    setPath( curve );
    update();
    return;
}


void TrajectoryItemWrapper::updateObject()
{
    updateCurve();
    updateState();
}


bool TrajectoryItemWrapper::isVisible() const
{
    return object->getVisibility();
}


void TrajectoryItemWrapper::setupPen()
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


void TrajectoryItemWrapper::clear()
{
    clearData();
}


void TrajectoryItemWrapper::clearData()
{

    object = nullptr;
    clearCurve();
}


void TrajectoryItemWrapper::clearCurve()
{
    prepareGeometryChange();
    curve = QPainterPath();

    intersection_points.clear();
    hidden_subpaths.clear();
}



