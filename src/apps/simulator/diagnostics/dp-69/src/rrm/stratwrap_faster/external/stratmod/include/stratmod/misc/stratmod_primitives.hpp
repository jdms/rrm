/********************************************************************************/
/*                                                                              */
/* This file is part of the "Stratigraphy Modeller Library" (StratModLib)       */
/* Copyright (C) 2017, Julio Daniel Machado Silva.                              */
/*                                                                              */
/* StratModLib is free software; you can redistribute it and/or                 */
/* modify it under the terms of the GNU Lesser General Public                   */
/* License as published by the Free Software Foundation; either                 */
/* version 3 of the License, or (at your option) any later version.             */
/*                                                                              */
/* StratModLib is distributed in the hope that it will be useful,               */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU            */
/* Lesser General Public License for more details.                              */
/*                                                                              */
/* You should have received a copy of the GNU Lesser General Public             */
/* License along with StratModLib.  If not, see <http://www.gnu.org/licenses/>, */
/* or write to the Free Software Foundation, Inc., 51 Franklin Street,          */
/* Fifth Floor, Boston, MA  02110-1301  USA.                                    */
/*                                                                              */
/********************************************************************************/



#ifndef STRATMOD_PRIMITIVES_HPP
#define STRATMOD_PRIMITIVES_HPP

#include "stratmod/misc/stratmod_win_dll_export_handler.hpp"

namespace stratmod {

/* Avoids warning C4251 in VS2013, see macro definition in file
 * 'win_dll_export_handler.hpp' 
 */
STRATMOD_SILENCE_MSVC_WIN_DLL_EXPORT_WARNING
struct SModellerImplementation; 

namespace detail {
    STRATMOD_SILENCE_MSVC_WIN_DLL_EXPORT_WARNING
    class SModellerAccess;
} // namespace detail
} // namespace stratmod

// To allow cereal to access private members of a class:
//
// friend class cereal::access;
//
namespace cereal {
    class access;
}

// To allow cereal to assign version numbers to structs defined in the private
// scope of a class (useful when using the PIMPL idiom):
//
// template<typename T, typename B> friend struct cereal::detail::Version;
//
namespace cereal {
    namespace detail {
        template<typename T, typename B> struct Version;
    }
}

#endif

