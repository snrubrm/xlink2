#include "xlink2/xlink2PtclResourceAccessorELink.h"
#include <nn/vfx/Resource.h>
#include <ptcl/seadPtclSystem.h>

namespace xlink2 {

bool PtclResourceAccessorELink::searchEmitterSetID(sead::ptcl::PtclSystem* system, const char* name,
                                                   s32& resource_id, s32& emitter_set_id) {
    resource_id = -1;
    emitter_set_id = -1;
    const s32 num = system->GetResourceNumMaybe();
    for (s32 i = 0; i < num; ++i) {
        const nn::vfx::Resource* resource = system->GetResourceMaybe(i);
        if (!resource || !resource->IsAvailableMaybe())
            continue;
        const s32 id = system->SearchEmitterSetId(name, i);
        if (id >= 0) {
            resource_id = i;
            emitter_set_id = id;
            return true;
        }
    }
    return false;
}

}  // namespace xlink2
