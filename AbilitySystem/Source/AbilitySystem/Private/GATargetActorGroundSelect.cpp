// Fill out your copyright notice in the Description page of Project Settings.


#include "GATargetActorGroundSelect.h"

#include "DrawDebugHelpers.h"
#include "Abilities/GameplayAbility.h"

AGATargetActorGroundSelect::AGATargetActorGroundSelect()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AGATargetActorGroundSelect::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);


    FVector LookingPoint;
    GetPlayerLookingPoint(LookingPoint);

    DrawDebugSphere(
        GetWorld(),
        LookingPoint,
        Radius,
        32,
        FColor::Red,
        false,
        -1,
        0,
        2.0f
    );
}

void AGATargetActorGroundSelect::StartTargeting(UGameplayAbility* Ability)
{
    Super::StartTargeting(Ability);

    this->MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
}

void AGATargetActorGroundSelect::ConfirmTargetingAndContinue()
{
    //Super::ConfirmTargetingAndContinue();


    FVector ViewLocation;
    GetPlayerLookingPoint(ViewLocation);

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

bool AGATargetActorGroundSelect::GetPlayerLookingPoint(FVector& OutViewPoint)
{
    OutViewPoint = FVector();


    FVector ViewPoint;
    FRotator ViewRotation;
    MasterPC->GetPlayerViewPoint(ViewPoint, ViewRotation);

    FHitResult HitResult;

    FCollisionQueryParams QueryParams;
    QueryParams.bTraceComplex = true;

    APawn* MasterPawn = MasterPC->GetPawn();
    if (MasterPawn)
    {
        QueryParams.AddIgnoredActor(MasterPawn);
    }


    bool bTryTrace = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        ViewPoint,
        ViewPoint + ViewRotation.Vector() * 10000.0f,
        ECC_Visibility,
        QueryParams
    );
    if (bTryTrace)
    {
        OutViewPoint = HitResult.ImpactPoint;
    }

    return bTryTrace;
}
