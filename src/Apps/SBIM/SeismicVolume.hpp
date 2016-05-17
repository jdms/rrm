/*
 * SeismicVolume.hpp
 *
 *  Created on: Apr 14, 2016
 *      Author: felipe
 */

#ifndef _SBIM_SEISMICVOLUME_HPP_
#define _SBIM_SEISMICVOLUME_HPP_

#include <string>
#include <iostream>
#include <algorithm>

namespace RRM
{
	/** \brief  The class SeismicVolume is responsible to read images from a Seismic data.
	 *          The images are on raw format.
	 */
	class SeismicVolume
	{
		public:
			SeismicVolume ( );
			~SeismicVolume ( );

			bool read ( std::string _filepath );

			void getSlice( int x, int y, int z);

			void getTopSlice( );

		public:

			/// FIXME Hard Coded Seismic data
			/// TODO
			unsigned short width  = 596;
			unsigned short height = 291;
			unsigned short depth  = 297;

			unsigned short * seismic_short = nullptr;
			unsigned short * id            = nullptr;
			unsigned short * distance      = nullptr;


			// Sesimic data layout
			// d = depth
			// w = width
			// h = height
 			// d * width * height + h * width + w

			// The top slice: max depth

			std::vector<float> seismic_data_;
			std::vector<unsigned short> seismic_id_;
			std::vector<unsigned short> seismic_distance_;

			std::vector< std::vector<unsigned char> >  images_slices_;

	};

} /* namespace RRM */

#endif /* _SBIM_SEISMICVOLUME_HPP_ */
