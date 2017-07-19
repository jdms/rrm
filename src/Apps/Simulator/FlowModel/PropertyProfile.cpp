/*
 * PropertyProfile.cpp
 *
 *  Created on: May 25, 2017
 *      Author: felipe
 */

#include "PropertyProfile.hpp"

namespace RRM
{

        //// Property Profile
	PropertyProfile::PropertyProfile( const std::string& _element_type_,
									  const std::string& _other_name,
                                      const std::string& _other_entity,
                                      const std::string& _other_unit,
									  const std::string& _other_dimension) : element_type_(_element_type_),
																			 name_(_other_name),
                                                                             entity_(_other_entity),
                                                                             unit_(_other_unit),
                                                                             dimension_(_other_dimension)
        {

        }

		std::string PropertyProfile::elementType() const
		{
			return this->element_type_;
		}

        std::string PropertyProfile::name() const
        {
            return this->name_;
        }

        std::string PropertyProfile::entity() const
        {
            return this->entity_;
        }

        std::string PropertyProfile::unit() const
        {
            return this->unit_;
        }

        std::string PropertyProfile::dimension() const
        {
            return this->dimension_;
        }

		void PropertyProfile::elementType(const std::string& _other_element_type)
		{
			this->element_type_ = _other_element_type;
		}

        void PropertyProfile::setName(const std::string& _other_name)
        {
            this->name_ = _other_name;
        }

        void PropertyProfile::setEntity(const std::string& _other_entity)
        {
            this->entity_ = _other_entity;
        }

        void PropertyProfile::setUnit(const std::string& _other_unit)
        {
            this->unit_ = _other_unit;
        }

        void PropertyProfile::setDimension(const std::string& _other_dimension)
        {
            this->dimension_ = _other_dimension;
        }

        void PropertyProfile::print() const
        {
            std::cout << "------------------: " << std::endl;
			std::cout << "Element Type      : " << this->element_type_ << std::endl;
            std::cout << "Name				: " << this->name_ << std::endl;
            std::cout << "Entity			: " << this->entity_ << std::endl;
            std::cout << "Unit				: " << this->unit_ << std::endl;
            std::cout << "Dimension			: " << this->dimension_ << std::endl;
            std::cout << "******************: " << std::endl;
        }

} /* namespace RRM */
