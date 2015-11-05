/*
 * Face.hpp
 *
 *  Created on: Aug 21, 2015
 *      Author: felipe
 */

#ifndef _CORE_TOPOLOGY_FACE_HPP_
#define _CORE_TOPOLOGY_FACE_HPP_

#include <utility> // int

/// FIXME Implement Traits to allow customization of the class.

#include <CGAL/HalfedgeDS_face_base.h>

#include <list>

namespace RRM
{
	namespace Topology
	{
		/// @see http://doc.cgal.org/4.7/HalfedgeDS/classCGAL_1_1HalfedgeDS__face__base.html
		template <class Refs, class Traits>
		class Face : public CGAL::HalfedgeDS_face_base<Refs,CGAL::Tag_true>
		{
			public:
				Face( ){

				}

				~Face() {

				}

			protected:

			private:

		};
	} /* namespace Topology */
} /* namespace RRM */


#endif /* _CORE_TOPOLOGY_FACE_HPP_ */
