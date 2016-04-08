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

			typedef Face<Real>		Self;

			Face ( )
			{
				id_ = 0;
			}

			Face ( const Self& _face)
			{
				*this = _face;
			}

			~Face ( )
			{

			}

			Self& operator=(const Self& other )
			{
				// Assign to all  member.
				id_ = other.id_;

				return *this;
			}

		public:
			unsigned int id_;
	};

} /* namespace RRM */

#endif /* SRC_APPS_MODEL_TOPOLOGY_FACE_HPP_ */
