#ifndef PLANIN_SERIALIZATION_PRIMITIVES_HPP
#define PLANIN_SERIALIZATION_PRIMITIVES_HPP


#if defined( BUILD_WITH_SERIALIZATION )
    #include "cereal/cereal.hpp"

#else /* BUILD_WITH_SERIALIZATION */ 
    #define CEREAL_CLASS_VERSION(...) 

    namespace cereal {
        class access {};
    }

#endif /* BUILD_WITH_SERIALIZATION */ 


#endif
