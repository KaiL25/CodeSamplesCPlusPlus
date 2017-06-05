// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Enums.h"
#include "Interactable.h"
#include "Firearm.generated.h"

class ACyberpunkProjectPlayerController;
class AAmmo;

UCLASS()
class CYBERPUNKPROJECT_API AFirearm : public AInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFirearm();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
	void InitiateShot();

	UFUNCTION()
	bool IsReloadPossible();

	UFUNCTION()
	void Shoot();

	UFUNCTION()
	void Reload(uint16 &AmmoInPouch);

	UFUNCTION()
	EFirearmType GetFirearmType();

	UFUNCTION()
	EFireType GetFireType();	

	UFUNCTION()
	void AddPickedUpAmmo(uint16 PickedUpAmmoAmount, uint16 &AmmoInPouch, uint16 MaxAmmoInPouch);

	UFUNCTION()
	virtual void Interacting(APlayerCharacter* Player) override;

	EFirearmType GetAmmoType();
	uint8 GetAmmoAmountInFirearm();
	void SetAmmoAmountInFirearm(uint8 NewAmmoAmountInFirearm);		
	uint8 GetMaxAmountInFirearm();
	void ShowFirearmMesh(bool Status);
	void DeactivateBoxCollider();	
	EWeaponNames GetWeaponName();
	//Used when player, or npc already has this type of weapon to salvage ammo from it.
	uint8 ExtractAmmo(uint16 RequestedAmmo, uint16 &AmmoInPouch, uint16 MaxAmmoInPouch);
	void SetAimLaserActive(bool Status);

	//FOR DEBUGGING
	UFUNCTION(BlueprintImplementableEvent, Category = "Firearm")
	void PrintHitResult();

protected:	

	UPROPERTY(EditAnywhere, Category = Firearm)
	bool bPickedUp;		

	UPROPERTY(EditAnywhere, Category = Firearm)
	UStaticMeshComponent* _firearmMesh;

	UPROPERTY(EditAnywhere, Category = Firearm)
	UStaticMeshComponent* _aimLaser;

	UPROPERTY(EditAnywhere, Category = Firearm)
	USceneComponent* _raycastStart;

	UPROPERTY(EditAnywhere, Category = Firearm)
	uint8 _damage;

	UPROPERTY(EditAnywhere, Category = Firearm)
	float _shotInterval;	
		
	float _timeSinceLastShot;

	UPROPERTY(EditAnywhere, Category = Firearm)
	EFirearmType _firearmType;

	UPROPERTY(EditAnywhere, Category = Firearm)
	EFireType _fireType;

	///Max amount of ammo that can fit in the firearm (magazine)
	UPROPERTY(EditAnywhere, Category = Ammo)
	uint8 _MAXAMMOINFIREARM;	

	UPROPERTY(EditAnywhere, Category = Firearm)
	UAudioComponent* _shotSound;

	UPROPERTY(EditAnywhere, Category = Firearm)
	UAudioComponent* _emptySound;

	UPROPERTY(EditAnywhere, Category = Firearm)
	UAudioComponent* _reloadSound;

	UPROPERTY(EditAnywhere, Category = Firearm)
	EWeaponNames _weaponName;

	///Necessary for communication with UI
	UPROPERTY(VisibleAnywhere, Category = Firearm)
	ACyberpunkProjectPlayerController* _playerController;

	///Amount of ammo in the firearm (magazine)
	UPROPERTY(VisibleAnywhere, Category = Firearm)
	uint8 _amountInFirearm;	

	EFirearmType _ammoType;	
};
