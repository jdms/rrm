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


#ifndef PLANIN_CUBIC_HPP
#define PLANIN_CUBIC_HPP

#include <cmath> 
/* #include <iostream> */ //debug

#include "planin/basis_function_2d.hpp" 

class Cubic : public BasisFunction2D 
{
    public: 
        double operator()( double x1, double x2 ) const  
        {
            double r = sqrt(x1*x1 + x2*x2); 

            return r*r*r;
        }

        unsigned int get_order() const  
        {
            return order; 
        }

        double Dx( double x1, double x2 ) const
        {
            double r = sqrt(x1*x1 + x2*x2); 

            return 3*r*x1;
        }

        double Dy( double x1, double x2 ) const
        {
            double r = sqrt(x1*x1 + x2*x2); 

            return 3*r*x2;
        }

        double Dxx( double x1, double x2 ) const
        {
            double r = sqrt(x1*x1 + x2*x2); 

            if ( r < epsilon )
            {
                return 3*(r + x1);
            }

            return 3*(r + x1*x1/r);
        }

        double Dxy( double x1, double x2 ) const
        {
            double r = sqrt(x1*x1 + x2*x2); 

            if ( r < epsilon )
            {
                return r/2;
            }

            return 3*x1*x2/r;
        }

        double Dyy( double x1, double x2 ) const
        {
            double r = sqrt(x1*x1 + x2*x2); 

            if ( r < epsilon )
            {
                return 3*(r + x2);
            }

            return 3*(r + x2*x2/r);
        }

    private: 
        static const unsigned int order = 1; 
        double epsilon = 1E-9; 
};

#endif 

