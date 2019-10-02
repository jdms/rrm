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


#ifndef __BASIS_FUNCTION_2D__ 
#define __BASIS_FUNCTION_2D__ 

#include "planin/core.hpp" 

class BasisFunction2D
{
    public: 
        double operator()( double x, double y ) const { UNUSED(x); UNUSED(y); return 0; }

        double Dx( double x, double y ) const { UNUSED(x); UNUSED(y); return 0; }
        double Dy( double x, double y ) const { UNUSED(x); UNUSED(y); return 0; }

        double Dxx( double x, double y ) const { UNUSED(x); UNUSED(y); return 0; }
        double Dxy( double x, double y ) const { UNUSED(x); UNUSED(y); return 0; }
        double Dyy( double x, double y ) const { UNUSED(x); UNUSED(y); return 0; }

        int isSmooth() const { return 0; } 
        unsigned int get_order() const { return 0; } 
}; 

#endif 

