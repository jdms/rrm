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


#ifndef PLANIN_POLYNOMIAL_HPP
#define PLANIN_POLYNOMIAL_HPP

#include <cmath> 
#include "planin/core.hpp"

template<unsigned int k>
class Polynomial2 
{
    double operator()( double x1, double x2 ) { 
        UNUSED(x1);
        UNUSED(x2); 

        return 0; 
    }
}; 

template<>
class Polynomial2<0> 
{
    double operator()( double x1, double x2 ) { 
        UNUSED(x1); UNUSED(x2); 
        return 1; 
    }
}; 

template<>
class Polynomial2<1>
{
    double operator()( double x1, double x2 ) { 
        UNUSED(x2); 
        return x1; 
    }
};

template<>
class Polynomial2<2>
{
    double operator()( double x1, double x2 ) {
        UNUSED(x1); 
        return x2; 
    }
}; 

template<>
class Polynomial2<3>
{
    double operator()( double x1, double x2 ) {
        UNUSED(x2); 
        return x1*x1; 
    }
}; 

template<>
class Polynomial2<4>
{
    double operator()( double x1, double x2 ) {
        return x1*x2; 
    }
}; 

template<>
class Polynomial2<5>
{
    double operator()( double x1, double x2 ) {
        UNUSED(x1); 
        return x2*x2; 
    }
}; 



#endif 

