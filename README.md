Rapid reservoir modelling
===

Sketch-based geological 3D modelling: Exploring uncertainty with interactive prototyping
-----------------------------------------------------------------------

Rapid reservoir modelling (RRM) provides a unified graphical user interface (GUI) containing all the
necessary tools to create, visualize and test 3D geological models built from
cross-section and contour sketches. At its core sits a new modelling framework,
designed to create models with guarantees of geological consistency.
A description of geological operators and applications from a user's point of
view is available in [Jacquemyn et al
(2021)](https://doi.org/10.1144/jgs2020-187), while a description of flow
diagnostics is available in [Petrovskyy et al.
(2023)](https://doi.org/10.1016/j.ijggc.2023.103855). See also
[https://rapidreservoir.org](https://rapidreservoir.org).

RRM is built to allow interactive operation: modelling and visualization of
individual surfaces are performed in milliseconds on commodity, over the shelf
hardware and computational time scales linearly with the number of surfaces.

New versions of RRM, and these notes can be found at the [git
repository](https://bitbucket.org/rapidreservoirmodelling/rrm).

Video tutorials:
 [How to sketch in 3D?](https://youtu.be/aRD9ENV4BGU)
and [How to use geological operators?](https://youtu.be/Sy_OubDrvX8)

## Downloads

The latest stable release can be found at:

| Platform                | Files                                                                                                                     |
|-------------------------|---------------------------------------------------------------------------------------------------------------------------|
| Windows ZIP (portable)  | [rrm-1.1.0-windows-x86_64.zip](https://bitbucket.org/rapidreservoirmodelling/rrm/downloads/rrm-1.1.0-windows-x86_64.zip)  |

To run RRM, unpack a ZIP archive to your preferred folder and simply run the
executable `RRM.exe` (installation is not required).  All Windows binaries have
been tested in Windows 10.


## Building RRM form source

### Minimal Requirements

RRM uses the [CMake](https://cmake.org/download/) (version 3.20 or higher is
required) build system, and its main Sketching GUI currently depends on the
following external libraries that must be obtained and properly installed
before RRM can be compiled.

- [Qt5] (components: Core, Gui, Widgets, OpenGL, Svg)
- [Eigen3]
- [Glew]

[Qt5]: https://www.qt.io/download
[Eigen3]: http://eigen.tuxfamily.org/index.php?title=Main_Page
[Glew]: http://glew.sourceforge.net/

Instructions on how to obtain these libraries are platform dependent and will
be provided below.  [Git](https://git-scm.com/downloads) is also required. The necessary feature list is C++17.

### Building the Sketching GUI and Flow Diagnostics

This version of RRM includes both a Sketching GUI and a Flow Diagnostics App,
and both were built and tested in the following environments:

- Windows 10, MSVC 14.34
- Ubuntu 22.04, GCC 11.3

The Flow Diagnostics App requires VTK 8.2 in addition to the dependencies
required by the main Sketching GUI.  To compile both RRM's Sketching GUI and
the Flow Diagnostics App, we suggest using the
[vcpkg](https://vcpkg.io/en/index.html) C/C++ package manager to acquire and
build its dependencies.  To simplify the building process, we currently use
a [fork](https://github.com/dp-69/vcpkg) of `vcpkg`'s main repository, which
incorporates changes required for a smooth compilation process. 

#### Windows 10

The following instructions assume that our fork of `vcpkg`'s repository will be
cloned into the ```C:\``` folder, and that all commands are issued from a **x64
Native Tools Command Prompt for VS 2022** command prompt (which is a part of
Microsoft's Visual Studio toolset).

- Step 1 - Installing RRM's dependencies in vcpkg:
```cmd
> git clone https://github.com/dp-69/vcpkg.git
> cd vcpkg
> bootstrap-vcpkg.bat
> vcpkg.exe install rrmlibs-legacy:x64-windows --overlay-ports=ports-rrm/core
```

- Step 2 - RRM compilation:
```cmd
> git clone https://bitbucket.org/rapidreservoirmodelling/rrm.git
> cd rrm
> cmake --preset=win-rel
> cmake --build --preset=win-rel-rrm
```

- Step 3 - Executing RRM:
```cmd
> cd build\Release\bin
> RRM.exe
```


#### Ubuntu 22.04

The following instructions assume that `vcpkg`'s repository will be cloned into
the user's home folder, and that all commands are issued from a terminal.

- Step 1 - Installing RRM's dependencies in vcpkg:
```cmd
> git clone https://github.com/dp-69/vcpkg.git
> cd vcpkg
> ./bootstrap-vcpkg.sh
> ./vcpkg install rrmlibs-legacy --overlay-ports=ports-rrm/core
```

- Step 2 - RRM compilation:
```cmd
> git clone https://bitbucket.org/rapidreservoirmodelling/rrm.git
> cd rrm
> cmake --preset=lin-rel
> cmake --build --preset=lin-rel-rrm
```

- Step 3 - Executing RRM:
```cmd
> cd build/Release/bin
> ./RRM
```


### Building only the Sketching GUI on Windows x64

In what follows we will require that Microsoft's Visual Studio (either version
2019 or 2022) is installed and both `git` and `cmake` are available in the
user's `PATH`.  For the sake of these steps, the versions of `git` and `cmake`
that are bundled with Visual Studio are sufficient.

#### Installing Qt, Eigen3 and GLEW

We again suggest the use of
[vcpkg](https://docs.microsoft.com/en-us/cpp/build/vcpkg?view=msvc-160) (see
also [this link](https://vcpkg.readthedocs.io/en/latest/) for more details on
using this tool) package manager to install Qt, Eigen3 and GLEW -- but now
a vanilla version of `vcpkg` obtained directly from its repository is
sufficient (but notice that those libraries can be installed by other means as
well).  We'll assume that `vcpkg` is installed to `C:\dev\vcpkg`, as in the
example:

```
> git clone https://github.com/microsoft/vcpkg C:\dev\vcpkg
```

And from the `C:\dev\vcpkg` directory issue:

```
> bootstrap-vcpkg.bat
> vcpkg.exe install qt5:x64-windows eigen3:x64-windows glew:x64-windows
```

#### Running CMake

We will describe how to run CMake from Visual Studio's "x64 Native Tools
Command Prompt". It is also possible to configure the project directly from
Visual Studio -- more information can be found from Visual Studio's and `vcpkg`
help documents. Assuming that `vcpkg` was installed in `C:\dev\vcpkg` as in the
previous step, we create a `build` folder inside RRM's source tree (here
denoted as `rrm_root`) and issue the command:

```
> rrm_root\build\cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\dev\vcpkg\scripts\buildsystems\vcpkg.cmake
```

This command creates and configures a Visual Studio project in the `build`
directory.  The `-DCMAKE_TOOLCHAIN_FILE` option instructs CMake to use the
versions of Qt5, Eigen3 and GLEW that were installed with `vcpkg`.

To compile RRM in Release mode, use:

```
> rrm_root\build\cmake --build . --config Release
```

The binary, object files and resources will be build in folder
`rrm_root\build\bin\Release`.  Alternatively, one could also open the
`rrm_root\build\RRM.sln` solution file, created in the first CMake run,
directly from Visual Studio and continue from there.


## Copyright

The [RRM project's](https://rapidreservoir.org) research results are a copyright of the respective current and past members:

### University of Calgary, Canada
Clarissa Coda Marques<br/>
Julio Daniel Silva<br/>
Felipe Moura de Carvalho<br/>
Emilio Vital Brazil<br/>
[Sicilia Judice](http://www.linkedin.com/in/sicilia-judice)<br/>
Fazilatur Rahman<br/>
Mario Costa Sousa  

### Delft University of Technology, Netherlands<sup>[1](#footnote-delft)</sup>
[Dmytro Petrovskyy](https://www.linkedin.com/in/dmytro-petrovskyy/)<br/>
Zhao Zang<br/>
[Sebastian Geiger](https://www.tudelft.nl/en/staff/s.geiger/)

### Imperial College London, UK
Carl Jacquemyn<br/>
Margaret E.H. Pataki<br/>
[Gary J. Hampson](https://www.linkedin.com/in/gary-hampson-icl/)<br/>
Matthew D. Jackson  

This software also includes [third party
libraries](https://bitbucket.org/rapidreservoirmodelling/rrm/src/main/rrm_3rd_party_libraries.md)
developed outside of the RRM project. 
An overview of these libraries including brief descriptions and the last known
URL their code was made available at can be found
[here](https://bitbucket.org/rapidreservoirmodelling/rrm/src/main/rrm_3rd_party_libraries.md).
Please, check the source code for details.

<sup><a name="footnote-delft">1</a></sup> Parts of the code were developed while S. Geiger and D. Petrovskyy were at Heriot-Watt University, with support from Z. Zhang.

## License

RRM is Free Software released under the 
[GPLv3](https://www.gnu.org/licenses/gpl.html) license.  It includes 
[third party libraries](https://bitbucket.org/rapidreservoirmodelling/rrm/src/main/rrm_3rd_party_libraries.md)
with other licenses, which we understand to be compatible. An overview of these libraries including
brief descriptions, the last known URL the code was made available at, the date
it was retrieved, the license(s) in which the code was made available by their
authors at the retrieving date, and URL links to publicly available copies of
such licenses is available
[here](https://bitbucket.org/rapidreservoirmodelling/rrm/src/main/rrm_3rd_party_libraries.md).
Please, check the source code for details on the licenses that apply to each
piece of software.


## Sponsors

This software was developed during the first phase of the [RRM
project](https://rapidreservoir.org) (from 2014 to 2018), which was sponsored
by:

- Equinor
- ExxonMobil
- IBM Research Brazil/IBM Centre for Advanced Studies (CAS) Alberta
- Petrobras
- Shell 

The current phase of the project (from 2019 to 2023) is being sponsored by:

- Equinor
- ExxonMobil
- Petrobras
- Petronas 
- Shell 

We thank our present and past sponsors for helping to make this research possible.
