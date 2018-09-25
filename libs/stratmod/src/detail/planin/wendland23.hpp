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


#ifndef __WENDLAND_23__
#define __WENDLAND_23__

#include <cmath> 
/* #include <iostream> */ //debug

#include "basis_function_2d.hpp" 

class Wendland23 : public BasisFunction2D 
{
    public: 
        Wendland23( double support = 1.0 ) : support_size(support) {} 

        Wendland23( const Wendland23& ) = default; 
        Wendland23& operator=( const Wendland23& ) = default; 

        double operator()( double x1, double x2 ) const
        {
            double r = sqrt( x1*x1 + x2*x2 )/support_size;
            if ( r >= 1 ) { 
                return 0; 
            }
            return pow(1-r, 8) * ( 32.0*pow(r, 3) + 25.0*pow(r,2) + 8.0*r + 1.0 );
        }

        double Dx( double x, double y ) const 
        {
            double r = sqrt( x*x + y*y )/support_size; 
            if ( r >= 1 ) { 
                return 0; 
            }

            /* double rminus1to7 = pow(1-r, 7); // */ 
            double val = -1/support_size * ( 352.0 * r*r + 154.0 * r + 22.0 ) * (x/support_size) * pow(1-r,7); //(1-r)*(1-r)*(1-r)*(1-r)*(1-r)*(1-r)*(1-r);

            return val; 

        }

        double Dy( double x, double y ) const 
        {
            double r = sqrt( x*x + y*y )/support_size; 
            if ( r >= 1 ) { 
                return 0; 
            }

            /* double rminus1to7 = pow(1-r, 7); // */ 
            double val = -1/support_size * ( 352.0 * r*r + 154.0 * r + 22.0 ) * (y/support_size) * pow(1-r,7); //(1-r)*(1-r)*(1-r)*(1-r)*(1-r)*(1-r)*(1-r);

            return val; 

        }

/* if (i==j), */
/*     val = -(1/delta)^2 * (352.0*r.^2 + 154.0*r + 22) .* (1-r).^7 + (1/delta)^2 * (3168*r + 528) .* (xi/delta).^2 .* (1-r).^6; */
/* else */
    /* val = (1/delta)^2 * (3168*r + 528) .* (xi/delta) .* (xj/delta) .* (1-r).^6; */
/* end */
        double Dxx( double x, double y ) const
        {
            double r = sqrt( x*x + y*y )/support_size; 
            if ( r >= 1 ) { 
                return 0; 
            }

            double supp_sq = support_size * support_size;
            double one_minus_r_6 = pow(1-r,6);
            double one_minus_r_7 = one_minus_r_6 * (1-r);

            double val = -(1/supp_sq) * (352.0*r*r + 154.0*r + 22) * one_minus_r_7 + (1/supp_sq) * (3168*r + 528) * (x*x/supp_sq) * one_minus_r_6;
            /* double val = -(1/support_size)^2 * (352.0*r.^2 + 154.0*r + 22) .* (1-r).^7 + (1/support_size)^2 * (3168*r + 528) .* (x/support_size).^2 .* (1-r).^6; */

            return val;
        }

        double Dxy( double x, double y ) const
        {
            double r = sqrt( x*x + y*y )/support_size; 
            if ( r >= 1 ) { 
                return 0; 
            }

            double supp_sq = support_size * support_size;
            double one_minus_r_6 = pow(1-r,6);

            double val = (1/supp_sq) * (3168*r + 528) * (x/support_size) * (y/support_size) * one_minus_r_6;
            /* double val = (1/support_size)^2 * (3168*r + 528) .* (x/support_size) .* (y/support_size) .* (1-r).^6; */

            return val;
        }

        double Dyx( double x, double y ) const
        {
            return Dxy(x, y);
        }

        double Dyy( double x, double y ) const
        {
            double r = sqrt( x*x + y*y )/support_size; 
            if ( r >= 1 ) { 
                return 0; 
            }

            double supp_sq = support_size * support_size;
            double one_minus_r_6 = pow(1-r,6);
            double one_minus_r_7 = one_minus_r_6 * (1-r);

            double val = -(1/supp_sq) * (352.0*r*r + 154.0*r + 22) * one_minus_r_7 + (1/supp_sq) * (3168*r + 528) * (y*y/supp_sq) * one_minus_r_6;
            /* double val = -(1/support_size)^2 * (352.0*r.^2 + 154.0*r + 22) .* (1-r).^7 + (1/support_size)^2 * (3168*r + 528) .* (y/support_size).^2 .* (1-r).^6; */

            return val;
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
        /* double epsilon = 1E-9; */ 
        double support_size = 1.0; 

        static const int smoothness = 1; 
};

#endif 

