// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityTypes.generated.h"


class UGameplayAbilityBase;


UENUM(BlueprintType)
enum class EAbilityCostType : uint8
{
	Health,
	Mana,
	Strength,
};

USTRUCT(BlueprintTYpe)
struct FGameplayAbilityInfo
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayAbilityInfo")
	float CooldownDuration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayAbilityInfo")
	float Cost = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayAbilityInfo")
	EAbilityCostType CostType = EAbilityCostType::Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayAbilityInfo")
	class UMaterialInstance* UIMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayAbilityInfo")
	TSubclassOf<UGameplayAbilityBase> GameplayAbilityBaseClass = nullptr;


	FGameplayAbilityInfo();
	FGameplayAbilityInfo(
		float InCooldownDuration,
		float InCost,
		EAbilityCostType InCostType,
		UMaterialInstance* InUIMaterial,
		TSubclassOf<UGameplayAbilityBase> InGameplayAbilityBaseClass
	);
};
