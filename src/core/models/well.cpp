#include "well.h"

Well::Well()
{
    defineIndex();
    initialize();
}


void Well::defineIndex()
{
    index = number_of_wells;
    number_of_wells++;
}

void Well::setIndex( const std::size_t id_ )
{
    index = id_;
    number_of_wells = id_ + 1;
}

std::size_t Well::getIndex() const
{
    return index;
}


void Well::setName( const std::string& name_ )
{
    name = name_;
}

void Well::getName( std::string& name_ ) const
{
     name_ = name;
}


void Well::setType( const Settings::Well::WellType& type_ )
{
    type = type_;
}

void Well::getType( Settings::Well::WellType& type_ ) const
{
    type_ = type;
}


void Well::setStartPoint( double x_, double y_, double z_ )
{
    start.x = x_;
    start.y = y_;
    start.z = z_;
}

void Well::getStartPoint( double& x_, double& y_, double& z_ ) const
{
    x_ = start.x;
    y_ = start.y;
    z_ = start.z;
}


void Well::setEndPoint( double x_, double y_, double z_ )
{
    end.x = x_;
    end.y = y_;
    end.z = z_;
}

void Well::getEndPoint( double& x_, double& y_, double& z_ ) const
{
    x_ = end.x;
    y_ = end.y;
    z_ = end.z;
}



void Well::setCurve( const PolyCurve& curve_ )
{
    curve = curve_;
}

PolyCurve Well::getCurve() const
{
    return curve;
}

void Well::removeCurve()
{
    curve.clear();
}


void Well::setVisible( const bool status_ )
{
    is_visible = status_;
}

bool Well::isVisible() const
{
    return is_visible;
}


void Well::clear()
{
    removeCurve();
    initialize();
}

void Well::initialize()
{
    type = Settings::Well::WellType::INJECTOR;
    name = "Well " + std::to_string( index );
    is_visible = true;
}
