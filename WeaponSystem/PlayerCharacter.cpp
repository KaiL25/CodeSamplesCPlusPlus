// Fill out your copyright notice in the Description page of Project Settings.

#include "CyberpunkProject.h"
#include "PlayerCharacter.h"
#include "Interactable.h"
#include "Engine.h"
#include "ElevatorPanel.h"
#include "Firearm.h"
#include "PlayerCharacterAnimInstance.h"
#include "CyberpunkProjectPlayerController.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

												// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

												   // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
												   // are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	
	_originalCameraArmLength = 100;
	_zoomCameraArmLength = 60;
	_originalFieldOfView = 90;
	_zoomFieldOfView = 75;

	_cameraCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CameraCollisionSphere"));
	_cameraCollisionSphere->SetupAttachment(FollowCamera);
	_cameraCollisionSphere->bGenerateOverlapEvents = true;
	_cameraCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnBeginOverlapCameraCollisionSphere);
	_cameraCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnEndOverlapCameraCollisionSphere);	

	_pistolHolsterSocket = CreateDefaultSubobject<USceneComponent>(TEXT("PistolHolsterSocket"));
	_pistolHandSocket = CreateDefaultSubobject<USceneComponent>(TEXT("PistolHandSocket"));
	_rifleHolsterSocket = CreateDefaultSubobject<USceneComponent>(TEXT("RifleHolsterSocket"));
	_rifleHandSocket = CreateDefaultSubobject<USceneComponent>(TEXT("RifleHandSocket"));
	_pistolHolsterSocket->SetupAttachment(GetMesh(), "PistolHolster");
	_pistolHandSocket->SetupAttachment(GetMesh(), "Firearm");
	_rifleHolsterSocket->SetupAttachment(GetMesh(), "RifleHolster");
	_rifleHandSocket->SetupAttachment(GetMesh(), "Firearm");	
	_currentFirearm = 0;
		
	this->Tags.Add("Player");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	_animInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	_playerController = Cast<ACyberpunkProjectPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));	
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

// Called every frame
void APlayerCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction("SelectVerticalUp", IE_Pressed, this, &APlayerCharacter::SelectVerticalUp);
	PlayerInputComponent->BindAction("SelectVerticalDown", IE_Pressed, this, &APlayerCharacter::SelectVerticalDown);
	PlayerInputComponent->BindAction("SelectHorizontalRight", IE_Pressed, this, &APlayerCharacter::SelectHorizontalRight);
	PlayerInputComponent->BindAction("SelectHorizontalLeft", IE_Pressed, this, &APlayerCharacter::SelectHorizontalLeft);
	PlayerInputComponent->BindAction("Accept", IE_Pressed, this, &APlayerCharacter::Accept);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &APlayerCharacter::EquipNextWeapon);
	PlayerInputComponent->BindAction("PreviousWeapon", IE_Pressed, this, &APlayerCharacter::EquipPreviousWeapon);
	//PlayerInputComponent->BindAction("Draw/HolsterWeapon", IE_Pressed, this, &APlayerCharacter::EquipWeapon);
	PlayerInputComponent->BindAction("ShootSemiAutomaticFirearm", IE_Pressed, this, &APlayerCharacter::ShootSemiAutomaticFirearm);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::Reload);
	PlayerInputComponent->BindAxis("Aiming", this, &APlayerCharacter::Aiming);
	PlayerInputComponent->BindAxis("ShootAutomaticFirearm", this, &APlayerCharacter::ShootAutomaticFirearm);	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::MoveForward(float Value)
{	
	if (!_cameraFocusChanged)
	{
		if ((Controller != NULL) && (Value != 0.0f))
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::MoveRight(float Value)
{	
	if (!_cameraFocusChanged)
	{
		if ((Controller != NULL) && (Value != 0.0f))
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::SelectVerticalUp()
{
	if (_cameraFocusChanged)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Current input horizontal: %f "), 1));
		AElevatorPanel * elevatorPanel = Cast<AElevatorPanel>(_currentInteractable);
		elevatorPanel->SelectNewButtonVertical(1);
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::SelectVerticalDown()
{
	if (_cameraFocusChanged)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Current input horizontal: %f "), -1));
		AElevatorPanel * elevatorPanel = Cast<AElevatorPanel>(_currentInteractable);
		elevatorPanel->SelectNewButtonVertical(-1);
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::SelectHorizontalRight()
{
	if (_cameraFocusChanged)
	{
		AElevatorPanel * elevatorPanel = Cast<AElevatorPanel>(_currentInteractable);
		elevatorPanel->SelectNewButtonHorizontal(1);
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::SelectHorizontalLeft()
{
	if (_cameraFocusChanged)
	{
		AElevatorPanel * elevatorPanel = Cast<AElevatorPanel>(_currentInteractable);
		elevatorPanel->SelectNewButtonHorizontal(-1);
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::OnBeginOverlapCameraCollisionSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this)
	{		
		GetMesh()->SetVisibility(false, true);
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::OnEndOverlapCameraCollisionSphere(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == this)
	{		
		GetMesh()->SetVisibility(true, true);
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::Accept()
{
	AElevatorPanel * elevatorPanel = Cast<AElevatorPanel>(_currentInteractable);
	elevatorPanel->PanelInput();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::PickUpWeapon(AFirearm* NewFirearm)
{
	if (!IsWeaponInPossession(NewFirearm->GetWeaponName()))
	{
		_firearmsInPossession.Add(NewFirearm);
		_firearmsInPossession[_currentFirearm]->DeactivateBoxCollider();
		_currentFirearm = _firearmsInPossession.Num() - 1;

		//Pistols and smgs go into the pistol socket
		if (_firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Pistol || _firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_SMG)
		{
			_firearmsInPossession[_currentFirearm]->AttachRootComponentTo(_pistolHolsterSocket, "PistolHolster", EAttachLocation::SnapToTarget, true);
		}

		//Any Rifles and Shotguns go into the rifle socket
		else if (_firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Rifle || _firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Shotgun)
		{
			_firearmsInPossession[_currentFirearm]->AttachRootComponentTo(_rifleHolsterSocket, "RifleHolster", EAttachLocation::SnapToTarget, true);
		}		
	}

	//If the weapon is already in possession add its ammo if possible...
	else
	{		
		RefillAmmo(NewFirearm);	
	}

	_playerController->UpdateCurrentAmmoCarried(_ammoPouch.GetAmmoOfTypeInPouch(_firearmsInPossession[_currentFirearm]->GetAmmoType()));
	_playerController->UpdateCurrentAmmoInFirearm(_firearmsInPossession[_currentFirearm]->GetAmmoAmountInFirearm());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::DropWeapon()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

bool APlayerCharacter::IsWeaponInPossession(EWeaponNames WeaponName)
{
	for (int i = 0; i < _firearmsInPossession.Num(); i++)
	{
		if (_firearmsInPossession[i]->GetWeaponName() == WeaponName)
		{
			return true;			
		}
	}
	return false;
	
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::RefillAmmo(AFirearm* FirearmToExtractAmmoFrom)
{
	for (int i = 0; i < _firearmsInPossession.Num(); i++)
	{
		if (_firearmsInPossession[i]->GetFirearmType() == FirearmToExtractAmmoFrom->GetFirearmType())
		{
			//NEEDS TO BE SIMPLIFIED
			_ammoPouch.SetAmmoOfTypeInPouch(FirearmToExtractAmmoFrom->GetAmmoType(), _ammoPouch.GetAmmoOfTypeInPouch(FirearmToExtractAmmoFrom->GetAmmoType()) + (FirearmToExtractAmmoFrom->ExtractAmmo(GetAmmoNeeded(i), _ammoPouch.GetAmmoOfTypeInPouch(FirearmToExtractAmmoFrom->GetAmmoType()), _ammoPouch.GetMaxAmmoOfTypeInPouch(FirearmToExtractAmmoFrom->GetAmmoType()))));
			break;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

uint16 APlayerCharacter::GetAmmoNeeded(uint8 WeaponIndex)
{		
	return  _ammoPouch.GetMaxAmmoOfTypeInPouch(_firearmsInPossession[WeaponIndex]->GetAmmoType()) - _ammoPouch.GetAmmoOfTypeInPouch(_firearmsInPossession[WeaponIndex]->GetAmmoType());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::EquipNewWeapon()
{
	if (_firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Pistol || _firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_SMG)
	{
		if (_pistolMontage != nullptr)
		{
			PlayAnimMontage(_pistolMontage, 3.f, "EquipPistol");
			_firearmsInPossession[_currentFirearm]->AttachRootComponentTo(_pistolHandSocket, "Firearm", EAttachLocation::SnapToTarget, true);
		}
	}

	else if (_firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Rifle || _firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Shotgun)
	{
		if (_rifleMontage != nullptr)
		{
			PlayAnimMontage(_rifleMontage, 3.f, "EquipRifle");
			_firearmsInPossession[_currentFirearm]->AttachRootComponentTo(_rifleHandSocket, "Firearm", EAttachLocation::SnapToTarget, true);
		}
	}
	_firearmsInPossession[_currentFirearm]->SetAimLaserActive(true);
	_animInstance->SetWeaponHolstered(false);
	_startedHolsteringFirearm = false;	
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::UnequipOldWeapon()
{
	if (_firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Pistol || _firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_SMG)
	{
		PlayAnimMontage(_pistolMontage, 3.f, "HolsterPistol");
	}

	else if (_firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Rifle || _firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Shotgun)
	{
		PlayAnimMontage(_rifleMontage, 3.f, "HolsterRifle");
	}

	if (_firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Pistol || _firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_SMG)
	{
		if (_pistolMontage != nullptr)
		{
			StopAnimMontage(_pistolMontage);
			_firearmsInPossession[_currentFirearm]->AttachRootComponentTo(_pistolHolsterSocket, "PistolHolster", EAttachLocation::SnapToTarget, true);
		}
	}

	else if (_firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Rifle || _firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Shotgun)
	{
		if (_rifleMontage != nullptr)
		{
			StopAnimMontage(_rifleMontage);
			_firearmsInPossession[_currentFirearm]->AttachRootComponentTo(_rifleHolsterSocket, "RifleHolster", EAttachLocation::SnapToTarget, true);			
		}
	}
	_firearmsInPossession[_currentFirearm]->SetAimLaserActive(false);
	_animInstance->SetWeaponHolstered(true);

	
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::EquipNextWeapon()
{
	if (_firearmsInPossession.Num() > 1)
	{
		if (_isAiming)
		{
			UnequipOldWeapon();
		}

		if (_currentFirearm < _firearmsInPossession.Num() - 1)
		{
			_currentFirearm++;
		}

		else
		{
			_currentFirearm = 0;
		}

		_playerController->UpdateCurrentAmmoCarried(_ammoPouch.GetAmmoOfTypeInPouch(_firearmsInPossession[_currentFirearm]->GetAmmoType()));
		_playerController->UpdateCurrentAmmoInFirearm(_firearmsInPossession[_currentFirearm]->GetAmmoAmountInFirearm());

		if (_isAiming)
		{
			EquipNewWeapon();
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::EquipPreviousWeapon()
{
	if (_firearmsInPossession.Num() > 1)
	{
		if (_isAiming)
		{
			UnequipOldWeapon();
		}

		if (_currentFirearm > 0)
		{
			_currentFirearm--;
		}

		else
		{
			_currentFirearm = _firearmsInPossession.Num() - 1;
		}

		_playerController->UpdateCurrentAmmoCarried(_ammoPouch.GetAmmoOfTypeInPouch(_firearmsInPossession[_currentFirearm]->GetAmmoType()));
		_playerController->UpdateCurrentAmmoInFirearm(_firearmsInPossession[_currentFirearm]->GetAmmoAmountInFirearm());

		if (_isAiming)
		{
			EquipNewWeapon();
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::Aiming(float Value)
{
	if (_firearmsInPossession.Num() > 0)
	{
		if (Value > 0)
		{
			_isAiming = true;
			EquipWeapon();
		}

		else 
		{
			_isAiming = false;
			HolsterWeapon();
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::EquipWeapon()
{
	CameraBoom->TargetArmLength = _zoomCameraArmLength;
	FollowCamera->FieldOfView = _zoomFieldOfView;
	FollowCamera->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	if (_animInstance->GetWeaponHolstered())
	{

		if (_firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Pistol || _firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_SMG)
		{
			if (_pistolMontage != nullptr)
			{
				PlayAnimMontage(_pistolMontage, 3.f, "EquipPistol");
				_firearmsInPossession[_currentFirearm]->AttachRootComponentTo(_pistolHandSocket, "Firearm", EAttachLocation::SnapToTarget, true);
			}
		}

		else if (_firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Rifle || _firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Shotgun)
		{
			if (_rifleMontage != nullptr)
			{
				PlayAnimMontage(_rifleMontage, 3.f, "EquipRifle");
				_firearmsInPossession[_currentFirearm]->AttachRootComponentTo(_rifleHandSocket, "Firearm", EAttachLocation::SnapToTarget, true);
			}
		}
		_firearmsInPossession[_currentFirearm]->SetAimLaserActive(true);
		_animInstance->SetWeaponHolstered(false);
		_startedHolsteringFirearm = false;		
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//


void APlayerCharacter::HolsterWeapon()
{
	if (!_startedHolsteringFirearm)
	{
		if (_firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Pistol || _firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_SMG)
		{
			PlayAnimMontage(_pistolMontage, 3.f, "HolsterPistol");
		}

		else if (_firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Rifle || _firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Shotgun)
		{
			PlayAnimMontage(_rifleMontage, 3.f, "HolsterRifle");
		}
		
		CameraBoom->TargetArmLength = _originalCameraArmLength;
		FollowCamera->FieldOfView = _originalFieldOfView;
		FollowCamera->bUsePawnControlRotation = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;		
		_firearmsInPossession[_currentFirearm]->SetAimLaserActive(false);
		_startedHolsteringFirearm = true;
	}

	if (_animInstance->GetWeaponHolstered())
	{
		if (_firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Pistol || _firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_SMG)
		{
			if (_pistolMontage != nullptr)
			{
				StopAnimMontage(_pistolMontage);
				_firearmsInPossession[_currentFirearm]->AttachRootComponentTo(_pistolHolsterSocket, "PistolHolster", EAttachLocation::SnapToTarget, true);
			}
		}

		else if (_firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Rifle || _firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Shotgun)
		{
			if (_rifleMontage != nullptr)
			{
				StopAnimMontage(_rifleMontage);
				_firearmsInPossession[_currentFirearm]->AttachRootComponentTo(_rifleHolsterSocket, "RifleHolster", EAttachLocation::SnapToTarget, true);
			}
		}		
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::ShootSemiAutomaticFirearm()
{
	if (_firearmsInPossession[_currentFirearm] != nullptr && _firearmsInPossession[_currentFirearm]->GetFireType() == EFireType::VE_Semiautomatic)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Firing semiautomatic weapon")));
		_firearmsInPossession[_currentFirearm]->InitiateShot();
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::ShootAutomaticFirearm(float Value)
{
	if (Value > 0 && _firearmsInPossession[_currentFirearm] != nullptr && _firearmsInPossession[_currentFirearm]->GetFireType() == EFireType::VE_Automatic)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Firing automatic weapon")));
		_firearmsInPossession[_currentFirearm]->InitiateShot();
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::Reload()
{
	//If the firearm is not holstered and the player possesses a firearm...
	if (!_animInstance->GetWeaponHolstered() && _firearmsInPossession[_currentFirearm] != nullptr)
	{		
		//If the firearm type is Pistol, or SMG and pistol montage has been assigned in the editor...
		if (_firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Pistol && _pistolMontage != nullptr)
		{
			//If the Pistol Reload anim is not already playing and the gun can be reloaded...
			if (_animInstance->Montage_GetCurrentSection(_pistolMontage) != "PistolReload" && _firearmsInPossession[_currentFirearm]->IsReloadPossible() && _ammoPouch._handgunAmmo > 0)
			{
				PlayAnimMontage(_pistolMontage, 1.0f, "PistolReload");
				_firearmsInPossession[_currentFirearm]->Reload(_ammoPouch._handgunAmmo);
			}
		}

		else if (_firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_SMG && _pistolMontage != nullptr)
		{
			//If the Pistol Reload anim is not already playing and the gun can be reloaded...
			if (_animInstance->Montage_GetCurrentSection(_pistolMontage) != "PistolReload" && _firearmsInPossession[_currentFirearm]->IsReloadPossible() && _ammoPouch._smgAmmo > 0)
			{
				PlayAnimMontage(_pistolMontage, 1.0f, "PistolReload");
				_firearmsInPossession[_currentFirearm]->Reload(_ammoPouch._smgAmmo);
			}
		}

		else if (_firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Rifle && _rifleMontage != nullptr)
		{			
			if (_animInstance->Montage_GetCurrentSection(_rifleMontage) != "Reload" && _firearmsInPossession[_currentFirearm]->IsReloadPossible() && _ammoPouch._rifleAmmo > 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Reload")));
				PlayAnimMontage(_rifleMontage, 1.0f, "ReloadRifleIronsights");
				_firearmsInPossession[_currentFirearm]->Reload(_ammoPouch._rifleAmmo);
			}
		}

		else if (_firearmsInPossession[_currentFirearm]->GetFirearmType() == EFirearmType::VE_Shotgun && _rifleMontage != nullptr)
		{			
			if (_animInstance->Montage_GetCurrentSection(_rifleMontage) != "Reload" && _firearmsInPossession[_currentFirearm]->IsReloadPossible() && _ammoPouch._shotgunAmmo > 0)
			{
				PlayAnimMontage(_rifleMontage, 1.0f, "ReloadRifleIronsights");
				_firearmsInPossession[_currentFirearm]->Reload(_ammoPouch._shotgunAmmo);
			}
		}
		
	}

	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("If weapon was not holstered _pistolWalkMontage, or _currentFirearm may be a nullptr.")));
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

bool APlayerCharacter::CanPickUpAmmo(EFirearmType AmmoType)
{
	return _ammoPouch.GetAmmoOfTypeInPouch(AmmoType) < _ammoPouch.GetMaxAmmoOfTypeInPouch(AmmoType);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::AddPickedUpAmmo(EFirearmType AmmoType, uint8 &AddedAmmoAmount)
{
	uint16 amountNeededToFill = _ammoPouch.GetMaxAmmoOfTypeInPouch(AmmoType) - _ammoPouch.GetAmmoOfTypeInPouch(AmmoType);
	if (AddedAmmoAmount > amountNeededToFill)
	{
		AddedAmmoAmount -= amountNeededToFill;
		_ammoPouch.SetAmmoOfTypeInPouch(AmmoType, amountNeededToFill);
	}

	else
	{
		_ammoPouch.SetAmmoOfTypeInPouch(AmmoType, AddedAmmoAmount);
		AddedAmmoAmount = 0;
	}
	_playerController->UpdateCurrentAmmoCarried(_ammoPouch.GetAmmoOfTypeInPouch(_firearmsInPossession[_currentFirearm]->GetAmmoType()));
			
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::SetInRangeOfInteractable(AInteractable* interactableReference)
{
	if (interactableReference != nullptr)
	{
		_currentInteractable = interactableReference;
		_inRangeOfInteractable = true;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::SetOutOfRangeOfInteractable()
{
	_inRangeOfInteractable = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::Interact()
{
	if (_inRangeOfInteractable)
	{
		_currentInteractable->Interacting(this);
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//

void APlayerCharacter::SetCameraFocusChanged(bool status)
{
	_cameraFocusChanged = status;
	if (status)
	{
		GetMesh()->SetVisibility(false);
	}

	else
	{
		GetMesh()->SetVisibility(true);
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------//


