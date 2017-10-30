#ifndef __SERIALIZATION_DEFINITIONS__
#define __SERIALIZATION_DEFINITIONS__


#if defined( BUILD_WITH_SERIALIZATION )
    #include "planin/cereal/cereal.hpp"

    #include "planin/cereal/types/vector.hpp"
    #include "planin/cereal/types/map.hpp"

    #include "planin/cereal/archives/portable_binary.hpp"
    #include "planin/cereal/archives/binary.hpp"
    #include "planin/cereal/archives/xml.hpp"
    #include "planin/cereal/archives/json.hpp"

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
