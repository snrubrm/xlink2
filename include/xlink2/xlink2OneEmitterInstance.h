#pragma once

#include <basis/seadTypes.h>
#include <math/seadVectorFwd.h>

namespace nn::vfx {
class EmitterSet;
}

namespace xlink2 {

// One shared emitter set (0x1470 bytes, the OneEmitterMgr array stride). No vtable: the manager
// stores the emitter set id at +0x0.
class OneEmitterInstance {
    friend class OneEmitterMgr;

public:
    void emitParticle(const sead::Vector3f&, const sead::Vector4f&, const sead::Vector2f&);

private:
    s32 mEmitterSetId;
    s32 mResourceId;
    u8 mGroupId;
    s32 mRefCount;
    s32 mHandleId;
    nn::vfx::EmitterSet* mEmitterSet;
    // Compared with the emitter set's create id before killing it.
    s32 mEmitterSetCreateId;
    u32 _24;
    // The asset name (strcmp / strcpy); its length and anything after it are not known.
    char mName[0x1470 - 0x28];
};
static_assert(sizeof(OneEmitterInstance) == 0x1470);

}  // namespace xlink2
