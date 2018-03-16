#include <array>

#include <fstream>
#include <iostream>

#include "tetrahedral_mesh_builder.hpp"



bool TetrahedralMeshBuilder::exportToTetgen( std::string filename )
{
    /* std::vector<Prism> prism_list; */
    /* std::vector<size_t> attribute_list; */

    /* bool status = buildPrismMesh(prism_list); */

    /* if ( status == false ) */
    /* { */
        /* return false; */
    /* } */

    /* % Create node file */
    std::ofstream nodeofs(filename + ".node");

    if ( !nodeofs.good() )
    {
        return false; 
    }

    std::vector<double> vcoords;
    size_t num_vertices = getVertexCoordinates(vcoords);

    /* Header: <# of points> <dimension (3)> <# of attributes> <boundary markers (0 or 1)> */
    nodeofs << num_vertices << " 3 0 0\n\n";

    /* Body: <point #> <x> <y> <z> [attributes] [boundary marker] */
    for ( size_t i = 0; i < vcoords.size()/3; ++i )
    {
        nodeofs << i << " " 
            << vcoords[3*i + 0] << " " 
            << vcoords[3*i + 1] << " "  
            << vcoords[3*i + 2] << std::endl;
    }


    /* % Create ele file */
    std::ofstream eleofs(filename + ".ele");

    if ( !eleofs.good() )
    {
        return false; 
    }

    std::vector<size_t> elelist;
    std::vector<size_t> attlist;
    /* size_t num_tetrahedra = getElementList(prism_list, elelist, attlist); */
    size_t num_tetrahedra = getTetrahedronList(elelist, attlist);

    /* Header: <# of tetrahedra> <nodes per tet. (4 or 10)> <region attribute (0 or 1)> */
    eleofs << num_tetrahedra << " 4 1\n\n";

    /* Body: <tetrahedron #> <node> <node> ... <node> [attribute] */
    for ( size_t t = 0; t < num_tetrahedra; ++t )
    {
        eleofs << t << "    " 
            << elelist[4*t + 0] << " " 
            << elelist[4*t + 1] << " " 
            << elelist[4*t + 2] << " " 
            << elelist[4*t + 3] << " "
            << attlist[t] << std::endl;
    }

    return true;
}

bool TetrahedralMeshBuilder::exportToVTK( std::string filename )
{
    /* Create vtk file */
    std::ofstream vtkfs(filename + ".vtk");

    if ( !vtkfs.good() )
    {
        return false; 
    }


    // Create mesh
    /* std::vector<Prism> prism_list; */

    std::vector<double> vcoords;
    std::vector<size_t> elist;
    std::vector<size_t> attribute_list;

    /* bool status = buildPrismMesh(prism_list); */

    /* if ( status == false ) */
    /* { */
        /* return false; */
    /* } */

    size_t num_vertices = getVertexCoordinates(vcoords);
    /* size_t num_tetrahedra = getElementList(prism_list, elelist, attlist); */
    size_t num_tetrahedra = getTetrahedronList(elist, attribute_list);


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

size_t TetrahedralMeshBuilder::tetrahedralize( std::vector<Tetrahedron> &tetrahedron_list )
{
    std::vector<Prism> prism_list;
    bool status = buildPrismMesh(prism_list);

    if ( status == false )
    {
        return 0;
    }

    tetrahedron_list.clear();
    std::vector<Tetrahedron> tetrahedra;

    for ( auto &p : prism_list )
    {
        tetrahedra = p.tetrahedralize();
        std::copy( tetrahedra.begin(), tetrahedra.end(), std::back_inserter(tetrahedron_list) );
    }

    return tetrahedron_list.size();
}

bool TetrahedralMeshBuilder::buildPrismMesh( std::vector<Prism> &prism_list )
{
    getDiscretization();

    if ( numSurfaces <= 1 )
    {
        return false;
    }

    // prism_list is shared along the for loop
    prism_list.clear();

    // ordered_surfaces if firstprivate along the for loop
    std::vector< std::pair<bool, size_t> > ordered_surfaces(numSurfaces);

    // these are private along the for loop
    std::multimap< TriangleHeights, size_t > list_of_triangle_heights;
    TriangleHeights triangle_heights;
    std::vector<size_t> valid_surfaces;
    std::vector<bool> cur_attribute;
    std::vector<std::vector<bool>> attribute_list;

    // Paralelize this loop?
    for ( size_t block = 0; block < numBlocks; ++block )
    {
        // For every triangle in the block, we'll get all surfaces' values and 
        // their correpondent prisms.  
        //
        // This loop: 
        //     1. Gets the surfaces' heights for every triangle
        //     2. Sort surfaces accordingly to their height (also, invalid surfaces come first).
        //     3. Use the sorting in (2) to compute attributes (i.e., the region that a prism belongs to).
        //     4. Build prisms from the valid surfaces with the attribute computed in (3).
        //

        //     1. Gets the surfaces' heights for every triangle
        for ( size_t prism = 0; prism < numPrismsPerBlock; ++prism )
        {
            list_of_triangle_heights.clear();

            for ( size_t surf_id = 0; surf_id < numSurfaces; ++surf_id )
            {
                triangle_heights = getTriangleHeights(prism, block, surf_id);

                list_of_triangle_heights.insert( 
                        std::make_pair(triangle_heights, surf_id) 
                        );
            }

            //     2. Sort surfaces accordingly to their height (also, invalid surfaces come first).
            size_t proc_surfaces = 0;
            for ( auto &it : list_of_triangle_heights )
            {
                ordered_surfaces[proc_surfaces] = std::make_pair(it.first.isValid(), it.second);
                ++proc_surfaces;
            }

            //     3. Use the sorting in (2) to compute attributes (i.e., the region that a prism belongs to).
            valid_surfaces.clear();
            attribute_list.clear();
            cur_attribute = std::vector<bool>(numSurfaces, false);

            for ( size_t s = 0; s < proc_surfaces; ++s )
            {
                cur_attribute[ ordered_surfaces[s].second ] = true;

                if ( ordered_surfaces[s].first == true )
                {
                    valid_surfaces.push_back(ordered_surfaces[s].second);
                    attribute_list.push_back(cur_attribute);
                }
            }


            //     4. Build prisms from the valid surfaces with the attribute computed in (3).
            for ( size_t s = 0; s < valid_surfaces.size() - 1; ++s )
            {
                prism_list.emplace_back( getPrism(prism, block, valid_surfaces[s], valid_surfaces[s+1]) );
                prism_list.back().setAttribute( attribute_list[s] );

            }
        }
    }

    if ( prism_list.empty() )
    {
        return false;
    }

    return true;
}


void TetrahedralMeshBuilder::getDiscretization()
{
    numBlocksX = PlanarSurface::getDiscretizationX();
    numBlocksY = PlanarSurface::getDiscretizationY();
    numBlocks = numBlocksX*numBlocksY;

    numVerticesX = 2*numBlocksX + 1;
    numVerticesY = 2*numBlocksY + 1;
    numVertices = numVerticesX*numVerticesY;

    numSurfaces = container_.size();
}

TriangleHeights TetrahedralMeshBuilder::getTriangleHeights( size_t triangle_pos, size_t bindex, size_t surface_id ) const
{
    TriangleHeights theights;

    auto setTriangleHeight = [&] (TriangleHeights &th, size_t vpos_in_triangle, size_t vpos_in_block, size_t surface_id) 
    {
        size_t  vindex_surface;
        bool vstatus;
        double height; 

        vindex_surface = getVertexIndexFromPositionInBlock(vpos_in_block, bindex);
        vstatus = container_[surface_id]->getHeight(vindex_surface, height);

        th.vertex_status[vpos_in_triangle] = vstatus;
        th.vertex_height[vpos_in_triangle] = height;
    };

    size_t vertex_position_in_block; 

    switch( triangle_pos )
    {
        case 0:
            vertex_position_in_block = 0;
            setTriangleHeight(theights, 0, vertex_position_in_block, surface_id);

            vertex_position_in_block = 1;
            setTriangleHeight(theights, 1, vertex_position_in_block, surface_id);

            vertex_position_in_block = 4;
            setTriangleHeight(theights, 2, vertex_position_in_block, surface_id);

            break;

        case 1:
            vertex_position_in_block = 0;
            setTriangleHeight(theights, 0, vertex_position_in_block, surface_id);

            vertex_position_in_block = 4;
            setTriangleHeight(theights, 1, vertex_position_in_block, surface_id);

            vertex_position_in_block = 3;
            setTriangleHeight(theights, 2, vertex_position_in_block, surface_id);

            break;

        case 2:
            vertex_position_in_block = 1;
            setTriangleHeight(theights, 0, vertex_position_in_block, surface_id);

            vertex_position_in_block = 2;
            setTriangleHeight(theights, 1, vertex_position_in_block, surface_id);

            vertex_position_in_block = 4;
            setTriangleHeight(theights, 2, vertex_position_in_block, surface_id);

            break;

        case 3:
            vertex_position_in_block = 2;
            setTriangleHeight(theights, 0, vertex_position_in_block, surface_id);

            vertex_position_in_block = 5;
            setTriangleHeight(theights, 1, vertex_position_in_block, surface_id);

            vertex_position_in_block = 4;
            setTriangleHeight(theights, 2, vertex_position_in_block, surface_id);

            break;

        case 4:
            vertex_position_in_block = 3;
            setTriangleHeight(theights, 0, vertex_position_in_block, surface_id);

            vertex_position_in_block = 4;
            setTriangleHeight(theights, 1, vertex_position_in_block, surface_id);

            vertex_position_in_block = 6;
            setTriangleHeight(theights, 2, vertex_position_in_block, surface_id);

            break;

        case 5:
            vertex_position_in_block = 4;
            setTriangleHeight(theights, 0, vertex_position_in_block, surface_id);

            vertex_position_in_block = 7;
            setTriangleHeight(theights, 1, vertex_position_in_block, surface_id);

            vertex_position_in_block = 6;
            setTriangleHeight(theights, 2, vertex_position_in_block, surface_id);

            break;

        case 6:
            vertex_position_in_block = 4;
            setTriangleHeight(theights, 0, vertex_position_in_block, surface_id);

            vertex_position_in_block = 5;
            setTriangleHeight(theights, 1, vertex_position_in_block, surface_id);

            vertex_position_in_block = 8;
            setTriangleHeight(theights, 2, vertex_position_in_block, surface_id);

            break;

        case 7:
            vertex_position_in_block = 4;
            setTriangleHeight(theights, 0, vertex_position_in_block, surface_id);

            vertex_position_in_block = 8;
            setTriangleHeight(theights, 1, vertex_position_in_block, surface_id);

            vertex_position_in_block = 7;
            setTriangleHeight(theights, 2, vertex_position_in_block, surface_id);

            break;

        default:
            break;
    }

    return theights;
}

TetrahedralMeshBuilder::Prism TetrahedralMeshBuilder::getPrism( size_t prism_pos, size_t bindex, size_t lower_surface, size_t upper_surface ) const
{
    Prism prism;

    auto setPrismVertex = [&] (Prism &p, size_t vpos_in_prism, size_t vpos_in_block, size_t surface_id) 
    {
        size_t  vindex_surface, vindex_vlist;
        bool vstatus;
        Point3 v; 

        vindex_surface = getVertexIndexFromPositionInBlock(vpos_in_block, bindex);
        vstatus = container_[surface_id]->getVertex3D(vindex_surface, v);
        vindex_vlist = getVertexIndexInVList(vindex_surface, surface_id); 
        p.setVertex(vpos_in_prism, v, vindex_vlist, vstatus);
    };

    size_t vertex_position_in_block; 

    switch( prism_pos )
    {
        case 0:
            // Bottom
            vertex_position_in_block = 0;
            setPrismVertex(prism, 0, vertex_position_in_block, lower_surface);

            vertex_position_in_block = 1;
            setPrismVertex(prism, 1, vertex_position_in_block, lower_surface);

            vertex_position_in_block = 4;
            setPrismVertex(prism, 2, vertex_position_in_block, lower_surface);

            // Top
            vertex_position_in_block = 0;
            setPrismVertex(prism, 3, vertex_position_in_block, upper_surface);

            vertex_position_in_block = 1;
            setPrismVertex(prism, 4, vertex_position_in_block, upper_surface);

            vertex_position_in_block = 4;
            setPrismVertex(prism, 5, vertex_position_in_block, upper_surface);

            break;

        case 1:
            // Bottom
            vertex_position_in_block = 0;
            setPrismVertex(prism, 0, vertex_position_in_block, lower_surface);

            vertex_position_in_block = 4;
            setPrismVertex(prism, 1, vertex_position_in_block, lower_surface);

            vertex_position_in_block = 3;
            setPrismVertex(prism, 2, vertex_position_in_block, lower_surface);

            // Top
            vertex_position_in_block = 0;
            setPrismVertex(prism, 3, vertex_position_in_block, upper_surface);

            vertex_position_in_block = 4;
            setPrismVertex(prism, 4, vertex_position_in_block, upper_surface);

            vertex_position_in_block = 3;
            setPrismVertex(prism, 5, vertex_position_in_block, upper_surface);

            break;

        case 2:
            // Bottom
            vertex_position_in_block = 1;
            setPrismVertex(prism, 0, vertex_position_in_block, lower_surface);

            vertex_position_in_block = 2;
            setPrismVertex(prism, 1, vertex_position_in_block, lower_surface);

            vertex_position_in_block = 4;
            setPrismVertex(prism, 2, vertex_position_in_block, lower_surface);

            // Top
            vertex_position_in_block = 1;
            setPrismVertex(prism, 3, vertex_position_in_block, upper_surface);

            vertex_position_in_block = 2;
            setPrismVertex(prism, 4, vertex_position_in_block, upper_surface);

            vertex_position_in_block = 4;
            setPrismVertex(prism, 5, vertex_position_in_block, upper_surface);

            break;

        case 3:
            // Bottom
            vertex_position_in_block = 2;
            setPrismVertex(prism, 0, vertex_position_in_block, lower_surface);

            vertex_position_in_block = 5;
            setPrismVertex(prism, 1, vertex_position_in_block, lower_surface);

            vertex_position_in_block = 4;
            setPrismVertex(prism, 2, vertex_position_in_block, lower_surface);

            // Top
            vertex_position_in_block = 2;
            setPrismVertex(prism, 3, vertex_position_in_block, upper_surface);

            vertex_position_in_block = 5;
            setPrismVertex(prism, 4, vertex_position_in_block, upper_surface);

            vertex_position_in_block = 4;
            setPrismVertex(prism, 5, vertex_position_in_block, upper_surface);

            break;

        case 4:
            // Bottom
            vertex_position_in_block = 3;
            setPrismVertex(prism, 0, vertex_position_in_block, lower_surface);

            vertex_position_in_block = 4;
            setPrismVertex(prism, 1, vertex_position_in_block, lower_surface);

            vertex_position_in_block = 6;
            setPrismVertex(prism, 2, vertex_position_in_block, lower_surface);

            // Top
            vertex_position_in_block = 3;
            setPrismVertex(prism, 3, vertex_position_in_block, upper_surface);

            vertex_position_in_block = 4;
            setPrismVertex(prism, 4, vertex_position_in_block, upper_surface);

            vertex_position_in_block = 6;
            setPrismVertex(prism, 5, vertex_position_in_block, upper_surface);

            break;

        case 5:
            // Bottom
            vertex_position_in_block = 4;
            setPrismVertex(prism, 0, vertex_position_in_block, lower_surface);

            vertex_position_in_block = 7;
            setPrismVertex(prism, 1, vertex_position_in_block, lower_surface);

            vertex_position_in_block = 6;
            setPrismVertex(prism, 2, vertex_position_in_block, lower_surface);

            // Top
            vertex_position_in_block = 4;
            setPrismVertex(prism, 3, vertex_position_in_block, upper_surface);

            vertex_position_in_block = 7;
            setPrismVertex(prism, 4, vertex_position_in_block, upper_surface);

            vertex_position_in_block = 6;
            setPrismVertex(prism, 5, vertex_position_in_block, upper_surface);

            break;

        case 6:
            // Bottom
            vertex_position_in_block = 4;
            setPrismVertex(prism, 0, vertex_position_in_block, lower_surface);

            vertex_position_in_block = 5;
            setPrismVertex(prism, 1, vertex_position_in_block, lower_surface);

            vertex_position_in_block = 8;
            setPrismVertex(prism, 2, vertex_position_in_block, lower_surface);

            // Top
            vertex_position_in_block = 4;
            setPrismVertex(prism, 3, vertex_position_in_block, upper_surface);

            vertex_position_in_block = 5;
            setPrismVertex(prism, 4, vertex_position_in_block, upper_surface);

            vertex_position_in_block = 8;
            setPrismVertex(prism, 5, vertex_position_in_block, upper_surface);

            break;

        case 7:
            // Bottom
            vertex_position_in_block = 4;
            setPrismVertex(prism, 0, vertex_position_in_block, lower_surface);

            vertex_position_in_block = 8;
            setPrismVertex(prism, 1, vertex_position_in_block, lower_surface);

            vertex_position_in_block = 7;
            setPrismVertex(prism, 2, vertex_position_in_block, lower_surface);

            // Top
            vertex_position_in_block = 4;
            setPrismVertex(prism, 3, vertex_position_in_block, upper_surface);

            vertex_position_in_block = 8;
            setPrismVertex(prism, 4, vertex_position_in_block, upper_surface);

            vertex_position_in_block = 7;
            setPrismVertex(prism, 5, vertex_position_in_block, upper_surface);

            break;

        default:
            break;
    }

    return prism;
}

size_t TetrahedralMeshBuilder::getVertexIndex( size_t i, size_t j ) const
{
    return i + j*numVerticesX;
}

size_t TetrahedralMeshBuilder::getVertexIndexInVList( size_t vindex, size_t surface_id ) const
{
    return vindex + surface_id*numVertices;
}

TetrahedralMeshBuilder::IndicesType TetrahedralMeshBuilder::getVertexIndices( size_t v ) const
{
    IndicesType indices{};

    if ( v < numVertices )
    {
        indices[0] = v % numVerticesX;
        indices[1] = (v - indices[0])/numVerticesX;
    }

    return indices;
}

size_t TetrahedralMeshBuilder::getBlockIndex( size_t i, size_t j ) const
{
    return i + j*numBlocksX;
}

TetrahedralMeshBuilder::IndicesType TetrahedralMeshBuilder::getBlockIndices( size_t b ) const
{
    IndicesType indices;

    indices[0] = b % numBlocksX;
    indices[1] = (b - indices[0])/numBlocksX;

    return indices;
};

size_t TetrahedralMeshBuilder::getVertexIndexFromPositionInBlock( size_t vpos,  size_t bindex ) const
{
    IndicesType indices = getBlockIndices(bindex);;

    size_t i = vpos % 3;
    size_t j = (vpos - i)/3;

    size_t iv = 2*indices[0] + i;
    size_t jv = 2*indices[1] + j;

    return getVertexIndex(iv, jv);
}

std::map<TetrahedralMeshBuilder::AttributeType, std::size_t> TetrahedralMeshBuilder::computeAttributeMap( const std::vector<Prism> &prism_list )
{
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

    return attributes_map;
}

bool TetrahedralMeshBuilder::mapPointsToAttributes( const std::vector<Point3> &points, std::vector<int> &attrib_list )
{
    if ( points.empty() )
    {
        return false;
    }

    std::vector<Prism> prism_list;
    bool status = buildPrismMesh(prism_list);

    if ( status == false )
    {
        return false;
    }

    auto attributes_map = computeAttributeMap(prism_list);

    size_t size_att = TetrahedralMeshBuilder::numSurfaces;

    auto getAttribute = [size_att]( std::vector<size_t> &&a )-> std::vector<bool> {
        std::vector<bool> attrib;

        attrib.resize( size_att, false );

        /* std::cout << "Current attrb: "; */
        /* for ( auto i : a ) */
        /*     std::cout << i << ", "; */
        /* std::cout << "\n"; */

        if ( a.empty() )
        {
            return attrib;
        }

        for ( size_t i = 0; i < a.size(); ++i )
        {
            attrib[ a[i] ] = true;
        }

        /* std::cout << "Current bool attrb: "; */
        /* for ( auto i : attrib ) */
        /*     std::cout << i << ", "; */
        /* std::cout << "\n"; */

        return attrib;
    };


    attrib_list.resize( points.size() );

    std::vector<bool> attrib;
    int num_attrib;

    for ( size_t i = 0; i < points.size(); ++i )
    {
        attrib = getAttribute( container_.getSurfacesBelowPoint( points[i] ) );

        auto iter = attributes_map.find(attrib);
        if ( iter != attributes_map.end() )
        {
            num_attrib = iter->second;
        }
        else
        {
            num_attrib = -1;
        }

        attrib_list[i] = num_attrib;
        /* std::cout << "Num_attrib: " << num_attrib << "\n"; */
    }

    return true;
}

