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

    // Inline-only in the original; the debug string establishes the member name.
    // Original 129864 and 7086f4 repeat the event/create-id check, logging and system fade.
    void fadeIfLoopEffect()
    {
        auto* event = getEvent();
        if (event && event->getCreateId() == getCreateId()) {
            auto* user_instance = event->getUserInstance();
            user_instance->checkAndErrorCallInCalc(
                "HandleELink::fadeIfLoopEffect(%s)",
                solveOffset<char>(event->getAssetCallTable()->keyNamePos));
            user_instance->printLogFadeOrKill(
                getEvent(), "HandleELink::fadeIfLoopEffect(%s)",
                solveOffset<char>(getEvent()->getAssetCallTable()->keyNamePos));
            getEvent()->fadeBySystem();
        }
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

    void kill()
    {
        auto* event = getEvent();
        if (event && event->getCreateId() == getCreateId()) {
            auto* user_instance = event->getUserInstance();
            user_instance->checkAndErrorCallInCalc(
                "HandleELink::kill(%s)",
                solveOffset<char>(event->getAssetCallTable()->keyNamePos));
            user_instance->printLogFadeOrKill(
                getEvent(), "HandleELink::kill(%s)",
                solveOffset<char>(getEvent()->getAssetCallTable()->keyNamePos));
            getEvent()->kill();
        }
    }

    void setPosition(const sead::Vector3f& position, f32 scale = 1.0f)
    {
        auto* event = static_cast<EventELink*>(getEvent());
        if (event && event->getCreateId() == getCreateId()) {
            event->mDelayEmitParam.position = position;
            event->mDelayEmitParam.scale.set(scale, scale, scale);
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

    /// Same as setMatrix(matrix), but with a scale vector (inline-only in the original: the event's delay emit parameter
    /// gets the matrix and the scale, then flag1 bit 3).
    void setMatrix(const sead::Matrix34f& matrix, const sead::Vector3f& scale)
    {
        auto* event = static_cast<EventELink*>(getEvent());
        if (event && event->getCreateId() == getCreateId()) {
            event->mDelayEmitParam.matrix34f = matrix;
            event->mDelayEmitParam.scale = scale;
            event->mDelayEmitParam.flag1.setBit(3);
        }
    }

    /// Sets the delay emit parameter `_0xdc` and flag1 bit 21 (inline-only in the original; the name is a placeholder,
    /// requested for MoonMove::calc_).
    void setDelayEmitParam0xDC(s32 value)
    {
        auto* event = static_cast<EventELink*>(getEvent());
        if (event && event->getCreateId() == getCreateId()) {
            event->mDelayEmitParam._0xdc = value;
            event->mDelayEmitParam.flag1.setBit(21);
        }
    }

    sead::Vector3f setMtxUp(const sead::Vector3f&, const sead::Vector3f&, f32);
    HandleELink* setMtxZ(const sead::Vector3f&, const sead::Vector3f&, f32);
};
static_assert(sizeof(HandleELink) == 0x10, "xlink2::HandleELink size mismatch");

}  // namespace xlink2