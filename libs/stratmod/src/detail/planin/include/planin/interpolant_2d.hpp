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


#ifndef PLANIN_INTERPOLANT_2D_HPP
#define PLANIN_INTERPOLANT_2D_HPP

#include <vector> 
#include <cstdint>

#include "serialization_primitives.hpp"

#include "planin/core.hpp" // Dummy definitions to be replaced by the project's definitions. 
#include "planin/kernels.hpp"
#include "planin/polynomial.hpp" 

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

        bool setFillDistance( double value );
        double getFillDistance();

        bool interpolate(); 

        void clear(); 

        bool getRawData( std::vector<Point2> &points, std::vector<double> &fevals );

    private:
        Kernel k_; 
        unsigned int dim_; 
        unsigned int poly_dim_; 

        std::vector<Point2> points_;
        std::vector<double> fevals_; 
        std::vector<double> weights_; 

        double fill_distance_ = 1.E-11; // based on the fact that DBL_EPSILON ~ 1.E-16

        bool interpolant_is_set_ = false;  

        // Cereal provides an easy way to serialize objects
        friend class cereal::access;

        template<typename Archive>
        void serialize( Archive &ar, const std::uint32_t version );
};

#if defined( BUILD_WITH_SERIALIZATION )
    #include "cereal/types/vector.hpp"

    template<typename Archive>
    void Interpolant2D::serialize( Archive &ar, const std::uint32_t version )
    {
        ar(
            dim_, 
            poly_dim_, 
            points_, 
            fevals_, 
            weights_, 
            interpolant_is_set_
          );

        if ( version >= 2 )
        {
            ar(
                fill_distance_
              );
        }

        /* if ( version >= 3 ) */
        /* { */
        /*     ar( */
        /*         k_ */
        /*       ); */
        /* } */
    }

    CEREAL_CLASS_VERSION(Interpolant2D, 2);

#else
    template<typename Archive>
    void Interpolant2D::serialize( Archive &, const std::uint32_t ) {}

#endif /* BUILD_WITH_SERIALIZATION */

#endif
