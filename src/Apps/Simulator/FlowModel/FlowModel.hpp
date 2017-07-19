/*
 * FlowModel.hpp
 *
 *  Created on: Apr 30, 2017
 *      Author: felipe
 */

#ifndef _FLOWMODEL_HPP_
#define _FLOWMODEL_HPP_

#include <Eigen/Dense>
#include <vector>


#include "Tucano/BoundingBox3.hpp"


// OpenVolume Mesh
#include "Core/Geometry/Mesh/OpenVolumeMeshDefines.hpp"
#include "Core/Geometry/Mesh/MeshGenerator.hpp"

#include "././libs/Tucano/BoundingBox3.hpp"




namespace RRM
{

    class FlowModel
    {
        public:

			enum class TYPE{ TRIANGLES, QUADRILATERAL, TETRAHEDRAL, HEXAHEDRAL };

            FlowModel();
            ~FlowModel() = default ;

			std::shared_ptr<OpenVolumeMesh::TetrahedralMeshV3d> getPtrTetrahedralMesh();
			std::shared_ptr<OpenVolumeMesh::HexahedralMesh3d>   getPtrHexahedralMesh();

			bool createTetrahedonMesh(const std::vector<float> &_vertices, const std::vector<unsigned int>& _cells);
			bool createHexahedonMesh(const std::vector<float> &_vertices, const std::vector<unsigned int>& _cells);

			void FlowModel::loadMesh(std::shared_ptr<OpenVolumeMesh::TetrahedralMeshV3d> _ptr_mesh);
			void FlowModel::loadMesh(std::shared_ptr<OpenVolumeMesh::HexahedralMesh3d> _ptr_mesh);


			/// OpenVolumeMesh Integration				
			void updateGeometry(std::vector < unsigned int >& faces_array, std::vector<float>& vertices_array_);

			void updateTetrahedronColors(const std::string& _property_name, const std::string& _entity_name, const std::string& _dimension, std::vector< double >& _values);
			void updateHexahedronColors(const std::string& _property_name, const std::string& _entity_name, const std::string& _dimension, std::vector< double >& _values);

			void uploadHexahedron(std::vector < unsigned int >& faces_array, std::vector<float>& vertices_array_);
			void uploadTetrahedron(std::vector < unsigned int >& faces_array, std::vector<float>& vertices_array_);


			void updateTetrahedronVertexScalarProperty(const std::string& _property_name, std::vector< double >& _values);
			void updateTetrahedronVertexVectorProperty(const std::string& _property_name, std::vector< double >& _values);
			void updateTetrahedronCellScalarProperty(const std::string& _property_name, std::vector< double >& _values);
			void updateTetrahedronCellVectorProperty(const std::string& _property_name, std::vector< double >& _values);

			
			void updateHexahedronVertexScalarProperty(const std::string& _property_name, std::vector< double >& _values);
			void updateHexahedronVertexVectorProperty(const std::string& _property_name, std::vector< double >& _values);
			void updateHexahedronCellScalarProperty(const std::string& _property_name, std::vector< double >& _values);
			void updateHexahedronCellVectorProperty(const std::string& _property_name, std::vector< double >& _values);

			/// @TODO Later, move this fucntions to FlowvizualizationController 
			Eigen::Affine3d getModelMatrix() const;


	private:
		/// OpenVolume Mesh Integration ---------------------------------------------->
		// Tetrahedron Mesh
		// Draw GL_TRIANGLES -> Triangles
		std::shared_ptr<OpenVolumeMesh::TetrahedralMeshV3d> ptr_tetrahedron_mesh_;
		// Hexahedron Mesh
		// Draw GL_LINES_ADJACENCY -> Triangle strip		
		//   2*-->-*3     	  
		//   | *    |   
		//   |   *  |
		//   0*-->-*1
		std::shared_ptr<OpenVolumeMesh::HexahedralMesh3d> ptr_hexahedron_mesh_;

		TYPE mesh_type;

		/// @TODO Later, move this fucntions to FlowvizualizationController 
		/// Bring model from Model coodinates to OpenGL coordiates
		Eigen::Affine3d model_matrix_;
		/// BoundingBox in OpenGL coordiates
		Tucano::BoundingBox3<double> bounding_box_;

    };

} /* namespace RRM */

#endif /* _FLOWMODEL_HPP_ */
