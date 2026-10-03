#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "xlink2/xlink2EventELink.h"
#include "xlink2/xlink2Handle.h"
#include "xlink2/xlink2Util.h"
#include "xlink2/xlink2UserInstanceELink.h"

namespace xlink2 {
class HandleELink : public Handle /*, nn::vfx::Handle*/{
public:
    using Handle::Handle;

    HandleELink(UserInstanceELink* user_instance, const char* asset_key_name)
    {
        user_instance->searchAndHold(asset_key_name, this);
    }

    void fade()
    {
        auto* event = getEvent();
        if (event && event->getCreateId() == getCreateId()) {
            auto* user_instance = event->getUserInstance();
            user_instance->checkAndErrorCallInCalc(
                "HandleELink::fade(%s)",
                solveOffset<char>(event->getAssetCallTable()->keyNamePos));
            user_instance->printLogFadeOrKill(
                getEvent(), "HandleELink::fade(%s)",
                solveOffset<char>(getEvent()->getAssetCallTable()->keyNamePos));
            getEvent()->fade(-1);
        }
    }

    void setPosition(const sead::Vector3f& position)
    {
        auto* event = static_cast<EventELink*>(getEvent());
        if (event && event->getCreateId() == getCreateId()) {
            event->mDelayEmitParam.position = position;
            event->mDelayEmitParam.scale.set(1.0f, 1.0f, 1.0f);
            event->mDelayEmitParam.flag1.setBit(2);
        }
    }

    void setMatrix(const sead::Matrix34f& matrix)
    {
        auto* event = static_cast<EventELink*>(getEvent());
        if (event && event->getCreateId() == getCreateId()) {
            event->mDelayEmitParam.matrix34f = matrix;
            event->mDelayEmitParam.scale.set(1.0f, 1.0f, 1.0f);
            event->mDelayEmitParam.flag1.setBit(3);
        }
    }

    sead::Vector3f setMtxUp(const sead::Vector3f&, const sead::Vector3f&, f32);
    HandleELink* setMtxZ(const sead::Vector3f&, const sead::Vector3f&, f32);
};
static_assert(sizeof(HandleELink) == 0x10, "xlink2::HandleELink size mismatch");

}  // namespace xlink2