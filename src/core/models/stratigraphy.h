#pragma once
#include "object.h"

#include <memory>
#include <string>
#include <map>


/**
 *  A graphical representation to the stratigraphy to save its metadata
 */

class Stratigraphy : public Object
{
    public:

        unsigned int number_of_stratigraphies;

        /**
        * Constructor.
        */
        Stratigraphy();


        /**
        * Copy constructor.
        */
        Stratigraphy(const Stratigraphy & strat_);


        /**
        * Assignment operator.
        */
        Stratigraphy & operator=(const Stratigraphy & strat_);


        /**
        * Destructor.
        */
        ~Stratigraphy();


        /**
        * Method to set a new index to the stratigraphy
        * @param id_ a new index to the stratigraphy
        * @return void.
        */
        void setIndex(std::size_t id_ ) override;


        /**
        * This method clear all metada from stratigraphy
        * @return void.
        */
        void clear() override;


    protected:


        /**
        * This method defines automatically the index of the stratigraphy
        * @return void.
        */
        void defineId();


};

