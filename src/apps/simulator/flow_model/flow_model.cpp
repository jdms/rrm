/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "flow_model.hpp"

namespace RRM
{

    FlowModel::FlowModel()
    {
        // TODO Auto-generated constructor stub
        /// OpenVolume Mesh Integration ---------------------------------------------->
        // Tetrahedron Mesh
        ptr_tetrahedron_mesh_ = std::make_shared<OpenVolumeMesh::TetrahedralMeshV3d>();
        // Hexahedron Mesh
        ptr_hexahedron_mesh_ = std::make_shared<OpenVolumeMesh::HexahedralMesh3d>();

        model_matrix_.setIdentity();
        bounding_box_.reset();
    }

    bool FlowModel::createTetrahedonMesh(const std::vector<float> &_vertices, const std::vector<unsigned int>& _cells)
    {

            /// OpenVolume Mesh Interface

            // @see  TETGEN DOCUMENTATION at http://wias-berlin.de/software/tetgen/1.5/doc/manual/manual006.html
            // 5.2.4  .ele files


            // An.ele file contains a list of tetrahedra.

            //	First line : <# of tetrahedra> <nodes per tet. (4 or 10)>
            //	<region attribute(0 or 1)>
            //	Remaining lines list # of tetrahedra :
            //  <tetrahedron #> <node> <node> ... <node>[attribute]
            //	...
            //	Each tetrahedron has four corners(or ten corners if the - o2 switch is used).
            //  Nodes are indices into the corresponding.node file.
            //  The first four nodes are the corner vertices.If - o2 switch is used, the remaining six nodes are generated on the midpoints of the edges of the tetrahedron.
            //  Figure 20 shows how these corners and the second - order nodes are locally numbered.Second order nodes are output only.They are omitted by the mesh reconstruction(the - r switch).

            //                       3
            //                       /\
            //                      /  \
            //                     /    \
            //                    /      \
            //                   /        \
            //                0 /__________\ 1
            //                 / \        / \
            //                /   \      /   \
            //               /     \    /     \
            //              /       \  /       \
            //           3 /_________\/_________\ 3
            //            *          2           *


        if (this->ptr_tetrahedron_mesh_)
        {
            ptr_tetrahedron_mesh_->clear();
            MeshGenerator mesh_generator(*ptr_tetrahedron_mesh_);

            /////// Grab  the vertices
            for (std::size_t i = 0; i < _vertices.size(); i+=3)
            {
                mesh_generator.addVertex(OpenVolumeMesh::Vec3d(static_cast<double>(_vertices[i + 0]), static_cast<double>(_vertices[i + 1]), static_cast<double>(_vertices[i + 2])));
            }

            //std::map<int, int> dup;

            //std::set<FaceTuple> face_set;
            //std::set<unsigned int> node_set;

            /// Add faces and Cells
            for (std::size_t i = 0; i < _cells.size(); i += 4)
            {

                std::vector<unsigned int> cell = { _cells[i + 0], _cells[i + 1], _cells[i + 2], _cells[i + 3] };

                mesh_generator.addCell(cell);
            }

//            /// Euler Charactristic for Volume Mesh
//            int euler = (ptr_tetrahedron_mesh_->n_vertices() - ptr_tetrahedron_mesh_->n_edges() + ptr_tetrahedron_mesh_->n_faces() - ptr_tetrahedron_mesh_->n_cells());
//            std::cout << " === Mesh Information Shared Pointer ====  File FlowModel.cpp" << std::endl;
//            std::cout << "Size : " << " Faces :" << ptr_tetrahedron_mesh_->n_faces() << " | " << std::endl;
//            std::cout << "Size : " << " Edges :" << ptr_tetrahedron_mesh_->n_edges() << " | " << std::endl;
//            std::cout << "Size : " << " Vertices :" << ptr_tetrahedron_mesh_->n_vertices() << " | " << std::endl;
//            std::cout << "Size : " << " cells :" << ptr_tetrahedron_mesh_->n_cells() << std::endl;
//            std::cout << "Euler Characteristics : V-E+F-C= " << euler << std::endl;
//            std::cout << " ========================= " << std::endl;
        }

        return true;

    }

    bool FlowModel::createHexahedonMesh(const std::vector<float> &_vertices, const std::vector<unsigned int>& _cells)
    {
        // OpenVolumeMesh  - Hexahedron Class

        if (this->ptr_hexahedron_mesh_)
        {
            this->ptr_hexahedron_mesh_->clear();

            ///// Grab  the vertices
            std::vector<OpenVolumeMesh::VertexHandle> v(_cells.size());

            int index = 0;

            for (std::size_t i = 0; i < _vertices.size(); i += 3)
            {
                v[index++] = this->ptr_hexahedron_mesh_->add_vertex(OpenVolumeMesh::Vec3d(static_cast<double>(_vertices[i + 0]), static_cast<double>(_vertices[i + 1]), static_cast<double>(_vertices[i + 2])));
            }

            /// HWU Corner Point Orientation                        /// OpenVolumeMesh Hexahedron Orientation
            /// Botton Face {0,1,3,2} - Top face {4,5,7,6} ? :0~    /// Botton Face {0,1,2,3} - Top face {4,5,6,7}

            //	   6-------7                                        //	   5-------6
            //	  /|      /|										//	  /|      /|
            //	 / |     / |									    //	 / |     / |
            //	4--|----5  |                                        //	4--|----7  |
            //	|  2----|--3                                        //	|  3----|--2
            //	| /     | /                                         //	| /     | /
            //	0-------1                                           //	0-------1

            for (std::size_t i = 0; i < _cells.size(); i+=8)
            {
                std::vector<OpenVolumeMesh::VertexHandle> node = { v[_cells[i + 0]], v[_cells[i + 1]], v[_cells[i + 3]], v[_cells[i + 2]], v[_cells[i + 4]], v[_cells[i + 6]], v[_cells[i + 7]], v[_cells[i + 5]] };
                this->ptr_hexahedron_mesh_->add_cell(node);
            }

            /// Euler Charactristic for Volume Mesh
            int euler = (this->ptr_hexahedron_mesh_->n_vertices() - this->ptr_hexahedron_mesh_->n_edges() + this->ptr_hexahedron_mesh_->n_faces() - this->ptr_hexahedron_mesh_->n_cells());

//            std::cout << " === Mesh Hexahedron Information Shared Pointer ==== FlowModel.cpp" << std::endl;
//            std::cout << "Size : " << " Faces :" << this->ptr_hexahedron_mesh_->n_faces() << " | " << std::endl;
//            std::cout << "Size : " << " Edges :" << this->ptr_hexahedron_mesh_->n_edges() << " | " << std::endl;
//            std::cout << "Size : " << " Vertices :" << this->ptr_hexahedron_mesh_->n_vertices() << " | " << std::endl;
//            std::cout << "Size : " << " cells :" << this->ptr_hexahedron_mesh_->n_cells() << std::endl;
//            std::cout << "Euler Characteristics : V-E+F-C= " << euler << std::endl;
//            std::cout << " ========================= " << std::endl;

        }

        return true;
    }

    void FlowModel::loadMesh(std::shared_ptr<OpenVolumeMesh::TetrahedralMeshV3d> _ptr_mesh)
    {
        if (_ptr_mesh != nullptr)
        {
            _ptr_mesh->clear();
        }
    }

	void FlowModel::updateGeometry(std::vector<float>& vertices_array_, std::vector < unsigned int >& faces_array)
    {
        switch (this->mesh_type)
        {
        case TYPE::TETRAHEDRAL:
        {
			this->uploadTetrahedron(vertices_array_, faces_array);
            break;
        }

        case TYPE::HEXAHEDRAL:
        {
            this->uploadHexahedron(faces_array, vertices_array_);
            break;
        }
        case TYPE::QUADRILATERAL:
        {
            break;
        }

        default:
        {
            break;
        }
        }
    }
    //// Tetrahedron related funtion
    ///  Return the vertices normalized with the centre of the bounding box in (0,0,0)
	void FlowModel::uploadTetrahedron(std::vector<float>& vertices_array_, std::vector < unsigned int >& faces_array)
    {
        std::vector<OpenVolumeMesh::Vec3d> vd;

        if (this->ptr_tetrahedron_mesh_ != nullptr)
        {

            this->ptr_tetrahedron_mesh_->clone_vertices(vd);

            /// Construct a bounding box from the geomtry and use it to
            /// bring the model origin to the (0,0,0) and scale to 0-1.
            /// @TODO  transform this procedure into a function
            std::vector< Eigen::Vector4d > normalized_vertices;

            bounding_box_.reset();

            for (unsigned int it = 0; it < vd.size(); ++it)
            {
                normalized_vertices.push_back(Eigen::Vector4d(vd[it][0], vd[it][1], vd[it][2],1.0));
            }

            bounding_box_.fromPointCloud(normalized_vertices.begin(), normalized_vertices.end());

            double scale = 1.0 / (bounding_box_.diagonal()*0.25);

            Eigen::Affine3d t;
            Eigen::Affine3d s;

            t.setIdentity();
            s.setIdentity();

            t.translation() = -bounding_box_.center();
            s.scale(Eigen::Vector3d(scale, scale ,scale));

            model_matrix_ = s * t;

            for (unsigned int it = 0; it < normalized_vertices.size(); ++it)
            {
                normalized_vertices[it] = model_matrix_ * normalized_vertices[it];
            }

            bounding_box_.fromPointCloud(normalized_vertices.begin(), normalized_vertices.end());

            /// Swap back the transformed vertices
            for (std::size_t it = 0; it < normalized_vertices.size(); it++)
            {
                vd[it][0] = normalized_vertices[it].x();
                vd[it][1] = normalized_vertices[it].y();
                vd[it][2] = normalized_vertices[it].z();
            }

            this->ptr_tetrahedron_mesh_->swap_vertices(vd);

            OpenVolumeMesh::VertexHandle vh;

            faces_array.clear();
            vertices_array_.clear();

            /// For each cell , grab the information of each vertex
            for (OpenVolumeMesh::CellIter c_it = this->ptr_tetrahedron_mesh_->cells_begin(); c_it != this->ptr_tetrahedron_mesh_->cells_end(); c_it++)
            {
                // Get the face halfedges of face _fh
                std::vector<OpenVolumeMesh::HalfFaceHandle> halffaces = this->ptr_tetrahedron_mesh_->cell(*c_it).halffaces();

                for (const auto fh : halffaces)
                {
                    std::vector<OpenVolumeMesh::HalfEdgeHandle> halfedges = this->ptr_tetrahedron_mesh_->halfface(fh).halfedges();

                    for (const auto h : halfedges)
                    {
                        vh = this->ptr_tetrahedron_mesh_->halfedge(h).from_vertex();
                        faces_array.push_back(vh.idx());
                        vertices_array_.push_back(static_cast<float>(this->ptr_tetrahedron_mesh_->vertex(vh)[0]));
                        vertices_array_.push_back(static_cast<float>(this->ptr_tetrahedron_mesh_->vertex(vh)[1]));
                        vertices_array_.push_back(static_cast<float>(this->ptr_tetrahedron_mesh_->vertex(vh)[2]));
                    }
                }
            }
        }
    }

    std::shared_ptr<OpenVolumeMesh::TetrahedralMeshV3d> FlowModel::getPtrTetrahedralMesh()
    {
        return this->ptr_tetrahedron_mesh_;
    }

    /// Update Tetrahedon Mesh Colours
    void FlowModel::updateTetrahedronColors(const std::string& _property_name, const std::string& _entity_name, const std::string& _dimension, std::vector< double >& _values)
    {

        if (this->ptr_tetrahedron_mesh_)
        {
            if (_entity_name.compare("VProp") == 0)
            {
                if (_dimension.compare("double") == 0)
                {
                    updateTetrahedronVertexScalarProperty(_property_name, _values);
                }
                else if (_dimension.compare("Vector3D") == 0)
                {
                    std::cout << "Tetrahedron Mesh dimension not implemented " << _property_name << std::endl;
                }
                else
                {
                    std::cout << "Tetrahedron Mesh dimension not supported " << _property_name << std::endl;
                }
            }
            else  if (_entity_name.compare("CProp") == 0)
            {
                if (_dimension.compare("double") == 0)
                {
                    updateTetrahedronCellScalarProperty(_property_name, _values);
                }
                else if (_dimension.compare("Vector3D") == 0)
                {
                    std::cout << "Tetrahedron Mesh dimension not implemented " << _property_name << std::endl;
                }
                else
                {
                    std::cout << "Tetrahedron Mesh dimension not supported " << _property_name << std::endl;
                }
            }
            else
            {
                std::cout << "Tetrahedron Mesh entity not supported " << _property_name << std::endl;
            }

        }
        else
        {
            std::cout << "Tetrahedron Mesh not valid " << _property_name << std::endl;
        }
    }

    void FlowModel::updateTetrahedronVertexScalarProperty(const std::string& _property_name, std::vector< double >& _values)
    {
        /// Scalar Vertex Property

        _values.clear();

        if (this->ptr_tetrahedron_mesh_->vertex_property_exists<double>(_property_name))
        {
            OpenVolumeMesh::VertexPropertyT<double> vp = this->ptr_tetrahedron_mesh_->request_vertex_property<double>(_property_name);
            OpenVolumeMesh::VertexHandle vh;


            /// For each cell , grab the information of each vertex
            for (OpenVolumeMesh::CellIter c_it = this->ptr_tetrahedron_mesh_->cells_begin(); c_it != this->ptr_tetrahedron_mesh_->cells_end(); c_it++)
            {
                // Get the face halfedges of face _fh
                std::vector<OpenVolumeMesh::HalfFaceHandle> halffaces = this->ptr_tetrahedron_mesh_->cell(*c_it).halffaces();

                for (const auto fh : halffaces)
                {
                    std::vector<OpenVolumeMesh::HalfEdgeHandle> halfedges = this->ptr_tetrahedron_mesh_->halfface(fh).halfedges();

                    for (const auto h : halfedges)
                    {
                        vh = this->ptr_tetrahedron_mesh_->halfedge(h).from_vertex();
                        _values.push_back(vp[vh]);
                    }
                }
            }


        }
        else
        {
            std::cout << "Tetrahedron Vertex Property doesnt exist" << _property_name << std::endl;
        }

    }

    void FlowModel::updateTetrahedronVertexVectorProperty(const std::string& _property_name, std::vector< double >& _values)
    {

    }

    void FlowModel::updateTetrahedronCellScalarProperty(const std::string& _property_name, std::vector< double >& _values)
    {

        _values.clear();

        if (this->ptr_tetrahedron_mesh_->cell_property_exists<double>(_property_name))
        {
            /// Construct a bounding box from the geomtry and use it to
            /// bring the model origin to the (0,0,0) and scale to 0-1.
            /// @TODO  transform this procedure into a function

            OpenVolumeMesh::CellPropertyT<double> cp = this->ptr_tetrahedron_mesh_->request_cell_property<double>(_property_name);
            OpenVolumeMesh::VertexHandle vh;

            for (OpenVolumeMesh::CellIter c_it = this->ptr_tetrahedron_mesh_->cells_begin(); c_it != this->ptr_tetrahedron_mesh_->cells_end(); c_it++)
            {
                // Get the face halfedges of face _fh
                std::vector<OpenVolumeMesh::HalfFaceHandle> halffaces = this->ptr_tetrahedron_mesh_->cell(*c_it).halffaces();

                for (const auto fh : halffaces)
                {
                    std::vector<OpenVolumeMesh::HalfEdgeHandle> halfedges = this->ptr_tetrahedron_mesh_->halfface(fh).halfedges();

                    for (const auto h : halfedges)
                    {
                        _values.push_back(static_cast<float>(cp[*c_it]));
                    }
                }
            }

        }

    }

    void FlowModel::updateTetrahedronCellVectorProperty(const std::string& _property_name, std::vector< double >& _values)
    {

    }
    /// Hexahedron releated function
    std::shared_ptr<OpenVolumeMesh::HexahedralMesh3d>  FlowModel::getPtrHexahedralMesh()
    {
        return this->ptr_hexahedron_mesh_;
    }

    void FlowModel::uploadHexahedron(std::vector<unsigned int>& faces_array, std::vector<float>& vertices_array_)
    {
        /// OpenVolumeMesh Hexahedron Orientation
        /// Botton Face {0,1,2,3} - Top face {4,5,6,7}

        //	   5-------6
        //	  /|      /|
        //	 / |     / |
        //	4--|----7  |
        //	|  3----|--2
        //	| /     | /
        //	0-------1

        std::vector<OpenVolumeMesh::Vec3d> vd;

        if (ptr_hexahedron_mesh_ != nullptr)
        {
            this->ptr_hexahedron_mesh_->clone_vertices(vd);

            /// Construct a bounding box from the geomtry and use it to
            /// bring the model origin to the (0,0,0) and scale to 0-1.
            /// @TODO  transform this procedure into a function
            std::vector< Eigen::Vector4d > normalized_vertices;

            bounding_box_.reset();

            for (unsigned int it = 0; it < vd.size(); ++it)
            {
                normalized_vertices.push_back(Eigen::Vector4d(vd[it][0], vd[it][1], vd[it][2], 1.0));
            }

            bounding_box_.fromPointCloud(normalized_vertices.begin(), normalized_vertices.end());

            double scale = 1.0 / (bounding_box_.diagonal()*0.25);

            Eigen::Affine3d t;
            Eigen::Affine3d s;

            t.setIdentity();
            s.setIdentity();

            t.translation() = -bounding_box_.center();
            s.scale(Eigen::Vector3d(scale, scale, scale));

            model_matrix_ = s * t;

            for (unsigned int it = 0; it < normalized_vertices.size(); ++it)
            {
                normalized_vertices[it] = model_matrix_ * normalized_vertices[it];
            }

            bounding_box_.fromPointCloud(normalized_vertices.begin(), normalized_vertices.end());

            /// Swap back the transformed vertices
            for (std::size_t it = 0; it < normalized_vertices.size(); it++)
            {
                vd[it][0] = normalized_vertices[it].x();
                vd[it][1] = normalized_vertices[it].y();
                vd[it][2] = normalized_vertices[it].z();
            }


            this->ptr_hexahedron_mesh_->swap_vertices(vd);

            OpenVolumeMesh::VertexHandle vh;

            faces_array.clear();
            vertices_array_.clear();

            for (OpenVolumeMesh::CellIter c_it = this->ptr_hexahedron_mesh_->cells_begin(); c_it != this->ptr_hexahedron_mesh_->cells_end(); c_it++)
            {
                // Get the cell's halffaces c_it
                std::vector<OpenVolumeMesh::HalfFaceHandle> halffaces = this->ptr_hexahedron_mesh_->cell(*c_it).halffaces();

                for (const auto fh : halffaces)
                {
                    // Get the face halfedges of face _fh
                    std::vector<OpenVolumeMesh::HalfEdgeHandle> halfedges = this->ptr_hexahedron_mesh_->halfface(fh).halfedges();

                    // Hexahedron Mesh
                    // Draw GL_LINES_ADJACENCY -> Triangle strip
                    //   2*-->-*3
                    //   | *    |
                    //   |   *  |
                    //   0*-->-*1

                    vh = this->ptr_hexahedron_mesh_->halfedge(halfedges[0]).from_vertex();
                    faces_array.push_back(vh.idx());
                    vertices_array_.push_back(static_cast<float>(this->ptr_hexahedron_mesh_->vertex(vh)[0]));
                    vertices_array_.push_back(static_cast<float>(this->ptr_hexahedron_mesh_->vertex(vh)[1]));
                    vertices_array_.push_back(static_cast<float>(this->ptr_hexahedron_mesh_->vertex(vh)[2]));

                    vh = this->ptr_hexahedron_mesh_->halfedge(halfedges[1]).from_vertex();
                    faces_array.push_back(vh.idx());
                    vertices_array_.push_back(static_cast<float>(this->ptr_hexahedron_mesh_->vertex(vh)[0]));
                    vertices_array_.push_back(static_cast<float>(this->ptr_hexahedron_mesh_->vertex(vh)[1]));
                    vertices_array_.push_back(static_cast<float>(this->ptr_hexahedron_mesh_->vertex(vh)[2]));

                    vh = this->ptr_hexahedron_mesh_->halfedge(halfedges[3]).from_vertex();
                    faces_array.push_back(vh.idx());
                    vertices_array_.push_back(static_cast<float>(this->ptr_hexahedron_mesh_->vertex(vh)[0]));
                    vertices_array_.push_back(static_cast<float>(this->ptr_hexahedron_mesh_->vertex(vh)[1]));
                    vertices_array_.push_back(static_cast<float>(this->ptr_hexahedron_mesh_->vertex(vh)[2]));

                    vh = this->ptr_hexahedron_mesh_->halfedge(halfedges[2]).from_vertex();
                    faces_array.push_back(vh.idx());
                    vertices_array_.push_back(static_cast<float>(this->ptr_hexahedron_mesh_->vertex(vh)[0]));
                    vertices_array_.push_back(static_cast<float>(this->ptr_hexahedron_mesh_->vertex(vh)[1]));
                    vertices_array_.push_back(static_cast<float>(this->ptr_hexahedron_mesh_->vertex(vh)[2]));

                }

            }

        }

    }
    /// Update Hexahedron Mesh Colours
    void FlowModel::updateHexahedronColors(const std::string& _property_name, const std::string& _entity_name, const std::string& _dimension, std::vector< double >& _values)
    {
        if (this->ptr_hexahedron_mesh_)
        {
            if (_entity_name.compare("VProp") == 0)
            {
                if (_dimension.compare("double") == 0)
                {
                    updateHexahedronVertexScalarProperty(_property_name, _values);
                }
                else if (_dimension.compare("Vector3D") == 0)
                {
                    std::cout << "Hexahedron Mesh dimension not implemented " << _property_name << std::endl;
                }
                else
                {
                    std::cout << "Hexahedron Mesh dimension not supported " << _property_name << std::endl;
                }
            }
            else  if (_entity_name.compare("CProp") == 0)
            {
                if (_dimension.compare("double") == 0)
                {
                    updateHexahedronCellScalarProperty(_property_name, _values);
                }
                else if (_dimension.compare("Vector3D") == 0)
                {
                    std::cout << "Hexahedron Mesh dimension not implemented " << _property_name << std::endl;
                }
                else
                {
                    std::cout << "Hexahedron Mesh dimension not supported " << _property_name << std::endl;
                }
            }
            else
            {
                std::cout << "Hexahedron Mesh entity not supported " << _property_name << std::endl;
            }

        }
        else
        {
            std::cout << "Hexahedron Mesh not valid " << _property_name << std::endl;
        }
    }

    void FlowModel::updateHexahedronVertexScalarProperty(const std::string& _property_name, std::vector< double >& _values)
    {
        /// Scalar Vertex Property
        _values.clear();

        if (this->ptr_hexahedron_mesh_->vertex_property_exists<double>(_property_name))
        {
            OpenVolumeMesh::VertexPropertyT<double> vp = this->ptr_hexahedron_mesh_->request_vertex_property<double>(_property_name);
            OpenVolumeMesh::VertexHandle vh;

            /// For each cell , grab the information of each vertex
            for (OpenVolumeMesh::CellIter c_it = this->ptr_hexahedron_mesh_->cells_begin(); c_it != this->ptr_hexahedron_mesh_->cells_end(); c_it++)
            {
                // Get the cell's halffaces c_it
                std::vector<OpenVolumeMesh::HalfFaceHandle> halffaces = this->ptr_hexahedron_mesh_->cell(*c_it).halffaces();

                for (const auto fh : halffaces)
                {

                    // Get the face halfedges of face _fh
                    std::vector<OpenVolumeMesh::HalfEdgeHandle> halfedges = this->ptr_hexahedron_mesh_->halfface(fh).halfedges();

                    // Hexahedron Mesh
                    // Draw GL_LINES_ADJACENCY -> Triangle strip
                    //   2*-->-*3
                    //   | *    |
                    //   |   *  |
                    //   0*-->-*1

                    vh = this->ptr_hexahedron_mesh_->halfedge(halfedges[0]).from_vertex();
                    _values.push_back(vp[vh]);

                    vh = this->ptr_hexahedron_mesh_->halfedge(halfedges[1]).from_vertex();
                    _values.push_back(vp[vh]);

                    vh = this->ptr_hexahedron_mesh_->halfedge(halfedges[3]).from_vertex();
                    _values.push_back(vp[vh]);

                    vh = this->ptr_hexahedron_mesh_->halfedge(halfedges[2]).from_vertex();
                    _values.push_back(vp[vh]);

                }

            }
        }
        else
        {
            std::cout << "Hexahedron Vertex Property doesnt exist" << _property_name << std::endl;
        }

    }

    void FlowModel::updateHexahedronVertexVectorProperty(const std::string& _property_name, std::vector< double >& _values)
    {

    }

    void FlowModel::updateHexahedronCellScalarProperty(const std::string& _property_name, std::vector< double >& _values)
    {
        /// Scalar Vertex Property
        _values.clear();

        if (this->ptr_hexahedron_mesh_->cell_property_exists<double>(_property_name))
        {
            /// Construct a bounding box from the geomtry and use it to
            /// bring the model origin to the (0,0,0) and scale to 0-1.
            /// @TODO  transform this procedure into a function

            OpenVolumeMesh::CellPropertyT<double> cp = this->ptr_hexahedron_mesh_->request_cell_property<double>(_property_name);
            OpenVolumeMesh::VertexHandle vh;

            for (OpenVolumeMesh::CellIter c_it = this->ptr_hexahedron_mesh_->cells_begin(); c_it != this->ptr_hexahedron_mesh_->cells_end(); c_it++)
            {
                // Get the cell's halffaces c_it
                std::vector<OpenVolumeMesh::HalfFaceHandle> halffaces = this->ptr_hexahedron_mesh_->cell(*c_it).halffaces();

                for (const auto fh : halffaces)
                {

                    // Get the face halfedges of face _fh
                    std::vector<OpenVolumeMesh::HalfEdgeHandle> halfedges = this->ptr_hexahedron_mesh_->halfface(fh).halfedges();

                    // Hexahedron Mesh
                    // Draw GL_LINES_ADJACENCY -> Triangle strip
                    //   2*-->-*3
                    //   | *    |
                    //   |   *  |
                    //   0*-->-*1

                    vh = this->ptr_hexahedron_mesh_->halfedge(halfedges[0]).from_vertex();
                    _values.push_back(cp[*c_it]);

                    vh = this->ptr_hexahedron_mesh_->halfedge(halfedges[1]).from_vertex();
                    _values.push_back(cp[*c_it]);

                    vh = this->ptr_hexahedron_mesh_->halfedge(halfedges[3]).from_vertex();
                    _values.push_back(cp[*c_it]);

                    vh = this->ptr_hexahedron_mesh_->halfedge(halfedges[2]).from_vertex();
                    _values.push_back(cp[*c_it]);

                }

            }

        }
        else
        {
            std::cout << "Hexahedron Vertex Property doesnt exist" << _property_name << std::endl;
        }

    }

    void FlowModel::updateHexahedronCellVectorProperty(const std::string& _property_name, std::vector< double >& _values)
    {

    }

    /// @TODO Later, move this fucntions to FlowvizualizationController
    Eigen::Affine3d FlowModel::getModelMatrix() const
    {
        return this->model_matrix_;
    }

	void FlowModel::uploadTetrahedron(const std::vector<float>&         _vertices,       /// Lisf of all vertices of the tetrahedon mesh
		                              const std::vector<unsigned int>&  _cells,			 /// List of all cells of the tetrahedron mesh. List of cells as a vertice list:
																						 /// Each four elements as indices to the list of vertices
								      std::vector<float>&               vertices_array_, /// To OpenGL buffers
									  std::vector < unsigned int >&     faces_array_	 /// To OpenGL buffers
									  )  
	{

		/// The 3D Tetrahedron Mesh come from Tetget.

		// @see  TETGEN DOCUMENTATION at http://wias-berlin.de/software/tetgen/1.5/doc/manual/manual006.html
		// 5.2.4  .ele files


		// An.ele file contains a list of tetrahedra.

		//	First line : <# of tetrahedra> <nodes per tet. (4 or 10)>
		//	<region attribute(0 or 1)>
		//	Remaining lines list # of tetrahedra :
		//  <tetrahedron #> <node> <node> ... <node>[attribute]
		//	...
		//	Each tetrahedron has four corners(or ten corners if the - o2 switch is used).
		//  Nodes are indices into the corresponding.node file.
		//  The first four nodes are the corner vertices.If - o2 switch is used, the remaining six nodes are generated on the midpoints of the edges of the tetrahedron.
		//  Figure 20 shows how these corners and the second - order nodes are locally numbered.Second order nodes are output only.They are omitted by the mesh reconstruction(the - r switch).
		//
		//                       3
		//                       /\
		//                      /  \
		//                     /    \
		//                    /      \
		//                   /        \
		//                0 /__________\ 1
		//                 / \        / \
		//                /   \      /   \
		//               /     \    /     \
		//              /       \  /       \
		//           3 /_________\/_________\ 3
		//            *          2           *

		/// From Flow Diagnostic Interface

		/// Each Cell has four vertices, {v0, v1, v2, v3 } and 4 Faces.
		/// Each Face has 3 vertice as listed bellow:
		/// [v0,v1,v2], 
		//  [v0,v2,v3], 
		/// [v2,v1,v3], 
		/// [v0,v3,v1]
		/// The vertices geometry are:
		/// [v0.x,v0.y,v0.z; v1.x,v1.y,v1.z; v2.x,v2.y,v2.z]  9 floats
		/// [v0.x,v0.y,v0.z; v2.x,v2.y,v2.z; v3.x,v3.y,v3.z]  9 floats
		/// [v2.x,v2.y,v2.z; v1.x,v1.y,v1.z; v3.x,v3.y,v3.z]  9 floats
		/// [v0.x,v0.y,v0.z; v3.x,v3.y,v3.z; v1.x,v1.y,v1.z]  9 floats


		/// Construct a bounding box from the geomtry and use it to
		/// bring the model origin to the (0,0,0) and scale to 0-1.
		/// @TODO  transform this procedure into a function
		std::vector< Eigen::Vector4d > normalized_vertices;

		bounding_box_.reset();

		for (unsigned int it = 0; it < _vertices.size(); it += 3)
		{
			normalized_vertices.push_back(Eigen::Vector4d(_vertices[it + 0], _vertices[it + 1], _vertices[it + 2], 1.0));
		}

		bounding_box_.fromPointCloud(normalized_vertices.begin(), normalized_vertices.end());

		double scale = 1.0 / (bounding_box_.diagonal()*0.25);

		Eigen::Affine3d t;
		Eigen::Affine3d s;

		t.setIdentity();
		s.setIdentity();

		t.translation() = -bounding_box_.center();
		s.scale(Eigen::Vector3d(scale, scale, scale));

		model_matrix_ = s * t;

		for (unsigned int it = 0; it < normalized_vertices.size(); ++it)
		{
			normalized_vertices[it] = model_matrix_ * normalized_vertices[it];
		}

		//bounding_box_.fromPointCloud(normalized_vertices.begin(), normalized_vertices.end());

		/// Swap back the transformed vertices
		std::vector<float> normalized;
		normalized.resize(_vertices.size());
		for (std::size_t it = 0; it < normalized_vertices.size(); it++)
		{
			normalized[3 * it + 0] = normalized_vertices[it].x();
			normalized[3 * it + 1] = normalized_vertices[it].y();
			normalized[3 * it + 2] = normalized_vertices[it].z();
		}


		std::size_t  number_of_cells = static_cast<int>( _cells.size() / 4.0f);

		faces_array_.clear();
		/// The number of faces  = 4 * cells
		//faces_array_.resize(number_of_cells * 12);
		/// The number of veritces  =  12 * number of cells 
		vertices_array_.clear();
		//vertices_array_.resize(number_of_cells * 36);

		std::size_t face_index = 0;

		/// Looping over the cells. Each cell 4 faces: Eeach Face 3 vertices
		for (std::size_t index = 0; index < _cells.size(); index += 4)
		{
			/// [v0,v1,v2] = 9 floats			
			/// Face 1			
			// v0
			vertices_array_.push_back(normalized_vertices[_cells[index+0]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index+0]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index+0]].z());
			faces_array_.push_back(index + 0);
			
			// v1
			vertices_array_.push_back(normalized_vertices[_cells[index+1]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index+1]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index+1]].z());
			faces_array_.push_back(index + 1);
			
			// v2
			vertices_array_.push_back(normalized_vertices[_cells[index + 2]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 2]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 2]].z());
			faces_array_.push_back(index + 2);

			/// [v0,v2,v3] 
			/// Face 2			
			// v0
			vertices_array_.push_back(normalized_vertices[_cells[index + 0]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 0]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 0]].z());
			faces_array_.push_back(index + 0);

			// v2
			vertices_array_.push_back(normalized_vertices[_cells[index + 2]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 2]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 2]].z());
			faces_array_.push_back(index + 2);

			// v3
			vertices_array_.push_back(normalized_vertices[_cells[index + 3]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 3]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 3]].z());
			faces_array_.push_back(index + 3);

			/// [v2,v1,v3] 
			/// Face 3
			// v2
			vertices_array_.push_back(normalized_vertices[_cells[index + 2]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 2]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 2]].z());
			faces_array_.push_back(index + 2);

			// v1
			vertices_array_.push_back(normalized_vertices[_cells[index + 1]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 1]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 1]].z());
			faces_array_.push_back(index + 1);

			// v2
			vertices_array_.push_back(normalized_vertices[_cells[index + 3]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 3]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 3]].z());
			faces_array_.push_back(index + 3);

			/// [v0,v3,v1]
			/// Face 4			
			// v0
			vertices_array_.push_back(normalized_vertices[_cells[index + 0]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 0]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 0]].z());
			faces_array_.push_back(index + 0);

			// v1
			vertices_array_.push_back(normalized_vertices[_cells[index + 3]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 3]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 3]].z());
			faces_array_.push_back(index + 3);

			// v2
			vertices_array_.push_back(normalized_vertices[_cells[index + 1]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 1]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 1]].z());
			faces_array_.push_back(index + 1);
		}

	}


} /* namespace RRM */



