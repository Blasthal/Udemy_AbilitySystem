// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "CharacterBase.generated.h"

UCLASS()
class ABILITYSYSTEM_API ACharacterBase
	: public ACharacter
	, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void AcquireAbility(TSubclassOf<UGameplayAbility> AbilityToAcquire);

	
private:
	// HP�ύX���̃R�[���o�b�N
	UFUNCTION()
	void OnHealthChanged(float Health, float MaxHealth);

protected:
	// HP�ύX���̃R�[���o�b�N
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnHealthChanged"))
	void BP_OnHealthChanged(float Health, float MaxHealth);

	// HP0���̃R�[���o�b�N
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnDie"))
	void BP_Die();


protected:
	// �����ɂƂ���Other���G���ǂ���
	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	bool IsOtherHostile(const ACharacterBase* Other) const;

private:
	void AutoDetermineTeamIDbyControllerType();


private:
	void Dead();


protected:
	// �A�r���e�B�V�X�e���R���|�[�l���g
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	class UAbilitySystemComponent* AbilitySystemComponent;

	// �����R���|�[�l���g
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	class UAttributeSetBase* AttributeSetBaseComponent;


private:
	bool bIsDead = false;
	uint8 TeamID = 255;
};
