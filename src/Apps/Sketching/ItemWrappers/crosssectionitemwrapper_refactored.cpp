#include "crosssectionitemwrapper_refactored.h"

#include <QPainter>
#include <QPen>

CrossSectionItemWrapper_Refactored::CrossSectionItemWrapper_Refactored( double w, double d ):
                                    width( w ), depth( d )
{
}


void CrossSectionItemWrapper_Refactored::paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
                                                QWidget* w )
{
    painter->setRenderHint( QPainter::Antialiasing );
    painter->setPen( current_pen );
    painter->drawLine( 0, depth, width, depth );
}

void CrossSectionItemWrapper_Refactored::setDimensions( double w, double d )
{
    width = w;
    depth = d;
}


void CrossSectionItemWrapper_Refactored::getDimensions( double& w, double& d ) const
{
    w = width;
    d = depth;
}


void CrossSectionItemWrapper_Refactored::setWidth( double w )
{
    width = w;
}


void CrossSectionItemWrapper_Refactored::setDepth( double d )
{
    depth = d;
}


double CrossSectionItemWrapper_Refactored::getWidth() const
{
    return width;
}


double CrossSectionItemWrapper_Refactored::getDepth() const
{
    return depth;
}


void CrossSectionItemWrapper_Refactored::setupPens()
{
    dynamic_csection_pen.setStyle( Qt::SolidLine );
    dynamic_csection_pen.setColor( Qt::yellow );
    dynamic_csection_pen.setWidth ( 3 );

    fixed_csection_pen.setStyle( Qt::DotLine );
    fixed_csection_pen.setColor( Qt::black );
    fixed_csection_pen.setWidth ( 2 );
}

void CrossSectionItemWrapper_Refactored::setCurrent( bool status )
{
    if( status == true )
        current_pen = dynamic_csection_pen;
    else
        current_pen = fixed_csection_pen;
}
