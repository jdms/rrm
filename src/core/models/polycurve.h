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



#ifndef POLYCURVE_H
#define POLYCURVE_H

#include <vector>

#include "./polygonalcurve/CurveN.hpp"

class PolyCurve
{

public:

    PolyCurve() = default;

    PolyCurve( const std::vector< Curve2D >& curve_ ){
        curve = curve_;
      }

    PolyCurve( const Curve2D& curve_ ){
        curve.clear();
        curve.push_back( curve_ );
      }


    PolyCurve( const std::vector< double >& vs_, const std::vector< std::size_t >& es_ = std::vector< std::size_t >() ){
        fromVector( vs_, es_ );
    }


    Curve2D getSubcurve( std::size_t index_ ) const{
        bool valid_ = !curve.empty();
        valid_ &= ( ( index_ < curve.size() ) && ( index_ >= 0 ) );

        if( valid_ == false )
            return Curve2D();

        return curve.at( index_ );
    }

    bool isEmpty() const{
        if( curve.empty() == true ) return true;

        std::size_t size_ = 0;
        for( auto c_: curve )
        {
            size_ += c_.size();
        }

        if( size_ <= CURVE_MIN_LENGHT )
            return true;

        return false;
    }

    void clear(){
        for( auto c_: curve )
        {
            c_.clear();
        }
        curve.clear();
    }

    void fromVector( const std::vector< double >& vs_, const std::vector< std::size_t >& es_ = std::vector< std::size_t >() ){
        std::size_t number_of_edges_ = es_.size()/2;
        std::size_t last_id_ = UINT_MAX;


        Curve2D c_;

        for( std::size_t i = 0; i < number_of_edges_; ++i )
        {

            std::size_t id0_ = es_[ 2*i ];
            std::size_t id1_ = es_[ 2*i + 1 ];

            double x0 = vs_[ 2*id0_ ];
            double y0 = vs_[ 2*id0_ + 1 ];
            double x1 = vs_[ 2*id1_ ];
            double y1 = vs_[ 2*id1_ + 1 ];

            if( last_id_ != id0_ )
            {
                curve.push_back( c_ );
                c_ = Curve2D();
            }

            c_.add( Point2D( x0, y0 ) );
            c_.add( Point2D( x1, y1 ) );

            last_id_ = id1_;

        }

    }

    void toVector( std::vector< double >& vs_, std::vector< std::size_t >& es_ ) const{
        int index_ = 0;
        for( auto c_: curve )
        {
            if( c_.size() < CURVE_MIN_LENGHT ) continue;

            for( unsigned int i = 0; i < c_.size(); ++i )
            {
                Point2D p_ = c_.at( i );

                vs_.push_back( p_.x() );
                vs_.push_back( p_.y() );

                if( ( i + 1 ) == c_.size() )
                {
                    es_.push_back( index_ );
                    es_.push_back( ++index_ );
                }

            }

        }
    }

    std::size_t getNumberOfSegments() const
    {
        return curve.size();
    }

protected:

    std::size_t CURVE_MIN_LENGHT = 3;
    std::vector< Curve2D > curve;

//    public:

//        PolyCurve(){}

//        PolyCurve( const std::vector< double >& vs_, const std::vector< std::size_t >& es_ =
//                std::vector< std::size_t >() )
//        {
//            setVertices( vs_ );
//            setEdges( es_ );
//        }


//        bool isEmpty() const
//        {
//            if( data.vertices.empty() == true )
//                return true;
//            else if( data.vertices.size() < 7 )
//                return true;

//            return false;
//        }


//        void setVertices( const std::vector< double >& vs_ )
//        {
//             data.vertices.clear();
//             data.vertices.assign( vs_.begin(), vs_.end() );
//        }

//        std::vector< double > getVertices() const
//        {
//            return data.vertices;
//        }


//        void setEdges( const std::vector< std::size_t >& es_ )
//        {
//             data.edges.clear();
//             data.edges.assign( es_.begin(), es_.end() );
//        }

//        std::vector< std::size_t > getEdges() const
//        {
//             return data.edges;
//        }


//        void clear()
//        {
//            if( data.vertices.empty() == true ) return;
//            data.vertices.clear();
//            data.edges.clear();
//        }

//        ~PolyCurve()
//        {

//        }

//    private:



//        struct PolyCurveData
//        {
//            std::vector< double > vertices;
//            std::vector< std::size_t > edges;
//        };

//        PolyCurveData data;

};

#endif // POLYCURVE_H
