#ifndef CONTAINER_H
#define CONTAINER_H

#include <map>







template< class Index, class Item >
class Container
{

    using Natural = std::size_t;


    public:


        using Iterator = typename std::map< Index, Item >::iterator;


        void addElement( Index id_, const Item& d );
        void removeElement( Index id_ );


        void setElement( const Index& id, const Item& d );
        Item  getElement( const Index& id );


        bool findElement( const Index& id ) const;


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
void Container< Index, Item >::addElement( Index id_, const Item& d )
{
    if( findElement( id_ ) == true )
        return;
    data[ id_ ] = d;
}


template <class Index, class Item >
void Container< Index, Item >::removeElement( Index id_ )
{
    if( findElement( id_ ) == false )
        return;
    data.erase( id_ );
}


template < class Index, class Item >
void  Container< Index, Item >::setElement( const Index& id_, const Item& d )
{
    data[ id_ ] = d;
}


template < class Index, class Item >
Item Container< Index, Item >::getElement( const Index& id_ )
{
    if( findElement( id_ ) == false )
        return Item();

    return data[ id_ ];

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
    for( auto d: data )
        d.clear();

    data.clear();
}


#endif // CONTAINER_H
