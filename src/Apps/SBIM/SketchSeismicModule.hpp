/*
 * SketchSeismicModule.hpp
 *
 *  Created on: Apr 18, 2016
 *      Author: felipe
 */

#ifndef _SBIM_SKETCHSEISMICMODULE_HPP_
#define _SBIM_SKETCHSEISMICMODULE_HPP_

#include <vector>

#include <QtCore/QtGlobal>

#include "SeismicSlice.hpp"

namespace RRM
{

	class SketchSeismicModule
	{
		public:
			SketchSeismicModule ( );
		       ~SketchSeismicModule ( );
		public:
		      std::vector<SeismicSlice<qreal>> seismic_slices_;
	};

} /* namespace RRM */

#endif /* _SBIM_SKETCHSEISMICMODULE_HPP_ */
