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


#ifndef PLANIN_THIN_PLATE_SPLINE_22_HPP
#define PLANIN_THIN_PLATE_SPLINE_22_HPP

#include <cmath> 
/* #include <iostream> */ //debug

#include "planin/basis_function_2d.hpp" 

class ThinPlateSpline22 : public BasisFunction2D 
{
    public: 
        double operator()( double x1, double x2 ) const  
        {
            double r2 = x1*x1 + x2*x2; 
            /* std::cout << "x = " << x1 << ", y = " << x2 << std::endl; */ //debug

            if ( r2 < epsilon ) { 
                return -r2/2; 
            }

            /* return ( r2*log( sqrt(r2) ) ); */ 
            return ( r2*log( r2 )/2 ); 
        }

        double Dx( double x1, double x2 ) const
        {
            double r2 = x1*x1 + x2*x2; 

            if ( r2 < epsilon )
            {
                /* return -2*x1*x1*x1/r2 + x1; */
                return -x1;
            }

            /* return x1 * ( 2*log( sqrt(r2) ) + 1 ); */
            return x1 * ( log(r2) + 1 );
        }

        double Dy( double x1, double x2 ) const
        {
            double r2 = x1*x1 + x2*x2; 

            if ( r2 < epsilon )
            {
                /* return -2*x2*x2*x2/r2 + x2; */
                return -x2;
            }

            /* return x2 * ( 2*log( sqrt(r2) ) + 1 ); */
            return x2 * ( log(r2) + 1 );
        }

        int isSmooth() const
        {
            return smoothness; 
        }

        unsigned int get_order() const  
        {
            return order; 
        }

    private: 
        static const unsigned int order = 2; 
        double epsilon = 1E-9; 

        static const int smoothness = 1; 
};

#endif 

