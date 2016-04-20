/*
 * SeismicSlice.hpp
 *
 *  Created on: Apr 14, 2016
 *      Author: felipe
 */

#ifndef _SBIM_SEISMICSLICE_HPP_
#define _SBIM_SEISMICSLICE_HPP_

#include "Model/CrossSection.hpp"

namespace RRM
{
	template < class Real >
	class SeismicSlice: public RRM::CrossSection<Real>
	{
		public:

			typedef RRM::CrossSection<Real>		Base;
			typedef SeismicSlice<Real>		Self;

			SeismicSlice ( ) : Base()
			{
				this->log ( );
				image_ = 0;
			}
			~SeismicSlice ( )
			{

			}

			// need to be tested, since this class will be stored in a stl::vector
			// @see http://stackoverflow.com/questions/17311382/how-to-call-base-class-copy-constructor-from-a-derived-class-copy-constructor
			SeismicSlice ( const Self& other ) : Base(other)
			{
				*this = other;
			}

			Self& operator=(const Self& other )
			{
				// @see http://stackoverflow.com/a/1226957
				Base::operator =(other);
				// Assign to all  member.
				this->image_ = other.image_;

				return *this;
			}



		public:

			unsigned int image_;

	};

} /* namespace RRM */

#endif /* _SBIM_SEISMICSLICE_HPP_ */
