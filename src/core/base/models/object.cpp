#include "object.h"

Object::Object()
{
    defineIndex();
    initialize();
}


void Object::defineIndex()
{
    index = number_of_surfaces;
    number_of_surfaces++;
}

void Object::setIndex( const size_t id_ )
{
    index = id_;
    number_of_surfaces = id_ + 1;
}

std::size_t Object::getIndex() const
{
    return index;
}



void Object::setName( const std::string& name_ )
{
    name = name_;
}

void Object::getName( std::string& name_ ) const
{
    name_ = name;
}



void Object::setType( const Object::Type &type_ )
{
    type = type_;
}

void Object::getType( Object::Type &type_ ) const
{
    type_ = type;
}



void Object::setColor( int r, int g, int b )
{
    color.r = r;
    color.g = g;
    color.b = b;

}


void Object::getColor( int& r, int& g, int& b ) const
{
    r = color.r;
    g = color.g;
    b = color.b;
}



void Object::setEditable( const bool status_ )
{
    is_editable = status_;
}

bool Object::isEditable() const
{
    return is_editable;
}



void Object::setSelectable( const bool status_ )
{
    is_selectable = status_;
}

bool Object::isSelectable() const
{
    return is_selectable;
}



void Object::setSelected( const bool status_ )
{
    if( isSelectable() == false ) return;
    is_selected = status_;
}


bool Object::isSelected() const
{
    return is_selected;
}



void Object::setVisible( const bool status_ )
{
    is_visible = status_;
}

bool Object::isVisible() const
{
    return is_visible;
}



bool Object::isEmpty() const
{
    return csection_curves.empty();
}



void Object::addCurve( std::size_t csection_id_, const PolyCurve& curve_ )
{
    if( isCurveAdmissible() == false ) return;
    csection_curves.addElement( csection_id_, curve_ );
}

PolyCurve Object::getCurve( std::size_t csection_id_ )
{
    //TODO: verificar se existe id_;
    return csection_curves.getElement( csection_id_ );
}

void Object::removeCurve( std::size_t csection_id_ )
{
    //TODO: verificar se existe id_;
    csection_curves.removeElement( csection_id_ );
}



Object::CrossSectionsContainer Object::getCrossSectionCurves() const
{
    return csection_curves;
}

void Object::removeCrossSectionCurves()
{
//    for( auto it: csection_curves )
//    {
//        removeCurve( it.first );
//    }

//    csection_curves.clear();
}



void Object::addTrajectory( const PolyCurve& traj_ )
{
    if( isTrajectoryAdmissible() == false ) return;
    trajectory = traj_;
}

void Object::removeTrajectory()
{
    trajectory.clear();
}

bool Object::hasTrajectory() const
{
    return !trajectory.isEmpty();
}



void Object::setSurface( const Surface& surface_ )
{
    surface = surface_;
}

void Object::removeSurface()
{
    surface.clear();
}



bool Object::isCurveAdmissible()
{
    if( hasTrajectory() == false )
        return true;

    bool has_enoughcurves = ( csection_curves.size() < CHANNEL_MAX_CSECTIONS )? true:false;
    return has_enoughcurves;
}



bool Object::isTrajectoryAdmissible()
{
    if( hasTrajectory() == true )
        return false;

    bool has_enoughcurves = ( csection_curves.size() < CHANNEL_MAX_CSECTIONS )? true:false;
    return has_enoughcurves;
}



void Object::clear()
{
    removeCrossSectionCurves();
    removeTrajectory();
    removeSurface();

    initialize();
}

void Object::initialize()
{
    type = Type::STRATIGRAPHY;
    name = "Surface " + std::to_string( index );

    is_editable = true;
    is_selectable = false;
    is_selected = false;
    is_visible = true;

}

