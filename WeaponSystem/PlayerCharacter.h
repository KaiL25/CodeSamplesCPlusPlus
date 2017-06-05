// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Enums.h"
#include "PlayerCharacter.generated.h"

class AInteractable;
class AFirearm;
class UPlayerCharacterAnimInstance;
class ACyberpunkProjectPlayerController;

USTRUCT()
struct FAmmoPouch
{
	GENERATED_USTRUCT_BODY();


	UPROPERTY(VisibleAnywhere, Category = "AmmoPouch")
	uint16 _handgunAmmo;

	UPROPERTY(VisibleAnywhere, Category = "AmmoPouch")
	uint16 _smgAmmo;

	UPROPERTY(VisibleAnywhere, Category = "AmmoPouch")
	uint16 _rifleAmmo;

	UPROPERTY(VisibleAnywhere, Category = "AmmoPouch")
	uint16 _shotgunAmmo;

	UPROPERTY(VisibleAnywhere, Category = "AmmoPouch")
	uint16 _laserAmmo;

	uint16& GetAmmoOfTypeInPouch(EFirearmType RequestedAmmoType)
	{
		if (RequestedAmmoType == EFirearmType::VE_Pistol)
		{
			return _handgunAmmo;
		}

		else if (RequestedAmmoType == EFirearmType::VE_SMG)
		{
			return _smgAmmo;
		}

		else if (RequestedAmmoType == EFirearmType::VE_Rifle)
		{
			return _rifleAmmo;
		}

		else 
		{
			return _shotgunAmmo;
		}		
	}

	void SetAmmoOfTypeInPouch(EFirearmType RequestedAmmoType, uint16 NewAmount)
	{
		if (RequestedAmmoType == EFirearmType::VE_Pistol)
		{
			 _handgunAmmo = NewAmount;
		}

		else if (RequestedAmmoType == EFirearmType::VE_SMG)
		{
			 _smgAmmo = NewAmount;
		}

		else if (RequestedAmmoType == EFirearmType::VE_Rifle)
		{
			 _rifleAmmo = NewAmount;
		}

		else if (RequestedAmmoType == EFirearmType::VE_Shotgun)
		{
			 _shotgunAmmo = NewAmount;
		}

		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Unknown ammo type was requested.")));			
		}	
	}

	uint16 GetMaxAmmoOfTypeInPouch(EFirearmType RequestedAmmoType)
	{
		if (RequestedAmmoType == EFirearmType::VE_Pistol)
		{
			return _MAXHANDGUNAMMO;
		}

		else if (RequestedAmmoType == EFirearmType::VE_SMG)
		{
			return _MAXSMGAMMO;
		}

		else if (RequestedAmmoType == EFirearmType::VE_Rifle)
		{
			return _MAXRIFLEAMMO;
		}

		else if (RequestedAmmoType == EFirearmType::VE_Shotgun)
		{
			return _MAXSHOTGUNAMMO;
		}

		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Unknown max ammo type was requested.")));
			return 0;
		}
	}

private:

	UPROPERTY(EditAnywhere, Category = "AmmoPouch")
	uint16 _MAXHANDGUNAMMO;

	UPROPERTY(EditAnywhere, Category = "AmmoPouch")
	uint16 _MAXSMGAMMO;

	UPROPERTY(EditAnywhere, Category = "AmmoPouch")
	uint16 _MAXRIFLEAMMO;

	UPROPERTY(EditAnywhere, Category = "AmmoPouch")
	uint16 _MAXSHOTGUNAMMO;

	UPROPERTY(EditAnywhere, Category = "AmmoPouch")
	uint16 _MAXLASERAMMO;
};

UCLASS()
class CYBERPUNKPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void SetInRangeOfInteractable(AInteractable* interactableReference);
	void SetOutOfRangeOfInteractable();
	void SetCameraFocusChanged(bool status);
	bool CanPickUpAmmo(EFirearmType AmmoType);
	void AddPickedUpAmmo(EFirearmType AmmoType, uint8 &AddedAmmoAmount);
	void PickUpWeapon(AFirearm* NewFirearm);

private:

	bool _startedHolsteringFirearm;
	bool _inRangeOfInteractable;
	bool _isAiming;	
	AInteractable* _currentInteractable;
	UPlayerCharacterAnimInstance* _animInstance;
	ACyberpunkProjectPlayerController* _playerController;

	UPROPERTY(EditAnywhere, Category = Player)
	USphereComponent* _cameraCollisionSphere;
	
	//Turn into an array later on.
	UPROPERTY(EditAnywhere, Category = Player)
	TArray<AFirearm*> _firearmsInPossession;

	UPROPERTY(VisibleAnywhere, Category = Player)
	uint8 _currentFirearm;	

	UPROPERTY(EditAnywhere, Category = Player)
	USceneComponent* _pistolHolsterSocket;

	UPROPERTY(EditAnywhere, Category = Player)
	USceneComponent* _pistolHandSocket;

	UPROPERTY(EditAnywhere, Category = Player)
	USceneComponent* _rifleHolsterSocket;

	UPROPERTY(EditAnywhere, Category = Player)
	USceneComponent* _rifleHandSocket;

	UPROPERTY(EditAnywhere, Category = Player)
	UAnimMontage* _pistolMontage;

	UPROPERTY(EditAnywhere, Category = Player)
	UAnimMontage* _rifleMontage;

	UPROPERTY(EditAnywhere, Category = Player)
	uint8 _originalCameraArmLength;

	UPROPERTY(EditAnywhere, Category = Player)
	uint8 _zoomCameraArmLength;

	UPROPERTY(EditAnywhere, Category = Player)
	uint8 _originalFieldOfView;

	UPROPERTY(EditAnywhere, Category = Player)
	uint8 _zoomFieldOfView;

	UPROPERTY(EditAnywhere, Category = Player)
	FAmmoPouch _ammoPouch;

	//Used to prevent player from moving while camera is looking at a different object.
	bool _cameraFocusChanged;

	UFUNCTION()
	void Interact();

	void SelectVerticalUp();
	void SelectVerticalDown();
	void SelectHorizontalRight();
	void SelectHorizontalLeft();
	void Accept();

	//Used when the player aims the weapon
	void EquipWeapon();
	//Used when the player stops aiming the weapon
	void HolsterWeapon();

	//Used when switching weapons
	void EquipNewWeapon();
	//Used when switching weapons
	void UnequipOldWeapon();

	void Aiming(float Value);
	void ShootSemiAutomaticFirearm();
	void ShootAutomaticFirearm(float value);
	void Reload();		
	void EquipNextWeapon();
	void EquipPreviousWeapon();
	void DropWeapon();
	bool IsWeaponInPossession(EWeaponNames WeaponName);
	void RefillAmmo(AFirearm* FirearmToExtractAmmoFrom);

	//Returns the amount of ammo carried the firearm needs for full ammo.
	uint16 GetAmmoNeeded(uint8 WeaponIndex);

	UFUNCTION()
	void OnBeginOverlapCameraCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnEndOverlapCameraCollisionSphere(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);
};
