#ifndef BUILD_WITH_CEREAL

namespace cereal {
    class access;
}

/* template<typename Archive> */
/* void ClassName::serialize( Archive &archive, const std::uint32_t version) */
/* { */
/*     (void)(version); */
/*     (void)(archive); */
/* } */

#include "interpolant_2d.hpp" 

template<typename Archive>
void Interpolant2D::serialize( Archive &archive, const std::uint32_t version)
{
    (void)(archive);
    (void)(version);
}


#include "interpolated_graph.hpp"

/* template<typename Archive> */
/* void InterpolatedGraph::serialize( Archive &archive, const std::uint32_t version) */
/* { */
/*     (void)(version); */
/*     (void)(archive); */
/* } */


#include "planar_surface.hpp"

/* template<typename Archive> */
/* void PlanarSurface::serialize( Archive &archive, const std::uint32_t version) */
/* { */
/*     (void)(version); */
/*     (void)(archive); */
/* } */


#include "srules.hpp"

/* template<typename Archive> */
/* void SRules::serialize( Archive &archive, const std::uint32_t version) */
/* { */
/*     (void)(version); */
/*     (void)(archive); */
/* } */



#else

#include "cereal/cereal.hpp"
#include "core.hpp"


/* Serializing core.hpp */

template<typename Archive>
void serialize( Archive &ar, Point2 &p, const std::int32_t version )
{
    (void)(version);
    ar( p.data[0], p.data[1] );
}

template<typename Archive>
void serialize( Archive &ar, Point4 &p, const std::int32_t version )
{
    (void)(version);
    ar( p.data[0], p.data[1], p.data[2], p.data[3] );
}


/* Serializing interpolant_2d.hpp */

#include "interpolant_2d.hpp" 
#include "cereal/types/vector.hpp"

template<typename Archive>
void Interpolant2D::serialize( Archive &archive, const std::uint32_t version )
{
    (void)(version);
    archive(dim_, poly_dim_, points_, fevals_, weights_, interpolant_is_set_);
}


/* Serializing interpolated_graph.hpp */

#include "interpolated_graph.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/list.hpp"
#include "cereal/types/set.hpp"
#include "cereal/types/memory.hpp"

template<typename Archive>
void InterpolatedGraph::serialize( Archive &archive, const std::uint32_t version)
{
    (void)(version);
    archive(num_instances_, id_, 
            surface_is_set_, 
            path_is_set_, 
            f, 
            path, path_origin, 
            upper_bound_, lower_bound_, 
            dependency_list_, 
            extruded_surface_
           );
}


/* Serialize planar_surface.hpp */

#include "planar_surface.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/list.hpp"
#include "cereal/types/memory.hpp"

/* template<typename Archive> */
/* void PlanarSurface::serialize( Archive &archive, const std::uint32_t version) */
/* { */
/*     (void)(version); */
/*     archive(discretization_X, discretization_Y, tolerance, */ 
/*             global_discretization_state_, */ 
/*             this_discretization_state_, */ 
/*             nX_, nY_, num_vertices_, */ 
/*             num_instances_, id_, */ 
/*             f, */ 
/*             coordinates_map_, */ 
/*             origin, lenght, */ 
/*             heights, */ 
/*             interpolant_is_set_, */ 
/*             mesh_is_set_, */ 
/*             upper_bound_, lower_bound_, */ 
/*             dependency_list_, */ 
/*             extruded_surface_ */
/*            ); */
/* } */


/* Serialize srules.hpp */

#include "srules.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/unordered_map.hpp"

template<typename Archive>
void SRules::serialize( Archive &archive, const std::uint32_t version)
{
    (void)(version);
    archive(container, dictionary,
            lower_bound_, define_above_, 
            upper_bound_, define_below_
           );
}


#endif
