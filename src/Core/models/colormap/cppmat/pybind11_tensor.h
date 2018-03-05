/* =================================================================================================

(c - MIT) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | github.com/tdegeus/cppmat

================================================================================================= */

#ifndef CPPMAT_TENSOR_PYBIND11_H
#define CPPMAT_TENSOR_PYBIND11_H

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "tensor.h"
#include "macros.h"

namespace py = pybind11;

namespace pybind11 {
namespace detail {

// =================================================================================================
// type caster: cppmat::cartesian::tensor4 <-> NumPy-array
// =================================================================================================

template <typename T> struct type_caster<cppmat::cartesian::tensor4<T>>
{
public:

  PYBIND11_TYPE_CASTER(cppmat::cartesian::tensor4<T>, _("cppmat::cartesian::tensor4<T>"));

  // Python -> C++
  // -------------

  bool load(py::handle src, bool convert)
  {
    // - basic pybind11 check
    if ( !convert && !py::array_t<T>::check_(src) ) return false;

    // - storage requirements : contiguous and row-major storage from NumPy
    auto buf = py::array_t<T, py::array::c_style | py::array::forcecast>::ensure(src);
    // - check
    if ( !buf ) return false;

    // - rank of the input array (number of indices) : should be exactly 4
    auto rank = buf.ndim();
    // - check
    if ( rank != 4 ) return false;

    // - read number of dimensions (shape in each direction)
    ssize_t nd = buf.shape()[0];

    // - the shape in each direction should be equal ( == nd )
    for ( ssize_t i = 0 ; i < rank ; ++i )
      if ( buf.shape()[i] != nd )
        return false;

    // - all checks passed : create the proper C++ variable
    value = cppmat::cartesian::tensor4<T>(nd, buf.data());

    // - signal successful variable creation
    return true;
  }

  // C++ -> Python
  // -------------

  static py::handle cast(
    const cppmat::cartesian::tensor4<T>& src, py::return_value_policy policy, py::handle parent
  )
  {
    // - create Python variable (all variables are copied)
    py::array a(std::move(src.shape()), std::move(src.strides(true)), src.data());

    // - release variable to Python
    return a.release();
  }
};

// =================================================================================================
// type caster: cppmat::cartesian::tensor2 <-> NumPy-array
// =================================================================================================

template <typename T> struct type_caster<cppmat::cartesian::tensor2<T>>
{
public:

  PYBIND11_TYPE_CASTER(cppmat::cartesian::tensor2<T>, _("cppmat::cartesian::tensor2<T>"));

  // Python -> C++
  // -------------

  bool load(py::handle src, bool convert)
  {
    // - basic pybind11 check
    if ( !convert && !py::array_t<T>::check_(src) ) return false;

    // - storage requirements : contiguous and row-major storage from NumPy
    auto buf = py::array_t<T, py::array::c_style | py::array::forcecast>::ensure(src);
    // - check
    if ( !buf ) return false;

    // - rank of the input array (number of indices) : should be exactly 2
    auto rank = buf.ndim();
    // - check
    if ( rank != 2 ) return false;

    // - read number of dimensions (shape in each direction)
    ssize_t nd = buf.shape()[0];

    // - the shape in each direction should be equal ( == nd )
    for ( ssize_t i = 0 ; i < rank ; ++i )
      if ( buf.shape()[i] != nd )
        return false;

    // - all checks passed : create the proper C++ variable
    value = cppmat::cartesian::tensor2<T>(nd, buf.data());

    // - signal successful variable creation
    return true;
  }

  // C++ -> Python
  // -------------

  static py::handle cast(
    const cppmat::cartesian::tensor2<T>& src, py::return_value_policy policy, py::handle parent
  )
  {
    // - create Python variable (all variables are copied)
    py::array a(std::move(src.shape()), std::move(src.strides(true)), src.data());

    // - release variable to Python
    return a.release();
  }
};

// =================================================================================================
// type caster: cppmat::cartesian::tensor2s <-> NumPy-array
// =================================================================================================

template <typename T> struct type_caster<cppmat::cartesian::tensor2s<T>>
{
public:

  PYBIND11_TYPE_CASTER(cppmat::cartesian::tensor2s<T>, _("cppmat::cartesian::tensor2s<T>"));

  // Python -> C++
  // -------------

  bool load(py::handle src, bool convert)
  {
    // - basic pybind11 check
    if ( !convert && !py::array_t<T>::check_(src) ) return false;

    // - storage requirements : contiguous and row-major storage from NumPy
    auto buf = py::array_t<T, py::array::c_style | py::array::forcecast>::ensure(src);
    // - check
    if ( !buf ) return false;

    // - rank of the input array (number of indices) : should be exactly 2
    auto rank = buf.ndim();
    // - check
    if ( rank != 2 ) return false;

    // - read number of dimensions (shape in each direction)
    ssize_t nd = buf.shape()[0];

    // - the shape in each direction should be equal ( == nd )
    for ( ssize_t i = 0 ; i < rank ; ++i )
      if ( buf.shape()[i] != nd )
        return false;

    // - all checks passed : create the proper C++ variable
    value = cppmat::cartesian::tensor2s<T>(nd, buf.data(), true);

    // - signal successful variable creation
    return true;
  }

  // C++ -> Python
  // -------------

  static py::handle cast(
    const cppmat::cartesian::tensor2s<T>& src, py::return_value_policy policy, py::handle parent
  )
  {
    // - convert to dense tensor
    cppmat::cartesian::tensor2<T> tmp = src;

    // - create Python variable (all variables are copied)
    py::array a(std::move(tmp.shape()), std::move(tmp.strides(true)), tmp.data());

    // - release variable to Python
    return a.release();
  }
};

// =================================================================================================
// type caster: cppmat::cartesian::tensor2d <-> NumPy-array
// =================================================================================================

template <typename T> struct type_caster<cppmat::cartesian::tensor2d<T>>
{
public:

  PYBIND11_TYPE_CASTER(cppmat::cartesian::tensor2d<T>, _("cppmat::cartesian::tensor2d<T>"));

  // Python -> C++
  // -------------

  bool load(py::handle src, bool convert)
  {
    // - basic pybind11 check
    if ( !convert && !py::array_t<T>::check_(src) ) return false;

    // - storage requirements : contiguous and row-major storage from NumPy
    auto buf = py::array_t<T, py::array::c_style | py::array::forcecast>::ensure(src);
    // - check
    if ( !buf ) return false;

    // - rank of the input array (number of indices) : should be exactly 2
    auto rank = buf.ndim();
    // - check
    if ( rank != 2 ) return false;

    // - read number of dimensions (shape in each direction)
    ssize_t nd = buf.shape()[0];

    // - the shape in each direction should be equal ( == nd )
    for ( ssize_t i = 0 ; i < rank ; ++i )
      if ( buf.shape()[i] != nd )
        return false;

    // - all checks passed : create the proper C++ variable
    value = cppmat::cartesian::tensor2d<T>(nd, buf.data(), true);

    // - signal successful variable creation
    return true;
  }

  // C++ -> Python
  // -------------

  static py::handle cast(
    const cppmat::cartesian::tensor2d<T>& src, py::return_value_policy policy, py::handle parent
  )
  {
    // - convert to dense tensor
    cppmat::cartesian::tensor2<T> tmp = src;

    // - create Python variable (all variables are copied)
    py::array a(std::move(tmp.shape()), std::move(tmp.strides(true)), tmp.data());

    // - release variable to Python
    return a.release();
  }
};

// =================================================================================================
// type caster: cppmat::cartesian::vector <-> NumPy-array
// =================================================================================================

template <typename T> struct type_caster<cppmat::cartesian::vector<T>>
{
public:

  PYBIND11_TYPE_CASTER(cppmat::cartesian::vector<T>, _("cppmat::cartesian::vector<T>"));

  // Python -> C++
  // -------------

  bool load(py::handle src, bool convert)
  {
    // - basic pybind11 check
    if ( !convert && !py::array_t<T>::check_(src) ) return false;

    // - storage requirements : contiguous and row-major storage from NumPy
    auto buf = py::array_t<T, py::array::c_style | py::array::forcecast>::ensure(src);
    // - check
    if ( !buf ) return false;

    // - rank of the input array (number of indices) : should be exactly 1
    auto rank = buf.ndim();
    // - check
    if ( rank != 1 ) return false;

    // - read number of dimensions (shape in each direction)
    ssize_t nd = buf.shape()[0];

    // - the shape in each direction should be equal ( == nd )
    for ( ssize_t i = 0 ; i < rank ; ++i )
      if ( buf.shape()[i] != nd )
        return false;

    // - all checks passed : create the proper C++ variable
    value = cppmat::cartesian::vector<T>(nd, buf.data());

    // - signal successful variable creation
    return true;
  }

  // C++ -> Python
  // -------------

  static py::handle cast(
    const cppmat::cartesian::vector<T>& src, py::return_value_policy policy, py::handle parent
  )
  {
    // - create Python variable (all variables are copied)
    py::array a(std::move(src.shape()), std::move(src.strides(true)), src.data());

    // - release variable to Python
    return a.release();
  }
};

// =================================================================================================

}} // namespace pybind11::detail

#endif
