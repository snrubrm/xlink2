#pragma once

#include <basis/seadTypes.h>

namespace xlink2 {
class OneEmitterInstance;

// Returned in two registers: the instance and the handle id it had when the handle was made.
struct OneEmitterInstanceHandle {
    OneEmitterInstance* mInstance = nullptr;
    s32 mId = -1;
};

}  // namespace xlink2
