#pragma once
#include "object.h"

#include <memory>
#include <string>
#include <map>


class Stratigraphy : public Object
{
	public:

		static unsigned int number_of_stratigraphies;

		Stratigraphy();
		Stratigraphy(const Stratigraphy & strat_);
		~Stratigraphy();

		Stratigraphy & operator=(const Stratigraphy & strat_);

        void setIndex(std::size_t id_ ) override;

        void setSurface( const Surface& surface_) ;
        const Surface& getSurface() const ;

        bool addCurve( double csection_, const PolyCurve& curve_ ) ;
        const PolyCurve& getCurve( double csection_ ) const ;
        void removeCurve( double csection_ );

        std::map< double, PolyCurve > getCurves() const ;
        void removeCurves() ;

        void addTrajectory(const PolyCurve& traj_) ;
        void removeTrajectory() ;
        bool getTrajectory( PolyCurve& traj_ ) const ;

        void surfaceDone() ;


	protected:

        void defineId();

	protected:

		

        Surface surface;
        std::map< double, PolyCurve > curves;
        PolyCurve trajectory;

};

