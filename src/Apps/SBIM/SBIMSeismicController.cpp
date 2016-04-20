/*
 * SBIMSeismicController.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: felipe
 */

#include "SBIMSeismicController.hpp"

namespace RRM
{

	SBIMSeismicController::SBIMSeismicController ( QObject*  parent) : QObject(parent)
	{

	}

	SBIMSeismicController::~SBIMSeismicController ( )
	{

	}

	bool SBIMSeismicController::addSeismicSlice( unsigned int _seismic_slice_index )
	{
		return this->sketch_seismic_module_.addSeismicSlice(_seismic_slice_index);
	}

} /* namespace RRM */
