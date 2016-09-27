#ifndef STRATIGRAPHYRENDERABLE_HPP
#define STRATIGRAPHYRENDERABLE_HPP


#include "Stratigraphy.hpp"


class StratigraphyRenderable
{

	public:
	
    StratigraphyRenderable(){ strat = nullptr; }
	~StratigraphyRenderable(){}
	
	
	inline void setGeoData( Stratigraphy* const& s ){ strat = s; }
    inline Stratigraphy* getGeoData(){ return strat; }
    inline bool isEmpty(){ if( strat == nullptr ) return true; return true; }

    virtual void init(){}
    virtual void draw(){}


	protected:
	
		Stratigraphy* strat;

};


#endif
