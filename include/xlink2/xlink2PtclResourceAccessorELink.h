#pragma once

#include <basis/seadTypes.h>

namespace sead::ptcl {
class PtclSystem;
}

namespace xlink2 {
class PtclResourceAccessorELink {
public:
    bool searchEmitterSetID(sead::ptcl::PtclSystem* system, const char* name, s32& resource_id,
                            s32& emitter_set_id);
};
}  // namespace xlink2
