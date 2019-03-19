#ifndef __SERIALIZATION_DEFINITIONS__
#define __SERIALIZATION_DEFINITIONS__


#if defined( BUILD_WITH_SERIALIZATION )
    //
    // The Cereal library is located inside the planin dir
    //
    #include "detail/planin/cereal/cereal.hpp"

    #include "detail/planin/cereal/types/vector.hpp"
    #include "detail/planin/cereal/types/map.hpp"

    #include "detail/planin/cereal/archives/portable_binary.hpp"
    #include "detail/planin/cereal/archives/binary.hpp"
    #include "detail/planin/cereal/archives/xml.hpp"
    #include "detail/planin/cereal/archives/json.hpp"

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
