#include <fstream>
#include <chrono>

#include "smodeller.hpp"
#include "sutilities.hpp"
#include "planin/planin.hpp"

bool exportVTKFile( std::string filename, std::vector<double> &, std::vector<size_t> &, std::vector<int> &);

int main()
{
    SModeller model;
    bool status = model.loadBinary("test.rrm" );
    if ( !status )
    {
        std::cerr << "Failed to load file\n";

        return 1;
    }
    std::cout << "Loaded model\n";

    model.changeDiscretization(8,8);

    std::vector<double> vcoords;
    std::vector<std::vector<size_t>> elements;
    std::vector<int> regions;

    status = model.getTetrahedralMesh(vcoords, elements) > 0;
    if ( status == false )
    {
        std::cerr << "Failed to get tet mesh\n";

        return 1;
    }
    std::cout << "Got a mesh\n";

    std::vector<size_t> element_list; 
    std::vector<int> att_list;

    for ( size_t i = 0; i < elements.size(); ++i )
        for ( size_t j = 0; j < elements[i].size()/4; ++j )
        {
            element_list.push_back(elements[i][4*j + 0]);
            element_list.push_back(elements[i][4*j + 1]);
            element_list.push_back(elements[i][4*j + 2]);
            element_list.push_back(elements[i][4*j + 3]);

            att_list.push_back(i);
        }

    std::cout << "Created att list\n";
    /* model.undo(); */

    SUtilities u(model);
    u.exportToVTK("test2");

    status = u.getTetrahedralMeshRegions(vcoords, element_list, regions);
    if ( status == false )
    {
        std::cerr << "Failed to get tet attributes\n";

        return 1;
    }
    std::cout << "Got tet attributes\n";

    if ( regions.size() != att_list.size() )
    {
        std::cerr << "Number of elements differ, regions: " << regions.size() << ", attributes: " << att_list.size() << "\n";

        return 1;
    }

    size_t matched = 0, wrong = 0, null = 0;
    for ( size_t i = 0; i < regions.size(); ++i )
    {
        if ( regions[i] == -1 )
            ++null;
        if ( regions[i] != att_list[i] )
        {
            /* std::cerr << "Attribute for element " << i */ 
            /*     << " differ, given: " << att_list[i] */ 
            /*     << ", computed: " << regions[i] << "\n"; */
            ++wrong;
        }
        else
        {
            ++matched;
        }
    }

    std::cout << "Matched: " << matched << ", wrong: " << wrong << ", null: " << null << ", mislabelled: " << wrong - null << "\n";

    /* auto time1 = std::chrono::high_resolution_clock::now(); */

    /* status = u.exportToTetgen("test"); */
    /* if ( !status ) */
    /* { */
    /*     std::cout << "Failed to export mesh\n"; */
    /* } */

    /* auto time2 = std::chrono::high_resolution_clock::now(); */

    /* std::cout << "Time to build mesh and export file: " << std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count() << "\n"; */

    return exportVTKFile("test_regions2", vcoords, element_list, regions);
}



bool exportVTKFile( std::string filename, std::vector<double> &vcoords, std::vector<size_t> &elist, std::vector<int> &attribute_list)
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

    size_t num_vertices = vcoords.size()/3;
    /* size_t num_tetrahedra = getElementList(prism_list, elelist, attlist); */
    size_t num_tetrahedra = elist.size()/4; //, attribute_list);


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
