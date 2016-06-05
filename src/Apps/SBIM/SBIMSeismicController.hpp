/*
 * SBIMSeismicController.hpp
 *
 *  Created on: Apr 14, 2016
 *      Author: felipe
 */

#ifndef _SBIM_SBIMSEISMICCONTROLLER_HPP_
#define _SBIM_SBIMSEISMICCONTROLLER_HPP_

#include <QtCore/QObject>
#include <QtCore/QBuffer>
#include <QtCore/QByteArray>
#include <QtGui/QPixmap>

#include "SBIM/SketchSeismicModule.hpp"
#include "SBIM/SeismicVolume.hpp"

namespace RRM
{

	/**
	 * \brief Controller to SBIM Seismic Module
	 */

	class SBIMSeismicController: public QObject
	{
		Q_OBJECT

		public:

			typedef std::vector<unsigned char> 	    RRMImage;

			typedef RRM::SeismicSlice<qreal> 	      SeismicSlice;
			typedef typename SeismicSlice::CrossSection   CrossSection;
			typedef std::map<unsigned int, SeismicSlice > SeismicSlices;

			SBIMSeismicController ( QObject *parent = nullptr );
			~SBIMSeismicController ( );

			/// New cross Section
			bool setCurrentSeismicSlice ( unsigned int _index );

			/// It asks SketchModule to interpolate the curves
			/// @return false in case of all requirement are not met
			bool interpolateSketchies ( );

			bool addSeismicSlice ( unsigned int _seismic_slice_index, const QPixmap& _overlay_image );
			bool removeSeismicSlice ( );
			bool replaceSeismicSlice ( );

			bool readSeismic ( std::string _filepath );

			const SeismicSlices& getSeismicSlices() const;

			/// Converters
			RRMImage convertQPixmap2RRMImage (const QPixmap& _overlay_image);
		signals:
			void SetCrossSection ( const CrossSection& _seismic_slice);
		public:
			RRM::SeismicVolume seismic_data_;
			SketchSeismicModule<qreal> sketch_seismic_module_;

	};

} /* namespace RRM */

#endif /* _SBIM_SBIMSEISMICCONTROLLER_HPP_ */
