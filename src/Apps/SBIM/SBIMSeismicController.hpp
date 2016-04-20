/*
 * SBIMSeismicController.hpp
 *
 *  Created on: Apr 14, 2016
 *      Author: felipe
 */

#ifndef _SBIM_SBIMSEISMICCONTROLLER_HPP_
#define _SBIM_SBIMSEISMICCONTROLLER_HPP_


#include <QtCore/QObject>

#include "SBIM/SketchSeismicModule.hpp"

namespace RRM
{

	/**
	 * \brief Controller to SBIM Seismic Module
	 */

	class SBIMSeismicController : public QObject
	{
		Q_OBJECT

		public:
			SBIMSeismicController ( QObject *parent = nullptr );
			~SBIMSeismicController ( );

			/// new cross Section
			bool setCurrentSeismicSlice( unsigned int _current);

			/// It asks SketchModule to interpolate the curves
			/// @return false if it fail
			bool interpolateSketchies( );

			bool addSeismicSlice( unsigned int _seismic_slice_index );
			bool removeSeismicSlice( );
			bool replaceSeismicSlice( );
		public:

			SketchSeismicModule sketch_seismic_module_;


	};

} /* namespace RRM */

#endif /* _SBIM_SBIMSEISMICCONTROLLER_HPP_ */
