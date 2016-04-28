/*
 * SketchSeismicModule.hpp
 *
 *  Created on: Apr 18, 2016
 *      Author: felipe
 */

#ifndef _SBIM_SKETCHSEISMICMODULE_HPP_
#define _SBIM_SKETCHSEISMICMODULE_HPP_

#include <map>

#include <QtCore/QtGlobal>
#include <QtGui/QPixmap>

#include "SBIM/SeismicSlice.hpp"

namespace RRM
{
	/**
	 * \brief SketchSiesmic manager.
	 */
	class SketchSeismicModule
	{
		public:
			typedef RRM::SeismicSlice<qreal> 	 	SeismicSlice;
			typedef std::pair< SeismicSlice ,QPixmap>       SeismicPair;

			SketchSeismicModule ( );
			~SketchSeismicModule ( );

			bool setCurrentSlice ( unsigned int _index );

			unsigned int currentSlice ( ) const;

			bool addSeismicSlice( unsigned int _seismic_slice_index, const QPixmap& _overlay_image );

			unsigned int numberOfSeismicSlices ( ) const;

		public:
			unsigned int current_slice_;
			std::map<unsigned int, SeismicPair > seismic_slices_;

	};

} /* namespace RRM */

#endif /* _SBIM_SKETCHSEISMICMODULE_HPP_ */
