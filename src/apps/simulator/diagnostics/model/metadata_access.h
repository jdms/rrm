/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2021
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with RRM.  If not, see <http://www.gnu.org/licenses/>,
 * or write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA  02110-1301  USA.
 */

/**
 * @file metadata_access.hpp
 * @author Julio Daniel Machado Silva
 * @brief Utilities to simplify access to stratmod::SModeller metadata
 */

#pragma once

#include <functional>
#include <optional>

#include <QColor>

#include "stratmod/smodeller.hpp"

namespace model {

class MetadataAccess
{
    public:
        using Region = stratmod::Region;
        using Domain = stratmod::Domain;

        using ModelInterpretation = stratmod::ModelInterpretation;
        using Domains = stratmod::ModelInterpretation::Domains;
        using Curves = stratmod::ModelInterpretation::Curves;

        // Handles of interpretations to use when sharing data between flow diagnostics/sketching gui
        inline static std::string InterpretationHandle = "MainInterpretation";
        inline static std::string RegionsHandle = "CurrentRegions";

        static stratmod::SModeller& model() { return stratmod::SModeller::Instance(); }

        //////////////////////////////////////////////////////////////////////
        // Access volumetric metadata in stratmod
        //////////////////////////////////////////////////////////////////////

        static ModelInterpretation& clearIntrepetation()
        {
            auto& data = model().interpretations();
            data[InterpretationHandle] = ModelInterpretation();
            ModelInterpretation& current = data[InterpretationHandle];
            return current;
        }

        static ModelInterpretation& interpretation()
        {
            auto& data = model().interpretations();
            ModelInterpretation& current = data[InterpretationHandle];
            return current;
        }

        /// Update domains and check if they are consistent
        ///
        /// Consistency means that any single region must not appear in more
        /// than one domain.
        ///
        /// If inconsistencies are found erase *all* ofending domains.
        static void enforceDomainsConsistency()
        {
            auto& current = interpretation();

            // If current interpretation is empty it is (vacuously) consistent
            if (current.empty())
            {
                return;
            }

            // Try to update domains, clear interpretation if it is not possible
            bool success = current.updateDomains();
            if (!success)
            {
                clearIntrepetation();
                return;
            }

            // Enforce consistentency
            auto consistent_domains_ids = current.getConsistentDomainsIndices();
            auto consistentDomain = [&](int i) -> bool {
                for (auto j : consistent_domains_ids)
                {
                    if (i == j)
                    {
                        return true;
                    }
                }
                return false;
            };
            for (auto i : current.getDomainsIndices())
            {
                if (!consistentDomain(i))
                {
                    current.eraseDomain(i);
                }
            }
        }

        static ModelInterpretation& resetRegions()
        {
            auto& data = model().interpretations();
            ModelInterpretation& current = data[RegionsHandle];
            current = model().getRegions();
            return current;
        }

        static ModelInterpretation& regionsInterpretation()
        {
            auto& data = model().interpretations();
            ModelInterpretation& current = data[RegionsHandle];
            if (current.empty())
            {
                current = model().getRegions();
            }
            return current;
        }

        static bool updateRegions()
        {
            auto& current = regionsInterpretation();

            // Try to update regions, reset regions' interpretation if it is not possible
            bool success = current.updateDomains();
            if (!success)
            {
                resetRegions();
            }

            return success;
        }

        static void enforceRegionsConsistency()
        {
            auto& current = regionsInterpretation();

            // Try to update regions, clear interpretation if it is not possible
            bool success = current.updateDomains();
            if (!success)
            {
                resetRegions();
                return;
            }

            // Enforce consistentency
            auto consistent_regions_ids = current.getConsistentDomainsIndices();
            auto consistentRegion = [&](int i) -> bool {
                for (auto j : consistent_regions_ids)
                {
                    if (i == j)
                    {
                        return true;
                    }
                }
                return false;
            };
            for (auto i : current.getDomainsIndices())
            {
                if (!consistentRegion(i))
                {
                    current.eraseDomain(i);
                }
            }

            // Reassign metadata when region forward map is unambiguous
            auto new_regions = model().getRegions();
            for (auto& [_, old_region_i] : current.domains)
            {
                if (old_region_i.regions.size() == 1)
                {
                    int new_id = old_region_i.regions.begin()->id();
                    auto iter = new_regions.domains.find(new_id);
                    if (iter != new_regions.domains.end())
                    {
                        auto& [_, new_region] = *iter;
                        new_region.metadata = old_region_i.metadata;
                    }
                }
            }

            regionsInterpretation() = std::move(new_regions);
        }

        static Domains& domains()
        {
            ModelInterpretation& current = interpretation();
            return current.domains;
        }

        static Domains& regions()
        {
            ModelInterpretation& current = regionsInterpretation();
            return current.domains;
        }

        static Curves& curves()
        {
            auto& current = interpretation();
            return current.curves;
        }

        static std::string& name(Domain& d)
        {
            return d.metadata.name;
        }

        static std::string& description(Domain& d)
        {
            return d.metadata.description;
        }

        static bool getColor(const Domain& d, QColor& c)
        {
            if (!d.metadata.color_rgb)
            {
                return false;
            }

            c = color(*d.metadata.color_rgb);
            return true;
        }

        static void setColor(Domain& d, const QColor& c)
        {
            d.metadata.color_rgb = std::make_optional(color(c));
        }

        //////////////////////////////////////////////////////////////////////
        // Access surface metadata in stratmod
        //////////////////////////////////////////////////////////////////////

        static bool getSurfaceMetadata(std::size_t surface_index,
                std::string& name,
                std::string& description,
                std::optional<QColor>& qcolor)
        {
            stratmod::SurfaceMetadata metadata;
            bool success = model().getSurfaceMetadata(surface_index, metadata);
            if (success)
            {
                name = metadata.name;
                description = metadata.description;
                if(metadata.color_rgb)
                {
                    qcolor = std::make_optional<QColor>(color(*metadata.color_rgb));
                }
            }
            return success;
        }

        static bool setSurfaceMetadata(std::size_t surface_index,
                const std::string& name,
                const std::string& description,
                const std::optional<QColor>& qcolor)
        {
            stratmod::SurfaceMetadata metadata;
            metadata.name = name;
            metadata.description = description;
            if (qcolor)
            {
                metadata.color_rgb = color(*qcolor);
            }
            else
            {
                metadata.color_rgb = {};
            }
            bool success = model().setSurfaceMetadata(surface_index, metadata);
            return success;
        }

        //////////////////////////////////////////////////////////////////////
        // Misc utilities
        //////////////////////////////////////////////////////////////////////

        static QColor color(const Eigen::Vector3i& color_rgb)
        {
            int r = color_rgb[0];
            int g = color_rgb[1];
            int b = color_rgb[2];
            return QColor(r, g, b);
        }

        static Eigen::Vector3i color(const QColor& color)
        {
            int r, g, b;
            color.getRgb(&r, &g, &b);
            return Eigen::Vector3i({r, g, b});
        }
};

} // namespace model
