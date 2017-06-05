// Fill out your copyright notice in the Description page of Project Settings.

#include "CyberpunkProject.h"
#include "AmmoPickUp.h"
#include "PlayerCharacter.h"

void AAmmoPickUp::PickUpItem(AActor* Player = nullptr)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(Player);
	if (player->CanPickUpAmmo(_ammoType))
	{
		player->AddPickedUpAmmo(_ammoType, _ammoAmount);

		if (_ammoAmount <= 0)
		{
			_mesh->SetVisibility(false, true);
			_boxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);			
		}
	}
}


