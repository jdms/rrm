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

#ifndef STRATMOD_REGION_IMPL
#define STRATMOD_REGION_IMPL

#include "detail/serialization_definitions.hpp"

#include <algorithm>
#include <memory>
#include <optional>

#include "detail/smodeller_access.hpp"
#include "detail/smodeller_impl.hpp"

#include "stratmod/region.hpp"

namespace stratmod {

    struct Region::RegionImpl {
        VolumeMetadata metadata_;
        std::vector<std::size_t> lower_surfaces_{};
        std::vector<std::size_t> upper_surfaces_{};
        std::vector<ControllerSurfaceIndex> ordered_model_surfaces{};
        std::map<ControllerSurfaceIndex, ContainerSurfaceIndex> dictionary_;
        unsigned int model_id_;
        
        bool operator==(const RegionImpl& rimpl)
        {
            bool equal = true;
            equal &= lower_surfaces_ == rimpl.lower_surfaces_;
            equal &= upper_surfaces_ == rimpl.upper_surfaces_;
            equal &= ordered_model_surfaces == rimpl.ordered_model_surfaces;
            equal &= dictionary_ == rimpl.dictionary_;
            equal &= model_id_ == rimpl.model_id_;

            return equal;
        }

        std::optional<ContainerSurfaceIndex> getSurfaceIndex(ControllerSurfaceIndex surface_id)
        {
            std::optional<ControllerSurfaceIndex> sid;
            auto iter = dictionary_.find(surface_id);  

            if ( iter == dictionary_.end() ) {
                return sid; 
            }
            sid = std::make_optional(iter->second);

            return sid;
        }

        bool expired()
        {
            bool expired = true;

            /* if (!valid()) */
            /* { */
            /*     return expired; */
            /* } */

            auto current_model_id = detail::SModellerAccess::Get()->model_id_;
            if (model_id_ != current_model_id)
            {
                return expired;
            }

            auto current_ordered_model_surfaces = detail::SModellerAccess::Get()->getOrderedSurfacesIndices();
            if (ordered_model_surfaces != current_ordered_model_surfaces)
            {
                return expired;
            }

            return !expired;
        }

        bool valid() const
        {
            bool valid = true;

            valid &= !ordered_model_surfaces.empty();
            valid &= !lower_surfaces_.empty(); 
            valid &= !upper_surfaces_.empty();

            return valid;

        }

        bool comparable(const RegionImpl& r) const
        {
            bool comparable = (model_id_ == r.model_id_)
                && (ordered_model_surfaces == r.ordered_model_surfaces);

            return /* valid() && */ comparable;
        }

        void registerCurrentModelState()
        {
            auto smodeller_impl_ptr = detail::SModellerAccess::Get();
            ordered_model_surfaces = smodeller_impl_ptr->getOrderedSurfacesIndices();
            dictionary_ = smodeller_impl_ptr->dictionary_;
            model_id_ = smodeller_impl_ptr->model_id_;
        }

        std::vector<std::size_t> trim(std::vector<std::size_t> indices) const
        {
            std::vector<std::size_t> trim{};

            auto this_indices = ordered_model_surfaces;
            std::sort(this_indices.begin(), this_indices.end());

            auto current_indices = detail::SModellerAccess::Get()->getSurfacesIndices();
            std::sort(current_indices.begin(), current_indices.end());

            std::vector<std::size_t> pre_trim{};
            std::set_difference(
                    this_indices.begin(), this_indices.end(), 
                    current_indices.begin(), current_indices.end(), 
                    std::inserter(pre_trim, pre_trim.begin())
                    );

            std::sort(indices.begin(), indices.end());

            std::set_difference(
                    indices.begin(), indices.end(), 
                    pre_trim.begin(), pre_trim.end(), 
                    std::inserter(trim, trim.begin())
                    );

            std::sort(trim.begin(), trim.end());
            return trim;
        }

        bool isAncestorOf(const Region& r)
        {
            bool success = true;

            auto t_lower = trim(lower_surfaces_); 
            auto r_lower = r.pimpl_->lower_surfaces_;
            std::sort(r_lower.begin(), r_lower.end());
            if (t_lower.empty() || !std::includes(
                        r_lower.begin(), r_lower.end(),
                        t_lower.begin(), t_lower.end()
                        )
               ) 
            {
                success = false;
            }

            auto t_upper = trim(upper_surfaces_); 
            auto r_upper = r.pimpl_->upper_surfaces_;
            std::sort(r_upper.begin(), r_upper.end());
            if (t_upper.empty() || !std::includes(
                        r_upper.begin(), r_upper.end(),
                        t_upper.begin(), t_upper.end()
                        )
               )
            {
                success = false;
            }

            return success;
        }

        std::size_t numSurfaces()
        {
            return ordered_model_surfaces.size();
        }

        int maxNumRegions()
        {
            return detail::SModellerAccess::Get()->maxNumRegions();
        }

        std::vector<ControllerSurfaceIndex> getRegisteredSurfacesIndices()
        {
            return ordered_model_surfaces;
        }

        template<typename Archive>
        void serialize(Archive& ar, const std::uint32_t /* version */)
        {
            ar(metadata_);
            ar(lower_surfaces_);
            ar(upper_surfaces_);
            ar(ordered_model_surfaces);
            ar(dictionary_);
        }
    };

} // namespace stratmod

CEREAL_CLASS_VERSION(stratmod::Region::RegionImpl, 0);

#endif
