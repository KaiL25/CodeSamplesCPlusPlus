// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EWeaponType : uint8
{
	VE_Melee	UMETA(DisplayName = "Melee"),
	VE_Handgun	UMETA(DisplayName = "Handgun"),
	VE_Rifle 	UMETA(DisplayName = "Rifle"),
	VE_Heavygun	UMETA(DisplayName = "Heavygun")
};
/**
 * 
 */
UCLASS()
class CYBERPUNKPROJECT_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	EWeaponType GetEquippedWeaponType();
	void SetEquippedWeaponType(EWeaponType NewWeaponType);
	bool GetWeaponHolstered();

	UFUNCTION(BlueprintCallable, Category = UpperBodyAnimations)
	void SetWeaponHolstered(bool NewStatus);

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimInstance)
	bool bIsFalling;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimInstance)
	float _movementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimInstance)
	bool bWeaponHolstered;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimInstance)
	EWeaponType _equippedWeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerAnimInstance)
	bool bEquipped;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerAnimInstance)
	FRotator _aimingSpineRotator;
	
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateAnimationProperties();
	
private:
		
	UPlayerCharacterAnimInstance();
	
};
