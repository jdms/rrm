/********************************************************************************/
/*                                                                              */
/* This file is part of the "Stratigraphy Modeller Library" (StratModLib)       */
/* Copyright (C) 2017-2022, Julio Daniel Machado Silva.                         */
/*                                                                              */
/* StratModLib is free software; you can redistribute it and/or                 */
/* modify it under the terms of the GNU Lesser General Public                   */
/* License as published by the Free Software Foundation; either                 */
/* version 3 of the License, or (at your option) any later version.             */
/*                                                                              */
/* StratModLib is distributed in the hope that it will be useful,               */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU            */
/* Lesser General Public License for more details.                              */
/*                                                                              */
/* You should have received a copy of the GNU Lesser General Public             */
/* License along with StratModLib.  If not, see <http://www.gnu.org/licenses/>, */
/* or write to the Free Software Foundation, Inc., 51 Franklin Street,          */
/* Fifth Floor, Boston, MA  02110-1301  USA.                                    */
/*                                                                              */
/********************************************************************************/



#ifndef STRATMOD_MODEL_INTERPRETATION_HPP
#define STRATMOD_MODEL_INTERPRETATION_HPP

#include <optional>
#include <vector>
#include <set>
#include <unordered_map>

#include "stratmod/misc/stratmod_primitives.hpp"
#include "stratmod/metadata.hpp"
#include "stratmod/curve3d.hpp"
#include "stratmod/region.hpp"

namespace stratmod {

struct STRATMODLIB_DLL_HANDLER Domain {
    std::set<Region> regions{};
    VolumeMetadata metadata{};

    bool expired() const;

    bool empty() const;

    bool contains(int region_id) const;
    bool contains(const Region& region) const;

    bool setRegionMetadata(int region_id, const VolumeMetadata& metadata);
    bool setRegionMetadata(const Region& region, const VolumeMetadata& metadata);

    void setRegionsMetadata(const VolumeMetadata& metadata);

    enum class RegionsToKeepOnMerge { Theirs, Ours };
    using Keep = RegionsToKeepOnMerge;

    bool mergeRegions(std::set<Region> source_regions, const Keep& option = Keep::Ours);
};


class STRATMODLIB_DLL_HANDLER ModelInterpretation {
    public:
        using Domains = std::unordered_map<int, Domain>;
        using Curves = std::unordered_map<int, Curve3D>;
        using RegionToDomainMap = std::unordered_map<int, std::vector<int>>;

        Domains domains{};
        Curves curves{};
        CommonMetadata metadata{};

        bool expired() const;
        bool updateDomains();

        bool empty() const;

        RegionToDomainMap computeRegionToDomainMap() const;
        std::vector<int> getConsistentDomainsIndices() const;

        void setDomain(int domain_id, Domain d);
        bool eraseDomain(int domain_id);
        std::optional<Domain> getDomain(int domain_id) const; 
        std::vector<int> getDomainsIndices() const;

        void setCurve(int curve_id, Curve3D curve);
        bool eraseCurve(int curve_id);
        std::optional<Curve3D> getCurve(int curve_id) const; 
        std::vector<int> getCurvesIndices() const;

        //////////////////////////////////////////////////////////////////////
        // Syntatic sugar
        //////////////////////////////////////////////////////////////////////

        int addDomain(Domain d);
        Domains getDomainsNamed(const std::string& name) const;
        std::vector<int> findDomainsNamed(const std::string& name) const;

        int addCurve(Curve3D c);
        Curves getCurvesNamed(const std::string& name) const;
        std::vector<int> findCurvesNamed(const std::string& name) const;

    private:
        friend class cereal::access;

        template<typename Archive>
        void serialize(Archive&, const std::uint32_t);
};

} // stratmod

#endif
