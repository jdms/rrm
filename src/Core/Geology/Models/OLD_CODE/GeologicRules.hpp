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


/*
 * GeologicRules.hpp
 *
 *  Created on: Mar 19, 2016
 *      Author: felipe
 */

#ifndef _GEOLOGY_GEOLOGICRULES_HPP_
#define _GEOLOGY_GEOLOGICRULES_HPP_

namespace RRM
{

	/*!
	 *  @brief Enum that lists the geological rules�s types
	 */
	enum GeologicRules
	{
		Sketch, REMOVE_ABOVE_INTERSECTION, REMOVE_BELOW_INTERSECTION, FREE
	};

} /* namespace RRM */

#endif /* SRC_APPS_MODEL_GEOLOGY_GEOLOGICRULES_HPP_ */
