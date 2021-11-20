// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityBase.h"

FGameplayAbilityInfo UGameplayAbilityBase::GetAbilityInfo()
{
    UGameplayEffect* CoolDownEffect = GetCooldownGameplayEffect();
    UGameplayEffect* CostEffect = GetCostGameplayEffect();
    if (CoolDownEffect && CostEffect)
    {
        float CoolDownDuration = 0.0f;
        CoolDownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(1, CoolDownDuration);

        EAbilityCostType CostType = EAbilityCostType::Health;
        float Cost = 0.0f;
        if (CostEffect->Modifiers.Num() > 0.0f)
        {
            FGameplayModifierInfo ModifierInfo = CostEffect->Modifiers[0];
            ModifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(1, Cost);

            FGameplayAttribute CostAttribute = ModifierInfo.Attribute;
            FString AttributeName = CostAttribute.AttributeName;
            if (AttributeName == TEXT("Health"))
            {
                CostType = EAbilityCostType::Health;
            }
            else if (AttributeName == TEXT("Mana"))
            {
                CostType = EAbilityCostType::Mana;
            }
            else if (AttributeName == TEXT("Strength"))
            {
                CostType = EAbilityCostType::Strength;
            }
        }

        //UE_LOG(LogTemp, Warning, TEXT("GetClass()=%s"), *GetClass()->GetDisplayNameText().ToString());

        return FGameplayAbilityInfo(CoolDownDuration, Cost, CostType, UIMaterial, GetClass());
    }

    return FGameplayAbilityInfo();
}
