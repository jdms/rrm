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

#ifndef STRATMOD_REGION
#define STRATMOD_REGION

#include <memory>
#include <optional>
#include <vector>
#include <set>

#include "stratmod/misc/stratmod_primitives.hpp"
#include "stratmod/metadata.hpp"

namespace stratmod {

class STRATMODLIB_DLL_HANDLER Region {
    public:
        VolumeMetadata metadata{};

        static Region Outer();
        static std::optional<Region> Get(int region_id);

        Region(int region_id = -1);
        ~Region();

        Region(const Region&);
        Region& operator=(const Region&);

        Region(Region&&);
        Region& operator=(Region&&);

        bool expired() const;

        int id() const;

        std::set<Region> forwardMap() const;

        bool comparable(const Region& r) const;

        bool operator==(const Region& r) const;

        bool operator<(const Region& r) const;

    private:
        struct RegionImpl;
        std::unique_ptr<RegionImpl> pimpl_;

        friend class cereal::access;
        template<typename T, typename B> friend struct cereal::detail::Version;

        template<typename Archive>
        void serialize(Archive& ar, const std::uint32_t /* version */);
};

} // namespace stratmod

#endif
