#pragma once

#include <prim/seadSafeString.h>

#include "xlink2/xlink2EventSLink.h"
#include "xlink2/xlink2HandleSLink.h"
#include "xlink2/xlink2Resource.h"

namespace aal {
class AssetInfo;
class Handle;
}

namespace xlink2 {
class UserInstanceSLink;

class IEventCallbackSLink {
public:
    struct EventArg {
        UserInstanceSLink* pUserInstance {};
        // ksys::snd 0x71012cecc8 / 0x71012cf2c8 call aal::Handle::getSoundSource on it.
        aal::Handle* pHandle {};
        const ResAssetCallTable* pAssetCallTable {};
        AssetExecutorSLink* pAssetExecutor {};
        EventSLink* pEvent {};
    };

    virtual bool eventActivating(const EventArg&);
    virtual void eventActivated(const EventArg&);

    virtual u32 soundPrePlay(const EventArg&);

    virtual void eventCalced(const EventArg&);

    // Slot 4 (vtable 0x710251b730 of the derived ksys::snd::Unk_710251b730 has soundPlayed there).
    virtual void soundPlayed(const EventArg&);

    virtual u32 replaceAssetInfo(aal::AssetInfo*, const sead::SafeString&, const UserInstanceSLink*,
                                 const ResAssetCallTable*);
};
}  // namespace xlink2