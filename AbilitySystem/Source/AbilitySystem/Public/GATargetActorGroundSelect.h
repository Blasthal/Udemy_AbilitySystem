// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GATargetActorGroundSelect.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API AGATargetActorGroundSelect : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()


public:
    AGATargetActorGroundSelect();

    virtual void Tick(float DeltaSeconds) override;
	virtual void StartTargeting(UGameplayAbility* Ability) override;
    virtual void ConfirmTargetingAndContinue() override;

    UFUNCTION(BlueprintCallable, Category = "GroundSelect")
    bool GetPlayerLookingPoint(FVector& OutViewPoint);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GroundSelect", meta=(ExposeOnSpawn=true))
    float Radius = 100.0f;
};
