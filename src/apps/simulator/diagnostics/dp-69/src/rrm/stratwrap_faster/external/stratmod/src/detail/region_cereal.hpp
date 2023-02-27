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

#ifndef STRATMOD_REGION_CEREAL
#define STRATMOD_REGION_CEREAL

#include "detail/serialization_definitions.hpp"
#include "cereal/archives/binary.hpp"

#include "detail/region_impl.hpp"

#include "stratmod/region.hpp"

namespace stratmod {

    template<typename Archive>
    inline void Region::serialize(Archive& ar, const std::uint32_t /* version */)
    {
        ar(metadata);
        ar(pimpl_);
    }

    template void Region::RegionImpl::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, const std::uint32_t);
    template void Region::RegionImpl::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, const std::uint32_t);
    template void Region::RegionImpl::serialize<cereal::PortableBinaryInputArchive>(cereal::PortableBinaryInputArchive&, const std::uint32_t);
    template void Region::RegionImpl::serialize<cereal::PortableBinaryOutputArchive>(cereal::PortableBinaryOutputArchive&, const std::uint32_t);
    template void Region::RegionImpl::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, const std::uint32_t);
    template void Region::RegionImpl::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, const std::uint32_t);
    template void Region::RegionImpl::serialize<cereal::XMLInputArchive>(cereal::XMLInputArchive&, const std::uint32_t);
    template void Region::RegionImpl::serialize<cereal::XMLOutputArchive>(cereal::XMLOutputArchive&, const std::uint32_t);

    template void Region::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, const std::uint32_t);
    template void Region::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, const std::uint32_t);
    template void Region::serialize<cereal::PortableBinaryInputArchive>(cereal::PortableBinaryInputArchive&, const std::uint32_t);
    template void Region::serialize<cereal::PortableBinaryOutputArchive>(cereal::PortableBinaryOutputArchive&, const std::uint32_t);
    template void Region::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, const std::uint32_t);
    template void Region::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, const std::uint32_t);
    template void Region::serialize<cereal::XMLInputArchive>(cereal::XMLInputArchive&, const std::uint32_t);
    template void Region::serialize<cereal::XMLOutputArchive>(cereal::XMLOutputArchive&, const std::uint32_t);

} // stratmod

CEREAL_CLASS_VERSION(stratmod::Region, 0);

#endif
