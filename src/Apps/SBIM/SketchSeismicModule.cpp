/*
 * SketchSeismicModule.cpp
 *
 *  Created on: Apr 18, 2016
 *      Author: felipe
 */

#include "SketchSeismicModule.hpp"

namespace RRM
{

	SketchSeismicModule::SketchSeismicModule ()
	{
		this->current_slice_ = 0;
	}

	SketchSeismicModule::~SketchSeismicModule ( )
	{

	}

	bool SketchSeismicModule::addSeismicSlice ( unsigned int _seismic_slice_index )
	{
		/// The slice already exist
		if ( this->seismic_slices_.count( _seismic_slice_index ))
		{
			return false;
		}
		/// Add new CrossSection
		else
		{
			this->seismic_slices_[_seismic_slice_index] = RRM::SeismicSlice<qreal>();
		}
	}

} /* namespace RRM */
