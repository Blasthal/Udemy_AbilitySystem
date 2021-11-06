// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSetBase.h"

#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"


UAttributeSetBase::UAttributeSetBase()
{
}


void UAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const bool bSuccess = (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health)));
	if (!bSuccess)
	{
		return;
	}

	// �N�����v
	const float ClampedCurrentValue = FMath::Clamp(Health.GetCurrentValue(), 0.0f, MaxHealth.GetCurrentValue());
	Health.SetCurrentValue(ClampedCurrentValue);

	// ���x�[�X�̕��͕ʂɑ�����K�v�����Ȃ��H
	//const float ClampedBaseValue = FMath::Clamp(Health.GetBaseValue(), 0.0f, MaxHealth.GetCurrentValue());
	//Health.SetBaseValue(ClampedBaseValue);

	UE_LOG(LogTemp, Warning, TEXT("Damaged. Health: %f"), Health.GetCurrentValue());

	// HP�̕ϓ����������
	OnHealthChange.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());
}
