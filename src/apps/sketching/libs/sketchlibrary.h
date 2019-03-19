#ifndef SKETCHLIBRARY_H
#define SKETCHLIBRARY_H

#include <vector>

#include "./PolygonalCurve/CurveN.hpp"
#include "./core/models/polycurve.h"

class SketchLibrary1
{

    public:

//        SketchLibrary() = default;

        static Curve2D monotonicInX( const Curve2D& sketch_ )
        {

            if( sketch_.size() < SKETCH_MIN_LENGHT ) return sketch_;

            Curve2D ms_;

            unsigned int start = 0;

            ms_.add( sketch_[ start ] );

            for(unsigned int i = start; i < sketch_.size() - 1; ++i )
            {
                if( sketch_[ i ].x() != sketch_[ i + 1 ].x() ) break;

                ms_.add( sketch_[ i + 1 ] );
                start = i+1;
            }

            if( sketch_[ start ].x() <= sketch_[ start + 1 ].x() )
            {
                unsigned int temp = start;
                for( unsigned int i = start; i < sketch_.size() - 1; ++i )
                {
                    if( sketch_[ temp ].x() >= sketch_[ i + 1 ].x() ) continue;

                    ms_.add( sketch_[ i + 1 ] );
                    temp = i + 1;
                }

            }

            else if( sketch_[ start ].x() >= sketch_[ start + 1 ].x() )
            {
               unsigned int temp = start;
                for( unsigned int i = start; i < sketch_.size() - 1; ++i )
                {
                    if( sketch_[ temp ].x() <= sketch_[ i + 1 ].x() ) continue;
                    ms_.add( sketch_[ i + 1 ] );
                    temp = i + 1;
                }

            }

            return ms_;

        }

        static Curve2D monotonicInY( const Curve2D& sketch_ )
        {

            if( sketch_.size() < SKETCH_MIN_LENGHT ) return sketch_;

            Curve2D ms_;

            unsigned int start = 0;

            ms_.add( sketch_[ start ] );

            for( unsigned int i = start; i < sketch_.size() - 1; ++i )
            {
                if( sketch_[ i ].y() != sketch_[ i + 1 ].y() ) break;

                ms_.add( sketch_[ i + 1 ] );
                start = i+1;
            }

            if( sketch_[ start ].y() <= sketch_[ start + 1 ].y() )
            {
                unsigned int temp = start;
                for( unsigned int i = start; i < sketch_.size() - 1; ++i )
                {
                    if( sketch_[ temp ].y() >= sketch_[ i + 1 ].y() ) continue;

                    ms_.add( sketch_[ i + 1 ] );
                    temp = i + 1;
                }

            }

            else if( sketch_[ start ].y() >= sketch_[ start + 1 ].y() )
            {
                unsigned int temp = start;
                for( unsigned int i = start; i < sketch_.size() - 1; ++i )
                {
                    if( sketch_[ temp ].y() <= sketch_[ i + 1 ].y() ) continue;
                    ms_.add( sketch_[ i + 1 ] );
                    temp = i + 1;
                }

            }

            return ms_;

        }

        static bool overSketch( Curve2D& sketch_, const Curve2D& subpath_ )
        {
            if( subpath_.size() < SKETCH_MIN_LENGHT ) return false;

            Curve2D cpy_subpath_ = subpath_;
            Curve2D rest_;

            double err_ = 1.0;
            double aprox_factor_ = 36.0;

            sketch_ = cpy_subpath_.overSketch( sketch_, rest_, err_, aprox_factor_ );
            return true;

        }

        static Curve2D smooth( Curve2D& sketch_ )
        {
            Curve2D cpy_ = sketch_;
            cpy_.lineFilter();
            cpy_.meanFilter();
            cpy_.meanFilter();
            return cpy_;
        }


        static void toVector( const Curve2D& curve_, std::vector< double >& vertices_, std::vector< std::size_t >& edges_ )
        {
            if( curve_.isEmpty() == true ) return;

            std::size_t index_ = 0;

            for( unsigned int i = 0; i < curve_.size(); ++i )
            {
                const Point2D& p_ = curve_.at( i );

                vertices_.push_back( p_.x() );
                vertices_.push_back( p_.y() );

                edges_.push_back( index_ );
                edges_.push_back( ++index_ );
            }
        }


    private:

        static const unsigned int SKETCH_MIN_LENGHT = 3;

};





#endif // SKETCHLIBRARY_H
