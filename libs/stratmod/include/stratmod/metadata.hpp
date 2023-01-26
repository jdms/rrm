/* This file is part of the stratmod library */
/* Copyright (C) 2017-2020, Julio Daniel Machado Silva. */

/* The stratmod library is free software: you can redistribute it and/or modify it */
/* under the terms of the GNU General Public License as published by the Free */
/* Software Foundation, either version 3 of the License, or (at your option) any */
/* later version. */

/* The stratmod library is distributed in the hope that it will be useful, but */
/* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or */
/* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more */
/* details. */

/* You should have received a copy of the GNU General Public License */
/* along with the stratmod library.  If not, see <https://www.gnu.org/licenses/>. */

#ifndef STRATMOD_METADATA_PRIMITIVES
#define STRATMOD_METADATA_PRIMITIVES

#include <map>
#include <memory>
#include <string>
#include <optional>

#include "Eigen/Dense"

namespace stratmod {

struct ScalarProperty {
    double value{};
    std::string name{};
    std::string unit{};

    bool operator==(const ScalarProperty& rhs) const
    {
        bool equal = true;
        equal &= (value == rhs.value);
        equal &= (name == rhs.name);
        equal &= (unit == rhs.unit);

        return equal;
    }
};


struct Vector3dProperty {
    Eigen::Vector3d value{};
    std::string name{};
    std::string unit{};

    bool operator==(const Vector3dProperty& rhs) const
    {
        bool equal = true;
        equal &= (value == rhs.value);
        equal &= (name == rhs.name);
        equal &= (unit == rhs.unit);

        return equal;
    }
};


struct Matrix3dProperty {
    Eigen::Matrix3d value{};
    std::string name{};
    std::string unit{};

    bool operator==(const Matrix3dProperty& rhs) const
    {
        bool equal = true;
        equal &= (value == rhs.value);
        equal &= (name == rhs.name);
        equal &= (unit == rhs.unit);

        return equal;
    }
};

struct MatrixXdProperty {
    Eigen::MatrixXd value{};
    std::string name{};
    std::string unit{};

    bool operator==(const MatrixXdProperty& rhs) const
    {
        bool equal = true;
        equal &= (value == rhs.value);
        equal &= (name == rhs.name);
        equal &= (unit == rhs.unit);

        return equal;
    }
};


struct CommonMetadata
{
    virtual ~CommonMetadata() = default;

    std::string name{};
    std::string description{};
    std::optional<Eigen::Vector3i> color_rgb{};

    virtual bool empty() const
    {
        return name.empty() && description.empty() && !color_rgb.has_value();
    }

    protected:
        bool isEqual(const CommonMetadata& rhs) const
        {
            bool equal = true;
            equal &= (name == rhs.name);
            equal &= (description == rhs.description);
            equal &= (color_rgb == rhs.color_rgb);

            return equal;
        }
};


struct CurveMetadata : public CommonMetadata
{
    std::map<std::string, ScalarProperty> scalar_data{};
    /* std::map<std::string, Vector3dProperty> vector_data{}; */
    std::map<std::string, MatrixXdProperty> matrix_data{};

    virtual bool empty() const override
    {
        return CommonMetadata::empty() && scalar_data.empty() /* && vector_data.empty() */ && matrix_data.empty();
    }

    bool operator==(const CurveMetadata& rhs)
    {
        bool equal = CommonMetadata::isEqual(rhs);
        if (!equal)
        {
            return false;
        }

        equal &= (scalar_data == rhs.scalar_data);
        /* equal &= (vector_data == rhs.vector_data); */
        equal &= (matrix_data == rhs.matrix_data);

        return equal;
    }

    bool operator!=(const CurveMetadata& rhs)
    {
        return !operator==(rhs);
    }

    std::unique_ptr<CurveMetadata> clone() const
    {
        std::unique_ptr<CurveMetadata> bptr(static_cast<CurveMetadata*>(this->doClone()));
        return bptr;
    }

    private:
        virtual CurveMetadata* doClone() const
        {
            return new CurveMetadata(*this);
        }
};


struct SurfaceMetadata : public CommonMetadata
{
    std::map<std::string, ScalarProperty> scalar_data{};
    /* std::map<std::string, Vector3dProperty> vector_data{}; */
    std::map<std::string, MatrixXdProperty> matrix_data{};

    virtual bool empty() const override
    {
        return CommonMetadata::empty() && scalar_data.empty() /* && vector_data.empty() */ && matrix_data.empty();
    }

    bool operator==(const SurfaceMetadata& rhs)
    {
        bool equal = CommonMetadata::isEqual(rhs);
        if (!equal)
        {
            return false;
        }

        equal &= (scalar_data == rhs.scalar_data);
        /* equal &= (vector_data == rhs.vector_data); */
        equal &= (matrix_data == rhs.matrix_data);

        return equal;
    }

    bool operator!=(const SurfaceMetadata& rhs)
    {
        return !operator==(rhs);
    }

    std::unique_ptr<SurfaceMetadata> clone() const
    {
        std::unique_ptr<SurfaceMetadata> bptr(static_cast<SurfaceMetadata*>(this->doClone()));
        return bptr;
    }

    private:
        virtual SurfaceMetadata* doClone() const
        {
            return new SurfaceMetadata(*this);
        }
};


struct VolumeMetadata : public CommonMetadata
{
    std::map<std::string, ScalarProperty> scalar_data{};
    /* std::map<std::string, Vector3dProperty> vector_data{}; */
    std::map<std::string, MatrixXdProperty> matrix_data{};

    virtual bool empty() const override
    {
        return CommonMetadata::empty() && scalar_data.empty() /* && vector_data.empty() */ && matrix_data.empty();
    }

    bool operator==(const VolumeMetadata& rhs)
    {
        bool equal = CommonMetadata::isEqual(rhs);
        if (!equal)
        {
            return false;
        }

        equal &= (scalar_data == rhs.scalar_data);
        /* equal &= (vector_data == rhs.vector_data); */
        equal &= (matrix_data == rhs.matrix_data);

        return equal;
    }

    bool operator!=(const VolumeMetadata& rhs)
    {
        return !operator==(rhs);
    }

    std::unique_ptr<VolumeMetadata> clone() const
    {
        std::unique_ptr<VolumeMetadata> bptr(static_cast<VolumeMetadata*>(this->doClone()));
        return bptr;
    }

    private:
        virtual VolumeMetadata* doClone() const
        {
            return new VolumeMetadata(*this);
        }
};

} // namespace stratmod

#endif
