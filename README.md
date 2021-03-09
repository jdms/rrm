RRM
===

Sketch-based geological 3D modelling: Exploring uncertainty with interactive prototyping
-----------------------------------------------------------------------

RRM provides a unified graphical user interface (GUI) containing all the
necessary tools to create, visualize and test 3D geological models built from
cross-section and contour sketches. At its core sits a new modelling framework,
designed to create models with guarantees of geological consistency. Detailed
description of geological operators and applications are available in [Jacquemyn et al (2021)](https://doi.org/10.1144/jgs2020-187) 

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

| Platform                | Files                                                                                                                          |
|-------------------------|--------------------------------------------------------------------------------------------------------------------------------|
| Windows ZIP (portable)  | [rrm-1.0.0-rc-windows-x86_64.zip](https://bitbucket.org/rapidreservoirmodelling/rrm/downloads/rrm-1.0.0-rc-windows-x86_64.zip) |

To run RRM, unpack a ZIP archive to your preferred folder and simply run the
executable `RRM.exe` (installation is not required).  All Windows binaries have
been tested in Windows 10.


## Building RRM form source

### Requirements

RRM uses the [CMake](https://cmake.org/download/) (version 3.10 or higher is
required) build system, and it currently depends on the following external
libraries that must be obtained and properly installed before RRM can be
compiled.

- [Qt5](https://www.qt.io/download)

- [Eigen3](http://eigen.tuxfamily.org/index.php?title=Main_Page)

- [Glew](http://glew.sourceforge.net/)

Instructions on how to obtain these libraries are platform dependent and will
be provided below.  [Git](https://git-scm.com/downloads) is also required.


### Building on Windows x64

RRM supports 64bit builds with the [Microsoft Visual Studio
2019](https://visualstudio.microsoft.com/vs/) compiler in Windows.  In what
follows we will require that both `git` and `cmake` be available in the user's
`PATH`.  For the sake of these steps, the versions of `git` and `cmake` that
are bundled with Visual Studio 2019 are sufficient.

#### Installing Qt, Eigen3 and GLEW

In Windows, RRM requires the use of the
[vcpkg](https://docs.microsoft.com/en-us/cpp/build/vcpkg?view=msvc-160) (see
also [this link](https://vcpkg.readthedocs.io/en/latest/) for more details on
using this tool) package manager to install Qt, Eigen3 and GLEW.  We'll assume
that `vcpkg` is installed to `C:\dev\vcpkg`, as in the example:

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
Clarissa Coda Marques  
Julio Daniel Silva  
Felipe Moura de Carvalho  
Emilio Vital Brazil  
Sicilia Judice  
Fazilatur Rahman  
Mario Costa Sousa  

### Heriot-Watt University, UK
Dmytro Petrovskyy  
Zhao Zang  
Sebastian Geiger  

### Imperial College London, UK
Carl Jacquemyn  
Margaret E.H. Pataki  
Gary J. Hampson  
Matthew D. Jackson  

This software also includes [third party
libraries](https://bitbucket.org/rapidreservoirmodelling/rrm/src/main/rrm_3rd_party_libraries.md)
developed outside of the RRM project. 
An overview of these libraries including brief descriptions and the last known
URL their code was made available at can be found
[here](https://bitbucket.org/rapidreservoirmodelling/rrm/src/main/rrm_3rd_party_libraries.md).
Please, check the source code for details.


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
project](https://rapidreservoir.org), which was sponsored by:

- Equinor
- ExxonMobil
- IBM Research Brazil/IBM Centre for Advanced Studies (CAS) Alberta
- Petrobras
- Shell 

The current phase of the project is being sponsored by:

- Equinor
- ExxonMobil
- Petrobras
- Petronas 
- Shell 

We thank our present and past sponsors for helping to make this research possible.
