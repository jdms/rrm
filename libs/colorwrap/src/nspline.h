/* Copyright (c) 2018 Julio Daniel Machado Silva */

/* Licensed under the Apache License, Version 2.0 (the "License"); */
/* you may not use this file except in compliance with the License. */
/* You may obtain a copy of the License at */

/* http://www.apache.org/licenses/LICENSE-2.0 */

/* Unless required by applicable law or agreed to in writing, software */
/* distributed under the License is distributed on an "AS IS" BASIS, */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/* See the License for the specific language governing permissions and */
/* limitations under the License. */


#ifndef __NSPLINE_H__
#define __NSPLINE_H__

/* #include <cmath> // just because of the function 'pow' */ 
#include <vector>

/* MSVC is unable to honour the SYSTEM directive in CMake, thus creating a
 * stream of warnings steaming from Eigen.  The following pragma(s) is (are)
 * used to prevent such behaviour.  */

/* Prevent warnings of type: */ 
/*     >> 'conversion' conversion from 'type1' to 'type2', possible loss of data
 *     */
#if defined(_WIN32)
    #pragma warning( push )
    #pragma warning( disable : 4244 ) //__pragma( warning(disable : 4244) ) 
#endif

#include <Eigen/Dense>


using namespace Eigen;
using namespace std;


inline double __identity_map( double x ) { return x; }
inline double __D_identity_map( double ) { return 1; }
inline double __D2_identity_map( double ) { return 0; }


class NSpline {

    public:
        NSpline( double (*f) ( double ) = __identity_map, /* = diffeo, change of coordinates */ 
                double (*Df) ( double ) = __D_identity_map, double (*D2f) ( double ) = __D2_identity_map ) 
        {
            _H = f;
            _DH = Df;
            _D2H = D2f;

            initialized = false; 
        }
        
        NSpline( vector<double> C /* = centers */, vector<double> F /* = function evaluations */ ) {
            _H = __identity_map; 
            _DH = __D_identity_map; 
            _D2H = __D2_identity_map;
            
            init( C, F );
        }
        
        /* NSpline( NSpline &s ) */ 
        /* { */
        /*     _s = new NSpline; */ 
        /*     *_s = s; */
            
        /*     _H = [this] ( double x ) -> double { return _s->eval(x); }; */
        /*     _DH = [this] ( double x ) -> double { return _s->evalD(x); }; */ 
        /*     _D2H = [this] ( double x ) -> double { return _s->evalD2(x); }; */ 

        /*     initialized = false; */             
        /* } */

        /* NSpline operator=( NSpline &s ) */ 
        /* { */
        /*     NSpline _s_out; */ 
            
        /*     _s_out.initialized = s.initialized; */
        /*     _s_out._H = s._H; */
        /*     _s_out._DH = s._DH; */
        /*     _s_out._D2H = s._D2H; */ 

        /*     _s_out._s = s._s; */
        /*     _s_out._C = s._C; */
        /*     _s_out._alpha = s._alpha; */
        /*     _s_out._beta = s._beta; */

        /*     return _s_out; */ 
        /* } */

        /* ~NSpline() {} */

        bool init( vector<double> C /* = centers */, vector<double> F /* = function evaluations */  ) {

            /* assert( C.size() == F.size() ); */

            if ( C.size() != F.size() )
            {
                return false;
            }

            if ( C.empty() )
            {
                return false;
            }

            size_t m = C.size();
            _C.resize(m);

            for ( size_t i = 0; i < m; ++i ) {
                _C(i) = _H( C.at(i) );
            }

            VectorXd _F(m+2);
            /* VectorXd _F(m); */
            for ( size_t i = 0; i < m; ++i ) {
                _F(i) = F.at(i);
            }

            _F(m) = 0;
            _F(m+1) = 0;

            MatrixXd A = buildA();
            MatrixXd P = buildP();

            MatrixXd T( m + 2, m + 2 );

            T.block( 0, 0, m, m ) = A;
            T.block( 0, m, m, 2 ) = P;
            T.block( m, 0, 2, m ) = P.transpose();
            T.block( m, m, 2, 2 ) = MatrixXd::Constant( 2, 2, 0 );
            
            VectorXd y = T.fullPivHouseholderQr().solve( _F );
            _alpha = y.block( 0, 0, m, 1 );
            _beta = y.block( m, 0, 2, 1 );

            /* MatrixXd invAF = A.ldlt().solve( _F ); */
            /* MatrixXd invAP = A.ldlt().solve( P ); */

            /* MatrixXd B = P.transpose() * invAP; */ 
            /* _beta = B.ldlt().solve( P.transpose() * invAF ); */
            /* _alpha = invAF - invAP * _beta; */

            initialized = true;
            return true;
        }

        double operator()( double x )
        {
            return eval(x);
        }

        double D( double x )
        {
            return evalD(x);
        }

        double D2( double x )
        {
            return evalD2(x);
        }

        double eval( double x ) {

            /* assert( initialized ); */
            if ( !initialized )
            {
                return 0;
            }

            double y = 0;
            
            for ( int i = 0; i < _C.size(); ++i ) {
                y += _alpha(i) * phi( _H(x) - _C(i) );
            }
            y += _beta(0) + _beta(1) * _H(x);
            
            return y;
        }

        double evalD( double x ) { 

            /* assert( initialized ); */
            if ( !initialized )
            {
                return 0;
            }

            double dy = 0;

            for ( int i = 0; i < _C.size(); ++i ) {
                dy += _alpha(i) * dphi( _H(x) - _C(i) ) * _DH(x); 
            }
            dy += _beta(1) * _DH(x);
            
            return dy;
        }

        double evalD2( double x ) { 

            /* assert( initialized ); */
            if ( !initialized )
            {
                return 0;
            }

            double d2y = 0;

            for ( int i = 0; i < _C.size(); ++i ) {
                /* d2y += _alpha(i) * d2phi( _H(x) - _C(i) ) * pow(_DH(x), 2) + _alpha(i) * dphi( _H(x) - _C(i) ) * _D2H(x); */ 
                d2y += _alpha(i) * d2phi( _H(x) - _C(i) ) * _DH(x)*_DH(x) + _alpha(i) * dphi( _H(x) - _C(i) ) * _D2H(x); 
            }
            d2y += _beta(1) * _D2H(x); 
            
            return d2y;
        }

    private: 
        bool initialized; 

        std::function<double(double)> _H; // _H is a chnage of coordinates  
        std::function<double(double)> _DH; // Derivative of _H  
        std::function<double(double)> _D2H; // Second derivative of _H  

        /* NSpline *_s; */

        VectorXd _C; // centers 

        VectorXd _alpha; // cubic spline coeficients 
        VectorXd _beta;  // linear part coeficients 

        inline static double abs( double x ) 
        {
            return ( x >= 0 ? x : -x );
        }

        inline static double phi( double x ) { 

            /* return pow( abs(x), 3 ); */
            return x*x*abs(x);
        }

        inline static double dphi( double x ) { 

            return 3*abs(x) * x; 
        }
        
        inline static double d2phi( double x ) { 

            return 6*abs(x); 
        }

        MatrixXd buildA() {
            
            int m = static_cast<int>(_C.size());
            /* assert ( m > 0 ); */
            MatrixXd A(m,m);

            for ( int i = 0; i < m; ++i ) {
                for ( int j = 0; j < m; ++j ) {

                    A(i,j) = phi( _C(i) - _C(j) );
                }
            }
            double epsilon = 1E-05;

            if ( epsilon > 0 ) { 
                return A + epsilon * MatrixXd::Identity( m, m );
            } else { 
                return A; 
            }
        }

        MatrixXd buildP() {

            int m = static_cast<int>(_C.size());
            /* assert( m > 0 ); */
            MatrixXd P(m, 2);

            // P.block<1,m>(0,0) == P.block(0,0,1,m);
            P.block( 0, 0, m, 1 ) = MatrixXd::Constant(m, 1, 1);
            P.block( 0, 1, m, 1 ) = _C;

            return P;
        }

    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#if defined(_WIN32)
    #pragma warning( pop )
#endif 

#endif
