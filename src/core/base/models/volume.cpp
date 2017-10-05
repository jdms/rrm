#include "volume.h"

Volume::Volume()
{
    initialize();
}


void Volume::setOrigin( double ox_, double oy_, double oz_ )
{
    origin_x = ox_;
    origin_y = oy_;
    origin_z = oz_;
}

void Volume::getOrigin( double& ox_, double& oy_, double& oz_ ) const
{
    ox_ = origin_x;
    oy_ = origin_y;
    oz_ = origin_z;
}

void Volume::setGeometry( double w_, double h_, double l_ )
{
    width = w_;
    height = h_;
    lenght = l_;
}

void Volume::getGeometry( double& w_, double& h_, double& l_ ) const
{
    w_ = width ;
    h_ = height;
    l_ = lenght;
}


void Volume::setWidth( double w_ )
{
    width = w_;
}

double Volume::getWidth() const
{
    return width;
}


void Volume::setHeight( double h_  )
{
    height = h_;
}

double Volume::getHeight() const
{
    return  height;
}


void Volume::setLenght( double l_ )
{
    lenght = l_;
}

double Volume::getLenght() const
{
    return lenght;
}


void Volume::getFrontFace( std::vector< double >& vertices_ ) const
{
    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z + lenght );

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z + lenght );

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z + lenght );

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z + lenght );
}

void Volume::getRightFace( std::vector< double >& vertices_ ) const
{

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z + lenght );


    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z + lenght );

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z );

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z  );
}

void Volume::getBackFace( std::vector< double >& vertices_ ) const
{

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z );

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z );

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z );

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z );
}

void Volume::getLeftFace( std::vector< double >& vertices_ ) const
{

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z );

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z + lenght );

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z + lenght );

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z );
}

void Volume::getTopFace( std::vector< double >& vertices_ ) const
{

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z );

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z );

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z + lenght );

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y + height );
    vertices_.push_back( origin_z + lenght );
}

void Volume::getBottomFace( std::vector< double >& vertices_ ) const
{

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z );

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z );

    vertices_.push_back( origin_x + width );
    vertices_.push_back( origin_y );
    vertices_.push_back( origin_z + lenght );

    vertices_.push_back( origin_x );
    vertices_.push_back( origin_y  );
    vertices_.push_back( origin_z + lenght );
}


void Volume::addObject( std::size_t id_ )
{
    stratigraphies.insert( id_ );
}


void Volume::removeObject( std::size_t id_ )
{
    //TODO: check if id_ is valid
    stratigraphies.erase( id_ );
}

std::set< std::size_t > Volume::getObjects() const
{
    return stratigraphies;
}


void Volume::addCrossSection( std::size_t id_ )
{
    csections.insert( id_ );
}


void Volume::removeCrossSection( std::size_t id_ )
{
    //TODO: check if id_ is valid
    csections.erase( id_ );
}


std::set< std::size_t > Volume::getCrossSections() const
{
    return csections;
}


void Volume::setVisible( bool status_ )
{
    is_visible = status_;
}

bool Volume::isVisible() const
{
    return is_visible;
}

void Volume::setResizable( bool status_ )
{
    is_resizable = status_;
}

bool Volume::isResizable() const
{
    return is_resizable;
}


void Volume::clear()
{
    stratigraphies.clear();
    csections.clear();

    initialize();
}

void Volume::initialize()
{
    setOrigin( 0.0, 0.0, 0.0 );
    setGeometry( 500, 500, 500 );
    setVisible( true );
    setResizable( true );
}
