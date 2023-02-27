#ifndef STRATMOD_CURVE3D_CEREAL
#define STRATMOD_CURVE3D_CEREAL

#include "detail/serialization_definitions.hpp"

#include "stratmod/curve3d.hpp"

namespace stratmod {

    /* template<typename Archive> */
    /* void Region::save(Archive& ar, const std::uint32_t /1* version *1/) const */
    /* { */
    /*     ar(pimpl_) */
    /* } */

    /* template<typename Archive> */
    /* void Region::load(Archive& ar, const std::uint32_t /1* version *1/) */
    /* { */
    /*     ar(pimpl_); */
    /* } */

    template<typename Archive>
    void serialize(Archive& ar, Curve3D& c, const std::uint32_t /* version */)
    {
        ar(c.vertices);
        ar(c.edges);
        ar(c.metadata);
    }

} // namespace stratmod

CEREAL_CLASS_VERSION(stratmod::Curve3D, 0);

#endif
