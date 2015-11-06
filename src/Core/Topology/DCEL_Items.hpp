#ifndef _CORE_TOPOLOGY_DCEL_ITEMS_HPP_
#define _CORE_TOPOLOGY_DCEL_ITEMS_HPP_

#include <CGAL/HalfedgeDS_min_items.h>

#include "Halfedge.hpp"
#include "Vertex.hpp"
#include "Face.hpp"

struct RRM_Items: public CGAL::HalfedgeDS_min_items {
	template<class Refs, class Traits>
	struct Vertex_wrapper {
		typedef RRM::Topology::Vertex<Refs, Traits> Vertex;
	};

	template<class Refs, class Traits>
	struct Halfedge_wrapper {
		typedef RRM::Topology::Halfedge<Refs, Traits> Halfedge;
	};

	template<class Refs, class Traits>
	struct Face_wrapper {
		typedef RRM::Topology::Face<Refs, Traits> Face;
	};
};


#endif /* _CORE_TOPOLOGY_VERTEX_HPP_ */
