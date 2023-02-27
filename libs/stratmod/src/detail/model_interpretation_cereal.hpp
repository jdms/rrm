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

#ifndef STRATMOD_MODEL_INTERPRETATION_CEREAL
#define STRATMOD_MODEL_INTERPRETATION_CEREAL

#include "detail/serialization_definitions.hpp"

#include <map>
#include <memory>
#include <string>
#include <optional>

#include "detail/region_cereal.hpp"
#include "detail/curve3d_cereal.hpp"

#include "stratmod/model_interpretation.hpp"
#include "stratmod/metadata.hpp"
#include "stratmod/region.hpp"

#include "Eigen/Dense"


namespace stratmod {

template<typename Archive>
void serialize(Archive & ar, Domain& d, const std::uint32_t /* version */)
{
    ar(d.regions);
    ar(d.metadata);
}

template<typename Archive>
inline void ModelInterpretation::serialize(Archive& ar, const std::uint32_t /* version */)
{
    ar(domains);
    ar(curves);
    ar(metadata);
}

} // namespace stratmod

CEREAL_CLASS_VERSION(stratmod::Domain, 0);

CEREAL_CLASS_VERSION(stratmod::ModelInterpretation, 0);

#endif
