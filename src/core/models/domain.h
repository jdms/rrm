#ifndef DOMAIN_H
#define DOMAIN_H

#include <iostream>
#include <set>

#include "object.h"


/**
 *  A data structure to domains to save its metadata
 */


class Domain: public Object
{

    public:


        /**
        * Constructor.
        */
        Domain();


        /**
        * Copy constructor.
        */
        Domain( const Domain& dmn_ );


        /**
        * Assignment Operator
        */
        Domain& operator=(const Domain& dmn_);


        /**
        * Destructor.
        */
        ~Domain();


        /**
        * Method to add a region into the domain
        * @param index_ the index of the region
        * @return void.
        */
        void addRegion( const std::size_t& index_ );


        /**
        * Method to return if a region belongs to the domain
        * @param index_ the index of the region
        * @return boolean return true if the region belongs to the domain, and false otherwise
        */
        bool findRegion( const std::size_t& index_ ) const;


        /**
        * Method to remove a region from the domain
        * @param index_ the index of the region
        * @return void.
        */
        void removeRegion( const std::size_t& index_ );


        /**
        * Method to remove all regions from the domain
        * @return void.
        */
        void removeRegions();


        /**
        * Method to add a set of regions into the domain
        * @param regions_ a set of regions
        * @return void.
        */
        void setRegions( const std::set< std::size_t >& regions_ );


        /**
        * Method to return the regions which belongs to the domain
        * @param regions_
        * @return std::set< std::size_t > a set of regions indexes
        */
        std::set< std::size_t > getRegions() const;


        /**
        * Method to clear all metadata of the domain
        * @return void.
        */
        virtual void clear() override;


        /**
        * Method to set the volume of the domain
        * @param volume_ the volume of the domain, i.e., the sum of the volume of the
        * regions that it contains
        * @return void.
        */
        inline void setDomainVolume( double volume_ )
        {
            volume = volume_;
        }


        /**
        * Method to get the volume of the domain
        * @param volume_
        * @return the volume of the domain, i.e., the sum of the volume of the
        * regions that it contains
        */
        inline double getDomainVolume()
        { return volume; }


    protected:


        /**
        * This method remove all metadata from the structure and set the default values for the data.
        * @return void.
        */
        void initialize();


    private:

        std::set< std::size_t > index_regions;                              /**< A set of regions indexes which belongs to the domain */

        double volume = 0;                                                  /**< The volume of the domain, i.e., the sum of the volume of
                                                                                 the regions that it contains */

};

#endif // DOMAIN_H
