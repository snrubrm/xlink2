#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "xlink2/xlink2EventSLink.h"
#include "xlink2/xlink2Handle.h"
#include "xlink2/xlink2UserInstanceSLink.h"
#include "xlink2/xlink2Util.h"

namespace xlink2 {
class HandleSLink : public Handle /* aal::Handle*/ {
public:
    using Handle::Handle;

    HandleSLink(UserInstanceSLink* user_instance, const char* asset_key_name)
    {
        user_instance->searchAndHold(asset_key_name, this);
    }

    // Inline-only in the original; the debug string establishes the member name.
    // Original 129864, 7086f4 and 708bc4 repeat the event/create-id check, logging and system fade.
    void fadeIfLoopSound()
    {
        auto* event = getEvent();
        if (event && event->getCreateId() == getCreateId()) {
            auto* user_instance = event->getUserInstance();
            user_instance->checkAndErrorCallInCalc(
                "HandleSLink::fadeIfLoopSound(%s)",
                solveOffset<char>(event->getAssetCallTable()->keyNamePos));
            user_instance->printLogFadeOrKill(
                getEvent(), "HandleSLink::fadeIfLoopSound(%s)",
                solveOffset<char>(getEvent()->getAssetCallTable()->keyNamePos));
            getEvent()->fadeBySystem();
        }
    }

    void fade(int frame = -1)
    {
        auto* event = getEvent();
        if (event && event->getCreateId() == getCreateId()) {
            auto* user_instance = event->getUserInstance();
            user_instance->checkAndErrorCallInCalc(
                "HandleSLink::fade(%s)",
                solveOffset<char>(event->getAssetCallTable()->keyNamePos));
            user_instance->printLogFadeOrKill(
                getEvent(), "HandleSLink::fade(%s)",
                solveOffset<char>(getEvent()->getAssetCallTable()->keyNamePos));
            getEvent()->fade(frame);
        }
    }

    void setPosition(const sead::Vector3f& position)
    {
        auto* event = static_cast<EventSLink*>(getEvent());
        if (event && event->getCreateId() == getCreateId()) {
            event->mPosition = position;
            event->mBitFlag2.setBit(8);
            event->mBitFlag3.setBit(8);
        }
    }

    /// Sets the volume scale of the event (inline-only in the original; requested for AscendingCurrentShieldable::calc_).
    void setVolumeScale(f32 scale)
    {
        auto* event = static_cast<EventSLink*>(getEvent());
        if (event && event->getCreateId() == getCreateId()) {
            event->mVolumeScale = scale;
            event->mBitFlag2.setBit(0);
            event->mBitFlag3.setBit(0);
        }
    }

    void setMatrix(const sead::Matrix34f& matrix)
    {
        auto* event = static_cast<EventSLink*>(getEvent());
        if (event && event->getCreateId() == getCreateId()) {
            event->mPosition.set(matrix(0, 3), matrix(1, 3), matrix(2, 3));
            event->mBitFlag2.setBit(8);
            event->mBitFlag3.setBit(8);
        }
    }
};
static_assert(sizeof(HandleSLink) == 0x10, "xlink2::HandleSLink size mismatch");
}