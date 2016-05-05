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

			enum Module { BlankScreen , Seismic };

			typedef RRM::SeismicSlice<qreal> 	       SeismicSlice;
			typedef std::map<unsigned int, SeismicSlice >  SeismicSlices;

			ExtrusionController ( QObject* parent = 0 );
			virtual ~ExtrusionController ( );

		public slots:
			/// Initialize it with the bounding box of the scene
			bool initialize     (  float _x_min,
	                                                               float _y_min,
								       float _z_min,
								       float _x_max,
								       float _y_max,
								       float _z_max);



			// All the geometry will have the following layout as:
			// Linear
			// layout(lines_adjacency) in;  -> 4 Vertex
			// layout(triangle_strip, max_vertices=4) out -> 2 Triangles;
			// Mesh Layout:
			// - Geometry   vec4
			// *v0------v1*
			//         -
			//      -
		  	//   -
			// *v2------v3*
			// - Normal     vec4
			// - Colour     vec4
			// - Attributes vec4
			/// Seismic Module
			std::vector<Eigen::Vector4f> getcubeMesh ( );
			std::vector<Eigen::Vector4f> getPlanes   ( const std::vector<unsigned int>& );

			std::vector<Eigen::Vector4f> sketchLinearInterpolation ( );
			// Radial Basis Function
			std::vector<Eigen::Vector4f> sketchRBFInterpolation ( );
			/// Take the current SBIM and normalize/update the 3D visualization
			void updateSeismicSlices ( const SeismicSlices& _seismic_slices);

		signals:

		public:
			Module module_ = Module::Seismic;
		public:

			//   -------- 1.0
			//  |        |
			//  |        |
			//   -------- 0.0
			//

			/// Black Screen Module
			//  On Black Screen, only planes 0 and 1.0 exist

			/// Seismic Module
			std::map<unsigned int, SeismicSlice > seismic_slices_;

			// It used normalise the scene
			Eigen::Vector4f center_;

			// Used to normalize and centralize the object into the scene
			float scale_;
			float scale_x_ = 0.0f;
			float scale_y_ = 0.0f;
			float scale_z_ = 0.0f;

			// Created by the Bounding box of the geological model
			// Normalized to 0-1 using the maximum dimension and centred at (0.0,0.0,0.0)
			Eigen::Affine3f trasnform_matrix_;

			Eigen::Vector4f min_;
                        Eigen::Vector4f max_;
	};

} /* namespace RRM */

#endif /* _EXTRUSIONCONTROLLER_HPP_ */
