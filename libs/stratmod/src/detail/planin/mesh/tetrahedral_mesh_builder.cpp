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

#include <algorithm>

bool TetrahedralMeshBuilder::getOrderedSurfaceIndicesList ( std::vector<size_t> &ordered_surface_indices )
{
    /* std::vector<size_t> ordered_surface_indices = {}; */

    bool status = true;
    if ( !mesh_is_built )
    {
        status = buildPrismMesh(prism_list);
    }

    if ( status == false )
    {
        return false;
    }

    std::cout << "\n\n\nTrying to order the surfaces.\n\n";

    auto set_minus = [] ( std::vector<size_t> minuend, std::vector<size_t> subtrahend ) -> std::vector<size_t>
    {
        std::vector<size_t> result;

        std::sort(minuend.begin(), minuend.end());
        std::sort(subtrahend.begin(), subtrahend.end());

        std::set_difference(minuend.begin(), minuend.end(), subtrahend.begin(), subtrahend.end(), std::back_inserter(result));

        return result;
    };

    /* auto set_union = [] ( std::vector<size_t> lhs, std::vector<size_t> rhs ) -> std::vector<size_t> */
    /* { */
    /*     std::vector<size_t> result; */

    /*     std::sort(lhs.begin(), lhs.end()); */
    /*     std::sort(rhs.begin(), rhs.end()); */

    /*     std::set_union(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::back_inserter(result)); */

    /*     return result; */
    /* }; */

    auto cat_in_place = [] ( std::vector<size_t> &lhs, std::vector<size_t> rhs ) -> void 
    {
        if ( rhs.empty() )
        {
            return;
        }

        std::copy(rhs.begin(), rhs.end(), std::back_inserter(lhs));

        return;
    };

    std::vector<size_t> lower_bound, upper_bound, current;
    size_t attribute;
    bool success;

    ordered_surface_indices.clear();

    for ( auto it = attributes_map.begin(); it != attributes_map.end(); ++it )
    {
        attribute = it->second;
        success = mapAttributeToBoundingSurfaces(attribute, lower_bound, upper_bound);
        if ( !success )
        {
            std::cout << "\nFailed to get lower and upper bounds\n" << std::flush;
            return false;
        }

        if ( it == attributes_map.begin() )
        {
            std::cout << "Initial step:\n    lower_bound --> ";
            for ( auto &i : lower_bound )
            {
                std::cout << i << " ";
            }

            if ( lower_bound.size() > 1 )
            {
                std::cout << "\nInitial step: lower_bound.size() =" << lower_bound.size() << "\n" << std::flush;
                /* return false; */
            }

            /* ordered_surface_indices.push_back( lower_bound.back() ); */
            cat_in_place(ordered_surface_indices, lower_bound);

            std::cout << "\n    ordered_surface_indices --> ";
            for ( auto &i : ordered_surface_indices )
            {
                std::cout << i << " ";
            }
            std::cout << "\n" << std::flush;
        }
        else
        {
            current = set_minus(lower_bound, ordered_surface_indices);

            std::cout << "Intermediate step:\n    lower_bound --> ";
            for ( auto &i : lower_bound )
            {
                std::cout << i << " ";
            }
            std::cout << "\n    current --> ";
            for ( auto &i : current )
            {
                std::cout << i << " ";
            }

            if ( current.size() > 1 )
            {
                std::cout << "\nIntermediate step: current.size() =" << current.size() << "\n" << std::flush;
                /* return false; */
            }

            /* ordered_surface_indices.push_back( current.back() ); */
            cat_in_place(ordered_surface_indices, current);

            std::cout << "\n    ordered_surface_indices --> ";
            for ( auto &i : ordered_surface_indices )
            {
                std::cout << i << " ";
            }
            std::cout << "\n" << std::flush;
        }
    }

    std::cout << "Final step:\n    upper_bound --> ";
    for ( auto &i : upper_bound )
    {
        std::cout << i << " ";
    }

    if ( upper_bound.size() > 1 )
    {
        std::cout << "\nFinal step: upper_bound.size() =" << upper_bound.size() << "\n" << std::flush;
        /* return false; */
    }
    
    /* ordered_surface_indices.push_back( upper_bound.back() ); */
    cat_in_place(ordered_surface_indices, upper_bound);
    
    std::cout << "\n    ordered_surface_indices --> ";
    for ( auto &i : ordered_surface_indices )
    {
        std::cout << i << " ";
    }
    std::cout << "\n" << std::flush;

    //
    // O0 = empty;
    //
    // Step 1: #( L(R1) ) = 1; #( R(R1) ) > 1;
    //          O1 = L(R1)
    //
    // Step k: #( L(Rk) \ O{k-1} ) = 1; #( R(Rk) ) > 1;
    //          Sk = L(Rk) \ O{k-1}
    //          Ok = { O{k-1} < Sk } 
    //
    // Step n: n = num_regions = num_surfaces - 1; #( L(Rn) \ O{n-1} ) = 1; #( R(Rn) ) = 1;
    //          Sn = L(Rn) \ O{n-1}
    //          On = { O{n-1} < Sn < R(Rn) }
    //

    return true;
}

size_t TetrahedralMeshBuilder::tetrahedralize( std::vector<Tetrahedron> &tetrahedron_list )
{
    /* std::vector<Prism> prism_list; */
    /* bool status = buildPrismMesh(prism_list); */

    bool status = true;
    if ( !mesh_is_built )
    {
        status = buildPrismMesh(prism_list);
    }

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

    attributes_map = computeAttributeMap(prism_list);
    mesh_is_built = true;

    return true;
}

bool TetrahedralMeshBuilder::buildTriangleMesh( const std::vector<std::vector<double>> &curves_vertices, const std::vector<std::vector<bool>> &curves_vertices_status, 
        std::vector<size_t> &triangle_list, std::vector<size_t> &triangle_attributes, bool positively_oriented )
{
    if ( numSurfaces <= 1 )
    {
        return false;
    }

    if ( curves_vertices.empty() || curves_vertices_status.empty() )
    {
        return false;
    }

    bool status = true;
    if ( !mesh_is_built )
    {
        status = buildPrismMesh(prism_list);
    }

    if ( status == false )
    {
        return false;
    }

    // TODO: check whether all curves_vertices have the same number of vertices
    size_t numCurves = curves_vertices.size();
    size_t numCurveVertices = curves_vertices.front().size()/3;
    size_t numCurveBlocks = numCurveVertices - 1;

    auto getEdgeHeight = []( size_t block, const std::vector<double> &curve_vlist, const std::vector<bool> &curve_valid_vertices ) -> EdgeHeights 
    {
        // curve_vlist = { v0x, v0y, v0z, v1x, v1y, v1z, ... }

        EdgeHeights eheights;

        eheights.vertex_status[0] = curve_valid_vertices[block + 0];
        eheights.vertex_height[0] = curve_vlist[ 3*(block + 0) + 2 ];

        eheights.vertex_status[0] = curve_valid_vertices[block + 1];
        eheights.vertex_height[1] = curve_vlist[ 3*(block + 1) + 2];

        return eheights;
    };

    auto getFirstVertexInBlockForCurve = [&]( size_t block, size_t curve_id ) -> size_t
    {
        return block + curve_id * numCurveVertices;
    };

    auto getTriangle = [&]( size_t block, size_t base_id, size_t top_id ) -> std::vector<size_t>
    {
        std::vector<size_t> triangles;

        /* positively_oriented == true */
        /*                             */
        /* v2         v3 */
        /* *----------* */
        /* |         /| */
        /* |        / | */
        /* |  t1   /  | */
        /* |      /   | */
        /* |     /    | */
        /* |    /     | */
        /* |   /      | */
        /* |  /  t0   | */
        /* | /        | */
        /* |/         | */
        /* *----------* */
        /* v0         v1 */
        /*                              */
        /* t0 = { v0, v1, v3 } */
        /* t1 = { v0, v3, v2 } */

        size_t v0, v1, v2, v3;

        v0 = getFirstVertexInBlockForCurve(block, base_id);
        v1 = getFirstVertexInBlockForCurve(block, base_id) + 1;
        v2 = getFirstVertexInBlockForCurve(block, top_id);
        v3 = getFirstVertexInBlockForCurve(block, top_id) + 1;

        if ( positively_oriented )
        {
            triangles = { v0, v1, v3, v0, v3, v2 }; 
        }
        else
        {
            triangles = { v0, v3, v1, v0, v2, v3 };
        }

        return triangles;
    };

    auto getAttribute = [&]( std::vector<bool> &attribute ) -> size_t 
    {
        auto it = attributes_map.find(attribute);

        return it->second;
    };

    triangle_list.clear();
    triangle_attributes.clear();

    // ordered_surfaces if firstprivate along the for loop
    std::vector< std::pair<bool, size_t> > ordered_curves(numCurves);

    // these are private along the for loop
    std::multimap< EdgeHeights, size_t > list_of_edge_heights;
    EdgeHeights edge_heights;
    std::vector<size_t> valid_curves;
    std::vector<bool> cur_attribute;
    std::vector<std::vector<bool>> attribute_list;

    std::vector<size_t> cur_triangles;

    // Paralelize this loop?
    for ( size_t block = 0; block < numCurveBlocks; ++block )
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
        /* for ( size_t prism = 0; prism < numPrismsPerBlock; ++prism ) */
        {
            list_of_edge_heights.clear();

            for ( size_t curve_id = 0; curve_id < numCurves; ++curve_id )
            {
                edge_heights = getEdgeHeight(block, curves_vertices[curve_id], curves_vertices_status[curve_id]);

                list_of_edge_heights.insert( std::make_pair(edge_heights, curve_id) );
            }

            //     2. Sort surfaces accordingly to their height (also, invalid surfaces come first).
            size_t proc_curves = 0;
            for ( auto &it : list_of_edge_heights )
            {
                ordered_curves[proc_curves] = std::make_pair(it.first.isValid(), it.second);
                ++proc_curves;
            }

            //     3. Use the sorting in (2) to compute attributes (i.e., the region that a prism belongs to).
            valid_curves.clear();
            attribute_list.clear();
            cur_attribute = std::vector<bool>(numSurfaces, false);

            for ( size_t s = 0; s < proc_curves; ++s )
            {
                cur_attribute[ ordered_curves[s].second ] = true;

                if ( ordered_curves[s].first == true )
                {
                    valid_curves.push_back(ordered_curves[s].second);
                    attribute_list.push_back(cur_attribute);
                }
            }


            //     4. Build prisms from the valid surfaces with the attribute computed in (3).
            for ( size_t s = 0; s < valid_curves.size() - 1; ++s )
            {
                cur_triangles = getTriangle(block, valid_curves[s], valid_curves[s+1]);
                std::copy( cur_triangles.begin(), cur_triangles.end(), std::back_inserter(triangle_list) );

                triangle_attributes.push_back( getAttribute(attribute_list[s]) );

            }
        }
    }

    if ( prism_list.empty() )
    {
        return false;
    }

    attributes_map = computeAttributeMap(prism_list);
    mesh_is_built = true;

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
    std::map< AttributeType, size_t > __attributes_map;

    //
    // Process attributes
    //

    std::vector< AttributeType > pre_processed_attributes;
    pre_processed_attributes.resize( prism_list.size() );

    for ( size_t p = 0; p < prism_list.size(); ++p )
    {
        pre_processed_attributes[p] = prism_list[p].getAttribute();
        __attributes_map.insert( std::make_pair(pre_processed_attributes[p], 0) );
    }

    size_t i = 0;

    for ( auto &att : __attributes_map )
    {
        att.second = i;
        ++i;
    }

    return __attributes_map;
}

bool TetrahedralMeshBuilder::mapPointsToAttributes( const std::vector<Point3> &points, std::vector<int> &attrib_list )
{
    if ( points.empty() )
    {
        return false;
    }

    /* std::vector<Prism> prism_list; */
    bool status = true;
    if ( !mesh_is_built )
    {
        status = buildPrismMesh(prism_list);
    }

    if ( status == false )
    {
        return false;
    }

    /* auto attributes_map = computeAttributeMap(prism_list); */

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
    auto &attributes_map_omp = attributes_map;

    #pragma omp parallel for shared(attrib_list, points, getAttribute, attributes_map_omp) private(attrib, num_attrib)
    for ( long int i = 0; i < static_cast<long int>( points.size() ); ++i )
    {
        attrib = getAttribute( container_.getSurfacesBelowPoint( points[i] ) );

        auto iter = attributes_map_omp.find(attrib);
        if ( iter != attributes_map_omp.end() )
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

bool TetrahedralMeshBuilder::mapAttributeToBoundingSurfaces( size_t attribute, std::vector<size_t> &lower_bound, std::vector<size_t> &upper_bound )
{
    /* std::vector<Prism> prism_list; */
    bool status = true;
    if ( !mesh_is_built )
    {
        status = buildPrismMesh(prism_list);
    }

    if ( status == false )
    {
        return false;
    }

    /* auto attributes_map = computeAttributeMap(prism_list); */

    auto it = attributes_map.begin();
    size_t index;

    for ( index = 0; (index < attribute) && (it != attributes_map.end()); ++index )
    {
        ++it;
    }

    if ( (it == attributes_map.end()) || (index != attribute) || (index != it->second) )
    {
        return false;
    }

    lower_bound.clear();
    upper_bound.clear();

    auto attrib = it->first;
    index = 0;

    for ( auto sid_is_present : attrib )
    {
        if ( sid_is_present )
        {
            lower_bound.push_back(index);
        }
        else
        {
            upper_bound.push_back(index);
        }

        ++index;
    }

    return true;
}

