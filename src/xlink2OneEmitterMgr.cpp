#include "xlink2/xlink2OneEmitterMgr.h"
#include <cstring>
#include <nn/vfx/EmitterSet.h>
#include <ptcl/seadPtclSystem.h>
#include "xlink2/xlink2SystemELink.h"

namespace xlink2 {

// NON_MATCHING: block layout and register choice around the found/free paths.
OneEmitterInstanceHandle OneEmitterMgr::createOneEmitterInstance(const char* name, u8 group) {
    OneEmitterInstanceHandle handle = searchOneEmitterInstanceHandle(name);
    if (!getOneEmitterInstance(handle)) {
        OneEmitterInstance* instance = getFreeOneEmitterInstance();
        if (instance) {
            const u8 group_id = group == 0xff ? 0 : group;
            const nn::vfx::System* system = SystemELink::instance()->getPtclSystem();
            const s32 num = system->GetResourceNumMaybe();
            for (s32 i = 0; i < num; ++i) {
                std::strcpy(instance->mName, name);
                instance->mEmitterSetId = system->SearchEmitterSetId(instance->mName, i);
                if (instance->mEmitterSetId != -1) {
                    instance->mResourceId = i;
                    instance->mGroupId = group_id;
                    instance->mHandleId = mNextHandleId++;
                    if (mNextHandleId < 0)
                        mNextHandleId = 0;
                    handle.mInstance = instance;
                    handle.mId = instance->mHandleId;
                    break;
                }
            }
        }
    }
    if (OneEmitterInstance* instance = getOneEmitterInstance(handle))
        ++instance->mRefCount;
    return handle;
}

OneEmitterInstanceHandle OneEmitterMgr::searchOneEmitterInstanceHandle(const char* name) {
    OneEmitterInstanceHandle handle;
    for (s32 i = 0; i < 64; ++i) {
        if (std::strcmp(mInstances[i].mName, name) == 0) {
            handle.mInstance = &mInstances[i];
            handle.mId = mInstances[i].mHandleId;
            return handle;
        }
    }
    return handle;
}

// NON_MATCHING: the original addresses the instance through its name pointer.
void OneEmitterMgr::releaseOneEmitterInstance(const char* name) {
    OneEmitterInstance* instance = searchOneEmitterInstanceHandle(name).mInstance;
    if (!instance || instance->mRefCount <= 0)
        return;
    if (--instance->mRefCount != 0)
        return;
    nn::vfx::EmitterSet* set = instance->mEmitterSet;
    if (set && instance->mEmitterSetCreateId == set->GetCreateId())
        set->Kill(true);
    instance->mRefCount = 0;
    instance->mHandleId = -1;
    instance->mName[0] = '\0';
}

OneEmitterInstance* OneEmitterMgr::getOneEmitterInstance(OneEmitterInstanceHandle handle) {
    if (!handle.mInstance)
        return nullptr;
    return handle.mInstance->mHandleId == handle.mId ? handle.mInstance : nullptr;
}

OneEmitterInstance* OneEmitterMgr::getFreeOneEmitterInstance() {
    for (s32 i = 0; i < 64; ++i) {
        if (mInstances[i].mName[0] == '\0')
            return &mInstances[i];
    }
    return nullptr;
}

}  // namespace xlink2
