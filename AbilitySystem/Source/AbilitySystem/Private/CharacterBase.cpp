// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "AttributeSetBase.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSetBaseComponent = CreateDefaultSubobject<UAttributeSetBase>(TEXT("AttributeSetBase"));
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	AttributeSetBaseComponent->OnHealthChange.AddDynamic(this, &ACharacterBase::OnHealthChanged);
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void ACharacterBase::AcquireAbility(TSubclassOf<UGameplayAbility> AbilityToAcquire)
{
	ensure(AbilityToAcquire);
	if (!AbilityToAcquire)
	{
		return;
	}

	ensure(AbilitySystemComponent);
	if (!AbilitySystemComponent)
	{
		return;
	}


	if (HasAuthority())
	{
		const FGameplayAbilitySpec AbilitySpec(AbilityToAcquire);
		AbilitySystemComponent->GiveAbility(AbilitySpec);

		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}


void ACharacterBase::OnHealthChanged(float Health, float MaxHealth)
{
	BP_OnHealthChanged(Health, MaxHealth);
}
