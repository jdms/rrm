#include <algorithm>
#include <memory>
#include <optional>

#include "detail/region_impl.hpp"

#include "stratmod/region.hpp"

namespace stratmod {

    /* struct Region::RegionImpl { */
    /*     VolumeMetadata metadata_; */
    /*     std::vector<std::size_t> lower_surfaces_{}; */
    /*     std::vector<std::size_t> upper_surfaces_{}; */
    /*     std::vector<ControllerSurfaceIndex> ordered_model_surfaces{}; */
    /*     std::map<ControllerSurfaceIndex, ContainerSurfaceIndex> dictionary_; */
        
    /*     bool operator==(const RegionImpl& rimpl) */
    /*     { */
    /*         bool equal = true; */
    /*         equal &= lower_surfaces_ == rimpl.lower_surfaces_; */
    /*         equal &= upper_surfaces_ == rimpl.upper_surfaces_; */
    /*         equal &= ordered_model_surfaces == rimpl.ordered_model_surfaces; */
    /*         equal &= dictionary_ == rimpl.dictionary_; */

    /*         return equal; */
    /*     } */

    /*     std::optional<ContainerSurfaceIndex> getSurfaceIndex(ControllerSurfaceIndex surface_id) */
    /*     { */
    /*         std::optional<ControllerSurfaceIndex> sid; */
    /*         auto iter = dictionary_.find(surface_id); */  

    /*         if ( iter == dictionary_.end() ) { */
    /*             return sid; */ 
    /*         } */
    /*         sid = std::make_optional(iter->second); */

    /*         return sid; */
            
    /*     } */

    /*     bool expired() */
    /*     { */
    /*         bool expired = false; */

    /*         auto current_ordered_model_surfaces = detail::SModellerAccess::Get()->getOrderedSurfacesIndices(); */
    /*         expired |= (ordered_model_surfaces != current_ordered_model_surfaces); */

    /*         return expired; */
    /*     } */

    /*     void registerCurrentModelState() */
    /*     { */
    /*         auto smodeller_impl_ptr = detail::SModellerAccess::Get(); */
    /*         ordered_model_surfaces = smodeller_impl_ptr->getOrderedSurfacesIndices(); */
    /*         dictionary_ = smodeller_impl_ptr->dictionary_; */
    /*     } */

    /*     std::vector<std::size_t> trim(std::vector<std::size_t> indices) const */
    /*     { */
    /*         std::vector<std::size_t> trim{}; */

    /*         auto this_indices = ordered_model_surfaces; */
    /*         std::sort(this_indices.begin(), this_indices.end()); */

    /*         auto current_indices = detail::SModellerAccess::Get()->getSurfacesIndices(); */
    /*         std::sort(current_indices.begin(), current_indices.end()); */

    /*         std::vector<std::size_t> pre_trim{}; */
    /*         std::set_difference( */
    /*                 this_indices.begin(), this_indices.end(), */ 
    /*                 current_indices.begin(), current_indices.end(), */ 
    /*                 std::inserter(pre_trim, pre_trim.begin()) */
    /*                 ); */

    /*         std::sort(indices.begin(), indices.end()); */

    /*         std::set_difference( */
    /*                 indices.begin(), indices.end(), */ 
    /*                 pre_trim.begin(), pre_trim.end(), */ 
    /*                 std::inserter(trim, trim.begin()) */
    /*                 ); */

    /*         return trim; */
    /*     } */

    /*     bool isAncestorOf(const Region& r) */
    /*     { */
    /*         auto t_lower = trim(lower_surfaces_); */ 
    /*         auto r_lower = r.pimpl_->lower_surfaces_; */
    /*         std::sort(r_lower.begin(), r_lower.end()); */

    /*         if (t_lower.empty() || !std::includes( */
    /*                     r_lower.begin(), r_lower.end(), */
    /*                     t_lower.begin(), t_lower.end() */
    /*                     ) */
    /*            ) */ 
    /*         { */
    /*             return false; */
    /*         } */

    /*         auto t_upper = trim(upper_surfaces_); */ 
    /*         auto r_upper = r.pimpl_->upper_surfaces_; */
    /*         if (t_upper.empty() || !std::includes( */
    /*                     r_upper.begin(), r_upper.end(), */
    /*                     t_upper.begin(), t_upper.end() */
    /*                     ) */
    /*            ) */
    /*         { */
    /*             return false; */
    /*         } */

    /*         return true; */
    /*     } */

    /*     /1* template<typename Archive> *1/ */
    /*     /1* void save(Archive& ar, const std::uint32_t /2* version *2/) const *1/ */
    /*     /1* { *1/ */
    /*     /1*     ar(metadata); *1/ */
    /*     /1*     ar(lower_surfaces_); *1/ */
    /*     /1*     ar(upper_surfaces_); *1/ */
    /*     /1*     ar(ordered_model_surfaces); *1/ */
    /*     /1*     ar(dictionary_); *1/ */
    /*     /1* } *1/ */

    /*     /1* template<typename Archive> *1/ */
    /*     /1* void load(Archive& ar, const std::uint32_t /2* version *2/) *1/ */
    /*     /1* { *1/ */
    /*     /1*     ar(metadata); *1/ */
    /*     /1*     ar(lower_surfaces_); *1/ */
    /*     /1*     ar(upper_surfaces_); *1/ */
    /*     /1*     ar(ordered_model_surfaces); *1/ */
    /*     /1*     ar(dictionary_); *1/ */
    /*     /1* } *1/ */

    /*     template<typename Archive> */
    /*     void serialize(Archive& ar, const std::uint32_t /1* version *1/) */
    /*     { */
    /*         ar(metadata); */
    /*         ar(lower_surfaces_); */
    /*         ar(upper_surfaces_); */
    /*         ar(ordered_model_surfaces); */
    /*         ar(dictionary_); */
    /*     } */
    /* }; */

    /* template<typename Archive> */
    /* void Region::save(Archive& ar, const std::uint32_t /1* version *1/) const */
    /* { */
    /*     ar(pimpl_) */
    /* } */

    /* template<typename Archive> */
    /* void Region::load(Archive& ar, const std::uint32_t /1* version *1/) */
    /* { */
    /*     ar(pimpl_); */
    /* } */

    /* template<typename Archive> */
    /* void Region::serialize(Archive& ar, const std::uint32_t /1* version *1/) */
    /* { */
    /*     ar(pimpl_); */
    /* } */

    std::optional<Region> Region::Get(int region_id)
    {
        std::optional<Region> r;
        int max_num_regions = detail::SModellerAccess::Get()->maxNumRegions();

        if (max_num_regions < 1)
        {
            return r;
        }

        if ((region_id < 0) || (region_id >= max_num_regions))
        {
            r = std::make_optional(Region::Outer());
        }

        r = std::make_optional(Region(region_id));
        return r;
    }

    Region Region::Outer()
    {
        return Region(-1);
    }

    Region::Region(int region_id)
    {
        pimpl_ = std::make_unique<RegionImpl>();
        pimpl_->registerCurrentModelState();
        if (pimpl_->maxNumRegions() < 1)
        {
            return;
        }
        if (pimpl_->numSurfaces() < 2)
        {
            return;
        }
        if ((region_id < 0) || (region_id >= pimpl_->maxNumRegions()))
        {
            return;
        }

        std::vector<std::size_t> lower_surfaces{}, upper_surfaces{};
        for (std::size_t i = 0; i <= static_cast<std::size_t>(region_id); ++i)
        {
            lower_surfaces.push_back(pimpl_->getRegisteredSurfacesIndices()[i]);
        }
        for (std::size_t i = static_cast<std::size_t>(region_id + 1); i <= pimpl_->numSurfaces() -1; ++i)
        {
            upper_surfaces.push_back(pimpl_->getRegisteredSurfacesIndices()[i]);
        }
        pimpl_->lower_surfaces_ = lower_surfaces;
        pimpl_->upper_surfaces_ = upper_surfaces;
    }

    Region::~Region() = default;

    Region::Region(const Region& r)
    {
        this->pimpl_ = std::make_unique<RegionImpl>(*r.pimpl_);
    }

    Region& Region::operator=(const Region& r)
    {
        this->pimpl_ = std::make_unique<RegionImpl>(*r.pimpl_);

        return *this;
    }

    Region::Region(Region&&) = default;
    Region& Region::operator=(Region&&) = default;

    bool Region::expired() const
    {
        return pimpl_->expired();
    }

    int Region::id() const
    {
        if (!pimpl_->valid())
        {
            return -1;
        }
        if (pimpl_->lower_surfaces_.size() < 1)
        {
            return -1;
        }
        if (pimpl_->lower_surfaces_.size() >= pimpl_->numSurfaces())
        {
            return -1;
        }

        int region_id = static_cast<int>(pimpl_->lower_surfaces_.size()) - 1;
        return region_id;
    }

        /* VolumeMetadata& metadata(); */
        /* VolumeMetadata metadata() const; */

    /* VolumeMetadata& Region::metadata() */
    /* { */
    /*     return pimpl_->metadata_; */
    /* } */

    /* VolumeMetadata Region::metadata() const */
    /* { */
    /*     return pimpl_->metadata_; */
    /* } */

    std::set<Region> Region::forwardMap() const
    {
        std::set<Region> fmap{};
        if (!pimpl_->valid())
        {
            fmap.insert(*this);
            return fmap;
        }

        int max_num_regions = detail::SModellerAccess::Get()->maxNumRegions();
        for (int i = 0; i < max_num_regions; ++i)
        {
            auto r = Region::Get(i);

            if (r.has_value() && this->pimpl_->isAncestorOf(r.value()))
            {
                r.value().metadata = metadata;
                fmap.insert(r.value());
            }
        }

        return fmap;
    }

    bool Region::comparable(const Region& r) const
    {
        bool comparable = pimpl_->comparable(*r.pimpl_);
        return comparable;
    }

    bool Region::operator==(const Region& r) const
    {
        bool equal = (*pimpl_ == *r.pimpl_);
        return equal;
    }

    bool Region::operator<(const Region& r) const
    {
        bool comparable = this->comparable(r);
        bool less_than = (this->id() < r.id());
        return comparable && less_than;
    }

} // stratmod
