// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTypes.h"


FGameplayAbilityInfo::FGameplayAbilityInfo()
{
}


FGameplayAbilityInfo::FGameplayAbilityInfo(
	float InCooldownDuration,
	float InCost,
	EAbilityCostType InCostType,
	UMaterialInstance* InUIMaterial,
	TSubclassOf<UGameplayAbilityBase> InGameplayAbilityBaseClass
) :
	CooldownDuration(InCooldownDuration),
	Cost(InCost),
	CostType(InCostType),
	UIMaterial(InUIMaterial),
	GameplayAbilityBaseClass(InGameplayAbilityBaseClass)
{
}
