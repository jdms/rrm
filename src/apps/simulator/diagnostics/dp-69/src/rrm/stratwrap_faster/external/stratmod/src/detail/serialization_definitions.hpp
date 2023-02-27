#ifndef STRATMOD_SERIALIZATION_DEFINITIONS_HPP
#define STRATMOD_SERIALIZATION_DEFINITIONS_HPP


#if defined( BUILD_WITH_SERIALIZATION )
    //
    // The Cereal library is located inside the planin dir
    //
    #include "cereal/cereal.hpp"
    #include "cereal/access.hpp"

    #include "cereal/types/vector.hpp"
    #include "cereal/types/map.hpp"
    #include "cereal/types/unordered_map.hpp"
    #include "cereal/types/optional.hpp"
    #include "cereal/types/memory.hpp"
    #include "cereal/types/polymorphic.hpp"

    #include "cereal/archives/portable_binary.hpp"
    #include "cereal/archives/binary.hpp"
    #include "cereal/archives/xml.hpp"
    #include "cereal/archives/json.hpp"

    #include "detail/eigen_cereal_support.hpp"

    /* using access = cereal::access; */

    /* namespace cereal { */
    /*     namespace detail { */
    /*         template<typename T, typename B> */
    /*         struct Version; */
    /* } */

    /* template<typename T, typename B> */
    /* using version = cereal::detail::Version<T>; */

/* } */


#else /* BUILD_WITH_SERIALIZATION */ 
    /* namespace cereal { */
    /*     namespace detail { */
    /*         template<typename T> */
    /*         struct Version {}; */
    /*     } */
    /* } */

    /* template<typename T> */
    /* using version = cereal::detail::Version<T>; */

    #define CEREAL_CLASS_VERSION(...) 


#endif /* BUILD_WITH_SERIALIZATION */ 


#endif
