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


#ifndef PLANIN_THIN_PLATE_SPLINE_23_HPP
#define PLANIN_THIN_PLATE_SPLINE_23_HPP

#include <cmath> 
/* #include <iostream> */ //debug

#include "planin/basis_function_2d.hpp" 

class ThinPlateSpline23 : public BasisFunction2D 
{
    public: 
        ThinPlateSpline23() = default;
        ThinPlateSpline23( double ) {}
        ~ThinPlateSpline23() = default;

        ThinPlateSpline23( const ThinPlateSpline23& ) = default;
        ThinPlateSpline23& operator=( const ThinPlateSpline23& ) = default;

        double operator()( double x, double y ) const  
        {
            double r2 = x*x + y*y; 
            double r4 = r2*r2; 
            /* std::cout << "x = " << x << ", y = " << y << std::endl; */ //debug

            if ( r4 < epsilon ) { 
                return -r4/2; 
            }

            return ( r4*log( sqrt(r2) ) ); 
        }

        double Dx( double x, double y ) const 
        { 
            double r2 = x*x + y*y; 

            if ( r2 < epsilon )
            {
                return (x*r2 - 2*r2);
            }

            return x*r2*(1 + 2*log(r2));
        }

        double Dy( double x, double y ) const 
        { 
            double r2 = x*x + y*y; 

            if ( r2 < epsilon )
            {
                return (x*r2 - 2*r2);
            }

            return y*r2*(1 + 2*log(r2));
        }

        double Dxx( double x, double y ) const 
        { 
            double r2 = x*x + y*y; 

            if ( r2 < epsilon )
            {
                return 2*x*x;
            }

            return (4*x*x + r2)*log(r2) + 6*x*x + r2;
        }

        double Dxy( double x, double y ) const 
        { 
            double r2 = x*x + y*y; 

            if ( r2 < epsilon )
            {
                return 2*x*y;
            }

            return 2*x*y*(2*log(r2) + 3);
        }

        double Dyx( double x, double y ) const
        {
            return Dxy(x, y);
        }

        double Dyy( double x, double y ) const 
        { 
            double r2 = x*x + y*y; 

            if ( r2 < epsilon )
            {
                return 2*y*y;
            }

            return (4*y*y + r2)*log(r2) + 6*y*y + 2*r2;
        }

        int isSmooth() const 
        { 
            return 2; 
        }

        unsigned int get_order() const  
        {
            return order; 
        }

    private: 
        static const unsigned int order = 2; 
        double epsilon = 1E-9; 
};

#endif 

