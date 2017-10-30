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


#ifndef MATLAB_UTILITIES_H
#define MATLAB_UTILITIES_H

#include <vector> 
#include <string> 
#include <sstream>
#include <iostream>
#include <fstream>

using string = std::string; 

template< typename Real > 
bool load_matlab_vector( std::vector<Real> &x, std::string filename = "vector.mat" ) {  

    std::ifstream file( filename ); 
    if ( !file.good() ) { 
        return false; 
    }

    std::string line; 
    std::istringstream is; 

    Real value; 
    auto get_value = [&] () { 
        try { 
            is.clear(); 
            is.str(line); 
            is >> value;

            //                std::cout.precision( std::numeric_limits<Real>::max_digits10);
            //                std::cout << "Line read: " << line
            //                << ", value stored: " << std::scientific << value << std::endl;
        }
        catch( std::exception const &ex ) { 
            throw; 
        }
    };

    x.clear(); 
    while ( std::getline( file, line ) ) { 
        get_value(); 
        x.push_back(value); 
    }

    return true; 
}

template< typename Real > 
bool load_matlab_vector3( std::vector<Real> &x, std::string filename = "vector3.mat" ) {  

    std::ifstream file( filename ); 
    if ( !file.good() ) { 
        return false; 
    }

    std::string line; 
    std::istringstream is; 

    Real value1, value2, value3; 
    auto get_value = [&] () { 
        try { 
            is.clear(); 
            is.str(line); 
            is >> value1 >> value2 >> value3;

            //                std::cout.precision( std::numeric_limits<Real>::max_digits10);
            //                std::cout << "Line read: " << line
            //                << ", value stored: " << std::scientific << value << std::endl;
        }
        catch( std::exception const &ex ) { 
            throw; 
        }
    };

    x.clear(); 
    while ( std::getline( file, line ) ) { 
        get_value(); 
        x.push_back(value1); 
        x.push_back(value2); 
        x.push_back(value3); 
    }

    return true; 
}

template< typename Real > 
bool save_matlab_vector3( std::vector<Real> &x, std::string filename = "vector3.mat" ) {  

    std::ofstream file( filename ); 
    if ( !file.good() ) { 
        return false; 
    }

    size_t size = x.size(); 

    if ( size % 3 != 0 ) { 
        return false; 
    }

    for ( size_t i = 0; i < size/3; ++i ) { 
        try {
            file << x[3*i + 0] << " " 
                 << x[3*i + 1] << " " 
                 << x[3*i + 2] << std::endl; 
        }
        catch( std::exception &ex ) { 
            throw; 
        }
    }

    return true; 
}

template< typename Real >
bool load_matlab_tensor( std::vector<Real> &t, size_t &_sizeX, size_t &_sizeY, size_t &_sizeZ, std::string filename = "tensor.mat" ) {

    std::ifstream file( filename );
    if ( !file.good() ) {
        return false;
    }

    std::istringstream is;
    std::string line, headerX, headerY, headerZ;
    double sizeX = 0, sizeY = 0, sizeZ = 0;
    try {
        std::getline( file, headerX );
        is.clear();
        is.str(headerX);
        is >> sizeX;

        std::getline( file, headerY );
        is.clear();
        is.str(headerY);
        is >> sizeY;

        std::getline( file, headerZ );
        is.clear();
        is.str(headerZ);
        is >> sizeZ;
    }
    catch( std::exception const &ex ) {
        throw;
    }
    _sizeX = static_cast<size_t>(sizeX); 
    _sizeY = static_cast<size_t>(sizeY); 
    _sizeZ = static_cast<size_t>(sizeZ); 


    Real value;
    int size = sizeX * sizeY * sizeZ;
    auto get_value = [&] () {
        try {
            std::getline( file, line );
            is.clear();
            is.str(line);
            is >> value;

            //            std::cout.precision( std::numeric_limits<Real>::max_digits10);
            //            std::cout << "Line read: " << line
            //            << ", value stored: " << std::scientific << value << std::endl;
        }
        catch( std::exception const &ex ) {
            throw;
        }
    };

    t.resize( size );
    for ( int i = 0; i < size; ++i ) {
        get_value();
        t[i] = value;
        //        std::cout.precision( std::numeric_limits<Real>::max_digits10);
        //        std::cout << "Value stored: " << std::scientific << value << std::endl;
    }

    return true;
}

template< typename Real >
bool load_matlab_grid( std::vector<Real> &x, std::vector<Real> &y, std::vector<Real> &z, size_t &_sizeX, size_t &_sizeY, size_t &_sizeZ, std::string filename = "grid.mat" ) { 

    std::ifstream file( filename ); 
    if ( !file.good() ) { 
        return false; 
    }

    std::istringstream is; 
    std::string line, headerX, headerY, headerZ; 
    double sizeX = 0, sizeY = 0, sizeZ = 0;
    try { 
        std::getline( file, headerX );
        is.clear();
        is.str(headerX);
        is >> sizeX;

        std::getline( file, headerY ); 
        is.clear();
        is.str(headerY);
        is >> sizeY;

        std::getline( file, headerZ );
        is.clear();
        is.str(headerZ);
        is >> sizeZ;

        //        is.str( headerX + " " + headerY + " " + headerZ );
        //        is >> sizeX >> sizeY >> sizeZ;
    }
    catch( std::exception const &ex ) { 
        throw; 
    }

    _sizeX = static_cast<size_t>(sizeX); 
    _sizeY = static_cast<size_t>(sizeY); 
    _sizeZ = static_cast<size_t>(sizeZ); 

    Real value; 
    int size = static_cast<int>(sizeX * sizeY * sizeZ);
    auto get_value = [&] () { 
        try {
            std::getline( file, line );
            is.clear(); 
            is.str(line); 
            is >> value;

            //                std::cout.precision( std::numeric_limits<Real>::max_digits10);
            //                std::cout << "Line read: " << line
            //                << ", value stored: " << std::scientific << value << std::endl;
        }
        catch( std::exception const &ex ) { 
            throw; 
        }
    };

    x.resize( size ); 
    for ( int i = 0; i < size; ++i ) { 
        get_value(); 
        x[i] = value; 
    }

    y.resize( size ); 
    for ( int i = 0; i < size; ++i ) { 
        get_value(); 
        y[i] = value; 
    }

    z.resize( size ); 
    for ( int i = 0; i < size; ++i ) { 
        get_value(); 
        z[i] = value; 
    }

    return true; 
}

template< typename Real > 
bool save_matlab_grid( std::vector<Real> const &x, std::vector<Real> const &y, std::vector<Real> const &z, size_t size_x, size_t size_y, size_t size_z, std::string filename = "fgrid.mat" ) { 

    std::ofstream file( filename ); 
    if ( !file.good() ) { 
        return false; 
    }
    size_t size = size_x * size_y * size_z;
    if ( (size != x.size()) || (size != y.size()) || (size != z.size()) ) {
        return false;
    }

    try {
        file << size_x << std::endl
            << size_y << std::endl
            << size_z << std::endl;
    }
    catch( std::exception &ex ) {
        throw;
    }

    for ( size_t i = 0; i < size; ++i ) { 
        try {
            file << x[i] << std::endl; 
        }
        catch( std::exception &ex ) { 
            throw; 
        }
    }

    for ( size_t i = 0; i < size; ++i ) { 
        try {
            file << y[i] << std::endl; 
        }
        catch( std::exception &ex ) { 
            throw; 
        }
    }

    for ( size_t i = 0; i < size; ++i ) { 
        try {
            file << z[i] << std::endl; 
        }
        catch( std::exception &ex ) { 
            throw; 
        }
    }

    return true; 
}

template< typename Real >
bool save_matlab_tensor( std::vector<Real> const &t, int x_size, int y_size, int z_size, std::string filename = "ftensor.mat" ) {

    std::ofstream file( filename );
    if ( !file.good() ) {
        return false;
    }

    size_t size = x_size * y_size * z_size;
    if ( size != t.size() ) {
        return false;
    }

    try {
        file << x_size << std::endl
            << y_size << std::endl
            << z_size << std::endl;
    }
    catch( std::exception &ex ) {
        throw;
    }

    for ( size_t i = 0; i < size; ++i ) {
        try {
            file << t[i] << std::endl;
        }
        catch( std::exception &ex ) {
            throw;
        }
    }

    return true; 
}


template<typename Real, typename Point> 
class MatlabHelper { 
    public: 
        string input_grid = "grid.mat"; 
        string input_evaluations = "evaluations.mat"; 
        string input_weights = "weights.mat"; 
        string input_tensor = "tensor.mat"; 
        string input_map_domain = "map3_domain.mat"; 
        string input_map_image = "map3_image.mat"; 

        string output_grid = "ogrid.mat"; 
        string output_weights = "oweights.mat"; 
        string output_tensor = "otensor.mat"; 

        std::vector<Real> x, y, z, t, weights, weights3, fevaluations, fevaluations3; 
        std::vector<Real> map3_domain, map3_image; 

        bool load_grid() { 
            size_t sizeX = 0, sizeY = 0, sizeZ = 0; 
            has_grid = load_matlab_grid( x, y, z, sizeX, sizeY, sizeZ, input_grid ); 

            if ( !grid_size_is_set ) { 
                set_size(sizeX, sizeY, sizeZ); 
            }
            else if ( (sizeX != size_x ) || (sizeY != size_y) || (sizeZ != size_z) ) { 
                x.clear(); 
                y.clear(); 
                z.clear(); 
                return false; 
            }

            return has_grid; 
        }

        bool check_grid() { return has_grid; }

        bool load_tensor() { 
            size_t sizeX = 0, sizeY = 0, sizeZ = 0; 
            has_tensor = load_matlab_tensor( x, y, z, sizeX, sizeY, sizeZ, input_tensor ); 

            if ( !grid_size_is_set ) { 
                set_size(sizeX, sizeY, sizeZ); 
            }

            else if ( (sizeX != size_x ) || (sizeY != size_y) || (sizeZ != size_z) ) { 
                t.clear(); 
                has_tensor = false; 
                return false; 
            }

            return has_tensor; 
        }

        bool check_tensor() { return false; } 

        /* bool load_vector(); */ 

        bool load_fevaluations() { 
            bool result = load_matlab_vector( fevaluations, input_evaluations ); 
            if ( result == false ) { 
                fevaluations.clear();
                return false; 
            }

            return true; 
        }

        bool load_fevaluations3() { 
            bool result = load_matlab_vector3( fevaluations3, input_evaluations ); 
            if ( result == false ) { 
                fevaluations3.clear();
                return false; 
            }

            return true; 
        }

        bool load_weights() {  
            bool result = load_matlab_vector( weights, input_weights ); 
            if ( result == false ) { 
                weights.clear();
                return false; 
            }

            return true; 
        } 

        bool check_weights() { 
            if ( !has_samples ) { 
                return false; 
            }

            if ( weights.is_empty() ) { 
                return false; 
            } 

            if ( weights.size() != samples ) { 
                return false; 
            }

            return true; 
        }

        bool set_weights( std::vector<Real> &w ) { 
            if ( has_samples && (w.size() != samples) ) { 
                return false; 
            }

            weights.clear(); 
            weights.insert( weights.begin(), w.begin(), w.end() ); 

            return true; 
        }

        bool load_weights3() {  
            bool result = load_matlab_vector3( weights3, input_weights ); 
            if ( result == false ) { 
                weights3.clear();
                return false; 
            }

            return true; 
        } 

        bool check_weights3() { 
            if ( !has_samples ) { 
                return false; 
            }

            if ( weights3.is_empty() ) { 
                return false; 
            } 

            if ( weights3.size()/3 != samples ) { 
                return false; 
            }

            return true; 
        }

        bool set_weights3( std::vector<Real> &w ) { 
            if ( has_samples && (w.size() != samples) ) { 
                return false; 
            }

            weights3.clear(); 
            weights3.insert( weights3.begin(), w.begin(), w.end() ); 

            return true; 
        }

        bool load_map3() {  
            bool result = load_matlab_vector3( map3_domain, input_map_domain ); 
            result &= load_matlab_vector3( map3_image, input_map_image ); 
            result &= check_map3(); 

            if ( result == false ) { 
                map3_domain.clear(); 
                map3_image.clear(); 
                return false; 
            }

            return true; 
        } 

        bool check_map3() { 
            if ( map3_domain.empty() ) { 
                return false; 
            } 

            if ( map3_domain.size() != map3_image.size() ) { 
                return false; 
            }

            return true; 
        }

        bool save_grid() { 
            if ( !grid_size_is_set ) { 
                return false; 
            }

            if ( !has_grid ) { 
                return false; 
            } 

            return save_matlab_grid(x, y, z, size_x, size_y, size_z, output_grid ); 
        }

        void set_tensor( bool flag ) { has_tensor = flag; }
        bool save_tensor() { 
            if ( !grid_size_is_set ) { 
                return false; 
            }

            if ( !has_tensor ) { 
                return false; 
            } 

            return save_matlab_tensor(t, size_x, size_y, size_z, output_tensor ); 
        }

        bool get_size( size_t &_size_x, size_t &_size_y, size_t &_size_z ) { 
            if ( grid_size_is_set ) { 
                return false; 
            }

            _size_x = size_x; 
            _size_y = size_y; 
            _size_z = size_z; 

            return true; 
        }

        void set_size( size_t _size_x, size_t _size_y, size_t _size_z ) { 
            size_x = _size_x; 
            size_y = _size_y; 
            size_z = _size_z; 

            grid_size_is_set = true; 
        }

    private: 
        bool has_samples = false; 
        size_t samples= 0; 

        bool grid_size_is_set = false; 
        bool has_grid = false; 
        bool has_tensor = false; 
        size_t size_x = 0; 
        size_t size_y = 0; 
        size_t size_z = 0; 

        bool debug_mode = false; 
};

#endif

