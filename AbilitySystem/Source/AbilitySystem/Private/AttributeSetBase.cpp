// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSetBase.h"

#include "CharacterBase.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"


UAttributeSetBase::UAttributeSetBase()
{
}


void UAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// HP
	{
		const bool bSuccess = (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health)));
		if (bSuccess)
		{
			// クランプ
			const float ClampedCurrentValue = FMath::Clamp(Health.GetCurrentValue(), 0.0f, MaxHealth.GetCurrentValue());
			Health.SetCurrentValue(ClampedCurrentValue);

			// ※ベースの方は別に揃える必要無くない？
			//const float ClampedBaseValue = FMath::Clamp(Health.GetBaseValue(), 0.0f, MaxHealth.GetCurrentValue());
			//Health.SetBaseValue(ClampedBaseValue);

			UE_LOG(LogTemp, Warning, TEXT("Health Changed: %f"), Health.GetCurrentValue());

			// HPの変動を放送する
			OnHealthChange.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());

			// HP満タンかどうかのゲームプレイタグを更新する
			if (FMath::IsNearlyEqual(Health.GetCurrentValue(), MaxHealth.GetCurrentValue()))
			{
				ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetOwningActor());
				if (IsValid(CharacterBase))
				{
					CharacterBase->AddGameplayTag(CharacterBase->FullHealthTag);
				}
			}
			else
			{
				ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetOwningActor());
				if (IsValid(CharacterBase))
				{
					CharacterBase->RemoveGameplayTag(CharacterBase->FullHealthTag);
				}
			}
		}
	}

	// Mana
	{
		const bool bSuccess = (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Mana)));
		if (bSuccess)
		{
			// クランプ
			const float ClampedCurrentValue = FMath::Clamp(Mana.GetCurrentValue(), 0.0f, MaxMana.GetCurrentValue());
			Mana.SetCurrentValue(ClampedCurrentValue);

			// ※ベースの方は別に揃える必要無くない？

			UE_LOG(LogTemp, Warning, TEXT("Mana Changed: %f"), Mana.GetCurrentValue());

			// Manaの変動を放送する
			OnManaChange.Broadcast(Mana.GetCurrentValue(), MaxMana.GetCurrentValue());
		}
	}

	// Strength
	{
		const bool bSuccess = (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Strength)));
		if (bSuccess)
		{
			// クランプ
			const float ClampedCurrentValue = FMath::Clamp(Strength.GetCurrentValue(), 0.0f, MaxStrength.GetCurrentValue());
			Strength.SetCurrentValue(ClampedCurrentValue);

			// ※ベースの方は別に揃える必要無くない？

			UE_LOG(LogTemp, Warning, TEXT("Strength Changed: %f"), Strength.GetCurrentValue());

			// Strengthの変動を放送する
			OnStrengthChange.Broadcast(Strength.GetCurrentValue(), MaxStrength.GetCurrentValue());
		}
	}
}
