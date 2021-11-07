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
	// HP変更時のコールバック
	UFUNCTION()
	void OnHealthChanged(float Health, float MaxHealth);

protected:
	// HP変更時のコールバック
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnHealthChanged"))
	void BP_OnHealthChanged(float Health, float MaxHealth);

	// HP0時のコールバック
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnDie"))
	void BP_Die();

	
private:
	// Mana変更時のコールバック
	UFUNCTION()
	void OnManaChanged(float CurrentMana, float MaxMana);

protected:
	// Mana変更時のコールバック
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnManaChanged"))
	void BP_OnManaChanged(float CurerntMana, float MaxMana);


private:
	// Strength変更時のコールバック
	UFUNCTION()
	void OnStrengthChanged(float CurrentStrength, float MaxStrength);

protected:
	// Strength変更時のコールバック
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnStrengthChanged"))
	void BP_OnStrengthChanged(float CurerntStrength, float MaxStrength);


public:
	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void AddGameplayTag(FGameplayTag Tag);
	
	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void RemoveGameplayTag(FGameplayTag Tag);


protected:
	// 自分にとってOtherが敵かどうか
	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	bool IsOtherHostile(const ACharacterBase* Other) const;

private:
	void AutoDetermineTeamIDbyControllerType();


private:
	void Dead();


protected:
	// アビリティシステムコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	class UAbilitySystemComponent* AbilitySystemComponent;

	// 属性コンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	class UAttributeSetBase* AttributeSetBaseComponent;


private:
	bool bIsDead = false;
	uint8 TeamID = 255;


public:
	// HP満タン状態のゲームプレイタグ
	UPROPERTY(EditAnywhere, Category = "CharacterBase")
	FGameplayTag FullHealthTag;
};
