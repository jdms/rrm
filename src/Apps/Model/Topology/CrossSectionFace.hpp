/*
 * CrossSectionFace.hpp
 *
 *  Created on: Sep 24, 2015
 *      Author: felipe
 */

#ifndef _MODEL_CROSSSECTIONFACE_HPP_
#define _MODEL_CROSSSECTIONFACE_HPP_

#include <CGAL/HalfedgeDS_face_base.h>
#include <CGAL/Polygon_2.h>

namespace RRM
{

	template < class Refs , class Traits >
	class CrossSectionFace: public CGAL::HalfedgeDS_face_base<Refs, CGAL::Tag_true>
	{
		public:
			CrossSectionFace ( )
			{
				name = std::string("nameless");
			}
			virtual ~CrossSectionFace ( )
			{
			}

			std::string name;
	};

} /* namespace RRM */

#endif /* _MODEL_CROSSSECTIONFACE_HPP_ */
