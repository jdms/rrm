/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */



#ifndef CONTAINER_H
#define CONTAINER_H

#include <map>


template< class Index, class Item >
class Container
{

    using Natural = std::size_t;


    public:


        using Iterator = typename std::map< Index, Item >::iterator;


        bool addElement( Index id_, const Item& d );
        bool removeElement( Index id_ );


        void setElement( const Index& id, const Item& d );
        Item  getElement( const Index& id ) const;
        Item&  getElement( const Index& id );


        bool findElement( const Index& id ) const;

        void deleteElement( const Index& id_ );

        Iterator begin()
        {
            return data.begin();
        }

        Iterator end()
        {
            return data.end();
        }


        bool empty() const;
        Index size()  const{ return data.size(); }
        void clear();


    protected:

        std::map< Index, Item > data;


};



template < class Index, class Item >
bool Container< Index, Item >::addElement( Index id_, const Item& d )
{
    data[ id_ ] = d;
    return true;
}


template <class Index, class Item >
bool Container< Index, Item >::removeElement( Index id_ )
{
    data.erase( id_ );
    return true;
}


template < class Index, class Item >
void  Container< Index, Item >::setElement( const Index& id_, const Item& d )
{
    data[ id_ ] = d;
}


template < class Index, class Item >
Item Container< Index, Item >::getElement( const Index& id_ ) const
{
    return data.at( id_ );
}


template < class Index, class Item >
Item& Container< Index, Item >::getElement( const Index& id_ )
{
    return data[ id_ ];
}

template < class Index, class Item >
void Container< Index, Item >::deleteElement( const Index& id_ )
{
    if( findElement( id_ ) == false ) return;
    if( data[ id_ ] == nullptr ) return;

    delete data[ id_ ];
    data[ id_ ] = nullptr;
}


template < class Index, class Item >
bool Container< Index, Item >::findElement( const Index& id_ ) const
{
    auto search = data.find( id_ );

    if ( search != data.end() )
        return true;
    else
        return false;

}


template < class Index, class Item >
bool Container< Index, Item >::empty() const
{
    return data.empty();
}



template < class Index, class Item >
void Container< Index, Item >::clear()
{
    data.clear();
}


#endif // CONTAINER_H
