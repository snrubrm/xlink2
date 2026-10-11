#include "xlink2/xlink2SwitchContainer.h"
#include "xlink2/xlink2Event.h"
#include "xlink2/xlink2PropertyDefinition.h"
#include "xlink2/xlink2ResourceUtil.h"
#include "xlink2/xlink2UserResource.h"

namespace xlink2 {
SwitchContainer::~SwitchContainer() = default;

// NON_MATCHING: child-state loads and the shared completion return differ.
bool SwitchContainer::calc()
{
    bool check1 {false};
    if (mpEvent->getBitFlag().isOffBit(4)) {
        auto& accessor {mpEvent->getUserInstance()->getUser()->getUserResource()->getAccessor()};
        bool need_observe {accessor.isNeedObserve(*mpAssetCallTable)};
        bool bit_3_on {mpEvent->getBitFlag().isOnBit(3)};
        check1 = need_observe && !bit_3_on;

        if (check1) {
            auto* asset_ctb {getConditionMatchChildCallTable_()};
            if (mpChild != nullptr) {
                if (mpChild->getAssetCallTable() != asset_ctb) {
                    mpChild->fadeBySystem();
                    mpChild->destroy();
                    mpChild = nullptr;
                    
                    if (asset_ctb != nullptr)
                        createChildContainer_(*asset_ctb, nullptr);
                }
            }
            else if (asset_ctb != nullptr) {
                createChildContainer_(*asset_ctb, nullptr);
            }
        }

    }

    bool check2 {true};
    if (mpChild != nullptr) {
        if (mpChild->calc()) {
            mpChild->destroy();
            mpChild = nullptr;
            check2 = assetFinished();
        }
        else {
            check2 = false;
        }
    }

    return !check1 && check2;
}

// NON_MATCHING: property-index load ordering and register allocation differ.
const ResAssetCallTable* SwitchContainer::getConditionMatchChildCallTable_()
{
    auto* instance = mpEvent->getUserInstance();
    auto* param = static_cast<ResSwitchContainerParam*>(ResourceUtil::getResContainerParam(*mpAssetCallTable));
    s32 index = param->localPropertyNameIdx;
    const PropertyDefinition* definition;
    const PropertyValueType* values;
    if (param->isGlobal) {
        if (index < 0)
            return nullptr;
        definition = instance->getUser()->getSystem()->getPropertyDefinition(index);
        values = instance->getUser()->getSystem()->getPropertyValues();
    } else {
        if (index < 0)
            return nullptr;
        index = instance->getUser()->getUserResource()->getParam()->localPropertyIdxBuffer[index];
        if (index < 0)
            return nullptr;
        definition = instance->getUser()->getPropertyDefinition(index);
        values = instance->getPropertyValues();
    }
    if (!definition)
        return nullptr;

    auto* resource = instance->getUser()->getUserResource();
    for (s32 i = param->childrenStartIndex; i <= param->childrenEndIndex; ++i) {
        auto* table = resource->getAssetCallTableItem(i);
        if (table->conditionPos == 0)
            return table;

        switch (definition->getType()) {
        case PropertyType::Enum: {
            auto* condition = solveOffset<ResSwitchCondition>(table->conditionPos);
            const s32 value = values[index].valueInt;
            const s32 expected = param->isGlobal ? condition->localPropertyEnumNameIdx :
                                                  resource->getParam()->callTableParamBuffer[i].assetId;
            switch (condition->compareType) {
            case CompareType::Equal:
                if (value == expected)
                    return table;
                break;
            case CompareType::GreaterThan:
                if (value > expected)
                    return table;
                break;
            case CompareType::GreaterThanOrEqual:
                if (value >= expected)
                    return table;
                break;
            case CompareType::LessThan:
                if (value < expected)
                    return table;
                break;
            case CompareType::LessThanOrEqual:
                if (value <= expected)
                    return table;
                break;
            case CompareType::NotEqual:
                if (value != expected)
                    return table;
                break;
            }
            break;
        }
        case PropertyType::S32: {
            auto* condition = solveOffset<ResSwitchCondition>(table->conditionPos);
            const s32 value = values[index].valueInt;
            const s32 expected = condition->value;
            switch (condition->compareType) {
            case CompareType::Equal:
                if (value == expected)
                    return table;
                break;
            case CompareType::GreaterThan:
                if (value > expected)
                    return table;
                break;
            case CompareType::GreaterThanOrEqual:
                if (value >= expected)
                    return table;
                break;
            case CompareType::LessThan:
                if (value < expected)
                    return table;
                break;
            case CompareType::LessThanOrEqual:
                if (value <= expected)
                    return table;
                break;
            case CompareType::NotEqual:
                if (value != expected)
                    return table;
                break;
            }
            break;
        }
        case PropertyType::F32: {
            auto* condition = solveOffset<ResSwitchCondition>(table->conditionPos);
            const f32 value = values[index].valueFloat;
            const f32 expected = condition->valueFloat;
            switch (condition->compareType) {
            case CompareType::Equal:
                if (value == expected)
                    return table;
                break;
            case CompareType::GreaterThan:
                if (value > expected)
                    return table;
                break;
            case CompareType::GreaterThanOrEqual:
                if (value >= expected)
                    return table;
                break;
            case CompareType::LessThan:
                if (value < expected)
                    return table;
                break;
            case CompareType::LessThanOrEqual:
                if (value <= expected)
                    return table;
                break;
            case CompareType::NotEqual:
                if (value != expected)
                    return table;
                break;
            }
            break;
        }
        }
    }
    return nullptr;
}

bool SwitchContainer::start_(const ResAssetCallTable* asset_ctb)
{
    if (asset_ctb == nullptr) {
        UserInstance* user_instance {mpEvent->getUserInstance()};
        auto& accessor {user_instance->getUserResource()->getAccessor()};
        bool need_observe {accessor.isNeedObserve(*mpAssetCallTable)};
        u32 bit_flag {mpEvent->getBitFlag().getDirect()};
        if (!need_observe || bit_flag & 8) {
            const char* key_name {solveOffset<const char>(mpAssetCallTable->keyNamePos)};
            user_instance->printLogEmitFailed(*mpEvent, "switch container[%s] doesn't select child", key_name);
            return false;
        }
        
        return true;
    }

    return createChildContainer_(*asset_ctb, nullptr) != nullptr;
}

bool SwitchContainer::start()
{
    return start_(getConditionMatchChildCallTable_());
}

void SwitchContainer::printChildSelect_([[maybe_unused]] const ResAssetCallTable* asset_ctb) const
{
#ifdef SEAD_DEBUG
#endif 
}
} // namespace xlink2