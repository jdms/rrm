#ifndef CROSSSECTION_H
#define CROSSSECTION_H

#include <iostream>
#include <cmath>
#include <vector>


//TODO: remove this typedef later
//typedef int Object;

#include "Object.h"


class CrossSection1
{
    public:

        CrossSection1()
        {
            initialize();
        }

        inline void setDimensions( double width_, double height_ )
        {
            width = std::fabs( width_ );
            height = std::fabs( height_ );
        }

        inline void getDimensions( double& width_, double& height_ ) const
        {
            width_ = width;
            height_ = height;
        }


        inline void setWidth( double width_ )
        {
            width = std::fabs( width_ );
        }

        inline double getWidth() const
        {
            return width;
        }


        inline void setHeight( double height_ )
        {
            height = std::fabs( height_ );
        }

        inline double getHeight() const
        {
            return height;
        }


        inline void setZCoordinate( double z_ )
        {
            z_coordinate = z_;
        }

        inline double getZCoordinate() const
        {
            return z_coordinate;
        }


        inline void setObjectsReferenced( const std::vector< Object* >& objects_ )
        {
            objects_referenced = objects_;
        }

        inline void addObjectReferenced( Object* const& object_ )
        {
            objects_referenced.push_back( object_ );
        }


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

        inline const std::vector< Object* >& getObjectsReferenced() const
        {
            return objects_referenced;
        }


        inline void initialize()
        {
            clear();
            setDefaultValues();
        }


        inline void clear()
        {
            for ( auto &ob_: objects_referenced )
            {
                delete ob_;
            }

        }

        inline void setDefaultValues()
        {
            width = 1.0f;
            height = 1.0f;

            z_coordinate = 1.0f;
        }


    protected:

        double width;
        double height;

        double z_coordinate;

        std::vector< Object* > objects_referenced;

};

#endif // CROSSSECTION_H
