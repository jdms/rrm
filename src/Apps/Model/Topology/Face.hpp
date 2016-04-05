/*
 * Face.hpp
 *
 *  Created on: Apr 5, 2016
 *      Author: felipe
 */

#ifndef SRC_APPS_MODEL_TOPOLOGY_FACE_HPP_
#define SRC_APPS_MODEL_TOPOLOGY_FACE_HPP_

namespace RRM
{

	template < class Real>
	class Face
	{
		public:
			Face ( )
			{
				id_ = 0;
			}
			virtual ~Face ( )
			{

			}
		public:
			unsigned int id_;
	};

} /* namespace RRM */

#endif /* SRC_APPS_MODEL_TOPOLOGY_FACE_HPP_ */
