#pragma once

#include "gfx/seadColor.h"

#include "xlink2/xlink2Event.h"
#include "xlink2/xlink2UserInstanceELink.h"

namespace uking::action { class XLinkEventCreateAction; }
namespace ksys::act {
class Actor;
class Chemical;
class Unk_71024e6428;
}

namespace xlink2 {
class AssetExecutorELink;
class HandleELink;

class EventELink : public Event {
public:
    ~EventELink() override = default;

    // Effect::eventCreate 0x71011b1c14 allocates this 0x70-byte record and
    // installs it at event +0x190. Chemical state 0x7100e3b708 supplies the
    // actor/chemical pointers and sets bit 3 of mFlags. Other fields stay opaque.
    struct DelayEmitParamUnkF8 {
        u8 _0[8];
        ksys::act::Actor* mActor;
        ksys::act::Chemical* mChemical;
        f32 _18;
        u8 _1c[4];
        u32 _20;
        u32 mFlags;
        u8 _28[0x70 - 0x28];
    };
    static_assert(sizeof(DelayEmitParamUnkF8) == 0x70);

    struct DelayEmitParam {
        sead::Color4f color;
        sead::Vector3f position;
        sead::Vector3f scale;
        sead::Matrix34f matrix34f;
        void* _4;
        u32 _5;
        f32 emissionIntervalScale;
        u8 _0x68[12];
        // Set together with flag1 bit 10 by GameSceneSubsys5 0x7100905f9c (1.0 and its argument).
        f32 _0x74;
        f32 _0x78;
        u8 _0x7c[4];
        f32 directionalVel;
        u8 _0x84[44];
        sead::Color4f color0;
        sead::Color4f color1;
        u8 _0xd0[8];
        u32 _0xd8;
        s32 _0xdc;
        bool _0xe0;
        s32 particleCount;
        sead::BitFlag32 flag1;
        sead::BitFlag32 flag2;
        u8 _0xf0;
        DelayEmitParamUnkF8* _0xf8;

    };
    static_assert(sizeof(DelayEmitParam) == 0x100, "'xlink2::EventELink::DelayEmitParam' size mismatch");

    void callEventCreateCallback_() override;
    void callEventDestroyCallback_() override;

    void initializeImpl_() override;
    void doFinalize_() override;

    void fixDelayParam_() override;

    s32 getMtxSetType();

    void setManualParticleEmission(bool);
    void setManualParticleEmissionWithParticleCount(int);

    void emitParticle(sead::Vector3f&);
    AssetExecutorELink* getAliveAssetExecutor() const;

    UserInstanceELink* getUserInstanceELink() const { return static_cast<UserInstanceELink*>(mpUserInstance); }

private:
    friend class HandleELink;
    // XLinkEventCreateAction::oneShot_ 0x7100d35e2c ORs 0x10 into the word at +0x20 of the
    // object stored in DelayEmitParam::_0xf8 (event +0x190).
    friend class uking::action::XLinkEventCreateAction;
    friend class ksys::act::Unk_71024e6428;

    DelayEmitParam mDelayEmitParam;
};
static_assert(sizeof(EventELink) == 0x198, "xlink2::EventELink size mismatch");

}  // namespace xlink2