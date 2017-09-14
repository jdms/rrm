/*
 * MeshGenerator.cpp
 *
 *  Created on: Feb 21, 2017
 *      Author: felipe
 */

#include "mesh_generator.hpp"


    void MeshGenerator::rotate2Small(OpenVolumeMesh::FaceTuple& _t)
    {
        std::vector<int> c = { std::get<0>(_t), std::get<1>(_t), std::get<2>(_t) };

        std::vector<int>::iterator it;
        /// Find the smallest vertex
        it = std::min_element(c.begin(), c.end());

        std::vector<int> temp;
        /// Reordering
        temp = std::vector<int>(it, c.end());
        std::copy(c.begin(), it, back_inserter(temp));

        c = temp;
		_t = std::make_tuple(vertices_[c[0]], vertices_[c[1]], vertices_[c[2]]);
    }

	OpenVolumeMesh::Vec3d MeshGenerator::centroid(const std::vector<unsigned int>& _idx)
    {
        // Get cell's mid-point
		OpenVolumeMesh::Vec3d centroid_(0.0, 0.0, 0.0);
        double valence = 0.0;
		for (std::vector<unsigned int>::const_iterator it = _idx.begin(); it != _idx.end(); ++it)
        {
			centroid_ += this->mesh_.vertex(this->vertices_[*it]);
            valence += 1.0;
        }
        return (centroid_ /= valence);
    }

	void MeshGenerator::addVertex(const OpenVolumeMesh::Vec3d& vertex)
	{
			OpenVolumeMesh::VertexHandle vh = mesh_.add_vertex(vertex);
			this->vertices_.push_back(vh);
	}

	void MeshGenerator::addCell(std::vector<unsigned int>& _idx)
    {
		if ((_idx.size() > 8) || ((_idx.size() == 0)))
        {
            std::cerr << "The specified cell is not allow!" << std::endl;
			return;
        }
		else if (this->vertices_.size() == 0)
		{
			std::cerr << "Add Vertices first " << std::endl;
			return;
		}

		std::vector<OpenVolumeMesh::FaceTuple> tuples;

		std::sort(_idx.begin(), _idx.end());

		if (_idx.size() == 4)
        {
            // Create face tuple for all vertex combinations
			tuples.push_back(OpenVolumeMesh::FaceTuple(vertices_[_idx[0]], vertices_[_idx[1]], vertices_[_idx[2]]));
			tuples.push_back(OpenVolumeMesh::FaceTuple(vertices_[_idx[1]], vertices_[_idx[2]], vertices_[_idx[3]]));
			tuples.push_back(OpenVolumeMesh::FaceTuple(vertices_[_idx[0]], vertices_[_idx[2]], vertices_[_idx[3]]));
			tuples.push_back(OpenVolumeMesh::FaceTuple(vertices_[_idx[0]], vertices_[_idx[1]], vertices_[_idx[3]]));
        }
		else if (_idx.size() == 8)
        {
            // TODO Auto-generated constructor stub
            return;
        }

        // Collect cell's half-faces in here
        std::vector<OpenVolumeMesh::HalfFaceHandle> cell_halffaces;

        // Get cell's mid-point
		OpenVolumeMesh::Vec3d centroid_ = this->centroid(_idx);

		for (std::vector<OpenVolumeMesh::FaceTuple>::const_iterator it = tuples.begin(); it != tuples.end(); ++it)
        {

            // Check if face exists for current tuple
			OpenVolumeMesh::FaceMap::iterator f = face_map_.find(*it);
            if (f == face_map_.end())
            {
                // Face does not exist, create it

                // Find right orientation, s.t. normal
                // points inside the cell

				OpenVolumeMesh::Vec3d e1 = this->mesh_.vertex(std::get<1>(*it)) - this->mesh_.vertex(std::get<0>(*it));
				OpenVolumeMesh::Vec3d e2 = this->mesh_.vertex(std::get<2>(*it)) - this->mesh_.vertex(std::get<1>(*it));

                // Get face normal (cross product)
				OpenVolumeMesh::Vec3d n = (e1 % e2).normalize();

                std::vector<OpenVolumeMesh::VertexHandle> v_vec;
                v_vec.push_back(std::get<0>(*it));
                v_vec.push_back(std::get<1>(*it));
                v_vec.push_back(std::get<2>(*it));
                OpenVolumeMesh::FaceHandle fh = this->mesh_.add_face(v_vec);

                // Add face to face map
                face_map_[*it] = fh;

                // Check whether normal points inside cell
                if (((centroid_ - this->mesh_.vertex(std::get<0>(*it))) | n) > 0.0) {

                    // Normal points inside cell, just add half-face 0
                    // Add corresponding half-face to cell definition
                    cell_halffaces.push_back(this->mesh_.halfface_handle(fh, 0));

                }
                else {

                    // Normal points outside cell, just add half-face 1
                    // Add corresponding half-face to cell definition
                    cell_halffaces.push_back(this->mesh_.halfface_handle(fh, 1));
                }

            }
            else
            {               
                // Face exists, find right orientation
                OpenVolumeMesh::FaceHandle fh = f->second;

                std::vector<OpenVolumeMesh::HalfEdgeHandle> hes = this->mesh_.face(fh).halfedges();

                //assert(hes.size() == 3);

				OpenVolumeMesh::Vec3d e1 = this->mesh_.vertex(this->mesh_.halfedge(hes[0]).to_vertex()) -
                    this->mesh_.vertex(this->mesh_.halfedge(hes[0]).from_vertex());
				OpenVolumeMesh::Vec3d e2 = this->mesh_.vertex(this->mesh_.halfedge(hes[1]).to_vertex()) -
                    this->mesh_.vertex(this->mesh_.halfedge(hes[1]).from_vertex());

				OpenVolumeMesh::Vec3d n = (e1 % e2).normalize();

                if (((centroid_ - this->mesh_.vertex(this->mesh_.halfedge(hes[0]).from_vertex())) | n) > 0.0) {
                    // Normal points inside cell
                    cell_halffaces.push_back(this->mesh_.halfface_handle(fh, 0));
                }
                else {
                    // Normal points outisde cell
                    cell_halffaces.push_back(this->mesh_.halfface_handle(fh, 1));
                }
            }
        }

        // Check whether cell definition contains four half-faces
        //assert(cell_halffaces.size() == 4);

        // Finally, add cell
        this->mesh_.add_cell(cell_halffaces, true);

    }


