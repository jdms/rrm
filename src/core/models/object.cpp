#include "object.h"

Object::Object()
{
    defineIndex();
    initialize();
}


void Object::defineIndex()
{
    index = number_of_objects;
    number_of_objects++;
}

void Object::setIndex( const size_t id_ )
{
    index = id_;
    number_of_objects = id_ + 1;
}

std::size_t Object::getIndex() const
{
    return index;
}



void Object::setName( const std::string& name_ )
{
    name = name_;
}

std::string Object::getName() const
{
    return name;
}



void Object::setType( const Settings::Objects::ObjectType &type_ )
{
    type = type_;
}

Settings::Objects::ObjectType Object::getType() const
{
    return type;
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
//    if( isSelectable() == false ) return;
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



void Object::setActive( const bool status_ )
{
    is_active = status_;
}

bool Object::isActive() const
{
    return is_active;
}



void Object::setDone( const bool status_ )
{
    is_done = status_;
}


bool Object::isDone() const
{
    return is_done;
}

void Object::saveInformation( const std::string& text_ )
{
    clearInformation();
    text_information = text_;

}


const std::string& Object::getInformation()
{
    return text_information;
}


void Object::clearInformation()
{
    text_information.clear();
}


bool Object::isEmpty() const
{
    return csection_curves.empty();
}



bool Object::addCurve( double csection_id_, const PolyCurve& curve_ )
{
    if( isCurveAdmissible() == false ) return false;
    csection_curves.addElement( csection_id_, curve_ );
    return true;
}

bool Object::hasCurve( double csection_id_ ) const
{
    if( csection_curves.findElement( csection_id_ ) == false )
        return false;

    return ( csection_curves.getElement( csection_id_ ).isEmpty() == false );

}

PolyCurve Object::getCurve( double csection_id_ )
{
    if( csection_curves.findElement( csection_id_ ) == false )
        return PolyCurve();

    return csection_curves.getElement( csection_id_ );
}

bool Object::removeCurve( double csection_id_ )
{
    if( csection_curves.findElement( csection_id_ ) == false )
        return false;

    csection_curves.removeElement( csection_id_ );
    return true;
}

void Object::updateCurve( double csection_id_, const PolyCurve& curve_ )
{
    csection_curves.setElement( csection_id_, curve_ );
}

\

Object::CrossSectionsContainer Object::getCrossSectionCurves() const
{
    return csection_curves;
}

void Object::removeCrossSectionCurves()
{

    for ( CrossSectionsContainer::Iterator it =  csection_curves.begin(); it != csection_curves.end(); ++it )
    {
        if( ( it->second ).isEmpty() == true ) continue;
        ( it->second ).clear();
    }
    csection_curves.clear();

}



bool Object::addTrajectory( const PolyCurve& traj_ )
{
    if( isTrajectoryAdmissible() == false ) return false;

    trajectory = traj_;
    return true;
}


PolyCurve Object::getTrajectory()
{
    return trajectory;
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

Surface Object::getSurface() const
{
    return surface;
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

    bool has_less_than_max = ( csection_curves.size() < CHANNEL_MAX_CSECTIONS )? true:false;
    return has_less_than_max;
}




void Object::setMaxMin( double maxx_, double maxy_, double maxz_,
                        double minx_, double miny_, double minz_ )
{
    max.x = maxx_;
    max.y = maxy_;
    max.z = maxz_;

    min.x = minx_;
    min.y = miny_;
    min.z = minz_;


}


void Object::getMaxMin( double& maxx_, double& maxy_, double& maxz_,
                        double& minx_, double& miny_, double& minz_ ) const
{

    maxx_ = max.x;
    maxy_ = max.y;
    maxz_ = max.z;

    minx_ = min.x;
    miny_ = min.y;
    minz_ = min.z;

}



void Object::clear()
{

    name.clear();
    clearInformation();

    removeCrossSectionCurves();
    removeTrajectory();
    removeSurface();

    initialize();
}

void Object::initialize()
{
    type = Settings::Objects::ObjectType::STRATIGRAPHY;
    name = "Surface " + std::to_string( index );

    is_editable = true;
    is_selectable = false;
    is_selected = false;
    is_visible = false;
    is_active = true;
    is_done = false;

    color.r = 255;
    color.g = 0;
    color.b = 0;

    max.x = 0;
    max.y = 0;
    max.z = 0;

    min.x = 0;
    min.y = 0;
    min.z = 0;
}


void Object::resetAllObjects()
{
    number_of_objects = 0;
}
