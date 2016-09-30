/*
 * Constants.cpp
 *
 *  Created on: Aug 23, 2015
 *      Author: felipe
 */

#include "Constants.hpp"

namespace Math {

/// TODO inveser sqrt inverse 3

const double Math::Constants::Pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164;
const double Math::Constants::HalfPi = Math::Constants::Pi / 2.0;

const double Math::Constants::SqrtTwo = 1.4142135623730950488016887242096980785696718753769480731766797379907324784621;
const double Math::Constants::InverseSqrtTwo = 0.70710678118654752440084436210484903928483593756084;

const double Math::Constants::SqrtThree = 1.7320508075688772935274463415058723669428052538103806280558069794519330169088;
const double Math::Constants::InverseSqrtThree = 0.57735026918962576450;
const double Math::Constants::InverseThree = 0.333333333333333333333333333333333333333333333333;

const double Math::Constants::Degree2Radian = Math::Constants::Pi / 180.0;
const double Math::Constants::Radian2Degree = 180.0 / Math::Constants::Pi;

const double Math::Constants::Infinty = std::numeric_limits<double>::max();
const double Math::Constants::Epsilon = std::numeric_limits<double>::epsilon();

}
