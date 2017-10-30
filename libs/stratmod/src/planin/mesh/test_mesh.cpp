#include <chrono>

#include "../srules.hpp"
#include "tetrahedral_mesh_builder.hpp"

int main()
{
    SRules container;
    container.loadBinary("test.srules");

    size_t discretization = 64;

    auto time1 = std::chrono::high_resolution_clock::now();

    container.changeDiscretization(discretization, discretization);

    auto time2 = std::chrono::high_resolution_clock::now();
    std::cout << "Time to change discretization: " << std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count() << "\n";


    time1 = std::chrono::high_resolution_clock::now();

    TetrahedralMeshBuilder mb(container);
    mb.exportToVTK("test");

    time2 = std::chrono::high_resolution_clock::now();
    std::cout << "Time to build mesh and export file: " << std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count() << "\n";

    return 0;
}
