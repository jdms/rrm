#ifndef DOMAIN_H
#define DOMAIN_H

#include <iostream>
#include <set>

#include "object.h"

class Domain: public Object
{

    public:


        /**
        * Constructor.
        */
        Domain();


        /**
        * Constructor.
        */
        Domain( const Domain& dmn_ );


        /**
        * Constructor.
        */
        Domain& operator=(const Domain& dmn_);


        /**
        * Destructor.
        */
        ~Domain();


        /**
        * Method to add a region into the domain
        * @param index_ the index of the region
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void addRegion( const std::size_t& index_ );


        /**
        * Method to return if a region belongs to the domain
        * @param index_ the index of the region
        * @see testMeToo()
        * @see publicVar()
        * @return boolean return true if the region belongs to the domain, and false otherwise
        */
        bool findRegion( const std::size_t& index_ ) const;


        /**
        * Method to remove a region from the domain
        * @param index_ the index of the region
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void removeRegion( const std::size_t& index_ );


        /**
        * Method to remove all regions from the domain
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void removeRegions();


        /**
        * Method to add a set of regions into the domain
        * @param regions_ a set of regions
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setRegions( const std::set< std::size_t >& regions_ );


        /**
        * Method to return the regions which belongs to the domain
        * @param regions_
        * @see testMeToo()
        * @see publicVar()
        * @return std::set< std::size_t > a set of regions indexes
        */
        std::set< std::size_t > getRegions() const;


        /**
        * Method to clear all metadata of the domain
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        virtual void clear() override;


        /**
        * Method to set the volume of the domain
        * @param volume_ the volume of the domain, i.e., the sum of the volume of the
        * regions that it contains
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        inline void setDomainVolume( double volume_ )
        {
            volume = volume_;
        }


        /**
        * Method to get the volume of the domain
        * @param volume_
        * @see testMeToo()
        * @see publicVar()
        * @return the volume of the domain, i.e., the sum of the volume of the
        * regions that it contains
        */
        inline double getDomainVolume()
        { return volume; }


    protected:


        /**
        * This method remove all metadata from the structure and set the default values for the data.
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void initialize();


    private:

        std::set< std::size_t > index_regions;                              /**< A set of regions indexes which belongs to the domain */

        double volume = 0;                                                  /**< The volume of the domain, i.e., the sum of the volume of
                                                                                 the regions that it contains */

};

#endif // DOMAIN_H
