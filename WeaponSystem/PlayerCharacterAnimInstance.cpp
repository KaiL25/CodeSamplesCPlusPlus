// Fill out your copyright notice in the Description page of Project Settings.

#include "CyberpunkProject.h"
#include "PlayerCharacterAnimInstance.h"
#include "PlayerCharacter.h"
#include "Engine.h"

UPlayerCharacterAnimInstance::UPlayerCharacterAnimInstance()
{
	_equippedWeaponType = EWeaponType::VE_Handgun;
	bWeaponHolstered = true;
}

void UPlayerCharacterAnimInstance::UpdateAnimationProperties()
{
	//Get the pawn which is affected by our anim instance
	APawn* Pawn = TryGetPawnOwner();

	if (Pawn)
	{
		//Update our falling property
		bIsFalling = Pawn->GetMovementComponent()->IsFalling();

		//Update our movement speed
		_movementSpeed = Pawn->GetVelocity().Size();
	}

	if (!bWeaponHolstered)
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(Pawn);
		float temp = FMath::ClampAngle(player->GetBaseAimRotation().Pitch, -90, 90);
		_aimingSpineRotator.Yaw = -temp;		
	}
}

EWeaponType UPlayerCharacterAnimInstance::GetEquippedWeaponType()
{
	return _equippedWeaponType;
}

void UPlayerCharacterAnimInstance::SetEquippedWeaponType(EWeaponType NewWeaponType)
{
	_equippedWeaponType = NewWeaponType;
}

bool UPlayerCharacterAnimInstance::GetWeaponHolstered()
{
	return bWeaponHolstered;
}

void UPlayerCharacterAnimInstance::SetWeaponHolstered(bool NewStatus)
{
	bWeaponHolstered = NewStatus;
}


