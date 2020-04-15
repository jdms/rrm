#include <fstream>
#include <iostream>
#include <chrono>
#include <numeric>
#include <vector>

#include "stratmod/smodeller.hpp"
#include "stratmod/sutilities.hpp"
#include "planin/planin.hpp"

#include "nlohmann/json.hpp"
#include "argh/argh.h"

struct Domain {
    int index;
    std::vector<int> regions;
};

using json = nlohmann::json;

void to_json(json& j, const Domain& d);

void from_json(const json& j, Domain& d);

bool operator!=(const Domain& lhs, const Domain& rhs);

std::vector<int> parse_domains(const std::vector<Domain>& domain_list, std::size_t expected_number_of_regions);

enum callerCodes { 
    CALLED_WITHOUT_ARGUMENTS = 1,
    CALLED_WITHOUT_OUTPUT_FILE = 2, 
    CALLED_WITH_INPUT_AND_OUTPUT_FILES = 3, 
    WRONG_USAGE = 4 
}; 

bool adaptDiscretization( 
        double model_width, double model_length, 
        size_t &output_width_disc, size_t &output_length_disc, 
        size_t input_width_disc, size_t input_length_disc);

int main(int argc, char* argv[])
{
    /* // A file suffix is not needed as SModeller::exportToVTK() adds it automatically */
    /* std::string file_sufix = ""; // ".vtk"; */

    /* std::string progname = argv[0]; */
    /* std::string rrm_model, output; */

    /* if( argc == CALLED_WITHOUT_ARGUMENTS ) { */
    /*     argc = WRONG_USAGE; */
    /* } */

    /* if( argc >= WRONG_USAGE ) { */ 
    /*     std::cout << "\nUsage: " + progname + " rrm_model[.rrm] [output], where [output] is optional." << std::endl << std::endl; */
        
    /*     return WRONG_USAGE; */
    /* } */ 
    /* else if( argc == CALLED_WITHOUT_OUTPUT_FILE ) { */ 
    /*     rrm_model = argv[1]; */
    /*     output = input + file_sufix; */ 
    /* } */
    /* else if( argc == CALLED_WITH_INPUT_AND_OUTPUT_FILES ) { */
    /*     input = argv[1]; */
    /*     output = argv[2]; */
    /* } */

    argh::parser cmdl(argc, argv);


    // For debugging
    {
        /* std::cout << std::endl << cmdl[0] << " called with the following \n"; */
        /* std::cout << "\t--> positional args:\n"; */
        /* for (auto& pos_arg : cmdl.pos_args()) */
        /*     std::cout << '\t' << '\t' << pos_arg << '\n'; */

        /* std::cout << "\n\t--> flags:\n"; */
        /* for (auto& flag : cmdl.flags()) */
        /*     std::cout << '\t' << '\t' << flag << '\n'; */

        /* std::cout << "\n\t--> parameters:\n"; */
        /* for (auto& param : cmdl.params()) */
        /*     std::cout << '\t' << '\t' << param.first << " : " << param.second << '\n'; */
        /* std::cout << std::endl; */
    }

    if (cmdl[{"-h", "--help"}])
    {
        std::cout << "\nUsage: " << cmdl[0] << " [options] input_file_name.rrm [output_file_name]\n";
        std::string usage = R"(
    Reads `.rrm` file and exports tetrahedral mesh as `.vtk` file.  If a rrm
    metadata file is present `.rrm.json` in the same path as the `.rrm` file,
    it is read and any domain information is used to aggregate regions.  Items
    in brackets [] are optional and may be omitted.  Options can appear
    anywhere, but the `input_file` must precede the `output_file`.

    Options:

    "-h", "--help"                                Show this help message and exit.

    "-d", "--default-coordinates"                 Use default coordinates (vertices given in list of {width, length, height} triplets).

    "-o", "--opengl-coordinates"                  Use OpenGL coordinates (vertices given in list of {width, height, length} triplets).

    "-a", "--adapt-discretization"                Adapt discretization to get triangles as equant as possible.

    "-r=value", "--refine-discretization=value"   Refine discretization by `value` (> 0) times in each direction.

    "-w=value", "--width-discretization=value"    Set width discretization to `value` (> 0), overrides "-a" and "-r". 

    "-l=value", "--length-discretization=value"   Set length discretization to `value` (> 0), overrides "-a" and "-r".

    Example:

    To load a model, adapt its mesh and make the mesh 4 times finer in each direction:
    $rrm2vtk -a -r=4 filename.rrm

    )";

        std::cout << usage << std::endl << std::flush;
        return 0;
    }
    else
    {
        std::cout << "`" << cmdl[0] << " -h` for help and usage.\n\n";
        std::cout << std::flush;
    }

    SModeller model;
    if (cmdl[1].empty())
    {
        std::cerr << "Error: input rrm file is missing.\n";
        return 2;
    }

    std::string rrm_model = cmdl[1]; 
    std::cout << "Loading rrm model: " << rrm_model << " -- ";
    std::cout << std::flush;

    bool status = model.loadBinary(rrm_model) || model.loadJSON(rrm_model);
    if ( !status )
    {
        std::cerr << "failed to load file\n";
        return 11;
    }
    std::cout << "success\n";
    std::cout << std::flush;

    std::string output = rrm_model;
    if (!cmdl[2].empty())
    {
        output = cmdl[2];
    }
    std::cout << "Output vtk file: " << std::string(output + ".vtk") << std::endl;
    std::cout << std::flush;

    std::vector<int> domain_map = {};
    std::iota(domain_map.begin(), domain_map.end(), 0);

    std::cout << "Trying to load rrm metadata: " << std::string(rrm_model + ".json") << " -- ";
    std::cout << std::flush;
    json rrm_json;
    std::ifstream rrm_metadata(rrm_model + ".json");
    if (rrm_metadata.good())
    {
        try
        {
            rrm_metadata >> rrm_json;
            std::cout << "success\n";
            std::cout << std::flush;
        }
        catch(std::exception &e)
        {
            std::cerr << "failed to read file\n";
            std::cerr << "Error loading rrm metadata: " << e.what() << std::endl;
            std::cout << std::flush;
        }
        /* std::cout << std::setw(4) << rrm_json["domains"] << std::endl; */

        auto domain_list = rrm_json["domains"].get<std::vector<Domain>>();
        std::size_t expected_number_of_regions = rrm_json["regions"].size();
        domain_map = parse_domains(domain_list, expected_number_of_regions);
    }
    else
    {
        std::cerr << "failed to open file\n";
        std::cout << std::flush;
    }

    size_t width_discretization = model.getWidthDiscretization();
    size_t length_discretization = model.getLengthDiscretization();

    if (cmdl[{"-d", "--default-coordinates"}])
    {
        std::cout << "Using default coordinates (vertices given in list of {width, length, height} triplets)\n";
        std::cout << std::flush;
        model.useDefaultCoordinateSystem();
    }

    if (cmdl[{"-o", "--opengl-coordinates"}])
    {
        std::cout << "Using OpenGL coordinates (vertices given in list of {width, height, length} triplets)\n";
        std::cout << std::flush;
        model.useOpenGLCoordinateSystem();
    }

    if (cmdl[{"-a", "--adapt-discretization"}])
    {
        std::cout << "Adapting discretization to get triangles as equant as possible\n";
        std::cout << std::flush;
        double sx, sy, sz;
        model.getSize(sx, sy, sz);

        adaptDiscretization(sx, sy, width_discretization, length_discretization, width_discretization, length_discretization);
    }

    double refine_discretization = 4.;
    if ((cmdl({"-r", "--refine-discretization"}) >> refine_discretization) && (refine_discretization > 0))
    {
        std::cout << "Refining discretization by " << refine_discretization << " times\n";
        std::cout << std::flush;

        width_discretization = static_cast<size_t>(width_discretization * refine_discretization);
        length_discretization = static_cast<size_t>(length_discretization * refine_discretization);
    }

    double width_disc = 256;
    if ((cmdl({"-w", "--width-discretization"}) >> width_disc) && (width_disc > 0))
    {
        width_discretization = static_cast<size_t>(width_disc);
        std::cout << "Setting width discretization to " << width_discretization << "\n";
        std::cout << std::flush;
    }

    double length_disc = 256;
    if ((cmdl({"-l", "--lenght-discretization"}) >> length_disc) && (length_disc > 0))
    {
        length_discretization = static_cast<size_t>(length_disc);
        std::cout << "Setting length discretization to " << length_discretization << "\n";
        std::cout << std::flush;
    }


    /* std::cout << "\n\n\nTesting...\n"; */
    /* Domain d = {77, {0, 1, 2}}; */
    /* json jd = d; */
    /* std::cout << std::setw(4) << jd << std::endl; */
    /* auto d2 = jd.get<Domain>(); */

    /* if (d != d2) */
    /* { */
    /*     std::cout << "Error!\n"; */
    /* } */
    /* else */
    /* { */
    /*     std::cout << "Worked!\n"; */
    /* } */

    /* json jdl = domain_list; */
    /* std::cout << std::setw(4) << jdl << std::endl; */

    /* assert(d == d2); */

    // Actual logic of utility

    std::cout << "Changing discretization -- ";
    std::cout << std::flush;
    auto time1 = std::chrono::high_resolution_clock::now();
    model.changeDiscretization(width_discretization, length_discretization);
    auto time2 = std::chrono::high_resolution_clock::now();
    std::cout << "done\n";
    std::cout << "Time to change discretization: " 
        << std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count() << " (ms)\n";
    std::cout << std::flush;

    SUtilities u(model);
    std::cout << "Exporting tetrahedral mesh -- ";
    std::cout << std::flush;
    auto time3 = std::chrono::high_resolution_clock::now();
    status = u.exportToVTK(output, domain_map);
    auto time4 = std::chrono::high_resolution_clock::now();
    std::cout << "done\n";
    std::cout << "Time to build mesh and export file: " 
        << std::chrono::duration_cast<std::chrono::milliseconds>(time4 - time3).count() << " (ms)\n";
    std::cout << std::flush;


    // Final tests
    if ( !status )
    {
        std::cout << "Failed to export mesh\n";
        return 12;
    }

    std::cout << "Total time: " 
        << std::chrono::duration_cast<std::chrono::milliseconds>((time2 - time1) + (time4 - time3)).count() << " (ms)\n";

    return 0;
}

void to_json(json& j, const Domain& d)
{
    j = json{{"index", d.index}, {"regions", d.regions}};
}

void from_json(const json& j, Domain& d)
{
    j.at("index").get_to(d.index);
    j.at("regions").get_to(d.regions);
}

bool operator!=(const Domain& lhs, const Domain& rhs)
{
    if (lhs.index != rhs.index)
    {
        std::cerr << "\nlhs.index " << lhs.index << " differs from rhs.index " << rhs.index << "!\n";
        return true;
    }

    if (lhs.regions.size() != rhs.regions.size())
    {
        std::cerr << "\nregions' sizes differ!\n";
        return true;
    }

    for (size_t i = 0; i < lhs.regions.size(); ++i)
    {
        if (lhs.regions[i] != rhs.regions[i])
        {
            std::cerr << "\nlhs.regions[" << i << "] != rhs.regions[" << i << "]!\n";
            return true;
        }
    }

    return false;
}

std::vector<int> parse_domains(const std::vector<Domain>& domain_list, std::size_t expected_number_of_regions)
{
    /* std::vector<int> domain_map = {}; */

    int last_region = 0;
    int last_domain = 0;
    for (std::size_t i = 0; i < domain_list.size(); ++i)
    {
        auto& d = domain_list[i];
        for (std::size_t j = 0; j < d.regions.size(); ++j)
        {
            if (last_region < d.regions[j])
            {
                last_region = d.regions[j];
            }

            if (last_domain < d.index)
            {
                last_domain = d.index;
            }
        }

    }

    if (last_region < last_domain)
    {
        return {};
    }
    
    std::size_t number_of_regions = static_cast<std::size_t>(last_region) + 1;

    if (number_of_regions != expected_number_of_regions)
    {
        std::cerr << "\nWarning: found " 
            << number_of_regions << " regions to be mapped to domains while "
            << expected_number_of_regions << " were expected.\n";
    }

    number_of_regions = std::max(number_of_regions, expected_number_of_regions);

    /* std::size_t number_of_regions = static_cast<std::size_t>(last_region) + 1; */
    /* domain_map.resize(number_of_regions); */
    /* std::iota(domain_map.begin(), domain_map.end(), 0); */

    std::vector<int> domain_map(number_of_regions);
    std::vector<bool> processed_regions(number_of_regions, false);

    for (std::size_t i = 0; i < domain_list.size(); ++i)
    {
        auto& d = domain_list[i];
        for (std::size_t j = 0; j < d.regions.size(); ++j)
        {
            domain_map[d.regions[j]] = d.index;
            processed_regions[d.regions[j]] = true;
        }
    }

    for (std::size_t i = 0; i < domain_map.size(); ++i)
    {
        if (!processed_regions[i])
        {
            domain_map[i] = ++last_domain;
        }
    }

    return domain_map;
}

// Function "adaptDiscretization" below controls the base
// discretization that will be used to create the piecewise linear complex.
// Notice that every "block" is comprised of 8 triangles.
//
// "adaptDiscretization" will use the suggested value "input_width_disc"
// ("input_length_disc") to discretize the model's width (length) if the
// model's width size is bigger than its length size (and vice-versa for
// length).  The other dimension will be discretized with blocks as close
// to a square as possible.  
//

//
// Example: 
// 
// o*******o*******o*******o*******o  -
// *\v6    |v7    /*v8             *  |
// * \  t5 | t7  / *               *  |
// *  \    |    /  *               *  |
// *   \   |   /   *               *  |
// * t4 \  |  / t6 *               *  |
// *     \ | /     *               *  |
// *      \|/      *               *  |
// o-------o-------o       o       o  | length_discretization = 1
// *v3    /|\v4    *v5             *  |
// *     / | \     *               *  |
// * t1 /  |  \ t3 *               *  |
// *   /   |   \   *               *  |
// *  /    |    \  *               *  |
// * /  t0 | t2  \ *               *  |
// */      |      \*               *  |
// o*******o*******o*******o*******o  -
//  v0      v1      v2
//
// |-------------------------------|
//    width_discretization = 2
//
// Legend: 
//      blocks' boundaries are market with: '*'
//      triangles' boundaries are marked with: '|', '\', '/', '-'
//      vertices are marked with: 'o'
// 
//

bool adaptDiscretization( 
        double model_width, double model_length, 
        size_t &output_width_disc, size_t &output_length_disc, 
        size_t input_width_disc, size_t input_length_disc)
{
    if ( model_width >= model_length )
    {
        // will have "input_width_disc" blocks in width
        output_width_disc = input_width_disc;
        auto block_size = model_width/static_cast<double>(output_width_disc); 

        // how many blocks fit in the length dimension of the model
        auto num_blocks = model_length/block_size;

        if ( num_blocks < 1 )
        {
            output_length_disc = 1;
        }
        else
        {
            output_length_disc = std::round(num_blocks);
        }
    }
    else
    {
        // will have "input_length_disc" blocks in length
        output_length_disc = input_length_disc;
        auto block_size = model_length/static_cast<double>(output_length_disc);

        // how many blocks fit in the width dimension of the model
        auto num_blocks = model_width/block_size;

        if ( num_blocks < 1 )
        {
            output_width_disc = 1;
        }
        else
        {
            output_width_disc = std::round(num_blocks);
        }
    }

    double input_blocks = input_width_disc*input_length_disc;
    input_blocks = std::max(input_blocks, 1.);
    double output_blocks = output_width_disc*output_length_disc;
    output_blocks = std::max(output_blocks, 1.);

    /* std::cout << "\t\tWidth discretization in: " << input_width_disc << " -- out: " << output_width_disc << std::endl; */
    /* std::cout << "\t\tLength discretization in: " << input_length_disc << " -- out: " << output_length_disc << std::endl; */

    double factor = std::sqrt(input_blocks/output_blocks);
    output_width_disc = std::ceil(factor*output_width_disc);
    output_length_disc = std::ceil(factor*output_length_disc);

    /* std::cout << "\t\tWidth discretization in: " << input_width_disc << " -- out: " << output_width_disc << std::endl; */
    /* std::cout << "\t\tLength discretization in: " << input_length_disc << " -- out: " << output_length_disc << std::endl; */
    /* std::cout << "\t\tFactor: " << factor << std::endl; */

    return true;

};


