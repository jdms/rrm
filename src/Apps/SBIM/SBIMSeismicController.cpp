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

	bool SBIMSeismicController::setCurrentSeismicSlice ( unsigned int _index )
	{
		if (sketch_seismic_module_.seismic_slices_.count(_index) )
		{
			sketch_seismic_module_.setCurrentSlice(_index);

			return true;
		}

		return false;
	}

	bool SBIMSeismicController::interpolateSketchies ( )
	{
		return false;
	}

	bool SBIMSeismicController::addSeismicSlice( unsigned int _seismic_slice_index, const QPixmap& _overlay_image )
	{
		return this->sketch_seismic_module_.addSeismicSlice(_seismic_slice_index,_overlay_image);
	}

	bool SBIMSeismicController::removeSeismicSlice ( )
	{
		return false;
	}

	bool SBIMSeismicController::replaceSeismicSlice ( )
	{
		return false;
	}

} /* namespace RRM */
