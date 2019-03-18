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



#ifndef __STRATMOD_WIN_DLL_EXPORT_HANDLER_HPP__
#define __STRATMOD_WIN_DLL_EXPORT_HANDLER_HPP__


#if defined(_WIN32) && defined(STRATMOD_BUILD_SHARED_LIBRARY)
// #if defined(_WIN64)

    #if defined(_MSC_VER)
        /* Avoids warning C4251 in VS2013 : 
         *     --> identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2' 
         *     https://msdn.microsoft.com/en-us/library/esew7y1w.aspx
         *     https://msdn.microsoft.com/en-us/library/2c8f766e.aspx
         */
        #define STRATMOD_SILENCE_MSVC_WIN_DLL_EXPORT_WARNING __pragma( warning(disable : 4251) )

    #else
        #define STRATMOD_SILENCE_MSVC_WIN_DLL_EXPORT_WARNING 

    #endif
    // 
    // When building a "SHARED" library in Windows, Cmake exports the name
    // 
    // '<target>_EXPORTS', (here 'stratmod_EXPORTS', see preprocessor directives
    // below) 
    //
    // as a flag to allow the correct processing of the necessary dll handlers. The
    // name '<target>' is defined in the 'CMakeLists.txt' file with cmake's
    // reserved function 'add_library()'.
    //
    // Alternatively, one can define any name at compilation time to achieve the
    // same results. In cmake, this is simply done by appending a definition to be
    // passed to the compiler with cmake's 'add_definitions(-D[FLAG])' function,
    // here "[FLAG]" has to be substituted by the actual flag. Example:
    // "add_definitions(-DMY_COMPLIER_FLAG_TO_EXPORT_DLLS)
    //

    // this is not arbitrary, see previous comment 
    #if defined(stratmod_EXPORTS) 
        // the name 'STRATMODLIB_DLL_HANDLER' is my choice, I will call it the EXPORT MACRO. 
        #define STRATMODLIB_DLL_HANDLER __declspec(dllexport)

    #else //#if defined(stratmod_EXPORTS) 
        #define STRATMODLIB_DLL_HANDLER __declspec(dllimport)

    #endif

#else // #if defined(_WIN32) || defined(_WIN64)  
    #define STRATMOD_SILENCE_MSVC_WIN_DLL_EXPORT_WARNING 

    #define STRATMODLIB_DLL_HANDLER

#endif

//
// Now, any class that will be exposed in the DLL must be annotated with the 
// 'STRATMODLIB_DLL_HANDLER' macro. Example: 
//
//
// class STRATMODLIB_DLL_HANDLER myClass : public Base_Class { 
// (... implementation ...)  
// }; 
//

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// NOTICE THAT DIFFERENT DLLS MUST HAVE DIFFERENT EXPORT MACROS. 
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#endif //__WIN_DLL_EXPORT_HANDLER_HPP__

