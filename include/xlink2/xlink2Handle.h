#pragma once

#include <basis/seadTypes.h>

#include "xlink2/xlink2Event.h"
#include "xlink2/xlink2Locator.h"
#include "xlink2/xlink2UserInstance.h"

namespace xlink2 {
class Event;

class Handle {
public:
    // User-provided: a function-local static handle is initialised at run time (guarded stores of the two
    // fields), and handles are copied by value (eft::searchAndEmit{E,S}Link return a copy of such a static).
    Handle() {}
    // User-provided (empty): handles returned by value and discarded are destroyed like objects with a
    // non-trivial destructor.
    ~Handle() {}

    Handle(UserInstance* user_instance, const char* asset_key_name)
    {
        user_instance->searchAndEmitImpl(asset_key_name, this);
    }

    Handle(UserInstance* user_instance, const ResAssetCallTable& asset_ctb)
    {
        Locator locator {asset_ctb};
        user_instance->emitImpl(locator, this);
    }

    Handle(UserInstance* user_instance, const Locator& locator)
    {
        user_instance->emitImpl(locator, this);
    }

    Event* getEvent() { return static_cast<Event*>(mpResource); }
    s32 getCreateId() { return mCreateId; }
    Event* getEvent() const { return static_cast<Event*>(mpResource); }
    s32 getCreateId() const { return mCreateId; }

    /// Whether the event is still the one this handle was created for (event slots are reused: an event
    /// that was killed and replaced has a different create id). The fade / kill / setPosition / setMatrix
    /// functions of the handles are guarded by the same test.
    bool isActive()
    {
        auto* event = getEvent();
        return event && event->getCreateId() == getCreateId();
    }

    // Const handle queries in 0x7101241b6c use the same event/create-id test.
    bool isActive() const
    {
        auto* event = getEvent();
        return event && event->getCreateId() == getCreateId();
    }

    void reset()
    {
        mpResource = nullptr;
        mCreateId = 0;
    }

    void setResource(void* resource) { mpResource = resource; }
    void setCreateId(s32 create_id) { mCreateId = create_id; }

private:
    void* mpResource = nullptr;
    s32 mCreateId = 0;
};
}  // namespace xlink2