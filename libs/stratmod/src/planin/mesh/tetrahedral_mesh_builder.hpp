#ifndef __TETRAHEDRAL_MESH_BUILDER__
#define __TETRAHEDRAL_MESH_BUILDER__

#include "../srules.hpp"
#include "polyhedra.hpp"

#include <vector>
#include <string>

class TetrahedralMeshBuilder
{
    public:
        TetrahedralMeshBuilder() = delete;
        TetrahedralMeshBuilder( SRules &container ) : container_(container) {}

        ~TetrahedralMeshBuilder() = default;

        bool exportToTetgen( std::string );
        bool exportToVTK( std::string );

        size_t tetrahedralize( std::vector<Tetrahedron>& );

        template<typename VertexList, typename ElementList>
        size_t tetrahedralize( VertexList &, std::vector<ElementList> & );

        template<typename VertexList, typename ElementList, typename AttributeList>
        size_t tetrahedralize( VertexList &, ElementList &, AttributeList & );

        template<typename VertexList, typename ElementList, typename VolumeList>
        size_t tetrahedralize( VertexList &, std::vector<ElementList> &, VolumeList & );


    private:
        SRules &container_;

        // BUG: VS2013 does not support constexpr
        /* static constexpr size_t numPrismsPerBlock = 8; */

        const size_t numPrismsPerBlock = 8;

        size_t numBlocksX;
        size_t numBlocksY;
        size_t numBlocks;

        size_t numVerticesX;
        size_t numVerticesY;
        size_t numVertices;

        size_t numSurfaces;

        using IndicesType = std::array<size_t, 2>;

        struct TriangleHeights;


        // Methods
        bool buildPrismMesh( std::vector<Prism> &prism_list );

        void getDiscretization();

        TriangleHeights getTriangleHeights( size_t triangle_pos, size_t bindex, size_t surface_id ) const;
        Prism getPrism( size_t prism_pos, size_t bindex, size_t lower_surface, size_t upper_surface ) const;

        size_t getVertexIndex( size_t i, size_t j ) const;
        size_t getVertexIndexInVList( size_t vindex, size_t surface_id ) const;
        IndicesType getVertexIndices( size_t v ) const;

        size_t getBlockIndex( size_t i, size_t j ) const;
        IndicesType getBlockIndices( size_t b ) const;

        size_t getVertexIndexFromPositionInBlock( size_t vpos,  size_t bindex ) const;

        template<typename VertexList>
        size_t getVertexCoordinates( VertexList & );

        template<typename ElementList, typename AttributeList>
        size_t getElementList( const std::vector<Tetrahedron> &, ElementList &, AttributeList & );

        template<typename ElementList, typename AttributeList>
        size_t getElementList( const std::vector<Prism> &, ElementList &, AttributeList & );

        template<typename ElementList>
        size_t getElementList( const std::vector<Prism> &, std::vector<ElementList> & );

        template<typename ElementList, typename AttributeList, typename VolumeList>
        size_t getElementList( const std::vector<Prism> &, std::vector<ElementList> &, VolumeList & );
};

template<typename VertexList>
size_t TetrahedralMeshBuilder::getVertexCoordinates( VertexList &vlist ) 
{ 
    size_t num_vertices = 0;
    getDiscretization();

    num_vertices = numVertices*numSurfaces;
    const size_t numCoordinatesPerTriangle = 3;
    vlist.resize(num_vertices*numCoordinatesPerTriangle);

    size_t vindex;
    Point3 vcoords;

    for ( size_t surf_id = 0; surf_id < numSurfaces; ++surf_id )
    {
        for ( size_t v = 0; v < numVertices; ++v )
        {
            container_[surf_id]->getVertex3D(v, vcoords);
            vindex = getVertexIndexInVList(v, surf_id);

            vlist[3*vindex + 0] = vcoords.x;
            vlist[3*vindex + 1] = vcoords.y;
            vlist[3*vindex + 2] = vcoords.z;
        }
    }

    return num_vertices; 
}

template<typename ElementList, typename AttributeList>
size_t TetrahedralMeshBuilder::getElementList( const std::vector<Tetrahedron> &tetrahedra, ElementList &elist, AttributeList &alist ) 
{ 
    size_t num_tetrahedra = tetrahedra.size();;

    using AttributeType = decltype( tetrahedra[0].getAttribute() );
    std::map< AttributeType, size_t > attributes;

    elist.resize( 4*num_tetrahedra );

    std::vector<size_t> element;
    for ( size_t t = 0; t < num_tetrahedra; ++t )
    {
        element = tetrahedra[t].getConnectivity();

        elist[4*t + 0] = element[0];
        elist[4*t + 1] = element[1];
        elist[4*t + 2] = element[2];
        elist[4*t + 3] = element[3];

        attributes.insert( std::make_pair(tetrahedra[t].getAttribute(), 0) );
    }


    size_t i = 1;

    for ( auto &att : attributes )
    {
        att.second = i;
        ++i;
    }


    alist.resize( num_tetrahedra );
    auto it = attributes.begin();

    for ( size_t t = 0; t < num_tetrahedra; ++t )
    {
        it = attributes.find( tetrahedra[t].getAttribute() );
        alist[t] = it->second;
    }


    return num_tetrahedra; 
}

template<typename ElementList, typename AttributeList>
size_t TetrahedralMeshBuilder::getElementList( const std::vector<Prism> &prism_list, ElementList &elist, AttributeList &alist ) 
{ 
    using AttributeType = decltype( prism_list[0].getAttribute() );
    std::map< AttributeType, size_t > attributes_map;

    //
    // Process attributes
    //

    std::vector< AttributeType > pre_processed_attributes;
    pre_processed_attributes.resize( prism_list.size() );

    for ( size_t p = 0; p < prism_list.size(); ++p )
    {
        pre_processed_attributes[p] = prism_list[p].getAttribute();
        attributes_map.insert( std::make_pair(pre_processed_attributes[p], 0) );
    }

    size_t i = 1;

    for ( auto &att : attributes_map )
    {
        att.second = i;
        ++i;
    }


    //
    // Get tetrahedra
    //

    elist.clear();
    alist.clear();

    std::vector<size_t> element;
    std::vector<Tetrahedron> tetrahedra;

    auto it = attributes_map.begin();
    size_t num_tetrahedra = 0;

    for ( size_t p = 0; p < prism_list.size(); ++p )
    {
        tetrahedra = prism_list[p].tetrahedralize();

        for ( size_t t = 0; t < tetrahedra.size(); ++t )
        {
            element = tetrahedra[t].getConnectivity();
            std::copy(element.begin(), element.end(), std::back_inserter(elist));

            it = attributes_map.find( tetrahedra[t].getAttribute() );
            alist.push_back( it->second );

            ++num_tetrahedra;
        }
    }


    return num_tetrahedra; 
}

template<typename ElementList>
size_t TetrahedralMeshBuilder::getElementList( const std::vector<Prism> &prism_list, std::vector<ElementList> &elist )
{ 
    using AttributeType = decltype( prism_list[0].getAttribute() );
    std::map< AttributeType, size_t > attributes_map;

    //
    // Process attributes
    //

    std::vector< AttributeType > pre_processed_attributes;
    pre_processed_attributes.resize( prism_list.size() );

    for ( size_t p = 0; p < prism_list.size(); ++p )
    {
        pre_processed_attributes[p] = prism_list[p].getAttribute();
        attributes_map.insert( std::make_pair(pre_processed_attributes[p], 0) );
    }

    size_t i = 0;

    for ( auto &att : attributes_map )
    {
        att.second = i;
        ++i;
    }


    //
    // Get tetrahedra
    //

    elist.resize( attributes_map.size() );

    std::vector<size_t> element;
    std::vector<Tetrahedron> tetrahedra;

    auto it = attributes_map.begin();
    size_t num_tetrahedra = 0;
    size_t attribute;

    for ( size_t p = 0; p < prism_list.size(); ++p )
    {
        tetrahedra = prism_list[p].tetrahedralize();

        for ( size_t t = 0; t < tetrahedra.size(); ++t )
        {
            it = attributes_map.find( tetrahedra[t].getAttribute() );
            attribute = it->second;

            element = tetrahedra[t].getConnectivity();
            std::copy(element.begin(), element.end(), std::back_inserter( elist[attribute] ));

            ++num_tetrahedra;
        }
    }

    return num_tetrahedra; 
}

template<typename ElementList, typename AttributeList, typename VolumeList>
size_t TetrahedralMeshBuilder::getElementList( const std::vector<Prism> &prism_list, std::vector<ElementList> &elist, VolumeList &vlist )
{ 
    using AttributeType = decltype( prism_list[0].getAttribute() );
    std::map< AttributeType, size_t > attributes_map;

    //
    // Process attributes
    //

    std::vector< AttributeType > pre_processed_attributes;
    pre_processed_attributes.resize( prism_list.size() );

    for ( size_t p = 0; p < prism_list.size(); ++p )
    {
        pre_processed_attributes[p] = prism_list[p].getAttribute();
        attributes_map.insert( std::make_pair(pre_processed_attributes[p], 0) );
    }

    size_t i = 0;

    for ( auto &att : attributes_map )
    {
        att.second = i;
        ++i;
    }


    //
    // Get tetrahedra
    //

    elist.resize( attributes_map.size() );
    vlist.clear();

    std::vector<size_t> element;
    std::vector<Tetrahedron> tetrahedra;

    auto it = attributes_map.begin();
    size_t num_tetrahedra = 0;
    size_t attribute;

    for ( size_t p = 0; p < prism_list.size(); ++p )
    {
        tetrahedra = prism_list[p].tetrahedralize();

        for ( size_t t = 0; t < tetrahedra.size(); ++t )
        {
            it = attributes_map.find( tetrahedra[t].getAttribute() );
            attribute = it->second;

            element = tetrahedra[t].getConnectivity();
            std::copy(element.begin(), element.end(), std::back_inserter( elist[attribute] ));

            vlist.push_back(tetrahedra[t].getVolume());

            ++num_tetrahedra;
        }
    }

    return num_tetrahedra; 
}

template<typename VertexList, typename ElementList>
size_t TetrahedralMeshBuilder::tetrahedralize( VertexList &vcoords, std::vector<ElementList> &elist ) 
{ 
    std::vector<Prism> prism_list;
    bool status = buildPrismMesh(prism_list);

    if ( status == false )
    {
        return 0;
    }

    getVertexCoordinates(vcoords);
    size_t num_tetrahedra = getElementList(prism_list, elist);

    return num_tetrahedra; 
}

template<typename VertexList, typename ElementList, typename AttributeList>
size_t TetrahedralMeshBuilder::tetrahedralize( VertexList &vcoords, ElementList &elist, AttributeList &alist ) 
{ 
    std::vector<Prism> prism_list;
    bool status = buildPrismMesh(prism_list);

    if ( status == false )
    {
        return 0;
    }

    getVertexCoordinates(vcoords);
    size_t num_tetrahedra = getElementList(prism_list, elist, alist);

    return num_tetrahedra; 
}

template<typename VertexList, typename ElementList, typename VolumeList>
size_t TetrahedralMeshBuilder::tetrahedralize( VertexList &vcoords, std::vector<ElementList> &elist, VolumeList &vlist ) 
{  
    std::vector<Prism> prism_list;
    bool status = buildPrismMesh(prism_list);

    if ( status == false )
    {
        return 0;
    }

    getVertexCoordinates(vcoords);
    size_t num_tetrahedra = getElementList(prism_list, elist, vlist);

    return num_tetrahedra; 
}


#endif
