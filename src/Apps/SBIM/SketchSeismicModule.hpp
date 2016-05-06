/*
 * SketchSeismicModule.hpp
 *
 *  Created on: Apr 18, 2016
 *      Author: felipe
 */

#ifndef _SBIM_SKETCHSEISMICMODULE_HPP_
#define _SBIM_SKETCHSEISMICMODULE_HPP_

#include <map>

#include "SBIM/SeismicSlice.hpp"

namespace RRM
{
	/**
	 * \brief SketchSiesmic manager.
	 */
	template <class _Real>
	class SketchSeismicModule
	{
		public:
			typedef _Real					Real;
			typedef RRM::SeismicSlice<Real> 	 	SeismicSlice;
			typedef std::vector<unsigned char>	        RRMImage;

			SketchSeismicModule ( )
			{
				this->current_slice_ = 0;
			}

			~SketchSeismicModule ( )
			{

			}

			bool setCurrentSlice ( unsigned int _index )
			{
				this->current_slice_ = _index;
				return false;
			}

			unsigned int currentSlice ( ) const
			{
				return this->current_slice_;
			}

			bool addSeismicSlice( unsigned int _seismic_slice_index, const RRMImage& _overlay_image )
			{
				/// The slice already exist
				if ( this->seismic_slices_.count( _seismic_slice_index ) || (_seismic_slice_index == 0) )
				{
					return false;
				}
				/// Add new CrossSection
				else
				{
					this->seismic_slices_[_seismic_slice_index].id_    = _seismic_slice_index;
					this->seismic_slices_[_seismic_slice_index].image_ = _overlay_image;

				}

				std::cout << " Index " << _seismic_slice_index << std::endl;

				return true;

			}

			unsigned int numberOfSeismicSlices ( ) const
			{
				return	seismic_slices_.size();
			}

		public:
			unsigned int current_slice_;

			std::map<unsigned int, SeismicSlice > seismic_slices_;

	};

} /* namespace RRM */

#endif /* _SBIM_SKETCHSEISMICMODULE_HPP_ */
