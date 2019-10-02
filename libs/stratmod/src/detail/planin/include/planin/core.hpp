/******************************************************************************/
/*                                                                            */
/* This file is part of the "Planar Interpolation Library" (PlanInLib)        */
/* Copyright (C) 2016, Julio Daniel Machado Silva.                            */
/*                                                                            */
/* PlanInLib is free software; you can redistribute it and/or                 */
/* modify it under the terms of the GNU Lesser General Public                 */
/* License as published by the Free Software Foundation; either               */
/* version 3 of the License, or (at your option) any later version.           */
/*                                                                            */
/* PlanInLib is distributed in the hope that it will be useful,               */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          */
/* Lesser General Public License for more details.                            */
/*                                                                            */
/* You should have received a copy of the GNU Lesser General Public           */
/* License along with PlanInLib.  If not, see <http://www.gnu.org/licenses/>, */
/* or write to the Free Software Foundation, Inc., 51 Franklin Street,        */
/* Fifth Floor, Boston, MA  02110-1301  USA.                                  */
/*                                                                            */
/******************************************************************************/


#ifndef __CORE_HPP__
#define __CORE_HPP__

#define UNUSED(X) (void)(X)

#include <iostream> 
#include <vector>
#include <array>
#include <map>
#include <initializer_list> 
#include <type_traits>

#include "planin/serialization_primitives.hpp"

/** Floating point number type. */ 
using RealType = double; 

/** Unsigned integer number type. */ 
using NaturalType = unsigned long int; 


template<typename T, std::size_t n> 
NaturalType size( T (&)[n] ) { return n; } 

template<typename PointK>
bool assign( PointK &p, std::initializer_list<RealType> l ) 
{
    if ( l.size() > size(p.data) ) { 
        return false; 
    }

    std::copy(l.begin(), l.end(), p.data); 
    return true; 
}

struct Point2 { 
    union { 
        RealType data[2];
        struct { 
            RealType x0, x1; 
        };
        struct {
            RealType x, y; 
        };
        struct {
            RealType u, v; 
        };
        struct { 
            RealType width, height; 
        };
    };

    RealType& operator[]( NaturalType i ) { return data[i]; }; 
    RealType  operator[]( NaturalType i ) const { return data[i]; }; 
    NaturalType size() const { return ::size(data); } 
    RealType dot( const Point2 &p ) { return (u*p.u + v*p.v); }
}; 


using Vector2 = Point2;

struct Point4 { 
    union { 
        RealType data[4];
        struct { 
            RealType x0, x1, x2, x3; 
        };
        struct { 
            RealType x, y, z, t; 
        };
        struct {
            RealType u, v, w, s;
        }; 
        struct {
            RealType m11, m12, m21, m22;
        };
        struct { 
            RealType width, height, depth, time; 
        }; 
    };

    RealType& operator[]( NaturalType i ) { return data[i]; }; 
    RealType  operator[]( NaturalType i ) const { return data[i]; }; 
    NaturalType size() const { return ::size(data); } 
    /* friend RealType operator()( NaturalType i, NaturalType j) ; */
};

using Point3 = Point4; 

class Matrix22 : public Point4 
{
    public:
        RealType operator()( NaturalType i, NaturalType j ) const
        {
            return data[i + 2*j];
        }

        RealType& operator()( NaturalType i, NaturalType j )
        {
            return data[i + 2*j];
        }
};

template<typename Archive>
void serialize( Archive &ar, Point2 &p, const std::int32_t version )
{
    (void)(version);
    ar( p.data[0], p.data[1] );
}

CEREAL_CLASS_VERSION(Point2, 1);


template<typename Archive>
void serialize( Archive &ar, Point4 &p, const std::int32_t version )
{
    (void)(version);
    ar( p.data[0], p.data[1], p.data[2], p.data[3] );
}

CEREAL_CLASS_VERSION(Point4, 1);


template<typename Point2Type>
void assign( Point2 &p, Point2Type &&q ) 
{
    p[0] = static_cast<RealType>(q[0]); 
    p[1] = static_cast<RealType>(q[1]); 
}

template<typename Point4Type>
void assign( Point4 &p, Point4Type &&q ) 
{
    p[0] = static_cast<RealType>(q[0]); 
    p[1] = static_cast<RealType>(q[1]); 
    p[2] = static_cast<RealType>(q[2]); 
    p[3] = static_cast<RealType>(q[3]); 
}

template<typename Container, NaturalType k>
class PointWrapper { 
    private:
        Container &c; 
        const NaturalType index; 
        const long int dim; 

    public: 
        PointWrapper(Container &c_, NaturalType index_ = 0 ) : 
            c(c_), index(k*index_), 
            dim( (c.size() - index) >= k ? k : (c.size() - index) ) {} 
        
        auto operator[]( NaturalType i ) -> decltype( c[index + i] ) 
        { 
            return c[index + i]; 
        } 

        auto operator[]( NaturalType i ) const -> typename std::remove_reference<decltype( c[index + i] )>::type  
        { 
            return c[index + i]; 
        } 

        NaturalType size() const { return dim; } 

        template<typename Point>
        bool set( Point &&p ) 
        {
            if ( static_cast<long int>( p.size() ) > dim ) { 
                return false; 
            }

            for ( long int i = 0; i < dim; ++i ) { 
                c[index + i] = p[i]; 
            }

            return true; 
        }

        template<typename Point>
        bool get( Point &p ) const
        {
            if ( static_cast<long int>( p.size() ) < dim ) { 
                return false; 
            }

            for ( long int i = 0; i < dim; ++i ) { 
                p[i] = c[index + i]; 
            }

            return true; 
        }

}; 

template<typename Container, NaturalType k>
class ConstPointWrapper { 
    private:
        const Container &c; 
        const NaturalType index; 
        const NaturalType dim; 

    public: 
        ConstPointWrapper(const Container &c, NaturalType index = 0 ) : 
            c(c), index(index), 
            dim( (c.size() - index) >= k ? k : (c.size() - index) ) {} 
        
        auto operator[]( NaturalType i ) const -> typename std::remove_reference<decltype( c[index + i] )>::type  
        { 
            return c[index + i]; 
        } 

        NaturalType size() const { return dim; } 

        template<typename Point>
        bool get( Point &p ) const
        {
            if ( p.size() > dim ) { 
                return false; 
            }

            for ( NaturalType i = 0; i < dim; ++i ) { 
                p[i] = c[index + i]; 
            }

            return true; 
        }

        Point3 get() const
        {
            Point3 p { {{ c[index+0], c[index+1], c[index+2] }} };

            return p;
        }

}; 


/* class Point2D { */
/* public: */ 
/*     double x,y; */ 
/*     double &operator[]( unsigned int i ) { */ 
/*         if ( i == 0 ) */  
/*             return x; */ 

/*         if ( i == 1 ) */
/*             return y; */ 

/*         throw std::out_of_range( "Tried to access invalid index in Class Point2." ); */
/*     } */

/*     double operator[]( unsigned int i ) const { */ 
/*         if ( i == 0 ) */  
/*             return x; */ 

/*         if ( i == 1 ) */
/*             return y; */ 

/*         throw std::out_of_range( "Tried to access invalid index in Class Point2." ); */
/*     } */
/* }; */ 

/* class Point3D { */ 
/*     public: */ 
/*         double x, y, z; */ 
/*         double &operator[]( unsigned int i ) { */ 
/*             if ( i == 0 ) */  
/*                 return x; */ 

/*             if ( i == 1 ) */
/*                 return y; */ 

/*             if ( i == 2 ) */
/*                 return z; */ 

/*             throw std::out_of_range( "Tried to access invalid index in Class Point3." ); */
/*         } */

/*         double operator[]( unsigned int i ) const { */ 
/*             if ( i == 0 ) */  
/*                 return x; */ 

/*             if ( i == 1 ) */
/*                 return y; */ 

/*             if ( i == 2 ) */
/*                 return z; */ 

/*             throw std::out_of_range( "Tried to access invalid index in Class Point3." ); */
/*         } */
/* }; */ 


/* class BoundBox {}; */ 

/* class BBox2D { */ 
/*     public: */ 
/*         Point2D origin; */ 
/*         Point2D lenght; */ 
/* }; */ 

/* class BBox3D { */ 
/*     public: */
/*         Point3D origin; */ 
/*         Point3D lenght; */
/* }; */


#endif 
