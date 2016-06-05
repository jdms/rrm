#ifndef _EXTRUSIONCONTROLLER_HPP_
#define _EXTRUSIONCONTROLLER_HPP_

#include<Tucano/BoundingBox3.hpp>
#include<Eigen/Dense>

#include <vector>
#include <tuple>
#include <map>
#include <iostream>

#include <QtCore/QObject>

#include "SBIM/SeismicSlice.hpp"
#include "Model/CrossSection.hpp"

#include "Core/Geometry/Surface/Planin/planar_surface.hpp"
#include "Core/Geometry/Surface/Planin/core.hpp"

namespace RRM
{

	class ExtrusionController : public QObject
	{
		Q_OBJECT

		public:

			enum Module { BlankScreen , Seismic };

			typedef  RRM::CrossSection<qreal>   		CrossSection;

			typedef RRM::SeismicSlice<qreal> 	       	SeismicSlice;
			typedef std::map<unsigned int, SeismicSlice >  	SeismicSlices;

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


			// Black Screen
			void  setBlackScreenCrossSection	( const CrossSection& _cross_section );

			void  createBlackScreenExtrusionMesh 	( const std::vector<std::vector<Eigen::Vector3f> >& _patchies,
								  float stepx,
								  float stepz,
								  float volume_width,
								  Eigen::Vector3f center,
								  float diagonal,
								  std::vector<Eigen::Vector3f> &_patch);

			void  createBlackScreenCube 	( const Celer::BoundingBox3<float>& box,
							  std::vector<Eigen::Vector3f>& _cube );

			void updateBlackScreenMesh( float stepx, float stepz, float volume_width, std::vector<Eigen::Vector3f>& _cube,std::vector<Eigen::Vector3f> &_patch);

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
			std::vector<Eigen::Vector4f> getSketchLines ( );

			std::vector<Eigen::Vector4f> sketchLinearInterpolation ( );
			// Radial Basis Function
			std::vector<Eigen::Vector4f> sketchRBFInterpolation ( );
			/// Take the current SBIM and normalize/update the 3D visualization
			std::vector<Eigen::Vector4f> updateSeismicSlices ( std::vector<float>& vl,std::vector<float>& nl,std::vector<std::size_t>& fl);
			void setSeismicSlices ( const SeismicSlices& _seismic_slices);

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
			CrossSection cross_section_;
			//std::vector<Eigen::Vector3f> patch_;

			/// Seismic Module
			std::map<unsigned int, SeismicSlice > seismic_slices_;

			// It used normalise the scene
			Eigen::Vector4f center_;

			// Used to normalize and centralize the object into the scene
			float scale_ = 0.0f;
			float scale_x_ = 0.0f;
			float scale_y_ = 0.0f;
			float scale_z_ = 0.0f;

			// Created by the Bounding box of the geological model
			// Normalized to 0-1 using the maximum dimension and centred at (0.0,0.0,0.0)
			Eigen::Affine3f trasnform_matrix_;

			Eigen::Vector4f min_;
                        Eigen::Vector4f max_;

                        /// Planin Library
                        std::map<unsigned int, PlanarSurface::Ptr> surfaces;

	};

} /* namespace RRM */

#endif /* _EXTRUSIONCONTROLLER_HPP_ */
