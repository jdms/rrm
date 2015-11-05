/*
 * CurveEngine.hpp
 *
 *  Created on: Sep 28, 2015
 *      Author: felipe
 */

#ifndef _MODEL_CURVEENGINE_HPP_
#define _MODEL_CURVEENGINE_HPP_

#include "Core/Geometry/PolygonalCurve2D.hpp"

namespace RRM
{
	/// FIXME Should be an Interface ...
	template< class Real >
	class CurveEngine
	{
		public:
			typedef PolygonalCurve2D<Real> 		Curve2D;
			typedef typename Curve2D::Point2D	Point2D;

			CurveEngine ( );
			virtual ~CurveEngine ( );

			bool linearFilter( Curve2D& curve, Real step,  unsigned int interactions )
			{
				curve.linearFilter(step, interactions);

				return true;
			}
	};

} /* namespace RRM */

#endif /* _MODEL_CURVEENGINE_HPP_ */
