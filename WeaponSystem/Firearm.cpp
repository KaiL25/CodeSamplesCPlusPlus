// Fill out your copyright notice in the Description page of Project Settings.

#include "CyberpunkProject.h"
#include "Firearm.h"
#include "Engine.h"
#include "Ammo.h"
#include "NPC.h"
#include "PlayerCharacter.h"
#include "CyberpunkProjectPlayerController.h"


// Sets default values
AFirearm::AFirearm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	_firearmMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Firearm"));
	_raycastStart = CreateDefaultSubobject<USceneComponent>(TEXT("RaycastStart"));
	_shotSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ShotSound"));
	_shotSound->SetupAttachment(_raycastStart);
	_shotSound->bAutoActivate = false;
	_emptySound = CreateDefaultSubobject <UAudioComponent>(TEXT("EmptySound"));
	_emptySound->SetupAttachment(_raycastStart);
	_reloadSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ReloadSound"));
	_reloadSound->SetupAttachment(_firearmMesh);
	_aimLaser = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AimLaser"));
	_aimLaser->SetupAttachment(RootComponent);
	_firearmMesh->SetupAttachment(RootComponent);
	_raycastStart->SetupAttachment(_firearmMesh);		
	_firearmMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
	_timeSinceLastShot = 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

// Called when the game starts or when spawned
void AFirearm::BeginPlay()
{
	Super::BeginPlay();
	_playerController = Cast<ACyberpunkProjectPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	_amountInFirearm = _MAXAMMOINFIREARM;
	_ammoType = _firearmType;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

// Called every frame
void AFirearm::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (_timeSinceLastShot > 0)
	{
		_timeSinceLastShot -= DeltaTime;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void AFirearm::Interacting(APlayerCharacter* Player = nullptr)
{
	if (Player != nullptr)
	{
		Player->PickUpWeapon(this);		
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void AFirearm::DeactivateBoxCollider()
{
	_collisionBox->Deactivate();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void AFirearm::InitiateShot()
{
	if (GetAmmoAmountInFirearm() > 0 && _timeSinceLastShot <= 0)
	{
		Shoot();
	}

	else if (GetAmmoAmountInFirearm() == 0)
	{
		if (_emptySound != nullptr)
		{		
			_emptySound->Play();
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void AFirearm::Shoot()
{
	//Code for raycasting and particle effect.
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = _raycastStart->GetComponentLocation();
	FVector ForwardVector = -_raycastStart->GetForwardVector();
	FVector EndTrace = ((ForwardVector * 5000.f) + StartTrace);
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
	_timeSinceLastShot = _shotInterval;

	if (_shotSound != nullptr)
	{
		_shotSound->Play();
	}
	
	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))
	{
		PrintHitResult();
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), false, 5.0f);
		if (HitResult->GetActor()->IsA(ANPC::StaticClass()))
		{
			ANPC* npc = Cast<ANPC>(HitResult->GetActor());
			if ("Head" == HitResult->BoneName)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Headshot")));
				npc->NPCHeadShot();

			}

			else
			{
				npc->NPCTakeDamage(_damage);
			}			
		}		
	}

	SetAmmoAmountInFirearm(GetAmmoAmountInFirearm()-1);

	if (_playerController != nullptr)
	{
		_playerController->UpdateCurrentAmmoInFirearm(GetAmmoAmountInFirearm());
	}

	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("The player controller reference is a nullptr")));
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

bool AFirearm::IsReloadPossible()
{		
	//Returns true if the ammo in the firearm is lower than max.
	return GetAmmoAmountInFirearm() < GetMaxAmountInFirearm();	
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void AFirearm::Reload(uint16 &AmmoInPouch)
{
	uint8 ammoToReload = GetMaxAmountInFirearm() - GetAmmoAmountInFirearm();
	if (_reloadSound != nullptr)
	{
		_reloadSound->Play();
	}
	//If the amount carried is more or equal to the max amount of ammo that the firearm can hold...
	if (AmmoInPouch >= ammoToReload)
	{
		AmmoInPouch = AmmoInPouch - ammoToReload;
		SetAmmoAmountInFirearm(GetAmmoAmountInFirearm() + ammoToReload);
	}

	else
	{
		SetAmmoAmountInFirearm(GetAmmoAmountInFirearm() + AmmoInPouch);
		AmmoInPouch = 0;
	}

	if (_playerController != nullptr)
	{
		_playerController->UpdateCurrentAmmoInFirearm(GetAmmoAmountInFirearm());
		_playerController->UpdateCurrentAmmoCarried(AmmoInPouch);
	}

	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("The player controller reference is a nullptr")));
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

EFirearmType AFirearm::GetFirearmType()
{
	return _firearmType;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

EFireType AFirearm::GetFireType()
{
	return _fireType;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void AFirearm::AddPickedUpAmmo(uint16 PickedUpAmmoAmount, uint16 &AmmoInPouch, uint16 MaxAmmoInPouch)
{
	uint8 ammoToIncreaseBy = MaxAmmoInPouch - AmmoInPouch;

	if (ammoToIncreaseBy <= PickedUpAmmoAmount)
	{
		AmmoInPouch = MaxAmmoInPouch;
	}

	else
	{
		AmmoInPouch = AmmoInPouch + PickedUpAmmoAmount;
	}

	if (_playerController != nullptr)
	{
		_playerController->UpdateCurrentAmmoCarried(AmmoInPouch);
	}

	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("The player controller reference is a nullptr")));
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

uint8 AFirearm::GetAmmoAmountInFirearm()
{
	return _amountInFirearm;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void AFirearm::SetAmmoAmountInFirearm(uint8 NewAmmoAmountInFirearm)
{
	_amountInFirearm = NewAmmoAmountInFirearm;

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

uint8 AFirearm::GetMaxAmountInFirearm()
{
	return _MAXAMMOINFIREARM;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

EFirearmType AFirearm::GetAmmoType()
{
	return _ammoType;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void AFirearm::ShowFirearmMesh(bool Status)
{	
	_firearmMesh->SetVisibility(Status);	
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

EWeaponNames AFirearm::GetWeaponName()
{
	return _weaponName;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

uint8 AFirearm::ExtractAmmo(uint16 RequestedAmmo, uint16 &AmmoInPouch, uint16 MaxAmmoInPouch)
{
	uint8 extractedAmmo = 0;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Ammo needed: %d "), RequestedAmmo));

	if (MaxAmmoInPouch == RequestedAmmo)
	{
		extractedAmmo = _amountInFirearm;
		_amountInFirearm = 0;
	}

	else if (_amountInFirearm > RequestedAmmo)
	{
		extractedAmmo = RequestedAmmo;
		_amountInFirearm -= RequestedAmmo;
	}

	else if (_amountInFirearm < RequestedAmmo)
	{		
		extractedAmmo = _amountInFirearm;
		_amountInFirearm = 0;
	}
	
	return extractedAmmo;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void AFirearm::SetAimLaserActive(bool Status)
{
	if (Status)
	{
		_aimLaser->SetVisibility(true);
	}

	else
	{
		_aimLaser->SetVisibility(false);
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//