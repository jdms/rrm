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
        * Constructor.
        */
        Stratigraphy(const Stratigraphy & strat_);


        /**
        * Constructor.
        */
        Stratigraphy & operator=(const Stratigraphy & strat_);


        /**
        * Destructor.
        */
        ~Stratigraphy();


        /**
        * Method to set a new index to the stratigraphy
        * @param id_ a new index to the stratigraphy
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setIndex(std::size_t id_ ) override;


        /**
        * This method clear all metada from stratigraphy
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void clear() override;


    protected:


        /**
        * This method defines automatically the index of the stratigraphy
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void defineId();


};

