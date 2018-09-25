#ifndef __SERIALIZATION_PRIMITIVES__
#define __SERIALIZATION_PRIMITIVES__


#if defined( BUILD_WITH_SERIALIZATION )
    #include "cereal/cereal.hpp"

#else /* BUILD_WITH_SERIALIZATION */ 
    #define CEREAL_CLASS_VERSION(...) 

    namespace cereal {
        class access {};
    }

#endif /* BUILD_WITH_SERIALIZATION */ 


#endif
