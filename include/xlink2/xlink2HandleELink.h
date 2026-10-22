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

    /// Detaches this event's delayed-emit record from its actor / chemical owners. Inline-only in the
    /// original (name is a guess): Unk_7102450410::Entry's destructor (0x71007083b4) inlines exactly
    /// the event/create-id guard, the record pointer at event +0x190 and 16 zero bytes at record +8.
    /// It is not part of the handle destructor — Unk_71012419b4 holders such as MoonMove (D1
    /// 0x71001e6df8) and OnStateXLinkCreate (D1 0x7100631c0c) match with no teardown.
    void releaseDelayEmitParamMaybe()
    {
        auto* event = static_cast<EventELink*>(getEvent());
        if (event && event->getCreateId() == getCreateId()) {
            if (auto* param = event->mDelayEmitParam._0xf8) {
                param->mActor = nullptr;
                param->mChemical = nullptr;
            }
        }
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

    /// Sets the delay emit vector `_0xd0` (event +0x168) and flag1 bit 20 (inline-only in the original; the name is a
    /// placeholder, requested for the grass fire element 0x71012de95c).
    void setDelayEmitParam0xD0(const sead::Vector3f& value)
    {
        auto* event = static_cast<EventELink*>(getEvent());
        if (event && event->getCreateId() == getCreateId()) {
            event->mDelayEmitParam._0xd0 = value;
            event->mDelayEmitParam.flag1.setBit(20);
        }
    }

    /// Sets the delay emit parameter `_0x78` (with `_0x74` = 1) and flag1 bit 10 (inline-only in the original; the
    /// name is a placeholder, requested for GameSceneSubsys5 0x7100905f9c).
    void setDelayEmitParam0x78(f32 value)
    {
        auto* event = static_cast<EventELink*>(getEvent());
        if (event && event->getCreateId() == getCreateId()) {
            event->mDelayEmitParam._0x74 = 1.0f;
            event->mDelayEmitParam._0x78 = value;
            event->mDelayEmitParam.flag1.setBit(10);
        }
    }

    /// Stores `value` at +0x18 of the delay emit parameter's `_0xf8` object, if there is one (inline-only in the
    /// original; placeholder name, requested for GameSceneSubsys5 0x7100905f9c).
    void setDelayEmitParamF8Value(f32 value)
    {
        auto* event = static_cast<EventELink*>(getEvent());
        if (event && event->getCreateId() == getCreateId()) {
            if (event->mDelayEmitParam._0xf8)
                event->mDelayEmitParam._0xf8->_18 = value;
        }
    }

    // Inline-only in the original; name is a guess. The emitters at
    // 0x710126ea30/0x710126eab8/0x710126eb40/0x710126ebc8 and their calc paths
    // repeat this guarded mask update on the existing delay-parameter object.
    void setDelayEmitParamF8Flags(u32 flags)
    {
        auto* event = static_cast<EventELink*>(getEvent());
        if (event && event->getCreateId() == getCreateId()) {
            if (event->mDelayEmitParam._0xf8)
                event->mDelayEmitParam._0xf8->_20 |= flags;
        }
    }

    // Inline-only in the original; name is a guess. Rainbow1271458 repeats
    // this color assignment/flag update for both its main and mirror events.
    void setDelayEmitParamColor(const sead::Color4f& color)
    {
        auto* event = static_cast<EventELink*>(getEvent());
        if (event && event->getCreateId() == getCreateId()) {
            event->mDelayEmitParam.color = color;
            event->mDelayEmitParam.flag1.setBit(0);
        }
    }

    /// Sets the delay emit colour alpha and flag1 bit 1 (inline-only in the original: StasisMgr
    /// 0x710067ffbc stores it at the event's +0xa4 and ORs 2 into the word at +0x180).
    void setDelayEmitParamColorAlpha(f32 alpha)
    {
        auto* event = static_cast<EventELink*>(getEvent());
        if (event && event->getCreateId() == getCreateId()) {
            event->mDelayEmitParam.color.a = alpha;
            event->mDelayEmitParam.flag1.setBit(1);
        }
    }

    sead::Vector3f setMtxUp(const sead::Vector3f&, const sead::Vector3f&, f32);
    HandleELink* setMtxZ(const sead::Vector3f&, const sead::Vector3f&, f32);
};
static_assert(sizeof(HandleELink) == 0x10, "xlink2::HandleELink size mismatch");

}  // namespace xlink2
