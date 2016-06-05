#ifndef __WIN_DLL_EXPORT_HANDLER_HPP__
#define __WIN_DLL_EXPORT_HANDLER_HPP__

#if defined(_WIN32) || defined(_WIN64)

// 
// When building a "SHARED" library in Windows, Cmake exports the name
// 
// '<target>_EXPORTS', (here 'planin_EXPORTS', see preprocessor directives
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
#   ifdef planin_EXPORTS 
// the name 'PLANINLIB_DLL_EXPORT_HANDLER' is my choice, I will call it the EXPORT MACRO. 
#       define PLANINLIB_DLL_EXPORT_HANDLER __declspec(dllexport)
#   else
#       define PLANINLIB_DLL_EXPORT_HANDLER __declspec(dllimport)
#endif

#else // #if defined(_WIN32) || defined(_WIN64)  
#   define PLANINLIB_DLL_EXPORT_HANDLER
#endif

//
// Now, any class that will be exposed in the DLL must be annotated with the 
// 'PLANINLIB_DLL_EXPORT_HANDLER' macro. Example: 
//
//
// class PLANINLIB_DLL_EXPORT_HANDLER myClass : public Base_Class { 
// (... implementation ...)  
// }; 
//

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// NOTICE THAT DIFFERENT DLLS MUST HAVE DIFFERENT EXPORT MACROS. 
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#endif 

