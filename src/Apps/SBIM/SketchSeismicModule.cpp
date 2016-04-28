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

	bool SketchSeismicModule::addSeismicSlice ( unsigned int _seismic_slice_index, const QPixmap& _overlay_image )
	{
		/// The slice already exist
		if ( this->seismic_slices_.count( _seismic_slice_index ))
		{
			return false;
		}
		/// Add new CrossSection
		else
		{
			this->seismic_slices_[_seismic_slice_index] = SeismicPair(SeismicSlice(),_overlay_image);
			this->seismic_slices_[_seismic_slice_index].first.id_ = _seismic_slice_index;
		}

		return true;
	}

	bool SketchSeismicModule::setCurrentSlice ( unsigned int _index )
	{
		this->current_slice_ = _index;
		return false;
	}

	unsigned int SketchSeismicModule::currentSlice ( ) const
	{
		return this->current_slice_;
	}

	unsigned int SketchSeismicModule::numberOfSeismicSlices ( ) const
	{
		return	seismic_slices_.size();
	}

} /* namespace RRM */
