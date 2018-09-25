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


#include "interpolant_2d.hpp"

/* Disable MSVC warnings due to the following (GCC) unknown pragmas */
/* https://msdn.microsoft.com/en-us/library/2c8f766e.aspx 
 */
#pragma warning (disable : 4068)

// Eigen & GCC 5 : class std::binder2nd is deprecated
// http://stackoverflow.com/questions/30535933/eigen-gcc-5-class-stdbinder2nd-is-deprecated
// FIXED: for some weird reason, the "system" keyword is not working when
// including Eigen's headers in the "CMakeLists.txt" file.  Solution: used
// incorrect call to Eigen3_INCLUDE_DIRS, which needed to be changed to
// EIGEN3_INCLUDE_DIR
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
/* TODO: include special inclusion for Eigen in Windows */
#include "Eigen/Dense" 
#pragma GCC diagnostic pop 

/* #include "prettyprint.hpp" */ //debug 

Interpolant2D::Interpolant2D( Kernel k, unsigned int poly_dim ) : 
    k_(k), dim_(2) 
{
    UNUSED(dim_); 

    if ( (poly_dim == 2) || (poly_dim == 3) ) 
        poly_dim_ = poly_dim; 
    else
        poly_dim_ = 3; 
}  

double Interpolant2D::operator()( double x, double y ) 
{
    if ( !interpolantIsSet() ) { 
        return 0; 
    }

    double z = 0; 
    const size_t size = points_.size(); 

    for ( size_t i = 0; i < size; ++i ) 
    {
        z += weights_[i] * k_( x - points_[i].x, y - points_[i].y ); 
    }
    if ( poly_dim_ >= 2 ) {
        z += weights_[size]*1.0 + weights_[size + 1]*x; 
    }
    
    if ( poly_dim_ == 3 ) {
        z += weights_[size + 2]*y;
    }

    return z; 
}

double Interpolant2D::operator()( Point2 &p ) 
{
    return operator()(p.x, p.y); 
}

double Interpolant2D::Dx( double x, double y ) 
{ 
    if ( !interpolantIsSet() ) { 
        return 0; 
    }

    if ( k_.isSmooth() < 1 ) { 
        return 0; 
    }

    double z = 0; 
    const size_t size = points_.size(); 

    for ( size_t i = 0; i < size; ++i ) 
    {
        z += weights_[i] * k_.Dx( x - points_[i].x, y - points_[i].y ); 
    }

    if ( poly_dim_ >= 2 ) {
        z += weights_[size + 1]; 
    }

    return z; 
}
 
double Interpolant2D::Dx( Point2 &p ) 
{ 
    return Dx(p.x, p.y);
}

double Interpolant2D::Dy( double x, double y ) 
{
    if ( !interpolantIsSet() ) { 
        return 0; 
    }

    if ( k_.isSmooth() < 1 ) { 
        return 0; 
    }

    if ( poly_dim_ < 3 ) { 
        return 0; 
    }

    double z = 0; 
    const size_t size = points_.size(); 

    for ( size_t i = 0; i < size; ++i ) 
    {
        z += weights_[i] * k_.Dy( x - points_[i].x, y - points_[i].y ); 
    }
    z += weights_[size + 2]; 

    return z;    
} 

double Interpolant2D::Dy( Point2 &p ) 
{ 
    return Dy(p.x, p.y);
}

int Interpolant2D::isSmooth() 
{ 
    return k_.isSmooth();
}

bool Interpolant2D::interpolantIsSet() 
{
    return interpolant_is_set_; 
} 

bool Interpolant2D::addPointEvaluation( const Point2 &p, double feval ) 
{
    points_.push_back(p); 
    fevals_.push_back(feval); 

    return true; 
} 

bool Interpolant2D::addPointEvaluation( Point2 &&p, double feval ) 
{
    return addPointEvaluation(p, feval); 
}

bool Interpolant2D::addPointEvaluations( std::vector<Point2> &points, std::vector<double> &fevals ) 
{
    if ( points.size() != fevals.size() ) { 
        return false; 
    }

    std::copy( points.begin(), points.end(), points_.end() ); 
    std::copy( fevals.begin(), fevals.end(), fevals_.end() ); 

    return true; 
}

bool Interpolant2D::interpolate() 
{
    /* TODO: this methos assumes that input points_ is always valid, beware */
    if ( points_.size() != fevals_.size() ) { 
        return false; 
    }
    
    if ( points_.size() == 0 )
    {
        return false;
    }

    size_t size = points_.size(); 
    weights_.resize(size + poly_dim_); 

    auto local_fevals = fevals_; 
    for ( size_t i = 0; i < poly_dim_; ++i ) { 
        local_fevals.push_back(0.0);
    }

    const double eigenvalues_lower_bound = 0.01; 
    double aij = 0; 
    Point2 pi, pj; 
    Eigen::MatrixXd A(size + poly_dim_, size + poly_dim_); 
    Eigen::MatrixXd P(size, poly_dim_); 
    Eigen::Map<Eigen::VectorXd> weights( weights_.data(), size + poly_dim_, 1); 
    Eigen::Map<Eigen::VectorXd> fevaluations( local_fevals.data(), size + poly_dim_, 1); 

    for ( size_t i = 0; i < size; ++i ) { 
        pi = points_[i]; 
        P(i,0) = 1; 
        P(i,1) = pi.x; 
        if ( poly_dim_ == 3 )
            P(i,2) = pi.y; 

            /* std::cout << "P(" << i << ") = " << 1 << ", " << pi.x << ", " << pi.y << std::endl; */ //debug
        for ( size_t j = 0; j < size; ++j ) { 
            pj = points_[j]; 
            aij = k_(pi.x - pj.x, pi.y - pj.y); 
            aij += ( i == j ) ? eigenvalues_lower_bound : 0.0; 

            A(i,j) = aij; 
            /* std::cout << "A(" << i << ", " << j << ") = " << aij << std::endl; */ //debug
        }
    }

    A.block(0, size, size, poly_dim_) = P; 
    A.block(size, 0, poly_dim_, size) = P.transpose(); 
    A.block(size, size, poly_dim_, poly_dim_) = Eigen::MatrixXd::Constant(poly_dim_, poly_dim_, 0); 

    weights = A.householderQr().solve(fevaluations); 
    /* std::cout << "weights: " << weights << std::endl; */ //debug
    /* std::cout << "fevals: " << fevals_ << std::endl; */ //debug

    interpolant_is_set_ = true; 
    return true; 
}

void Interpolant2D::clear() 
{
    points_.clear(); 
    fevals_.clear(); 
    weights_.clear(); 

    interpolant_is_set_ = false; 
}


