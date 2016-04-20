/*
 * SeismicVolume.hpp
 *
 *  Created on: Apr 14, 2016
 *      Author: felipe
 */

#ifndef _SBIM_SEISMICVOLUME_HPP_
#define _SBIM_SEISMICVOLUME_HPP_


namespace RRM
{

	class SeismicVolume
	{
		public:
			SeismicVolume ( );
			~SeismicVolume ( );

			bool read( )
			{
				return true;
			}

			void getSlice( int x, int y, int z);

			void getTopSlice( );
		public:


	};

} /* namespace RRM */

#endif /* _SBIM_SEISMICVOLUME_HPP_ */
