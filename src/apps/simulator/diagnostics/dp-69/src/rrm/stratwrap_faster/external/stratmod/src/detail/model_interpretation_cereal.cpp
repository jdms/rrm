#include "detail/model_interpretation_cereal.hpp"
#include "stratmod/model_interpretation.hpp"

// Template specializations for testing.

template void stratmod::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, stratmod::Curve3D&, const std::uint32_t);
template void stratmod::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, stratmod::Curve3D&, const std::uint32_t);

template void stratmod::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, stratmod::Domain&, const std::uint32_t);
template void stratmod::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, stratmod::Domain&, const std::uint32_t);

template void stratmod::ModelInterpretation::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, const std::uint32_t);
template void stratmod::ModelInterpretation::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, const std::uint32_t);
