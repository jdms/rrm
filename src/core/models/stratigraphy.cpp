#include "stratigraphy.h"

#include <iostream>
#include <vector>


Stratigraphy::Stratigraphy()
{
	defineId();
}

Stratigraphy::Stratigraphy(const Stratigraphy& strat_)
{
}


Stratigraphy& Stratigraphy::operator=(const Stratigraphy& strat_)
{
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



Stratigraphy::~Stratigraphy()
{
    clear();
}
