#ifndef DOMAIN_H
#define DOMAIN_H

#include <iostream>
#include <set>

#include "object.h"

class Domain: public Object
{

    public:

        Domain();
        Domain( const Domain& dmn_ );
        Domain& operator=(const Domain& dmn_);
        ~Domain();

        void addRegion( const std::size_t& index_ );
        bool findRegion( const std::size_t& index_ ) const;

        void removeRegion( const std::size_t& index_ );
        void removeRegions();

        void setRegions( const std::set< std::size_t >& regions_ );
        std::set< std::size_t > getRegions() const;

        virtual void clear() override;

        inline void setDomainVolume( double volume_ )
        {
            volume = volume_;
        }
        inline double getDomainVolume()
        { return volume; }


    protected:

        void initialize();

    private:

        std::set< std::size_t > index_regions;
        double volume = 0;

};

#endif // DOMAIN_H
