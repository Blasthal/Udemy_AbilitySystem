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


private:
    UFUNCTION(BlueprintCallable, Category = "GroundSelect")
    bool GetPlayerLookingPoint(FVector& OutViewPoint);
    
    UPROPERTY(EditAnywhere, Category = "GroundSelect")
    bool bDrawDebug = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GroundSelect", meta=(AllowPrivateAccess = "true", ExposeOnSpawn=true))
    float Radius = 100.0f;


    USceneComponent* RootComponent = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GroundSelect", meta = (AllowPrivateAccess = "true"))
    UDecalComponent* DecalComponent = nullptr;
};
