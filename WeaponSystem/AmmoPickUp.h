// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PickUp.h"
#include "Enums.h"
#include "AmmoPickUp.generated.h"

/**
 * 
 */
UCLASS()
class CYBERPUNKPROJECT_API AAmmoPickUp : public APickUp
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditAnywhere, Category = AmmoPickUp)
	EFirearmType _ammoType;

	///The amount of ammo replenished when the player picks up the ammo.
	UPROPERTY(EditAnywhere, Category = AmmoPickUp)
	uint8 _ammoAmount;
	
	virtual void PickUpItem(AActor* Player) override;
	
	
};
