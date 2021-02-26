RRM 3^rd^ party libraries
=========================

February 25, 2021


## Introduction 

This document lists third party libraries that are used in the RRM project. It
includes brief descriptions, the last known URL the code was made available at,
the date it was retrieved, the license(s) in which the code was made available
by their authors at the retrieving date, and URL links to publicly available
copies of such licenses when possible.


## Libraries currently in use

-  [Qt](https://www.qt.io/)
    + Retrieved: July 17, 2020
    + License: [LGPL v3.0], [GPL v3.0], [GPL v2.0]
    + Description: Qt is a cross-platform application framework and widget
      toolkit for creating classic and embedded graphical user interfaces, and
      applications that run on various software and hardware platforms with
      little or no change in the underlying codebase, while still being a
      native application with native capabilities and speed.

-  [curvendimension](https://github.com/esperanc/curvendimension)
    + Retrieved: July 17, 2020
    + License: [LGPL v2.1]
    + Description: This is a C++ library for handling n-dimensional curves, or,
      actually, n-dimensional polygonal lines.

-  [Tucano](https://gitlab.com/LCG-UFRJ/tucano)
    + Retrieved: July 17, 2020
    + License: [GPL v3.0]
    + Description: Tucano Lib - A library for rapid prototyping with modern
      OpenGL and GLSL.

-  [Glew](http://glew.sourceforge.net/)
    + Retrieved: July 17, 2020
    + Licenses: [Glew Modified BSD], [MESA 3-D], [KHRONOS]
    + Description: The OpenGL Extension Wrangler Library (GLEW) is a
      cross-platform open-source C/C++ extension loading library. GLEW provides
      efficient run-time mechanisms for determining which OpenGL extensions are
      supported on the target platform. OpenGL core and extension functionality
      is exposed in a single header file. GLEW has been tested on a variety of
      operating systems, including Windows, Linux, Mac OS X, FreeBSD, Irix, and
      Solaris.

-  [Eigen3](http://eigen.tuxfamily.org/index.php?title=Main_Page)
    + Retrieved: July 17, 2020
    + License: [MPL 2.0], but also uses code released as [BSD-3-Clause], [LGPL v2.1], [MINPACK]
    + Description: Eigen is a C++ template library for linear algebra:
      matrices, vectors, numerical solvers, and related algorithms.

-  [TinyObjLoader](https://github.com/syoyo/tinyobjloader)
    + Retrieved: July 17, 2020
    + License: [MIT]
    + Description: Tiny but powerful single file wavefront obj loader written
      in C++. No dependency except for C++ STL. It can parse over 10M polygons
      with moderate memory and time.

-  [cereal](https://github.com/USCiLab/cereal) 
    + Retrieved: July 17, 2020
    + License: [BSD-3-Clause]
    + Description: cereal is a header-only C++11 serialization library. cereal
      takes arbitrary data types and reversibly turns them into different
      representations, such as compact binary encodings, XML, or JSON. cereal
      was designed to be fast, light-weight, and easy to extend - it has no
      external dependencies and can be easily bundled with other code or used
      standalone.

-  [Catch2](https://github.com/catchorg/Catch2)
    + Retrieved: July 17, 2020
    + License: [Boost Software License - Version 1.0]
    + Description: Catch stands for C++ Automated Test Cases in a Header and is
      a multi-paradigm test framework for C++, which also supports Objective-C
      (and maybe C). It is primarily distributed as a single header file,
      although certain extensions may require additional headers.

-  [glog](https://github.com/google/glog)
    + Retrieved: July 17, 2020
    + License: [BSD-3-Clause]
    + Description: Google glog is a library that implements application-level
      logging.  This library provides logging APIs based on C++-style streams
      and various helper macros.  You can log a message by simply streaming
      things to LOG to a particular severity level.

-  [argh](https://github.com/adishavit/argh)
    + Retrieved: July 17, 2020
    + License: [BSD-3-Clause]
    + Description: Frustration-free command line processing.

-  [ColorBrewer for C++](https://github.com/schulzch/colorbrewercpp)
    + Retrieved: July 17, 2020
    + License: [Apache 2.0]
    + Description: This header-only library implements the ColorBrewer palette
      using C++11.  About the ColorBrewer palette: web tool for guidance in
      choosing choropleth map color schemes, based on the research of Dr.
      Cynthia Brewer. Built and maintained by Axis Maps.

-  [LibQtx](https://bitbucket.org/libqxt/libqxt/overview)
    + Retrieved: July 17, 2020
    + License: [BSD-3-Clause], [LGPL v2.1]
    + Description: LibQxt is an extension library for Qt providing a suite of
      cross-platform utility classes to add functionality not readily available
      in the Qt toolkit by Qt Development Frameworks, Nokia. As of August 15,
      2018 it is stated in their website that LibQtx is no longer maintained.  

-  [predicates.c](https://www.cs.cmu.edu/~quake/robust.html)
    + Retrieved: July 17, 2020
    + License: Public domain
    + Description: C implementation of algorithms for exact addition and
      multiplication of floating-point numbers, and predicates for robustly
      performing the orientation and incircle tests used in computational
      geometry.  The algorithms and underlying theory are described in Jonathan
      Richard Shewchuk. "Adaptive Precision Floating-Point Arithmetic and Fast
      Robust Geometric Predicates." Technical Report CMU-CS-96-140, School of
      Computer Science, Carnegie Mellon University, Pittsburgh, Pennsylvania,
      May 1996.  (Submitted to Discrete & Computational Geometry.)

-  [JSON for Modern C++](https://github.com/nlohmann/json)
    + Retrieved: July 17, 2020
    + License: [MIT]
    + Description: There are myriads of JSON libraries out there, and each may
      even have its reason to exist. Our class had these design goals:
      Intuitive syntax. In languages such as Python, JSON feels like a first
      class data type. We used all the operator magic of modern C++ to achieve
      the same feeling in your code. Check out the examples below and you'll
      know what I mean. Trivial integration. Our whole code consists of
      a single header file json.hpp. That's it. No library, no subproject, no
      dependencies, no complex build system. The class is written in vanilla
      C++11. All in all, everything should require no adjustment of your
      compiler flags or project settings. Serious testing. Our class is heavily
      unit-tested and covers 100% of the code, including all exceptional
      behavior. Furthermore, we checked with Valgrind and the Clang Sanitizers
      that there are no memory leaks. Google OSS-Fuzz additionally runs fuzz
      tests against all parsers 24/7, effectively executing billions of tests
      so far. To maintain high quality, the project is following the Core
      Infrastructure Initiative (CII) best practices.

-  [Boost](https://www.boost.org)
    + Retrieved: July 17, 2020
    + License: [Boost Software License - Version 1.0]
    + Description: Boost provides free peer-reviewed portable C++ source
      libraries. We emphasize libraries that work well with the C++ Standard
      Library. Boost libraries are intended to be widely useful, and usable
      across a broad spectrum of applications. The Boost license encourages the
      use of Boost libraries for all users with minimal restrictions. We aim to
      establish "existing practice" and provide reference implementations so
      that Boost libraries are suitable for eventual standardization. Beginning
      with the ten Boost Libraries included in the Library Technical Report
      (TR1) and continuing with every release of the ISO standard for C++ since
      2011, the C++ Standards Committee has continued to rely on Boost as
      a valuable source for additions to the Standard C++ Library.


[LGPL v2.1]: https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html
[LGPL v3.0]: https://www.gnu.org/licenses/lgpl-3.0.en.html 
[GPL v2.0]: https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
[GPL v3.0]: https://www.gnu.org/licenses/gpl.html
[Affero GPL v3.0]: https://www.gnu.org/licenses/agpl-3.0.en.html
[Apache 2.0]: https://www.apache.org/licenses/LICENSE-2.0
[Boost Software License - Version 1.0]: https://www.boost.org/LICENSE_1_0.txt
[BSD-3-Clause]: https://opensource.org/licenses/BSD-3-Clause
[MIT]: https://opensource.org/licenses/MIT
[MPL 2.0]: https://www.mozilla.org/en-US/MPL/2.0/
[Glew Modified BSD]: http://glew.sourceforge.net/glew.txt
[MESA 3-D]: http://glew.sourceforge.net/mesa.txt
[MINPACK]: https://www.netlib.org/minpack/disclaimer
[KHRONOS]: http://glew.sourceforge.net/khronos.txt
[Shewchuck's license]: https://www.cs.cmu.edu/~quake/triangle.html
[Zlib-Libpng]: https://www.glfw.org/license.html

