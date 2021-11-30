// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GATargetActorGetAround.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API AGATargetActorGetAround : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
    virtual void StartTargeting(UGameplayAbility* Ability) override;
    virtual void ConfirmTargetingAndContinue() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GATargetActorAround", meta=(AllowPrivateAccess = "true", ExposeOnSpawn=true))
    float Radius = 100.0f;
};
