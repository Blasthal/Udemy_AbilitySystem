// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "AIController.h"
#include "AttributeSetBase.h"
#include "BrainComponent.h"
#include "GameplayAbilityBase.h"
#include "PlayerControllerBase.h"

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
	AttributeSetBaseComponent->OnManaChange.AddDynamic(this, &ACharacterBase::OnManaChanged);
	AttributeSetBaseComponent->OnStrengthChange.AddDynamic(this, &ACharacterBase::OnStrengthChanged);

	AutoDetermineTeamIDbyControllerType();

	// 初期ではHP満タン
	AddGameplayTag(FullHealthTag);
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


void ACharacterBase::AcquireAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilityToAcquire)
{
	for (TSubclassOf<UGameplayAbility> AbilityItem : AbilityToAcquire)
	{
		ensure(AbilityItem);
		if (!AbilityItem)
		{
			continue;
		}

		AcquireAbility(AbilityItem);


		if (AbilityItem->IsChildOf<UGameplayAbilityBase>())
		{
			TSubclassOf<UGameplayAbilityBase> AbilityBaseClass = *AbilityItem;
			if (AbilityBaseClass)
			{
				AddAbilityToUI(AbilityBaseClass);
			}
		}
	}
}


void ACharacterBase::OnHealthChanged(float Health, float MaxHealth)
{
	// 死んでいるなら何もしない
	if (bIsDead)
	{
		return;
	}


	BP_OnHealthChanged(Health, MaxHealth);

	// HP0の時は追加で処理を呼ぶ
	if (Health <= 0.0f)
	{
		bIsDead = true;

		Dead();
		BP_Die();
	}
}


void ACharacterBase::OnManaChanged(float CurrentMana, float MaxMana)
{
	BP_OnManaChanged(CurrentMana, MaxMana);
}


void ACharacterBase::OnStrengthChanged(float CurrentStrength, float MaxStrength)
{
	BP_OnStrengthChanged(CurrentStrength, MaxStrength);
}


void ACharacterBase::AddGameplayTag(FGameplayTag Tag)
{
	GetAbilitySystemComponent()->AddLooseGameplayTag(Tag);

	// 1つだけに絞る
	GetAbilitySystemComponent()->SetLooseGameplayTagCount(Tag, 1);
}


void ACharacterBase::RemoveGameplayTag(FGameplayTag Tag)
{
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(Tag);
}


bool ACharacterBase::IsOtherHostile(const ACharacterBase* Other) const
{
	ensure(Other);
	if (!Other)
	{
		return false;
	}

	return (TeamID != Other->TeamID);
}


void ACharacterBase::AutoDetermineTeamIDbyControllerType()
{
	if (const AController* SelfController = GetController())
	{
		if (SelfController->IsPlayerController())
		{
			TeamID = 0;
		}
	}
}


void ACharacterBase::Dead()
{
	DisableInputControl();
}


void ACharacterBase::EnableInputControl()
{
	// 死んでいるなら処理しない
	if (bIsDead)
	{
		return;
	}


	// プレイヤー側の入力を有効にする
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (IsValid(PlayerController))
	{
		PlayerController->EnableInput(PlayerController);
		return;
	}

	// AI側の入力を有効にする
	const AAIController* AIController = Cast<AAIController>(GetController());
	if (IsValid(AIController))
	{
		AIController->GetBrainComponent()->RestartLogic();
		return;
	}
}


void ACharacterBase::DisableInputControl()
{
	// プレイヤー側の入力を無効にする
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (IsValid(PlayerController))
	{
		PlayerController->DisableInput(PlayerController);
		return;
	}

	// AI側の入力を無効にする
	const AAIController* AIController = Cast<AAIController>(GetController());
	if (IsValid(AIController))
	{
		AIController->GetBrainComponent()->StopLogic("Dead");
		return;
	}
}


void ACharacterBase::HitStun(float StunDurationTimeSecond)
{
	DisableInputControl();

	GetWorldTimerManager().SetTimer(StunTimeHandle, this, &ACharacterBase::EnableInputControl, StunDurationTimeSecond, false, -1);
}


void ACharacterBase::AddAbilityToUI(TSubclassOf<UGameplayAbilityBase> AbilityToAdd)
{
    APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(GetController());
	if (!PlayerController)
	{
		return;
	}

	UGameplayAbilityBase* AbilityInstance = AbilityToAdd.Get()->GetDefaultObject<UGameplayAbilityBase>();
	ensure(AbilityInstance);
	if (!AbilityInstance)
	{
		return;
	}

    FGameplayAbilityInfo AbilityInfo = AbilityInstance->GetAbilityInfo();
	PlayerController->AddAbilityToUI(AbilityInfo);
}
