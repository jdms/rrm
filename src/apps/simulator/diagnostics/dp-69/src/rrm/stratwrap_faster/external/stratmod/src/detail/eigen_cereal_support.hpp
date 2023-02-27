/* Copyright 2020 Julio D. Machado Silva */

/* Permission is hereby granted, free of charge, to any person obtaining a copy of */
/* this software and associated documentation files (the "Software"), to deal in */
/* the Software without restriction, including without limitation the rights to */
/* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies */
/* of the Software, and to permit persons to whom the Software is furnished to do */
/* so, subject to the following conditions: */

/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software. */

/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE */
/* SOFTWARE. */


#ifndef UTILITY_EIGEN_SERIALIZATION_FUNCTIONS_FOR_CEREAL_SUPPORT
#define UTILITY_EIGEN_SERIALIZATION_FUNCTIONS_FOR_CEREAL_SUPPORT

//
// Inspired from: 
// https://gist.github.com/mtao/5798888
//
// Improvements from:
// https://stackoverflow.com/a/51944389
//

#include <cstddef>

#include <cereal/access.hpp>
#include <Eigen/Dense>

namespace cereal
{
    template <class Archive, class Derived>
    inline void save(Archive& ar, const Eigen::PlainObjectBase<Derived>& m)
    {
        using ArrayType = Eigen::PlainObjectBase<Derived>;

        if(ArrayType::RowsAtCompileTime == Eigen::Dynamic) {
            ar(m.rows());
        }
        if(ArrayType::ColsAtCompileTime == Eigen::Dynamic) {
            ar(m.cols());
        }

        using EigenIndexType = typename Eigen::EigenBase<Derived>::Index;
        const typename Derived::Scalar* data = m.data();

        for (EigenIndexType i = 0; i < m.size(); ++i)
        {
            ar(data[i]);
        }
    }

    template <class Archive, class Derived> 
    inline void load(Archive& ar, Eigen::PlainObjectBase<Derived>& m)
    {
        using ArrayType = Eigen::PlainObjectBase<Derived>;

        Eigen::Index rows = ArrayType::RowsAtCompileTime; 
        Eigen::Index cols = ArrayType::ColsAtCompileTime;

        if(rows == Eigen::Dynamic) {
            ar(rows);
        }
        if(cols == Eigen::Dynamic) {
            ar(cols);
        }
        m.resize(rows,cols);

        using EigenIndexType = typename Eigen::EigenBase<Derived>::Index;
        typename Derived::Scalar value;
        typename Derived::Scalar* data = m.data();

        for (EigenIndexType i = 0; i < m.size(); ++i)
        {
            ar(value);
            data[i] = value;
        }
    }
}

#endif
