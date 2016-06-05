/******************************************************************************/
/*                                                                            */
/* This file is part of the "Planar Interpolation Library" (PlanInLib)        */
/* Copyright (C) 2016, Julio Daniel Machado Silva.                            */
/*                                                                            */
/* PlanInLib is free software; you can redistribute it and/or                 */
/* modify it under the terms of the GNU Lesser General Public                 */
/* License as published by the Free Software Foundation; either               */
/* version 2.1 of the License, or (at your option) any later version.         */
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

/** Floating point number type. */ 
using __Real__ = double; 

/** Unsigned integer number type. */ 
using __Natural__ = unsigned long int;


template<typename T, size_t n> 
size_t size( T(&) [n] ) { return n; } 

template<typename PointK>
bool assign( PointK &p, std::initializer_list<__Real__> l ) 
{
    if ( l.size() > size(p.data) ) { 
        return false; 
    }

    std::copy(l.begin(), l.end(), p.data); 
    return true; 
}

struct Point2 { 
    union { 
        __Real__ data[2];
        struct {
            __Real__ x, y; 
        };
    };
    __Real__& operator[]( __Natural__ i ) { return data[i]; }; 
    __Real__  operator[]( __Natural__ i ) const { return data[i]; }; 
}; 

struct Point4 { 
    union { 
        __Real__ data[4]; 
        struct { 
            __Real__ x, y, z, w; 
        };
    };
    __Real__& operator[]( __Natural__ i ) { return data[i]; }; 
    __Real__  operator[]( __Natural__ i ) const { return data[i]; }; 
};

using Point3 = Point4; 

template<typename __Point2__>
void assign( Point2 &p, __Point2__ &&q ) 
{
    p[0] = q[0]; p[1] = q[1]; 
}

template<typename __Point4__>
void assign( Point4 &p, __Point4__ &&q ) 
{
    p[0] = q[0]; p[1] = q[1]; p[2] = q[2]; p[3] = q[3]; 
}



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
