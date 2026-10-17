#pragma once

#include <container/seadPtrArray.h>

#include "xlink2/xlink2Event.h"

namespace aal {
class Handle;
}

namespace uking::action { class XLinkEventCreateAction; }

namespace xlink2 {
class AssetExecutorSLink;
class HandleSLink;
class EventSLink : public Event {
public:
    ~EventSLink() override = default;

    void initializeImpl_() override;
    
    bool getPos(sead::Vector3f*) const;
    const sead::Vector3f& getVelocity() const;

    AssetExecutorSLink* getAliveAssetExecutor() const;
    s32 getAliveAssetExecutor(sead::PtrArray<AssetExecutorSLink>*) const;

    s32 getSoundHandle(sead::PtrArray<aal::Handle>*) const;

private:
    friend class HandleSLink;
    // XLinkEventCreateAction::oneShot_ 0x7100d35e2c writes mBitFlag2 bit 8, mPosition and
    // mBitFlag3 bit 8 in that order (event +0x98 / +0xbc..+0xc4 / +0x9a).
    friend class uking::action::XLinkEventCreateAction;

    alignas(4) sead::BitFlag16 mBitFlag2 {0};
    sead::BitFlag16 mBitFlag3 {0};
    f32 mVolumeScale {1.0};
    f32 mVolumeTvScale {-1.0};
    f32 mPitchScale {1.0};
    f32 mLpfScale {0.0};
    f32 mAngle {0.0};
    f32 mPriorityScale {1.0};
    u32 mBiquadType {0};
    f32 mBiquadValue {0.0};
    sead::Vector3f mPosition {0, 0, 0};
    sead::Vector3f mVelocity {0, 0, 0};
    void* _0xd8 {};
    u16 _0xe0 {0};
};
static_assert(sizeof(EventSLink) == 0xe8, "xlink2::EventSLink size mismatch");

}  // namespace xlink2