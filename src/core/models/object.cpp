/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "object.h"

Object::Object()
{
    defineIndex();
    initialize();

    std::cout << "Creating object " << this << std::endl << std::flush;
}

Object::Object( const Object & obj_ )
{
    this->index = obj_.index;
    this->name = obj_.name;
    this->log = obj_.log;
    this->color = obj_.color;
    this->min = obj_.min;
    this->max = obj_.max;
    this->editable = obj_.editable;
    this->selectable = obj_.selectable;
    this->selected = obj_.selected;
    this->visible = obj_.visible;
    this->active = obj_.active;
}

Object& Object::operator=( const Object & obj_ )
{
    this->index = obj_.index;
    this->name = obj_.name;
    this->log = obj_.log;
    this->color = obj_.color;
    this->min = obj_.min;
    this->max = obj_.max;
    this->editable = obj_.editable;
    this->selectable = obj_.selectable;
    this->selected = obj_.selected;
    this->visible = obj_.visible;
    this->active = obj_.active;
    return *this;
}

void Object::setLog(const std::string& log_)
{
    log = log_;
}

std::string Object::getLog() const
{
    return log;
}

Object::~Object()
{
    clear();
    std::cout << "Deleting object" << std::endl << std::flush;

}


///======================================================================


void Object::setIndex( std::size_t id_ )
{
    index = id_;
    number_of_objects = id_ + 1;

    // index =  id_
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


void Object::setColor(int r_, int g_, int b_)
{
    color.red = r_;
    color.green = g_;
    color.blue = b_;

}
void Object::getColor(int& r_, int& g_, int& b_) const
{
    r_ = color.red;
    g_ = color.green;
    b_ = color.blue;
}

void Object::setBoundingBox( double xmin_, double xmax_, double ymin_, double ymax_, double zmin_, double zmax_ )
{
    min.x = xmin_;
    max.x = xmax_;

    min.y = ymin_;
    max.y = ymax_;

    min.z = zmin_;
    max.z = zmax_;

}

void Object::getBoundingBox( double & xmin_, double & xmax_, double & ymin_, double & ymax_, double & zmin_, double & zmax_ )
{
    xmin_ = min.x;
    xmax_ = max.x;

    ymin_ = min.y;
    ymax_ = max.y;

    zmin_ = min.z;
    zmax_ = max.z;
}

void Object::setMaximumBounding( double xmax_, double ymax_, double zmax_ )
{
    max.x = xmax_;
    max.y = ymax_;
    max.z = zmax_;
}

void Object::setMinimumBounding( double xmin_, double ymin_, double zmin_ )
{
    min.x = xmin_;
    min.y = ymin_;
    min.z = zmin_;
}



void Object::setEditable( bool status_ )
{
    editable = status_;
}

bool Object::isEditable() const
{
    return editable;
}

void Object::setSelectable( bool status_ )
{
    selectable = status_;
}

bool Object::isSelectable() const
{
    return selectable;
}


void Object::setSelected( bool status_ )
{
    selected = status_;
}


bool Object::isSelected() const
{
    return selected;
}



void Object::setVisible( bool status_ )
{
    visible = status_;
}

bool Object::isVisible() const
{
    return visible;
}



void Object::setActive( bool status_ )
{
    active = status_;
}

bool Object::isActive() const
{
    return active;
}


void Object::removed()
{
    removeCrossSectionCurves();
    removeTrajectory();
    removeSurface();

    setVisible( false );

}

///======================================================================

// to be updated or removed



void Object::defineIndex()
{
    index = number_of_objects;
    number_of_objects++;
}


void Object::setType( const Settings::Objects::ObjectType &type_ )
{
    type = type_;
}

Settings::Objects::ObjectType Object::getType() const
{
    return type;
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

    return ( csection_curves1.empty() && trajectory.isEmpty() );
}



bool Object::addCurve( double csection_id_, const PolyCurve& curve_ )
{
//    if( isCurveAdmissible() == false ) return false;

    if( user_entered.find( csection_id_ ) != user_entered.end() )
        return false;

    csection_curves1[ csection_id_ ] = curve_;
    user_entered.insert( csection_id_ );

    setVisible( true );
    return true;
}

bool Object::hasCurve( double csection_id_ ) const
{
    if( csection_curves1.find( csection_id_ ) == csection_curves1.end() )
        return false;

    return !( user_entered.find( csection_id_ ) == user_entered.end() );
//    return ( csection_curves.getElement( csection_id_ ).isEmpty() == false );

}

PolyCurve Object::getCurve( double csection_id_ )
{
    if( csection_curves1.find( csection_id_ ) == csection_curves1.end() )
        return PolyCurve();

    return csection_curves1[ csection_id_ ];
}

bool Object::removeCurve( double csection_id_ )
{
    if( csection_curves1.find( csection_id_ ) == csection_curves1.end() )
        return false;

    csection_curves1.erase( csection_id_ );

    if( csection_curves1.empty() == true )
        setVisible( false );
    return true;
}

void Object::updateCurve( double csection_id_, const PolyCurve& curve_ )
{
    csection_curves1[ csection_id_]  = curve_;
}

std::map< double, PolyCurve > Object::getCurves()
{
   std::map< double, PolyCurve > curves_;

   for ( auto d_: user_entered )
   {
       if( csection_curves1.find( d_ ) == csection_curves1.end() ) continue;
       curves_[ d_ ] = csection_curves1[ d_ ];
   }


   return curves_;
}


Object::CrossSectionsContainer Object::getCrossSectionCurves() const
{
    return csection_curves;
}

void Object::removeCrossSectionCurves()
{

    for ( auto it: csection_curves1 )
    {
        if( ( it.second ).isEmpty() == true ) continue;
        ( it.second ).clear();
    }
    csection_curves1.clear();


//    for ( CrossSectionsContainer::Iterator it =  csection_curves.begin(); it != csection_curves.end(); ++it )
//    {
//        if( ( it->second ).isEmpty() == true ) continue;
//        ( it->second ).clear();
//    }
//    csection_curves.clear();

//    user_entered.clear();
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

const Surface& Object::getSurface() const
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

    bool has_enoughcurves = ( csection_curves1.size() < CHANNEL_MAX_CSECTIONS )? true:false;
    return has_enoughcurves;
}



bool Object::isTrajectoryAdmissible()
{
    if( hasTrajectory() == true )
        return false;

    bool has_less_than_max = ( user_entered.size() < CHANNEL_MAX_CSECTIONS )? true:false;
    return has_less_than_max;
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

    color.red = 255;
    color.green = 0;
    color.blue = 0;

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


void Object::clearPreviewCurves()
{
    CrossSectionsContainer::Iterator it =  csection_curves.begin();
    std::vector< double > previews_;

    for ( auto it: csection_curves1 )
    {
        double id_ = it.first;

        if( user_entered.find( id_ ) != user_entered.end() ) continue;
        previews_.push_back( id_ );
    }


//    for ( CrossSectionsContainer::Iterator it =  csection_curves.begin(); it != csection_curves.end(); ++it )
//    {
//        double id_ = it->first;

//        if( user_entered.find( id_ ) != user_entered.end() ) continue;
//        previews_.push_back( id_ );
//    }

    for( auto id_: previews_ )
        removeCurve( id_ );

}

