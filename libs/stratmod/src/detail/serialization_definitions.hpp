#ifndef __SERIALIZATION_DEFINITIONS__
#define __SERIALIZATION_DEFINITIONS__


#if defined( BUILD_WITH_SERIALIZATION )
    //
    // The Cereal library is located inside the planin dir
    //
    #include <cereal/cereal.hpp>

    #include <cereal/types/vector.hpp>
    #include <cereal/types/map.hpp>

    #include <cereal/archives/portable_binary.hpp>
    #include <cereal/archives/binary.hpp>
    #include <cereal/archives/xml.hpp>
    #include <cereal/archives/json.hpp>

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
