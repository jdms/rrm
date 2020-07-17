/******************************************************************************/
/*                                                                            */
/* This file is part of the "Planar Interpolation Library" (PlanInLib)        */
/* Copyright (C) 2016, Julio Daniel Machado Silva.                            */
/*                                                                            */
/* PlanInLib is free software; you can redistribute it and/or                 */
/* modify it under the terms of the GNU Lesser General Public                 */
/* License as published by the Free Software Foundation; either               */
/* version 3 of the License, or (at your option) any later version.           */
/*                                                                            */
/* PlanInLib is distributed in the hope that it will be useful,               */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          */
/* Lesser General Public License for more details.                            */
/*                                                                            */
/* You should have received a copy of the GNU Lesser General Public           */
/* License along with PlanInLib.  If not, see <http://www.gnu.org/licenses/>, */
/* or write to the Free Software Foundation, Inc., 51 Franklin Street,        */
/* Fifth Floor, Boston, MA  02110-1301  USA.                                  */
/*                                                                            */
/******************************************************************************/


#ifndef PLANIN_TETRAHEDRAL_MESH_BUILDER_HPP
#define PLANIN_TETRAHEDRAL_MESH_BUILDER_HPP

#include "../srules.hpp"
#include "polyhedra.hpp"

#include <vector>
#include <string>
#include <fstream>

class TetrahedralMeshBuilder
{
    public:
        using AttributeType = std::vector<bool>;
        using Tetrahedron = ::Tetrahedron<AttributeType>;
        using Prism = ::Prism<AttributeType>;

        TetrahedralMeshBuilder() = delete;
        TetrahedralMeshBuilder( SRules &container ) : container_(container) {}

        ~TetrahedralMeshBuilder() = default;

        bool exportToTetgen( std::string );

        template<typename VertexList, typename ElementList, typename AttributeList>
        bool exportToVTK( std::string, const VertexList&, const ElementList&, const AttributeList& );
        bool exportToVTK( std::string );
        bool exportToVTK( std::string, const std::vector<int>& region_to_domain_map );

        size_t tetrahedralize( std::vector<Tetrahedron>& );

        /* template<typename VertexList, typename ElementList> */
        /* size_t tetrahedralize( VertexList &, std::vector<ElementList> & ); */

        /* template<typename VertexList, typename ElementList, typename AttributeList> */
        /* size_t tetrahedralize( VertexList &, ElementList &, AttributeList & ); */

        /* template<typename VertexList, typename ElementList, typename VolumeList> */
        /* size_t tetrahedralize( VertexList &, std::vector<ElementList> &, VolumeList & ); */

        bool getOrderedSurfaceIndicesList ( std::vector<size_t> & );

        template<typename VertexList>
        size_t getRawVertexCoordinates( VertexList & );

        template<typename VertexList>
        size_t getVertexCoordinates( VertexList & );

        template<typename ElementList, typename AttributeList>
        size_t getTetrahedronList( ElementList &, AttributeList & );

        template<typename ElementList>
        size_t getTetrahedronList( std::vector<ElementList> & );

        template<typename VolumeList>
        bool getRegionVolumeList( VolumeList & );

        template<typename TriangleList, typename BoundaryAttributes = int, typename VolumeAttributes = size_t>
        bool getSimplicialComplex( TriangleList &, BoundaryAttributes &, VolumeAttributes & );

        template<typename VertexList, typename ElementList, typename VolumeList>
        static bool computeVolumes( VertexList &, ElementList &, VolumeList & );

        template<typename VertexList, typename ElementList, typename EdgeList>
        static bool computeBoundingCurves( VertexList &, ElementList &, EdgeList & );

        template<typename VertexList, typename ElementList, typename TriangleList>
        static bool computeBoundaryMesh( VertexList &, ElementList &, TriangleList & );

        bool mapPointsToAttributes( const std::vector<Point3> &points, std::vector<int> &attrib_list );

        bool mapAttributeToBoundingSurfaces( std::size_t attribute, std::vector<size_t> &lower_bound, std::vector<size_t> &upper_bound );


    private:
        SRules &container_;

        std::vector<Prism> prism_list;
        std::map< AttributeType, size_t > attributes_map;
        bool mesh_is_built = false;

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


        // Methods
        bool buildPrismMesh( std::vector<Prism> &prism_list );

        bool buildTriangleMesh( const std::vector<std::vector<double>> &curves, const std::vector<std::vector<bool>> &curves_vertices_status, 
                std::vector<size_t> &triangle_list, std::vector<size_t> &triangle_attributes, bool positively_oriented = true );

        void getDiscretization();

        TriangleHeights getTriangleHeights( size_t triangle_pos, size_t bindex, size_t surface_id ) const;
        Prism getPrism( size_t prism_pos, size_t bindex, size_t lower_surface, size_t upper_surface ) const;

        size_t getVertexIndex( size_t i, size_t j ) const;
        size_t getVertexIndexInVList( size_t vindex, size_t surface_id ) const;
        IndicesType getVertexIndices( size_t v ) const;

        size_t getBlockIndex( size_t i, size_t j ) const;
        IndicesType getBlockIndices( size_t b ) const;

        size_t getVertexIndexFromPositionInBlock( size_t vpos,  size_t bindex ) const;

        std::map<AttributeType, std::size_t> computeAttributeMap( const std::vector<Prism> &prism_list );

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
size_t TetrahedralMeshBuilder::getRawVertexCoordinates( VertexList &vlist ) 
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
            container_[surf_id]->getMappedVertex3D(v, vcoords);
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
size_t TetrahedralMeshBuilder::getTetrahedronList( ElementList &elist, AttributeList &alist )
{
    /* std::vector<Prism> prism_list; */
    bool status = true;
    if ( !mesh_is_built )
    {
        status = buildPrismMesh(prism_list);
    }

    if ( status == false )
    {
        return 0;
    }

    size_t num_tetrahedra = getElementList(prism_list, elist, alist);

    return num_tetrahedra; 
}

template<typename ElementList>
size_t TetrahedralMeshBuilder::getTetrahedronList( std::vector<ElementList> &elist )
{
    /* std::vector<Prism> prism_list; */
    bool status = true;
    if ( !mesh_is_built )
    {
        status = buildPrismMesh(prism_list);
    }

    if ( status == false )
    {
        /* std::cout << "Failed to build prism mesh\n"; */
        return 0;
    }

    size_t num_tetrahedra = getElementList(prism_list, elist);

    return num_tetrahedra; 
}

template<typename VolumeList>
bool TetrahedralMeshBuilder::getRegionVolumeList( VolumeList &vlist )
{
    bool status = true;
    if ( !mesh_is_built )
    {
        status = buildPrismMesh(prism_list);
    }

    if ( status == false )
    {
        /* std::cout << "Failed to build prism mesh\n"; */
        return 0;
    }

    std::vector<Tetrahedron> tetrahedra;

    auto it = attributes_map.begin();
    size_t attribute;

    vlist.clear();
    vlist.resize( attributes_map.size(), 0 );

    for ( size_t p = 0; p < prism_list.size(); ++p )
    {
        tetrahedra = prism_list[p].tetrahedralize();

        for ( size_t t = 0; t < tetrahedra.size(); ++t )
        {
            it = attributes_map.find( tetrahedra[t].getAttribute() );
            attribute = it->second;

            vlist[attribute] += tetrahedra[t].getVolume();
        }
    }

    return true;
}

template<typename ElementList, typename AttributeList>
size_t TetrahedralMeshBuilder::getElementList( const std::vector<Prism> &prism_list, ElementList &elist, AttributeList &alist ) 
{ 
    /* using AttributeType = decltype( prism_list[0].getAttribute() ); */
    /* std::map< AttributeType, size_t > attributes_map; */

    /* // */
    /* // Process attributes */
    /* // */

    /* std::vector< AttributeType > pre_processed_attributes; */
    /* pre_processed_attributes.resize( prism_list.size() ); */

    /* for ( size_t p = 0; p < prism_list.size(); ++p ) */
    /* { */
    /*     pre_processed_attributes[p] = prism_list[p].getAttribute(); */
    /*     attributes_map.insert( std::make_pair(pre_processed_attributes[p], 0) ); */
    /* } */

    /* size_t i = 1; */

    /* for ( auto &att : attributes_map ) */
    /* { */
    /*     att.second = i; */
    /*     ++i; */
    /* } */

    /* auto attributes_map = computeAttributeMap(prism_list); */

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
    /* using AttributeType = decltype( prism_list[0].getAttribute() ); */
    /* std::map< AttributeType, size_t > attributes_map; */

    /* // */
    /* // Process attributes */
    /* // */

    /* std::vector< AttributeType > pre_processed_attributes; */
    /* pre_processed_attributes.resize( prism_list.size() ); */

    /* for ( size_t p = 0; p < prism_list.size(); ++p ) */
    /* { */
    /*     pre_processed_attributes[p] = prism_list[p].getAttribute(); */
    /*     attributes_map.insert( std::make_pair(pre_processed_attributes[p], 0) ); */
    /* } */

    /* size_t i = 0; */

    /* for ( auto &att : attributes_map ) */
    /* { */
    /*     att.second = i; */
    /*     ++i; */
    /* } */

    /* auto attributes_map = computeAttributeMap(prism_list); */

    //
    // Get tetrahedra
    //

    elist.resize( attributes_map.size() );

    std::vector<size_t> element;
    std::vector<Tetrahedron> tetrahedra;

    auto it = attributes_map.begin();
    size_t num_tetrahedra = 0;
    size_t attribute;

    /* int step = 0; */
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
        /* std::cout << ++step << "\n"; */
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

/* template<typename VertexList, typename ElementList> */
/* size_t TetrahedralMeshBuilder::tetrahedralize( VertexList &vcoords, std::vector<ElementList> &elist ) */ 
/* { */ 
/*     std::vector<Prism> prism_list; */
/*     bool status = buildPrismMesh(prism_list); */

/*     if ( status == false ) */
/*     { */
/*         return 0; */
/*     } */

/*     getVertexCoordinates(vcoords); */
/*     size_t num_tetrahedra = getElementList(prism_list, elist); */

/*     return num_tetrahedra; */ 
/* } */

/* template<typename VertexList, typename ElementList, typename AttributeList> */
/* size_t TetrahedralMeshBuilder::tetrahedralize( VertexList &vcoords, ElementList &elist, AttributeList &alist ) */ 
/* { */ 
/*     std::vector<Prism> prism_list; */
/*     bool status = buildPrismMesh(prism_list); */

/*     if ( status == false ) */
/*     { */
/*         return 0; */
/*     } */

/*     getVertexCoordinates(vcoords); */
/*     size_t num_tetrahedra = getElementList(prism_list, elist, alist); */

/*     return num_tetrahedra; */ 
/* } */

/* template<typename VertexList, typename ElementList, typename VolumeList> */
/* size_t TetrahedralMeshBuilder::tetrahedralize( VertexList &vcoords, std::vector<ElementList> &elist, VolumeList &vlist ) */ 
/* { */  
/*     std::vector<Prism> prism_list; */
/*     bool status = buildPrismMesh(prism_list); */

/*     if ( status == false ) */
/*     { */
/*         return 0; */
/*     } */

/*     getVertexCoordinates(vcoords); */
/*     size_t num_tetrahedra = getElementList(prism_list, elist, vlist); */

/*     return num_tetrahedra; */ 
/* } */

template<typename VertexList, typename ElementList, typename AttributeList>
bool TetrahedralMeshBuilder::exportToVTK( std::string filename, const VertexList &vcoords, const ElementList &elist, const AttributeList &attribute_list )
{
    /* Create vtk file */
    std::ofstream vtkfs(filename + ".vtk");

    if ( !vtkfs.good() )
    {
        return false; 
    }


    // Create mesh
    /* std::vector<Prism> prism_list; */

    /* std::vector<double> vcoords; */
    /* std::vector<size_t> elist; */
    /* std::vector<size_t> attribute_list; */

    /* bool status = buildPrismMesh(prism_list); */

    /* if ( status == false ) */
    /* { */
        /* return false; */
    /* } */

    // size_t num_vertices = getVertexCoordinates(vcoords);
    /* size_t num_tetrahedra = getElementList(prism_list, elelist, attlist); */
    // size_t num_tetrahedra = getTetrahedronList(elist, attribute_list);
	size_t num_vertices = vcoords.size() / 3;
	size_t num_tetrahedra = elist.size() / 4;

    // Write file
    vtkfs << "# vtk DataFile Version 2.0\n";
    vtkfs << "Unstructured Grid\n";
    vtkfs << "ASCII\n";
    vtkfs << "DATASET UNSTRUCTURED_GRID\n\n";

    vtkfs << "POINTS " << num_vertices << " double\n";
    for ( size_t i = 0; i < num_vertices; ++i )
    {
        vtkfs << vcoords[3*i + 0] << " "
            << vcoords[3*i + 1] << " "
            << vcoords[3*i + 2] << "\n";
    }
    vtkfs << "\n";

    vtkfs << "CELLS " << num_tetrahedra << " " << num_tetrahedra*5 << "\n";
    for ( size_t i = 0; i < num_tetrahedra; ++i )
    {
        vtkfs << "4" << " "
            << elist[4*i + 0] << " " 
            << elist[4*i + 1] << " " 
            << elist[4*i + 2] << " " 
            << elist[4*i + 3] << "\n"; 
    }
    vtkfs << "\n";

    vtkfs << "CELL_TYPES " << num_tetrahedra << "\n";
    for ( size_t i = 0; i < num_tetrahedra; ++i )
    {
        // the number "10" corresponds to "VTK_TETRA"
        vtkfs << "10" << "\n";
    }
    vtkfs << "\n";

    vtkfs << "CELL_DATA " << num_tetrahedra << "\n";
    vtkfs << "SCALARS cell_scalars int 1\n";
    vtkfs << "LOOKUP_TABLE default\n";
    for ( size_t i = 0; i < num_tetrahedra; ++i )
    {
        vtkfs << attribute_list[i] << "\n";
    }
    vtkfs << "\n";

    return true;
}

template<typename TriangleList, typename BoundaryAttributes, typename VolumeAttributes>
bool TetrahedralMeshBuilder::getSimplicialComplex( TriangleList &, BoundaryAttributes &, VolumeAttributes & )
{

    // Get Left, Right, Front and Back Boundaries, then get all surfaces
    //
    // BoundaryAttributes: surface ids for triangles pertaining to a surface,
    // volume attribute for triangles in the bounding box
    //
    // VolumeAttributes: for volumes from 1 to (last-1) its lower bounding
    // surface is marked with the volume attribute, both surfaces from the
    // topmost volume are marked with its attribute
    //

    return true;
}

#endif
