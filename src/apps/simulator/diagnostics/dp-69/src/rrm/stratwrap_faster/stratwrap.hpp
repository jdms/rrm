#include <cmath>
#include <string>
#include <tuple>
#include <vector>

#include <stratmod/smodeller.hpp>
#include <stratmod/sutilities.hpp>

/// Wrapper for the classes defined in the stratmod library
class Stratwrap {
   public:
    /// Load a `.rrm` file
    bool load(const std::string& filename);

    /// Get model's bounding box discretization
    std::tuple<std::size_t, std::size_t> getDiscretization();

    /// Change model's bounding box discretization
    // Input: discretization_x is the number of blocks the segment [origin_x,
    // origin_x+size_x] will be discretized with, and similarly for discretization_y
    bool changeDiscretization(std::size_t discretization_x, std::size_t discretization_y);

    /// Get model's origin coordinates
    void getModelOrigin(double& origin_x, double& origin_y, double& origin_z);

    /// Get model's size
    /// Model extends from `origin_x` to `origin_x + size_x`, etc.
    void getModelSize(double& size_x, double& size_y, double& size_z);

    /// Get regions' ids from points in R3
    //
    /// Input: vector with points coordinates [p0x p0y p0z p1x p1y p1z ... pKx pKy pKz];
    /// where p0 = (p0x, p0y, p0z), ..., pK are points
    //
    /// Output: for each point returns -1 if cell lies outside of model or id of region
    /// point lies in otherwise -- ids are numbers in [0, n] where n is the number of
    /// distinct regions
    bool getRegionsCellsIDs(const std::vector<double>& points, std::vector<int>& ids);

    /// Get ids of the surfaces in model
    std::vector<std::size_t> getSurfacesIndices();

    /// Get ids of surfaces in model ordered by their stratigraphic order
    std::vector<std::size_t> getOrderedSurfacesIndices();

    /// Given a surface_id returns its mesh, or false if surface_id does not correspond to
    /// any surface in the model
    ///
    /// vertex_list contains vertex coordinates [v0x v0y v0z v1x v1y v1z ... vKx vKy vKz];
    /// where v0 = (v0x, v0y, p0z), ..., vK are vertices in the mesh
    ///
    /// triangle_list contains triangle connectivity [t0_v0 t0_v1 t0_v2 ... tK_v0 tK_v1
    /// tK_v2]; where triangle's vertices, such as t0 = (v0 v1 v2), are ordered such that
    /// the triangles' notmals point upwards
    bool getMesh(size_t surface_id,
                 std::vector<double>& vertex_list,
                 std::vector<std::size_t>& triangle_list);

    /// Fills in the data structures used by the FlowDiagnosticsInterface class to build a
    /// tetrahedral mesh
    template <typename TriangleMesh, typename CurveMesh>
    bool setPLCForSimulation(std::vector<TriangleMesh>& triangle_meshes,
                             std::vector<CurveMesh>& left_curves,
                             std::vector<CurveMesh>& right_curves,
                             std::vector<CurveMesh>& front_curves,
                             std::vector<CurveMesh>& back_curves,
                             size_t length_discretization = 16,
                             size_t width_discretization = 16);

  SModeller model;
   private:
    
};

/// Loads a RRM saved file
inline bool Stratwrap::load(const std::string& filename)
{
    bool success = model.loadBinary(filename);
    if (!success) {
        success = model.loadJSON(filename);

        // Force coordinate system in which heights are stored in the `z` coordinate
        model.useDefaultCoordinateSystem();
    }

    return success;
}

/// Change model's bounding box discretization
// Input: discretization_x is the number of blocks the segment [origin_x,
// origin_x+size_x] will be discretized with, and similarly for discretization_y
inline bool Stratwrap::changeDiscretization(std::size_t discretization_x,
                                     std::size_t discretization_y)
{
    return model.changeDiscretization(discretization_x, discretization_y);
}

/// Get model's bounding box discretization
inline std::tuple<std::size_t, std::size_t> Stratwrap::getDiscretization()
{
    return std::make_tuple<std::size_t, std::size_t>(model.getWidthDiscretization(),
                                                     model.getLengthDiscretization());
}

/// Get model's origin coordinates
inline void Stratwrap::getModelOrigin(double& origin_x, double& origin_y, double& origin_z)
{
    model.getOrigin(origin_x, origin_y, origin_z);
}

/// Get model's size
/// Model extends from `origin_x` to `origin_x + size_x`, etc.
inline void Stratwrap::getModelSize(double& size_x, double& size_y, double& size_z)
{
    model.getSize(size_x, size_y, size_z);
}

/// Get regions' ids from points in R3
//
/// Input: vector with points coordinates [p0x p0y p0z p1x p1y p1z ... pKx pKy pKz];
/// where p0 = (p0x, p0y, p0z), ..., pK are points
//
/// Output: for each point returns -1 if cell lies outside of model or id of region
/// point lies in otherwise -- ids are numbers in [0, n] where n is the number of
/// distinct regions
inline bool Stratwrap::getRegionsCellsIDs(const std::vector<double>& points,
                                   std::vector<int>& ids)
{
    return model.getVolumeAttributesFromPointList(points, ids);
}

/// Get ids of the surfaces in model
inline std::vector<std::size_t> Stratwrap::getSurfacesIndices()
{
    return model.getSurfacesIndices();
}

/// Get ids of surfaces in model ordered by their stratigraphic order
inline std::vector<std::size_t> Stratwrap::getOrderedSurfacesIndices()
{
    return model.getOrderedSurfacesIndices();
}

/// Given a surface_id returns its mesh, or false if surface_id does not correspond to
/// any surface in the model
inline bool Stratwrap::getMesh(size_t surface_id,
                        std::vector<double>& vertex_list,
                        std::vector<std::size_t>& triangle_list)
{
    return model.getMesh(surface_id, vertex_list, triangle_list);
}

/// Fills in the data structures used by the FlowDiagnosticsInterface class to build a
/// tetrahedral mesh, for convenience I copy the definitions found in class
/// FlowDiagnosticsInterface bellow.

/* FlowDiagnosticsInterface class surfaces description for tetrahedral meshing */
/* struct TriangleMesh { */
/*     std::vector<double> vertex_list;  // V x 3 list of vertex coordinates, where V is
 */
/*                                       // number of vertices */
/*     std::vector<std::size_t> */
/*         face_list;  // T x 3 list of triangle indices, where T is number of triangles
 */
/* }; */

/* Intersections of RRM's surfaces with the bounding box boundaries for tetrahedral */
/* meshing */
/* struct CurveMesh { */
/*     std::vector<double> vertex_list;  // V x 3 list of vertex coordinates, where V is
 */
/*                                       // number of vertices */
/*     std::vector<std::size_t> */
/*         edge_list;  // E x 2 list of edge indices, where E is number of edges */
/* }; */

template <typename TriangleMesh, typename CurveMesh>
bool Stratwrap::setPLCForSimulation(std::vector<TriangleMesh>& triangle_meshes,
                                    std::vector<CurveMesh>& left_curves,
                                    std::vector<CurveMesh>& right_curves,
                                    std::vector<CurveMesh>& front_curves,
                                    std::vector<CurveMesh>& back_curves,
                                    size_t length_discretization,
                                    size_t width_discretization)
{
    //
    // Reduce resolution for simulation
    //

    //
    // The function "adaptDiscretization" below controls the base
    // discretization that will be used to create the piecewise linear complex.
    // Notice that every "block" is comprised of 8 triangles.
    //
    // "adaptDiscretization" will use the suggested value "max_width_disc"
    // ("max_length_disc") to discretize the model's width (length) if the
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

    std::size_t diagnostics_width_discretization_ = length_discretization;
    std::size_t diagnostics_length_discretization_ = width_discretization;

    std::size_t modelling_width_discretization_ = model.getWidthDiscretization();
    std::size_t modelling_length_discretization_ = model.getLengthDiscretization();

    auto adaptDiscretization = [](double model_width,
                                  double model_length,
                                  size_t& output_width_disc,
                                  size_t& output_length_disc,
                                  size_t max_width_disc /*= 16*/,
                                  size_t max_length_disc /*= 16*/) -> bool {
        if (model_width >= model_length) {
            // will have "max_width_disc" blocks in width
            output_width_disc = max_width_disc;
            auto block_size = model_width / static_cast<double>(output_width_disc);

            // how many blocks fit in the length dimension of the model
            auto num_blocks = model_length / block_size;

            if (num_blocks < 1) {
                output_length_disc = 1;
            }
            else {
                output_length_disc = std::round(num_blocks);
            }
        }
        else {
            // will have "max_length_disc" blocks in length
            output_length_disc = max_length_disc;
            auto block_size = model_length / static_cast<double>(output_length_disc);

            // how many blocks fit in the width dimension of the model
            auto num_blocks = model_width / block_size;

            if (num_blocks < 1) {
                output_width_disc = 1;
            }
            else {
                output_width_disc = std::round(num_blocks);
            }
        }

        return true;
    };

    double model_width, model_length, height;
    model.getSize(model_width, height, model_length);

    // I would suggest to not reduce the values of max_width_disc and max_length_disc
    size_t max_width_disc = 16, max_length_disc = 16;

    adaptDiscretization(model_width,
                        model_length,
                        diagnostics_width_discretization_,
                        diagnostics_length_discretization_,
                        max_width_disc,
                        max_length_disc);

    model.changeDiscretization(diagnostics_width_discretization_,
                               diagnostics_length_discretization_);

    //
    // Get the PLC
    //

    std::vector<std::size_t> surfacesIDs = model.getSurfacesIndices();

    for (auto it : surfacesIDs) {
        TriangleMesh t;

        bool has_surface = model.getMesh(it, t.vertex_list, t.face_list);
        if (has_surface == false) continue;

        //
        // This loop changes the y-z coordinates of the vertices as RRM
        // understands the y coordinate as height and the z coordinate as
        // length, but Zhao's convention is the opposite.
        //
        double y, z;
        for (size_t i = 0; i < t.vertex_list.size() / 3; ++i) {
            y = t.vertex_list[3 * i + 1];
            z = t.vertex_list[3 * i + 2];

            t.vertex_list[3 * i + 1] = z;
            t.vertex_list[3 * i + 2] = y;
        }

        triangle_meshes.push_back(t);
    }

    std::vector<std::vector<double>> lb_vertex_lists, rb_vertex_lists, fb_vertex_lists,
        bb_vertex_lists;
    std::vector<std::vector<std::size_t>> lb_edge_lists, rb_edge_lists, fb_edge_lists,
        bb_edge_lists;

    SUtilities u(model);
    u.getLeftBoundary3DCurves(lb_vertex_lists, lb_edge_lists);
    u.getRightBoundary3DCurves(rb_vertex_lists, rb_edge_lists);
    u.getFrontBoundary3DCurves(fb_vertex_lists, fb_edge_lists);
    u.getBackBoundary3DCurves(bb_vertex_lists, bb_edge_lists);

    for (size_t i = 0; i < lb_vertex_lists.size(); ++i) {
        CurveMesh cm_lb, cm_rb, cm_fb, cm_bb;

        std::copy(lb_vertex_lists[i].begin(),
                  lb_vertex_lists[i].end(),
                  std::back_inserter(cm_lb.vertex_list));
        std::copy(lb_edge_lists[i].begin(),
                  lb_edge_lists[i].end(),
                  std::back_inserter(cm_lb.edge_list));

        std::copy(rb_vertex_lists[i].begin(),
                  rb_vertex_lists[i].end(),
                  std::back_inserter(cm_rb.vertex_list));
        std::copy(rb_edge_lists[i].begin(),
                  rb_edge_lists[i].end(),
                  std::back_inserter(cm_rb.edge_list));

        std::copy(fb_vertex_lists[i].begin(),
                  fb_vertex_lists[i].end(),
                  std::back_inserter(cm_fb.vertex_list));
        std::copy(fb_edge_lists[i].begin(),
                  fb_edge_lists[i].end(),
                  std::back_inserter(cm_fb.edge_list));

        std::copy(bb_vertex_lists[i].begin(),
                  bb_vertex_lists[i].end(),
                  std::back_inserter(cm_bb.vertex_list));
        std::copy(bb_edge_lists[i].begin(),
                  bb_edge_lists[i].end(),
                  std::back_inserter(cm_bb.edge_list));

        left_curves.push_back(cm_lb);
        right_curves.push_back(cm_rb);
        front_curves.push_back(cm_fb);
        back_curves.push_back(cm_bb);
    }

    // Use the following to return to the model original discretization
    model.changeDiscretization(modelling_width_discretization_,
                               modelling_length_discretization_);

    return true;
}
