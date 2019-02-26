RRM -- November 2018 release
============================

The Rapid Reservoir Modelling prototype is free software (mostly GPLv3,
with a few components and libraries having different, yet compatible,
licenses -- libraries' licenses are available in the libraries'
directories).

It is composed of four main pieces:

1.  a graphical user interface (GUI) that processes sketches and user
    inputs (such as pictures, etc.) to create geologic models, and
    provides multiple features to visualize and interact with user
    created geological model;

2.  a modeller library that effectively transforms user inputs into a
    correct geological model;

3.  a flow diagnostics GUI that allows setting petrophysical and fluid
    properties for flow simulation, and provides functionality to
    visualize the results of flow computations;

4.  and a flow diagnostics library that tetrahedralizes the models and
    computes flow diagnostics.

The RRM prototype is under active development, and improvements to both
the prototype and the build process are frequent. New versions of the
prototype, and these notes can be found at the [RRM git
repository](https://bitbucket.org:rapidreservoirmodelling/rrm).

Compilling RRM
--------------

The RRM prototype currently depends on the following external software,
that must be obtained and properly installed before RRM can be compiled.

For 32bit builds:

-   The Microsoft Visual Studio 2013 (32bit) compiler, available from
    Microsoft (this dependency is transitory, it was required for
    compatibility with libraries that were recently removed from the
    source code -- RRM is portable and can be compiled with newer
    versions of the Microsoft compiler or GCC);

-   [Qt](https://www.qt.io/download), version 5.8 (ms2013 build) for the
    GUI;

For 64bit builds:

-   The Microsoft Visual Studio 2017 compiler, available from Microsoft;

-   [Qt](https://www.qt.io/download), version 5.8 (ms2017\_64 build) for
    the GUI;

All versions depend on the cmake build system:

-   [CMake](https://cmake.org/download/), version 3.10 or higher, a
    modern, multi platform, build system.

### Using CMake

To create the config files that are necessary to compile the prototype,
we recommend creating a `build` directory inside the project's main
source directory. In what follows, we will describe how to run cmake
from Visual Studio's "Native Tools Command Prompt".

We note that the cmake utility is accompanied by a GUI interface
(cmake-gui) that allows tweaking building options from the GUI, and may
be used instead of the command line approach we adopt in this guide.

We assume that a "build" folder was created inside RRM's source tree.
CMake uses the "CMakeLists.txt" file provided with RRM (which is found
in RRM's root source directory) to properly configure the whole project,
however, RRM default configurations may not be adequate for every build.
We name two configurations that one may want to change. Both changes can
be made in RRM's main CMakeLists.txt file or in the aforementioned
cmake-gui utility.

1.  To choose whether RRM will be compiled in release or debug mode,
    change the `RRM_BUILD_TYPE` variable to `Release` or `Debug`,
    accordingly. Alternativelly, one may prefer to use option
    `RelWithDebInfo` to build an optimized binary with debugger
    sysmbols.

2.  RRM assumes by default that Qt was installed into its default folder
    `C:\Qt`, and Qt's cmake files can be found at:
    1.  `C:\Qt\5.8\msvc2013\lib\cmake\Qt5` (for 32bit builds); or
    2.  `C:\Qt\5.10\msvc2017_x64\lib\cmake\Qt5` (for 64bit builds).

    This choice is controlled by variable `Qt5_DIR`.

From inside this "build" directory, one of the following approaches
should be followed:

### Compiling RRM from the command line:

First, issue the command:

        > C:\path_to_rrm_source\build\cmake .. -G "NMake Makefiles" 

from Visual Studio's command prompt, which creates a set of Makefiles
that can be directly called by nmake as follows:

        > C:\path_to_rrm_source\build\nmake

This will compile RRM, and place its binary in dir
`C:\path_to_rrm_source\build\bin`. Given its simplicity, this is the
recommended approach.

### Compiling RRM from Visual Studio:

For 32bit builds, issue the command:

        > C:\path_to_rrm_source\build\cmake .. -G "Visual Studio 2013"

For 64bit builds, issue the command:

        > C:\path_to_rrm_source\build\cmake .. -G "Visual Studio 2017"

This creates a Visual Studio solution inside the "build directory",
which can be immediately opened and inspected in Visual Studio as if it
was created by Visual Studio itself. Details on how to use Visual Studio
are out of the scope of this guide.

After compilation, Visual Studio will place RRM's binary into its
predefined solution's path, which may differ from the "build" dir. An
extra step is required (in comparison to the command line approach), in
which the user has to manually copy file:

        > C:\path_to_rrm_source\libs\stratmod\stratmod\lib\stramod.dll

to RRM's binary dir (this step was automatically performed by cmake in
the command line approach).

Running RRM
-----------

After compilation, all support libraries should be copied to RRM's
binary dir.

First, the GLEW library (dll) must be copied into the bin folder. It can
be found at:

<http://glew.sourceforge.net>

Second, Qt dlls must be copied into RRM's bin folder. Within a Visual
Studio command prompt, and assuming that Qt was installed into its
default dir, this can be accomplished with the following command.

For 32bit builds:

        > C:\Qt\5.8\msvc2013\bin\windeployqt.exe --release RRM.exe

For 64bit builds:

        > C:\Qt\5.8\msvc2017_64\bin\windeployqt.exe --release RRM.exe

To use Qt libraries with debugger symbols, substitute `release` by
`debug` in the command above.

After release, these libraries will be copied into RRM's source tree,
inside directory "dlls", for both "Release" and "Debug" binaries. For
example, assuming RRM was compiled in release mode following the command
line approach of the previous section, the following command will be
sufficient:

        > C:\path_to_rrm_source> copy dlls\release\*.* build\bin
