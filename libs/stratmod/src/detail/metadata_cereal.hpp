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

#ifndef STRATMOD_METADATA_CEREAL_SERIALIZATION
#define STRATMOD_METADATA_CEREAL_SERIALIZATION

#include "detail/serialization_definitions.cpp"

#include <map>
#include <memory>
#include <string>
#include <optional>

#include "stratmod/metadata.hpp"

#include "Eigen/Dense"

namespace stratmod {

template<typename Archive>
void serialize(Archive& ar, ScalarProperty& p, const std::uint32_t /* version */)
{
    ar(p.value, p.name, p.unit);
}

template<typename Archive>
void serialize(Archive& ar, Vector3dProperty& p, const std::uint32_t /* version */)
{
    ar(p.value, p.name, p.unit);
}

template<typename Archive>
void serialize(Archive& ar, Matrix3dProperty& p, const std::uint32_t /* version */)
{
    ar(p.value, p.name, p.unit);
}

template<typename Archive>
void serialize(Archive& ar, MatrixXdProperty& p, const std::uint32_t /* version */)
{
    ar(p.value, p.name, p.unit);
}

/* template<typename Archive> */
/* void save(Archive & ar, const CommonMetadata& m, const std::uint32_t /1* version *1/) */
/* { */
/*     ar(m.name); */
/*     ar(m.description); */
/*     ar(m.color_rgb); */
/* } */

template<typename Archive>
void serialize(Archive & ar, CommonMetadata& m, const std::uint32_t /* version */)
{
    ar(m.name);
    ar(m.description);
    ar(m.color_rgb);
}

template<typename Archive>
void serialize(Archive& ar, CurveMetadata& m, const std::uint32_t /* version */)
{
    ar(cereal::base_class<CommonMetadata>(&m));
    ar(m.scalar_data);
    /* ar(m.vector_data); */
    ar(m.matrix_data);
}

/* template<typename Archive> */
/* void save(Archive& ar, const SurfaceMetadata& m, const std::uint32_t /1* version *1/) */
/* { */
/*     /1* ar(cereal::base_class<CommonMetadata>(this)); *1/ */
/*     ar(m.scalar_data); */
/*     ar(m.vector_data); */
/* } */

template<typename Archive>
void serialize(Archive& ar, SurfaceMetadata& m, const std::uint32_t /* version */)
{
    ar(cereal::base_class<CommonMetadata>(&m));
    ar(m.scalar_data);
    /* ar(m.vector_data); */
    ar(m.matrix_data);
}

/* template<typename Archive> */
/* void save(Archive& ar, const VolumeMetadata& m, const std::uint32_t /1* version *1/) */
/* { */
/*     /1* ar(cereal::base_class<CommonMetadata>(this)); *1/ */
/*     ar(m.scalar_data); */
/*     ar(m.vector_data); */
/*     ar(m.matrix_data); */
/* } */

template<typename Archive>
void serialize(Archive& ar, VolumeMetadata& m, const std::uint32_t /* version */)
{
    ar(cereal::base_class<CommonMetadata>(&m));
    ar(m.scalar_data);
    /* ar(m.vector_data); */
    ar(m.matrix_data);
}

} // namespace stratmod

CEREAL_CLASS_VERSION(stratmod::ScalarProperty, 0);

CEREAL_CLASS_VERSION(stratmod::Vector3dProperty, 0);

CEREAL_CLASS_VERSION(stratmod::Matrix3dProperty, 0);

CEREAL_CLASS_VERSION(stratmod::MatrixXdProperty, 0);

CEREAL_CLASS_VERSION(stratmod::CommonMetadata, 0);
/* CEREAL_REGISTER_TYPE(stratmod::CommonMetadata); */

CEREAL_CLASS_VERSION(stratmod::CurveMetadata, 0);
CEREAL_REGISTER_TYPE(stratmod::CurveMetadata);
CEREAL_REGISTER_POLYMORPHIC_RELATION(stratmod::CommonMetadata, stratmod::CurveMetadata);

CEREAL_CLASS_VERSION(stratmod::SurfaceMetadata, 0);
CEREAL_REGISTER_TYPE(stratmod::SurfaceMetadata);
CEREAL_REGISTER_POLYMORPHIC_RELATION(stratmod::CommonMetadata, stratmod::SurfaceMetadata);

CEREAL_CLASS_VERSION(stratmod::VolumeMetadata, 0);
CEREAL_REGISTER_TYPE(stratmod::VolumeMetadata);
CEREAL_REGISTER_POLYMORPHIC_RELATION(stratmod::CommonMetadata, stratmod::VolumeMetadata);

#endif
