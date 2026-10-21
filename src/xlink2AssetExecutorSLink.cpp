#include "xlink2/xlink2AssetExecutorSLink.h"

#include "xlink2/xlink2EventSLink.h"

#include <aal/aalSettings.h>
#include <aal/aalSystemAccessor.h>

#include "xlink2/xlink2ResourceAccessor.h"
#include "xlink2/xlink2ResourceAccessorSLink.h"
#include "xlink2/xlink2UserResourceSLink.h"
#include "xlink2/xlink2Util.h"

namespace xlink2 {

ResourceAccessor2::ResourceAccessor2(const UserInstance* instance, const ResAssetCallTable& table,
                                   s32 param_index, u64 overwrite_pos)
    : mpUserInstance(instance), mpUserResource(instance->getUserResource()), mpAssetCallTable(&table),
      mpOverwriteParam(nullptr), mParamValue(0), mNeedObserve(false)
{
    const auto* asset_param = solveOffset<ResAssetParam>(table.paramStartPos);
    if (asset_param->mask.isOnBit(param_index)) {
        const auto& param = asset_param->params[asset_param->mask.countRightOnBit(param_index) - 1];
        mParamValue = mpUserResource->getParam()->pCommonResourceParam->directValueTable[param.getValue()];
    }
    const auto* resource_param = mpUserResource->getParam();
    const auto table_index = &table - resource_param->userBinParam.pResAssetCallTable;
    mNeedObserve = resource_param->callTableParamBuffer[table_index].bitFlag.isOnBit(1);
    if (u32(overwrite_pos) != 0)
        mpOverwriteParam = solveOffset<ResTriggerOverwriteParam>(overwrite_pos);
}

// NON_MATCHING: shared distribution dispatch groups the exponent cases differently.
f32 ResourceAccessor2::getValue_(const ResParam& param) const
{
    const auto type = param.getRefType();
    f32 exponent;
    switch (type) {
    case ValueReferenceType::Direct:
        return sead::BitUtil::bitCast<f32>(
            mpUserResource->getParam()->pCommonResourceParam->directValueTable[param.getValue()]);
    case ValueReferenceType::Curve:
        return getCurveValue_(param);
    case ValueReferenceType::Random:
        return getRandomValue_(*getResRandomCallTable_(param));
    case ValueReferenceType::Random2Pow:
    case ValueReferenceType::Random2PowWeightMin:
    case ValueReferenceType::Random2PowWeightMax:
        exponent = 2.0f;
        break;
    case ValueReferenceType::Random3Pow:
    case ValueReferenceType::Random3PowWeightMin:
    case ValueReferenceType::Random3PowWeightMax:
        exponent = 3.0f;
        break;
    case ValueReferenceType::Random4Pow:
    case ValueReferenceType::Random4PowWeightMin:
    case ValueReferenceType::Random4PowWeightMax:
        exponent = 4.0f;
        break;
    case ValueReferenceType::Random1Point5Pow:
    case ValueReferenceType::Random1Point5PowWeightMin:
    case ValueReferenceType::Random1Point5PowWeightMax:
        exponent = 1.5f;
        break;
    default:
        return 0.0f;
    }
    const auto& table = *getResRandomCallTable_(param);
    if (type <= ValueReferenceType::Random1Point5Pow)
        return getRandomValue_(table, exponent);
    if (type <= ValueReferenceType::Random1Point5PowWeightMin)
        return getRandomValueWeightMin_(table, exponent);
    return getRandomValueWeightMax_(table, exponent);
}

AssetExecutorSLink::~AssetExecutorSLink() = default;

void AssetExecutorSLink::updateParam_()
{
    auto* accessor {static_cast<UserResourceSLink*>(getUserInstance()->getUserResource())->getAccessorPtr()};
    u32 overwrite_param_pos = reinterpret_cast<u64>(mpTriggerOverwriteParam);
    if (mParamFlag.isOnBit(0)) {
        if (accessor->isParamOverwritten(overwrite_param_pos, 3))
            mVolume = accessor->getOverwriteVolume(overwrite_param_pos, mpUserInstance);
        else
            mVolume = accessor->getVolume(*mpAssetCallTable, mpUserInstance);
    }

    if (mParamFlag.isOnBit(1)) {
        mVolumeTv = accessor->getVolumeTv(*mpAssetCallTable, mpUserInstance);
    }

    if (mParamFlag.isOnBit(2)) {
        if (accessor->isParamOverwritten(overwrite_param_pos, 6))
            mPitch = accessor->getOverwritePitch(overwrite_param_pos, mpUserInstance);
        else
            mPitch = accessor->getPitch(*mpAssetCallTable, mpUserInstance);
    }

    if (mParamFlag.isOnBit(3)) {
        if (accessor->isParamOverwritten(overwrite_param_pos, 7))
            mLpf = accessor->getOverwriteLpf(overwrite_param_pos, mpUserInstance);
        else
            mLpf = accessor->getLpf(*mpAssetCallTable, mpUserInstance);
    }

    if (mParamFlag.isOnBit(5)) {
        if (accessor->isParamOverwritten(overwrite_param_pos, 13))
            mPriority = accessor->getOverwritePriority(overwrite_param_pos, mpUserInstance);
        else
            mPriority = accessor->getPriority(*mpAssetCallTable, mpUserInstance);
    }

    applyParam_(false);
}

// 0x7100bd08dc
bool AssetExecutorSLink::isAssetValid() const
{
    return mHandle.isActive();
}

f32 AssetExecutorSLink::getPlayingTime() const
{
    return mHandle.getPlayingTime() * 1000.0f;
}

bool AssetExecutorSLink::isLoopEvent() const 
{
    if (!mpAssetCallTable)
        return false;

    auto* res_accessor = mpUserInstance->getUser()->getUserResource()->getAccessorPtr();
    return res_accessor->isLoopAsset(*mpAssetCallTable);
}

void AssetExecutorSLink::requestReEmit(bool check) 
{
    mBitFlag.changeBit(0, check);
}

bool AssetExecutorSLink::isRequestReEmit() const 
{
    return mBitFlag.isOnBit(0);
}

void AssetExecutorSLink::onFinalize_() 
{
    mpModelAssetConnection = nullptr;
}
// 0x7100bcf03c
void AssetExecutorSLink::fadeBySystem()
{
    if (mpAssetCallTable == nullptr)
        return;

    auto* accessor =
        static_cast<UserResourceSLink*>(getUserInstance()->getUserResource())->getAccessorPtr();
    const ResAssetCallTable& asset = *mpAssetCallTable;
    if (!accessor->isLoopAsset(asset) && !accessor->isAutoOneTimeFade(asset))
        return;

    if (mHandle.isActive())
        fade(-1);
    else
        mContainerType = ContainerType_Asset;
}

// 0x7100bcef5c
void AssetExecutorSLink::fade(s32 param_int)
{
    if (mHandle.isActive()) {
        s32 fade = param_int;
        if (fade < 0) {
            auto* accessor =
                static_cast<UserResourceSLink*>(getUserInstance()->getUserResource())->getAccessorPtr();
            const f32 stop = accessor->getStopFrameWithOverwrite(
                *mpAssetCallTable, reinterpret_cast<u64>(mpTriggerOverwriteParam), mpUserInstance);
            fade = static_cast<s32>(stop + (stop >= 0.0f ? 0.5f : -0.5f));
            if (fade == -1)
                fade = 0;
        }
        mHandle.stop(fade * aal::SystemAccessor::getSettings()->mCalcTimeStep, 0.0f);
    }
    mContainerType = ContainerType_Sequence;
}

// 0x7100bcf008
void AssetExecutorSLink::kill()
{
    mHandle.stop(-1.0f, 0.0f);
    mContainerType = ContainerType_Asset;
}
}  // namespace xlink2
