#pragma once

#include <container/seadObjList.h>

#include "xlink2/xlink2Handle.h"
#include "xlink2/xlink2System.h"

namespace xlink2 {
class HoldMgr : sead::hostio::Node {
public:
    HoldMgr(System*, sead::Heap*);
    virtual ~HoldMgr();

    struct HoldAssetInfo {
        Handle handle;
        UserInstance* userInstance;
        s32 _0;
    };

    u32 searchAndHold(const char*, Handle*, UserInstance*);

    void fade(const char*, UserInstance*, s32);
    void fade_(HoldAssetInfo*, s32);
    void fade(UserInstance*, s32);
    void fadeAll();

    void calc();

    void genMessage(sead::hostio::Context*);


private:
    System* mSystem;
    sead::CriticalSection mCriticalSection;
    sead::FixedObjList<HoldAssetInfo, 128> mHoldAssetInfoList;
    bool mEnabled;
};
static_assert(sizeof(HoldMgr) == 0x1888, "xlink2::HoldMgr size mismatch");

}  // namespace xlink2