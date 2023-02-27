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

#include <algorithm>
#include <memory>
#include <optional>

#include "detail/smodeller_access.hpp"
#include "detail/type_name.hpp"

#include "stratmod/metadata.hpp"
#include "stratmod/model_interpretation.hpp"

namespace stratmod {

//////////////////////////////////
// Domain implementation
//////////////////////////////////

bool Domain::expired() const
{
    for (auto& r : regions)
    {
        if (r.expired())
        {
            return true;
        }
    }

    return false;
}

bool Domain::empty() const
{
    return regions.empty() && metadata.empty();
}

bool Domain::contains(int region_id) const
{
    for (auto& r : regions)
    {
        if (region_id == r.id())
        {
            return true;
        }
    }

    return false;
}

bool Domain::contains(const Region& r) const
{
    auto iter = regions.find(r);
    if (iter == regions.end())
    {
        return false;
    }

    return true;
}

bool Domain::setRegionMetadata(int region_id, const VolumeMetadata& m)
{
    for (auto& r : regions)
    {
        if (region_id == r.id())
        {
            return setRegionMetadata(r, m);
        }
    }

    return false;
}

bool Domain::setRegionMetadata(const Region& r, const VolumeMetadata& m)
{
    auto node_handle = regions.extract(r);
    if (node_handle.empty())
    {
        return false;
    }

    node_handle.value().metadata = m;
    regions.insert(std::move(node_handle));

    return true;
}

void Domain::setRegionsMetadata(const VolumeMetadata& m)
{
    for (auto& r : regions)
    {
        setRegionMetadata(r, m);
    }
}

bool Domain::mergeRegions(std::set<Region> regions, const Keep& option)
{
    if (regions.empty())
    {
        return true;
    }

    for (auto& ours : this->regions)
    {
        for (auto &theirs : regions)
        {
            if (!ours.comparable(theirs))
            {
                return false;
            }
        }
    }

    switch (option)
    {
        case Keep::Theirs:
            regions.merge(this->regions);
            this->regions = std::move(regions);
            break;

        case Keep::Ours:

        default:
            this->regions.merge(regions);
    }

    return true;
}


//////////////////////////////////////////////////////////////////////////////
// Begin ModelInterpretationImpl related implementation
//////////////////////////////////////////////////////////////////////////////

        /* ModelInterpretation(); */
        /* virtual ~ModelInterpretation(); */

        /* ModelInterpretation(const ModelInterpretation&); */
        /* ModelInterpretation& operator=(const ModelInterpretation&); */

        /* ModelInterpretation(ModelInterpretation&&); */
        /* ModelInterpretation& operator=(ModelInterpretation&&); */

        /* Domains& domains(); */
        /* Domains domains() const; */

        /* Curves& curves(); */
        /* Curves curves() const; */

    /* protected: */
        /* struct ModelInterpretationImpl; */
        /* std::unique_ptr<ModelInterpretationImpl> pimpl_; */

/* struct ModelInterpretation::ModelInterpretationImpl { */
/*     Domains domains_{}; */

/*     Curves curves_{}; */
/* }; */

/* ModelInterpretation::ModelInterpretation() */
/* { */
/*     pimpl_ = std::make_unique<ModelInterpretationImpl>(); */ 
/* } */

/* ModelInterpretation::~ModelInterpretation() = default; */

/* ModelInterpretation::ModelInterpretation(const ModelInterpretation& m) */
/* { */
/*     this->pimpl_ = std::make_unique<ModelInterpretationImpl>(*m.pimpl_); */
/* } */

/* ModelInterpretation& ModelInterpretation::operator=(const ModelInterpretation& m) */
/* { */
/*     this->pimpl_ = std::make_unique<ModelInterpretationImpl>(*m.pimpl_); */
/*     return *this; */
/* } */

/* ModelInterpretation::ModelInterpretation(ModelInterpretation&&) = default; */

/* ModelInterpretation& ModelInterpretation::operator=(ModelInterpretation&&) = default; */

//////////////////////////////////////////////////////////////////////////////
// End of ModelInterpretationImpl related implementation
//////////////////////////////////////////////////////////////////////////////

bool ModelInterpretation::expired() const
{
    for (auto& [i, domain_i] : domains)
    {
        if (domain_i.expired())
        {
            return true;
        }
    }

    return false;
}

bool ModelInterpretation::updateDomains()
{
    if (empty() || !expired())
    {
        return true;
    }

    for (auto& [i, domain_i] : domains)
    {
        auto regions_domain_i = domain_i.regions;
        domain_i.regions.clear();

        for (auto& r : regions_domain_i)
        {
            domain_i.mergeRegions(r.forwardMap());
        }
    }

    return !expired();
}

bool ModelInterpretation::empty() const
{
    return domains.empty() && curves.empty() && metadata.empty();
}

void ModelInterpretation::setDomain(int domain_id, Domain d)
{
    domains[domain_id] = d;
}

bool ModelInterpretation::eraseDomain(int domain_id)
{
    if (auto iter = domains.find(domain_id); iter != domains.end())
    {
        domains.erase(iter);
        return true;
    }

    return false;
}

std::optional<Domain> ModelInterpretation::getDomain(int domain_id) const
{
    std::optional<Domain> d;

    if (auto iter = domains.find(domain_id); iter != domains.end())
    {
        auto& [id, domain] = *iter;
        d = std::make_optional(domain);
    }

    return d;
}

/* ModelInterpretation::Domains& ModelInterpretation::domains() */
/* { */
/*     return pimpl_->domains_; */
/* } */

/* ModelInterpretation::Domains ModelInterpretation::domains() const */
/* { */
/*     return pimpl_->domains_; */
/* } */

std::vector<int> ModelInterpretation::getDomainsIndices() const
{
    std::vector<int> domains_indices{};

    for (auto& [i, domain_i] : domains)
    {
        domains_indices.push_back(i);
    }

    return domains_indices;
}

void ModelInterpretation::setCurve(int curve_id, Curve3D curve)
{
    curves[curve_id] = curve;
}

bool ModelInterpretation::eraseCurve(int curve_id)
{
    if (auto iter = curves.find(curve_id); iter != curves.end())
    {
        curves.erase(iter);
        return true;
    }

    return false;
}

std::optional<Curve3D> ModelInterpretation::getCurve(int curve_id) const
{
    std::optional<Curve3D> c;

    if (auto iter = curves.find(curve_id); iter != curves.end())
    {
        auto& [id, curve] = *iter;
        c = std::make_optional(curve);
    }

    return c;
} 

/* ModelInterpretation::Curves& ModelInterpretation::curves() */
/* { */
/*     return pimpl_->curves_; */
/* } */

/* ModelInterpretation::Curves ModelInterpretation::curves() const */
/* { */
/*     return pimpl_->curves_; */
/* } */

std::vector<int> ModelInterpretation::getCurvesIndices() const
{
    std::vector<int> curves_indices{};

    for (auto& [i, curve_i] : curves)
    {
        curves_indices.push_back(i);
    }

    return curves_indices;
}

ModelInterpretation::RegionToDomainMap ModelInterpretation::computeRegionToDomainMap() const
{
    RegionToDomainMap map(detail::SModellerAccess::Get()->maxNumRegions());

    for (auto& [i, domain_i] : domains)
    {
        for (auto& r : domain_i.regions)
        {
            map[r.id()].push_back(i);
        }
    }

    return map;
}

std::vector<int> ModelInterpretation::getConsistentDomainsIndices() const
{
    std::vector<int> consistent_domains{};

    auto domains_intersect = [&](const Domain& lhs, const Domain& rhs) -> bool {
        std::set<Region> intersection{};
        std::set_intersection(
                lhs.regions.begin(), lhs.regions.end(), 
                rhs.regions.begin(), rhs.regions.end(),
                std::inserter(intersection, intersection.begin())
                );
        if (intersection.empty())
        {
            return false;
        }
        return true;
    };

    for (auto& [i, domain_i] : domains)
    {
        bool disjoint = true;
        for (auto& [j, domain_j] : domains)
        {
            if ( (i != j) && domains_intersect(domain_i, domain_j))
            {
                disjoint = false;
                break;
            }
        }

        if (disjoint && !domain_i.regions.empty())
        {
            consistent_domains.push_back(i);
        }
    }

    return consistent_domains;
}

//////////////////////////////////////////////////////////////////////
// Syntatic sugar
//////////////////////////////////////////////////////////////////////

int ModelInterpretation::addDomain(Domain d)
{
    int last_inserted_domain_id = 0;
    for (auto& [i, domain_i] : domains)
    {
        last_inserted_domain_id = std::max(last_inserted_domain_id, i);
    }

    int new_id = last_inserted_domain_id + 1;
    this->setDomain(new_id, d);
    return new_id;
}

int ModelInterpretation::addCurve(Curve3D c)
{
    int last_inserted_curve_id = 0;
    for (auto& [i, domain_i] : curves)
    {
        last_inserted_curve_id = std::max(last_inserted_curve_id, i);
    }

    int new_id = last_inserted_curve_id + 1;
    this->setCurve(new_id, c);
    return new_id;
}

ModelInterpretation::Domains ModelInterpretation::getDomainsNamed(const std::string& name) const
{
    Domains map{};
    for (auto& [i, domain_i] : domains)
    {
        if (domain_i.metadata.name == name)
        {
            map[i] = domain_i;
        }
    }

    return map;
} 

std::vector<int> ModelInterpretation::findDomainsNamed(const std::string& name) const
{
    std::vector<int> list{};
    for (auto& [i, domain_i] : domains)
    {
        if (domain_i.metadata.name == name)
        {
            list.push_back(i);
        }
    }

    return list;
} 

ModelInterpretation::Curves ModelInterpretation::getCurvesNamed(const std::string& name) const
{
    Curves map{};
    for (auto& [i, curve_i] : curves)
    {
        if (curve_i.metadata.name == name)
        {
            map[i] = curve_i;
        }
    }

    return map;
} 

std::vector<int> ModelInterpretation::findCurvesNamed(const std::string& name) const
{
    std::vector<int> list{};
    for (auto& [i, curve_i] : curves)
    {
        if (curve_i.metadata.name == name)
        {
            list.push_back(i);
        }
    }

    return list;
} 

} // namespace stratmod
