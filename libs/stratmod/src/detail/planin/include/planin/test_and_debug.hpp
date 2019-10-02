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


#ifndef __TEST_AND_DEBUG_HPP__
#define __TEST_AND_DEBUG_HPP__

#include <iostream>
#include <cmath> 
#include "planin/interpolated_graph.hpp"

template<typename T>
void print_data( T &input, T &output ) 
{
    if ( input.size() % 3 != 0 ) { 
        std::cout << "Error: input data size is not a multiple of 3.  " << std::endl; 
        return; 
    }

    if ( output.size() % 3 != 0 ) { 
        std::cout << "Error: output data size is not a multiple of 3.  " << std::endl; 
        return; 
    }

    std::cout << std::endl << "Debug information: " << std::endl; 
    std::cout << std::endl << "Input data: " << std::endl; 
    for ( size_t i = 0; i < input.size()/3; ++i ) 
    {
        std::cout << "Point " << i << ": (" << input[3*i + 0] << ", " << input[3*i + 1] << ", " << input[3*i + 2] << ") " << std::endl; 
    }

    std::cout << std::endl << "Output data: " << std::endl; 
    for ( size_t i = 0; i < output.size()/3; ++i ) 
    {
        std::cout << "Point " << i << ": (" << output[3*i + 0] << ", " << output[3*i + 1] << ", " << output[3*i + 2] << ") " << std::endl; 
    }
    std::cout << std::endl; 

}

template<typename T, typename K>
bool check_for_equality( T &input, T &output, K &&error = 0 )
{
    if ( input.size() != output.size() ) { 
        std::cout << "ERROR: input.size() != output.size().  " << std::endl; 
        return false; 
    }

    bool status = true; 

    using ValueType = typename T::value_type; 
    ValueType max_error, current_error; 
    max_error = static_cast<ValueType>(error); 

    for ( size_t i = 0; i < input.size(); ++i )
    {
        current_error = fabs( input[i] - output[i] ); 
        status &= ( current_error < static_cast<ValueType>(error) ); 

        if ( current_error > static_cast<ValueType>(error) ) { 
            max_error = ( max_error < current_error ) ? current_error : max_error; 
            std::cout << "ERROR: input[" << i 
                      << "] and output[" << i 
                      << "] differ by " 
                      <<  current_error  
                      << ", while error tolerance is " 
                      << static_cast<ValueType>(error)  << ".  "
                      << std::endl; 
        }
    }

    if ( status == false ) { 
        std::cout << "ERROR: max|input[i] - output[i]| = " << max_error << std::endl; 
        error = static_cast<K>(max_error); 
    }

    return status; 
}

bool test_interpolation()  
{
    /* Initialize variables. */ 
    InterpolatedGraph graph; 
    using Container = std::vector<double>; 

    Container input = {{ 0.0, 0.0, 1.0/2, 0.0, 1.0, -1.0/2, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0 }}; 
    Container output = {{ 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0 }}; 

    Point3 p; 
    for ( size_t i = 0; i < input.size()/3; ++i )
    {
        p[0] = input[i*3 + 0];
        p[1] = input[i*3 + 1];
        p[2] = input[i*3 + 2];

        graph.addPoint(p); 
    }


    /* Compute interpolant. */ 
    graph.generateSurface(); 


    /* Evaluate interpolant. */ 
    graph.getRawHeightMap(output); 


    /* Display data and run test. */ 
    print_data(input, output); 
    double error_tolarance = 1E-7; 
    bool test_result = check_for_equality( input, output, error_tolarance ); 


    /* Finish up. */ 
    if ( test_result == false ) { 
        std::cout << std::endl <<  "INTERPOLATION TEST: FAILED.  " << std::endl; 
    }
    else { 
        std::cout << std::endl << "Interpolation test: SUCCESS.  " << std::endl; 
    }

    return test_result; 
}

#endif 

