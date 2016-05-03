#ifndef _EXTRUSIONCONTROLLER_HPP_
#define _EXTRUSIONCONTROLLER_HPP_

#include<Tucano/BoundingBox3.hpp>
#include<Eigen/Dense>

#include <vector>
#include <tuple>
#include <iostream>

#include <QtCore/QObject>

#include "SBIM/SeismicSlice.hpp"

namespace RRM
{

	class ExtrusionController : public QObject
	{
		Q_OBJECT

		public:
			typedef RRM::SeismicSlice<qreal> 	 	SeismicSlice;

			ExtrusionController ( );
			virtual ~ExtrusionController ( );

		public slots:

			bool initialize     (  float _x_min,
	                                                               float _y_min,
								       float _z_min,
								       float _x_max,
								       float _y_max,
								       float _z_max);



			std::vector<Eigen::Vector4f> getcubeMesh ( );
			std::vector<Eigen::Vector4f> getPlanes   ( const std::vector<unsigned int>& );

			// Linear
			// Mesh Layout:
			// - Geometry   vec4
			// - Normal     vec4
			// - Colour     vec4
			// - Attributes vec4


			std::vector<Eigen::Vector4f> sketchLinearInterpolation ( );
			// Radial Basis Function
			std::vector<Eigen::Vector4f> sketchRBFInterpolation ( );

		signals:

		public:

			//   -------- 1.0
			//  |        |
			//  |        |
			//   -------- 0.0
			//
			/// Black Screen
			//  On Black Screen, only planes 0 and 1.0 exist
			std::map<unsigned int, SeismicSlice > seismic_slices_;
			/// Seismic

			// It used normalise the scene
			Eigen::Vector4f center_;

			float normalize_;
			float normalize_x_;
			float normalize_y_;
			float normalize_z_;

			Eigen::Vector4f min_;
                        Eigen::Vector4f max_;
	};

} /* namespace RRM */

#endif /* _EXTRUSIONCONTROLLER_HPP_ */
