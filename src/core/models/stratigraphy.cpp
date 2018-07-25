#include "stratigraphy.h"

#include <iostream>
#include <vector>


Stratigraphy::Stratigraphy()
{
	defineId();
    setVisible( false );
	std::cout << "STRATIGRAPHY CREATED" << std::endl << std::flush;
}

Stratigraphy::Stratigraphy(const Stratigraphy& strat_)
{
//	this->surface = strat_.surface;
//	this->curves = strat_.curves;
//	this->trajectory = strat_.trajectory;

	std::cout << "STRATIGRAPHY CREATED" << std::endl << std::flush;

}


Stratigraphy& Stratigraphy::operator=(const Stratigraphy& strat_)
{
//	this->surface = strat_.surface;
//	this->curves = strat_.curves;
//	this->trajectory = strat_.trajectory;

	return *this;
}


void Stratigraphy::setIndex(std::size_t id_ )
{
	index = id_;
	number_of_stratigraphies = id_ + 1;
}

void Stratigraphy::defineId()
{
	index = number_of_stratigraphies;
	++number_of_stratigraphies;
}

void Stratigraphy::clear()
{
}


//void Stratigraphy::setSurface( const Surface& surface_)
//{
////	surface = surface_;
//}

//const Surface& Stratigraphy::getSurface() const
//{
//	return surface;
//}

//bool Stratigraphy::addCurve( double csection_, const PolyCurve& curve_ )
//{
//    if ( curves.find(csection_) != curves.end( )) return false;

//    std::cout << "Adding curve in stratigraphy" << std::endl << std::flush;
//    curves[csection_] = curve_;

//    return true;
//}

//const PolyCurve& Stratigraphy::getCurve(double csection_) const
//{
//    if (curves.find(csection_) == curves.end()) return PolyCurve();
//    return curves.at( csection_ );
//}

//void Stratigraphy::removeCurve( double csection_)
//{
//    if(curves.find(csection_) == curves.end()) return;
//    std::cout << "Removing curve in stratigraphy" << std::endl << std::flush;
//    curves.erase(csection_);
//}

//std::map<double, PolyCurve> Stratigraphy::getCurves() const
//{
//    return curves;
//}

//void Stratigraphy::removeCurves()
//{
//    std::map< double, PolyCurve >::iterator it = curves.begin();

//    while (it != curves.end())
//    {
//        const double c = it->first;
//        std::cout << "Removing " << c << std::endl << std::flush;

//        removeCurve( c );
//        curves.erase( c );

//        it = curves.begin();
//    }

//}

//void Stratigraphy::addTrajectory( const PolyCurve& traj_)
//{
//    std::cout << "Adding trajectory in stratigraphy" << std::endl << std::flush;
//    trajectory = traj_;
//}


//void Stratigraphy::removeTrajectory()
//{
//    std::cout << "Removing trajectory in stratigraphy" << std::endl << std::flush;
//    trajectory.clear();
//}


//bool Stratigraphy::getTrajectory( PolyCurve& traj_ ) const
//{
//    traj_ = trajectory;
//    return false;
//}

//void Stratigraphy::surfaceDone()
//{
//    removeCurves();
//    removeTrajectory();

////    surface = 1;
//	setEditable( false );
//}


Stratigraphy::~Stratigraphy()
{
//    removeCurves();
//    removeTrajectory();

	std::cout << "STATIGRAPHY DELETED" << std::endl << std::flush;
    clear();
}
