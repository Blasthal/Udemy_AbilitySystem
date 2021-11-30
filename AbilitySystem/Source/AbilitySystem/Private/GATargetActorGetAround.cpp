// Fill out your copyright notice in the Description page of Project Settings.


#include "GATargetActorGetAround.h"

#include "Abilities/GameplayAbility.h"

void AGATargetActorGetAround::StartTargeting(UGameplayAbility* Ability)
{
    Super::StartTargeting(Ability);

    this->OwningAbility = Ability;
    this->MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
}

void AGATargetActorGetAround::ConfirmTargetingAndContinue()
{
    // •sŽg—p
    //Super::ConfirmTargetingAndContinue();

    APawn* OwningPawn = MasterPC->GetPawn();
    ensure(OwningPawn);
    if (!OwningPawn)
    {
        TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
        return;
    }
    
    const FVector ViewLocation = OwningPawn->GetActorLocation();

    TArray<FOverlapResult> Overlaps;
    TArray<TWeakObjectPtr<AActor>> OverlappedActors;
    constexpr bool bTraceComplex = false;

    FCollisionQueryParams QueryParams;
    QueryParams.bTraceComplex = bTraceComplex;
    QueryParams.bReturnPhysicalMaterial = false;

    APawn* MasterPawn = MasterPC->GetPawn();
    if (MasterPawn)
    {
        QueryParams.AddIgnoredActor(MasterPawn);
    }

    const bool bTryOverlap = GetWorld()->OverlapMultiByObjectType(
        Overlaps,
        ViewLocation,
        FQuat::Identity,
        FCollisionObjectQueryParams(ECC_Pawn),
        FCollisionShape::MakeSphere(Radius),
        QueryParams
    );
    if (bTryOverlap)
    {
        for (const FOverlapResult OverlapResult : Overlaps)
        {
            APawn* OverlappedPawn = Cast<APawn>(OverlapResult.GetActor());
            if (!OverlappedPawn)
            {
                continue;
            }

            if (OverlappedActors.Contains(OverlappedPawn))
            {
                continue;
            }

            OverlappedActors.Add(OverlappedPawn);
        }
    }
    
    if (0 < OverlappedActors.Num())
    {
        const FGameplayAbilityTargetDataHandle TargetData = this->StartLocation.MakeTargetDataHandleFromActors(OverlappedActors);
        TargetDataReadyDelegate.Broadcast(TargetData);
    }
    else
    {
        TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
    }
}
