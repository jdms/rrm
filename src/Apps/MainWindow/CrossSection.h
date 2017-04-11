#ifndef CROSSSECTION_H
#define CROSSSECTION_H

#include <iostream>
#include <cmath>
#include <set>


//TODO: remove this typedef later
//typedef int Object;

#include "Volume.h"
#include "Object.h"


class CrossSection1
{
    public:

        CrossSection1()
        {
            initialize();
        }

        inline void setVolume( Volume* vol_ ){ volume = vol_; }
        inline Volume* const& getVolume(){ return volume; }


        inline void setDimensions( double width_, double height_ )
        {
            setWidth( width_ );
            setHeight( height_ );
        }

        inline void getDimensions( double& width_, double& height_ ) const
        {
            width_ = volume->getWidth();
            height_ = volume->getHeight();
        }


        inline void setWidth( double width_ )
        {
            volume->setWidth( std::fabs( width_ ) );
        }

        inline double getWidth() const
        {
            return volume->getWidth();
        }


        inline void setHeight( double height_ )
        {
            volume->setHeight( std::fabs( height_ ) );
        }

        inline double getHeight() const
        {
            return volume->getHeight();
        }


        inline void setZCoordinate( double z_ )
        {
            z_coordinate = z_;
        }

        inline double getZCoordinate() const
        {
            return z_coordinate;
        }


        inline void setObjectsReferenced( const std::set< Object* >& objects_ )
        {
            objects_referenced = objects_;
        }

        inline void addObjectReferenced( Object* const& object_ )
        {
            objects_referenced.insert( object_ );
        }


/*
        inline Object* getObjectReferencedofId( std::size_t id_ ) const
        {
            try
            {
                objects_referenced.at( id_ );
            }
            catch ( const std::exception& e )
            {
                std::cout << " a standard exception was caught as trying to access object of "
                          << id_ << ", with message '"
                          << e.what() << "'\n";
            }
        }
*/

        inline const std::set< Object* >& getObjectsReferenced() const
        {
            return objects_referenced;
        }


        inline void initialize()
        {
            clear();
        }


        inline void clear()
        {
            for ( auto &ob_: objects_referenced )
            {
                delete ob_;
            }

        }



    protected:

        Volume* volume;
        double z_coordinate;

        std::set< Object* > objects_referenced;

};

#endif // CROSSSECTION_H
