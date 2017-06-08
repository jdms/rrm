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


#ifndef __INTERPOLANT_2D__
#define __INTERPOLANT_2D__

#include <vector> 
#include <cstdint>

#include "serialization_primitives.hpp"

#include "core.hpp" // Dummy definitions to be replaced by the project's definitions. 
#include "kernels.hpp"
#include "polynomial.hpp" 

using Kernel = ThinPlateSpline22; 

class Interpolant2D 
{ 
    public: 
        Interpolant2D( Kernel k = Kernel(), unsigned int poly_dim = 3 ); 

        Interpolant2D( const Interpolant2D & ) = default; 
        Interpolant2D& operator=( const Interpolant2D & ) = default;

        /* Interpolant2D( Interpolant2D && ) = default; */ 
        /* Interpolant2D& operator=( Interpolant2D && ) = default; */ 

        double operator()( double x, double y ); 
        double operator()( Point2 &p ); 

        double Dx( double x, double y ); 
        double Dx( Point2 &p ); 

        double Dy( double x, double y ); 
        double Dy( Point2 &p ); 

        bool interpolantIsSet(); 
        int isSmooth(); 

        bool addPointEvaluation( const Point2 &p, double feval ); 
        bool addPointEvaluation( Point2 &&p, double feval ); 
        bool addPointEvaluations( std::vector<Point2> &points, std::vector<double> &fevals ); 

        bool interpolate(); 

        void clear(); 

    private:
        Kernel k_; 
        unsigned int dim_; 
        unsigned int poly_dim_; 

        std::vector<Point2> points_;
        std::vector<double> fevals_; 
        std::vector<double> weights_; 

        bool interpolant_is_set_ = false;  

        // Cereal provides an easy way to serialize objects
        friend class cereal::access;

        template<typename Archive>
        void serialize( Archive &ar, const std::uint32_t version );
};


#endif
