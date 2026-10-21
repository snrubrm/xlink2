#pragma once

#include <gfx/seadTextWriter.h>
#include "xlink2/xlink2OneEmitterInstance.h"
#include "xlink2/xlink2OneEmitterInstanceHandle.h"

namespace xlink2 {

class OneEmitterMgr {
public:
    OneEmitterInstanceHandle createOneEmitterInstance(const char* name, u8 group);
    OneEmitterInstanceHandle searchOneEmitterInstanceHandle(const char* name);

    s32 createOneEmitterInstanceImpl(const char*, u8);

    void releaseOneEmitterInstance(const char* name);

    OneEmitterInstance* getOneEmitterInstance(OneEmitterInstanceHandle handle);
    OneEmitterInstance* getFreeOneEmitterInstance();

    void drawDebugInformation(sead::TextWriter*);

private:
    OneEmitterInstance mInstances[64];
    // The next handle id (reset to 0 when it overflows).
    s32 mNextHandleId;
};

}  // namespace xlink2
