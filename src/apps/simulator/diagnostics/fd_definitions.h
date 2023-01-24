/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2021
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file fd_definitions.h
 * @author Julio Daniel Machado Silva
 * @brief Utilities to simplify access to stratmod::SModeller metadata
 */

#pragma once

#include <QColor>

#include "stratmod/smodeller.hpp"

/* namespace rrm::fd { */

class FlowDiagnosticsDefinitions
{
    public:
        using Domain = stratmod::Domain;
        using ModelInterpretation = stratmod::ModelInterpretation;

        /// Handles of interpretations to use when sharing data between flow diagnostics/sketching gui
        /// Must match values found in @model::MetadataAccess
        inline static std::string InterpretationHandle = "MainInterpretation";
        inline static std::string RegionsHandle = "CurrentRegions";

        // Handles of properties to save/load FD metadata in stratmod::SModeller
        inline static std::string poro = "poro";
        inline static std::string k_xy = "k_xy";
        inline static std::string k_z = "k_z";

       //////////////////////////////////////////////////////////////////////
        // The following methods are syntatic sugar to simplify access to flow
        // diagnostics metadata in the stratmod library
        //////////////////////////////////////////////////////////////////////

        /// Get pointer to domain whose id is @param domain_id
        /// 
        /// Returns nullptr if domain is not found
        static Domain* accessDomain(int domain_id)
        {
            Domain* dptr = nullptr;
            auto& domains = stratmod::SModeller::Instance().interpretations()[InterpretationHandle].domains;

            auto iter = domains.find(domain_id);
            if (iter == domains.end())
            {
                return dptr;
            }
            auto& [_, domain] = *iter;
            dptr = &domain;

            return dptr;
        }

        /// Get name of domain @param d if it is not empty, set its name otherwise
        ///
        /// Return @param name_ if domain has no name, domain's name otherwise
        static std::string getORsetName(Domain& d, const std::string& name_)
        {
            if (name(d).empty())
            {
                name(d) = name_;
            }

            return name(d);
        }

        /// If @param d is not null: get its name if it is not empty, set its name otherwise
        ///
        /// Return @param name_ if @param d is null or if domain has no name, domain's name otherwise
        static std::string getORsetName(Domain* d, const std::string& name_)
        {
            if (d == nullptr)
            {
                return name_;
            }

            return getORsetName(*d, name_);
        }

        /// Get color of domain @param d if it is set, set its color otherwise
        ///
        /// Return @param c_ if domain has no color, domain's color otherwise
        static QColor getORsetColor(Domain& d, const QColor& c_) 
        {
            QColor qc;
            if (!getColor(d, qc))
            {
                qc = c_;
                setColor(d, c_);
            }

            return qc;
        }

        /// If @param d is not null: get its color if it is set, set its color otherwise
        ///
        /// Return @param c_ if @param d is null or if domain has no color, domain's color otherwise
        static QColor getORsetColor(Domain* d, const QColor& c_) 
        {
            if (d == nullptr)
            {
                return c_;
            }

            return getORsetColor(*d, c_);
        }

        /// Get porosity of domain @param d if it is set, set its porosity otherwise
        ///
        /// Return @param val if domain's porosity is not set, domain's porosity otherwise
        static double getORsetPoro(Domain& d, double val,
                const std::string name = "Porosity", const std::string unit = "%")
        {
            auto& data = d.metadata.scalar_data;
            if (auto iter = data.find(poro); iter == data.end())
            {
                data[poro].value = val;
                data[poro].name = name;
                data[poro].unit = unit;

                return val;
            }

            return data[poro].value;
        }

        /// If @param d is not null: get its porosity if it is set, set its porosity otherwise
        ///
        /// Return @param val if @param d is null or if domain has no porosity, domain's porosity otherwise
        static double getORsetPoro(Domain* d, double val,
                const std::string name = "Porosity", const std::string unit = "%")
        {
            if (d == nullptr)
            {
                return val;
            }

            return getORsetPoro(*d, val, name, unit);
        }

        /// Get permeability_xy of domain @param d if it is set, set its permeability_xy otherwise
        ///
        /// Return @param val if domain's permeability_xy is not set, domain's permeability_xy otherwise
        static double getORsetPermXY(Domain& d, double val,
                const std::string name = "Permeability XY", const std::string unit = "MilliDarcy")
        {
            auto& data = d.metadata.scalar_data;
            if (auto iter = data.find(k_xy); iter == data.end())
            {
                data[k_xy].value = val;
                data[k_xy].name = name;
                data[k_xy].unit = unit;

                return val;
            }

            return data[k_xy].value;
        }

        /// If @param d is not null: get its permeability_xy if it is set, set its permeability_xy otherwise
        ///
        /// Return @param val if @param d is null or if domain has no permeability_xy, domain's porosity otherwise
        static double getORsetPermXY(Domain* d, double val,
                const std::string name = "Permeability XY", const std::string unit = "MilliDarcy")
        {
            if (d == nullptr)
            {
                return val;
            }

            return getORsetPermXY(*d, val, name, unit);
        }

        /// Get permeability_z of domain @param d if it is set, set its permeability_z otherwise
        ///
        /// Return @param val if domain's permeability_z is not set, domain's permeability_z otherwise
        static double getORsetPermZ(Domain& d, double val,
                const std::string name = "Permeability Z", const std::string unit = "MilliDarcy")
        {
            auto& data = d.metadata.scalar_data;
            if (auto iter = data.find(k_z); iter == data.end())
            {
                data[k_z].value = val;
                data[k_z].name = name;
                data[k_z].unit = unit;

                return val;
            }

            return data[k_z].value;
        }

        /// If @param d is not null: get its permeability_z if it is set, set its permeability_z otherwise
        ///
        /// Return @param val if @param d is null or if domain has no permeability_z, domain's porosity otherwise
        static double getORsetPermZ(Domain* d, double val,
                const std::string name = "Permeability Z", const std::string unit = "MilliDarcy")
        {
            if (d == nullptr)
            {
                return val;
            }

            return getORsetPermZ(*d, val, name, unit);
        }

        /// Set porosity of domain d
        static void setPoro(Domain& d, double val,
                const std::string name = "Porosity", const std::string unit = "%")
        {
            auto& data = d.metadata.scalar_data;
            data[poro].value = val;
            data[poro].name = name;
            data[poro].unit = unit;
        }

        /// Set porosity of domain d
        static bool setPoro(Domain* d, double val,
                const std::string name = "Porosity", const std::string unit = "%")
        {
            if (d == nullptr)
            {
                return false;
            }
            setPoro(*d, val, name, unit);
            return true;
        }

        /// Set permeability_xy of domain d
        static void setPermXY(Domain& d, double val,
                const std::string name = "Permeability XY", const std::string unit = "MilliDarcy")
        {
            auto& data = d.metadata.scalar_data;
            data[k_xy].value = val;
            data[k_xy].name = name;
            data[k_xy].unit = unit;
        }

        /// Set permeability_xy of domain d
        static bool setPermXY(Domain* d, double val,
                const std::string name = "Permeability XY", const std::string unit = "MilliDarcy")
        {
            if (d == nullptr)
            {
                return false;
            }
            setPermXY(*d, val, name, unit);
            return true;
        }

        /// Set permeability_z of domain d
        static void setPermZ(Domain& d, double val,
                const std::string name = "Permeability Z", const std::string unit = "MilliDarcy")
        {
            auto& data = d.metadata.scalar_data;
            data[k_z].value = val;
            data[k_z].name = name;
            data[k_z].unit = unit;
        }

        /// Set permeability_z of domain d
        static bool setPermZ(Domain* d, double val,
                const std::string name = "Permeability Z", const std::string unit = "MilliDarcy")
        {
            if (d == nullptr)
            {
                return false;
            }
            setPermZ(*d, val, name, unit);
            return true;
        }

        /// Get porosity of domain d
        template<typename T = std::string>
        static bool getPoro(Domain& d, double& val,
                T&& name = "Porosity", T&& unit = "%")
        {
            auto& data = d.metadata.scalar_data;
            if (auto iter = data.find(poro); iter !=data.end())
            {
                val = data[poro].value;
                name = data[poro].name;
                unit = data[poro].unit;
                return true;
            }

            return false;
        }

        /// Get porosity of domain d
        template<typename T = std::string>
        static bool getPoro(Domain* d, double& val,
                T&& name = "Porosity", T&& unit = "%")
        {
            if (d == nullptr)
            {
                return false;
            }
            return getPoro(*d, val, name, unit);
        }

        /// Get permeability_xy of domain d
        template<typename T = std::string>
        static bool getPermXY(Domain& d, double& val,
                T&& name = "Permeability XY", T&& unit = "MilliDarcy")
        {
            auto& data = d.metadata.scalar_data;
            if (auto iter = data.find(poro); iter !=data.end())
            {
                val = data[k_xy].value;
                name = data[k_xy].name;
                unit = data[k_xy].unit;
                return true;
            }

            return false;
        }

        /// Get permeability_xy of domain d
        template<typename T = std::string>
        static bool getPermXY(Domain* d, double& val,
                T&& name = "Permeability XY", T&& unit = "MilliDarcy")
        {
            if (d == nullptr)
            {
                return false;
            }
            return getPermXY(*d, val, name, unit);
        }

        /// Get permeability_z of domain d
        template<typename T = std::string>
        static bool getPermZ(Domain& d, double& val,
                T&& name = "Permeability Z", T&& unit = "MilliDarcy")
        {
            auto& data = d.metadata.scalar_data;
            if (auto iter = data.find(poro); iter !=data.end())
            {
                val = data[k_z].value;
                name = data[k_z].name;
                unit = data[k_z].unit;
                return true;
            }

            return false;
        }

        /// Get permeability_z of domain d
        template<typename T = std::string>
        static bool getPermZ(Domain* d, double& val,
                T&& name = "Permeability Z", T&& unit = "MilliDarcy")
        {
            if (d == nullptr)
            {
                return false;
            }
            return getPermZ(*d, val, name, unit);
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
};

/* } // namespace rrm::fd */
