/* =================================================================================================

(c - MIT) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | github.com/tdegeus/cppmat

================================================================================================= */

#ifndef CPPMAT_MATRIX_PYBIND11_H
#define CPPMAT_MATRIX_PYBIND11_H

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "periodic_matrix.h"
#include "macros.h"

namespace py = pybind11;

namespace pybind11 {
namespace detail {

// =================================================================================================
// type caster: cppmat::periodic::matrix <-> NumPy-array
// =================================================================================================

template <typename T> struct type_caster<cppmat::periodic::matrix<T>>
{
public:

  PYBIND11_TYPE_CASTER(cppmat::periodic::matrix<T>, _("cppmat::periodic::matrix<T>"));

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

    // - rank of the input array (number of indices)
    auto rank = buf.ndim();
    // - check
    if ( rank < 1 ) return false;

    // - shape of the input array
    std::vector<size_t> shape(rank);
    // - copy
    for ( ssize_t i = 0 ; i < rank ; i++ ) shape[i] = buf.shape()[i];

    // - all checks passed : create the proper C++ variable
    value = cppmat::periodic::matrix<T>(shape, buf.data());

    // - signal successful variable creation
    return true;
  }

  // C++ -> Python
  // -------------

  static py::handle cast(
    const cppmat::periodic::matrix<T>& src, py::return_value_policy policy, py::handle parent
  )
  {
    // - create Python variable (all variables are copied)
    py::array a(std::move(src.shape()), std::move(src.strides(true)), src.data());

    // - release variable to Python
    return a.release();
  }
};

}} // namespace pybind11::detail

#endif
