/*
 * SBIMSeismicController.hpp
 *
 *  Created on: Apr 14, 2016
 *      Author: felipe
 */

#ifndef _SBIM_SBIMSEISMICCONTROLLER_HPP_
#define _SBIM_SBIMSEISMICCONTROLLER_HPP_



namespace RRM
{

	class SBIMSeismicController
	{
		public:
			SBIMSeismicController ( );
			~SBIMSeismicController ( );

			// new cross Section
			void addSliceSeismic();
	};

} /* namespace RRM */

#endif /* _SBIM_SBIMSEISMICCONTROLLER_HPP_ */
