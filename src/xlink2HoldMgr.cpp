#include "xlink2/xlink2HoldMgr.h"
#include "xlink2/xlink2Event.h"

namespace xlink2 {

// NON_MATCHING: scheduling of the fixed-list initialization differs.
HoldMgr::HoldMgr(System* system, sead::Heap*) : mSystem(system), mEnabled(true) {}

HoldMgr::~HoldMgr() = default;

// NON_MATCHING: the original preserves raw list nodes across the body; the iterator preserves objects.
void HoldMgr::calc() {
    if (!mEnabled)
        return;
    auto lock = sead::makeScopedLock(mCriticalSection);
    for (auto it = mHoldAssetInfoList.begin(); it != mHoldAssetInfoList.end();) {
        auto& info = *it;
        ++it;
        if (info._0-- <= 0) {
            Event* event = info.handle.getEvent();
            if (event && event->getCreateId() == info.handle.getCreateId())
                event->fade(-1);
            info.userInstance = nullptr;
            mHoldAssetInfoList.erase(&info);
        }
    }
}

}  // namespace xlink2
