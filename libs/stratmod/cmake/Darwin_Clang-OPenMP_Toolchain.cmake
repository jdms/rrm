# Toolchain file to provide support for OpenMP in MacOS, based on clang-omp 
set(CMAKE_SYSTEM_NAME Darwin)

# Set the folowing version number to match OSX version number. 
# As of today: El Capitain (10.11) -> 11 
set(CMAKE_SYSTEM_VERSION 11)

# specify the cross compiler
# SET(CMAKE_C_COMPILER   /usr/local/bin/clang-omp)
# SET(CMAKE_CXX_COMPILER /usr/local/bin/clang-omp++)

set(LLVM_MACOS_CLANG /usr/local/opt/llvm/bin/clang++)

if (EXISTS "${LLVM_MACOS_CLANG}")
    set(CMAKE_C_COMPILER /usr/local/opt/llvm/bin/clang)
    set(CMAKE_CXX_COMPILER /usr/local/opt/llvm/bin/clang++)

    # Weird work-around to deal with compiler flags for non-native Toolchains: 
    # http://stackoverflow.com/questions/11423313/cmake-cross-compiling-c-flags-from-toolchain-file-ignored

    # UNSET( CMAKE_CXX_FLAGS CACHE )
    # SET( CMAKE_CXX_FLAGS "-v -std=c++1y -stdlib=libc++ -Wall -Wextra -fopenmp -O3" CACHE STRING "" FORCE )

    unset(CMAKE_C_FLAGS CACHE)
    set(CMAKE_C_FLAGS "-I/usr/local/opt/llvm/include -L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib")

    unset(CMAKE_CXX_FLAGS CACHE)
    set(CMAKE_CXX_FLAGS "-I/usr/local/opt/llvm/include -L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib")

    unset(CMAKE_EXE_LINKER_FLAGS CACHE)
    set(CMAKE_EXE_LINKER_FLAGS "-L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib")

    unset(CMAKE_MODULE_LINKER_FLAGS CACHE)
    set(CMAKE_MODULE_LINKER_FLAGS "-L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib")

    unset(CMAKE_SHARED_LINKER_FLAGS CACHE)
    set(CMAKE_SHARED_LINKER_FLAGS "-L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib")

    unset(CMAKE_STATIC_LINKER_FLAGS CACHE)
    set(CMAKE_STATIC_LINKER_FLAGS "-L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib")

else()
    message("Error trying to use OpenMP in MacOS, LLVM Clang was not found: turn USE_OPENMP OFF.")

endif()

# where is the target environment 
# SET(CMAKE_FIND_ROOT_PATH /usr/local/ /usr/)

# search for programs in the build host directories
# SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
# SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
