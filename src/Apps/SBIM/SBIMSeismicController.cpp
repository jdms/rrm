/*
 * SBIMSeismicController.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: felipe
 */

#include "SBIMSeismicController.hpp"

namespace RRM
{

	SBIMSeismicController::SBIMSeismicController ( QObject*  parent) : QObject(parent)
	{

	}

	SBIMSeismicController::~SBIMSeismicController ( )
	{

	}

	bool SBIMSeismicController::setCurrentSeismicSlice ( unsigned int _index )
	{
		if (sketch_seismic_module_.seismic_slices_.count(_index) )
		{
			sketch_seismic_module_.setCurrentSlice(_index);

			return true;
		}

		return false;
	}

	bool SBIMSeismicController::interpolateSketchies ( )
	{
		return false;
	}

	bool SBIMSeismicController::addSeismicSlice( unsigned int _seismic_slice_index, const QPixmap& _overlay_image )
	{
		return this->sketch_seismic_module_.addSeismicSlice(_seismic_slice_index,convertQPixmap2RRMImage(_overlay_image));
	}

	bool SBIMSeismicController::removeSeismicSlice ( )
	{
		return false;
	}

	bool SBIMSeismicController::replaceSeismicSlice ( )
	{
		return false;
	}

	SBIMSeismicController::RRMImage SBIMSeismicController::convertQPixmap2RRMImage (const QPixmap& _overlay_image)
	{
		/// @see http://doc.qt.io/qt-5/qpixmap.html#save-1
		QByteArray bytes;
		QBuffer buffer(&bytes);
		buffer.open(QIODevice::ReadWrite);
		_overlay_image.save(&buffer, "JPG");

		return SBIMSeismicController::RRMImage (bytes.constData(), bytes.constData() + bytes.size());
	}


} /* namespace RRM */
