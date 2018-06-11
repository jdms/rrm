/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _FLOWMODEL_PROPERTYPROFILE_HPP_
#define _FLOWMODEL_PROPERTYPROFILE_HPP_

#include <iostream>
#include <string>

namespace RRM
{

        class PropertyProfile
        {
        public:
            /// @see http://www.cplusplus.com/articles/y8hv0pDG/ copy and assingment functions
            PropertyProfile() = default;
            ~PropertyProfile() = default;
            PropertyProfile& operator=(const PropertyProfile&) = default;
            PropertyProfile( const std::string& element_type_,
                             const std::string& _other_name,
                             const std::string& _other_entity,
                             const std::string& _other_unit,
                             const std::string& _other_dimension );

            std::string elementType() const;
            std::string name() const;
            std::string entity() const;
            std::string unit() const;
            std::string dimension() const;

            void elementType(const std::string& _other_element_type);
            void setName(const std::string& _other_name);
            void setEntity(const std::string& _other_entity);
            void setUnit(const std::string& _other_unit);
            void setDimension(const std::string& _other_dimension);

            void print() const;

        private:
            std::string element_type_;
            std::string name_;
            std::string entity_;
            std::string unit_;
            std::string dimension_;
        };

} /* namespace RRM */

#endif /* _FLOWMODEL_PROPERTYPROFILE_HPP_ */
