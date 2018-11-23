#include "domain.h"

Domain::Domain()
{
    std::cout << "DOMAIN CREATED" << std::endl << std::flush;
    //  initialize();
}


Domain::Domain( const Domain& dmn_ )
{
    std::cout << "DOMAIN CREATED" << std::endl << std::flush;

    this->index_regions = dmn_.index_regions;
}


Domain& Domain::operator =( const Domain& dmn_ )
{
    this->index_regions = dmn_.index_regions;
    return *this;
}


Domain::~Domain()
{
    clear();
}



void Domain::addRegion( const std::size_t& index_ )
{
    index_regions.insert( index_ );
}


void Domain::removeRegion( const std::size_t& index_ )
{
    index_regions.erase( index_ );
}


void Domain::removeRegions()
{
    index_regions.clear();
}


bool Domain::findRegion( const std::size_t& index_ ) const
{
    if( index_regions.find( index_ ) == index_regions.end() ) return false;
    return true;
}


void Domain::setRegions( const std::set< std::size_t >& regions_ )
{
    index_regions = regions_;
}



std::set< std::size_t > Domain::getRegions() const
{
    return index_regions;
}


void Domain::initialize()
{
    index = 0;

    setVisible( true );
    setColor( 255, 0, 0 );
}
