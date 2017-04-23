
#include "CyberpunkProject.h"
#include "Elevator.h"
#include "ElevatorCallButton.h"
#include "SlidingDoor.h"
#include "CyberpunkProjectCharacter.h"
#include "CyberpunkProjectPlayerController.h"

// Sets default values
AElevator::AElevator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_elevatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Elevator"));
	_elevatorCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Elevator Camera"));
	_boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	_boxCollision->OnComponentBeginOverlap.AddDynamic(this, &AElevator::OnTriggerEntered);
	_boxCollision->OnComponentEndOverlap.AddDynamic(this, &AElevator::OnTriggerExit);
}

//---------------------------------------------------------------------------------------------------------------//

// Called when the game starts or when spawned
void AElevator::BeginPlay()
{
	Super::BeginPlay();
	_playerControllerReference = Cast<ACyberpunkProjectPlayerController>(GetWorld()->GetFirstPlayerController());
	_innerDoorsClosed = true;
	for (int i = 0; i < _innerElevatorDoors.Num(); i++)
	{
		_innerElevatorDoors[i]->OnDoorClosed.AddDynamic(this, &AElevator::DoorClosedMessage);
	}
}

//---------------------------------------------------------------------------------------------------------------//

// Called every frame
void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (_moving && _innerDoorsClosed)
	{		
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), _targetLocation, DeltaTime, _elevatorSpeed));
		//If the elevator has to go up
		if (_targetFloor > _currentFloor)
		{
			if (GetActorLocation().Z >= _targetLocation.Z - 1)
			{
				_moving = false;
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("arrived because z greater target location"));							
				 _currentFloor = _targetFloor;
				 _elevatorCallButtons[_currentFloor]->OpenOuterDoors();
				OpenInnerDoors();
				_innerDoorsClosed = false;
			}
		}
		//If the elevator has to go down
		else
		{
			if (GetActorLocation().Z <= _targetLocation.Z + 1)
			{
				_moving = false;
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("arrived because z lower target location"));				
				_currentFloor = _targetFloor;
				_elevatorCallButtons[_currentFloor]->OpenOuterDoors();
				OpenInnerDoors();
				_innerDoorsClosed = false;
			}
		}		
	}
}

//---------------------------------------------------------------------------------------------------------------//

void AElevator::CallElevator(uint8 targetFloor)
{
	if (!_moving)
	{		
		if (targetFloor == _currentFloor)
		{
			//Code to open both doors
			if (_elevatorCallButtons[_targetFloor] != NULL)
			{
				_elevatorCallButtons[_targetFloor]->OpenOuterDoors();
			}

			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("_elevatorCallButtons at i is NULL"));
			}
			OpenInnerDoors();
		}

		else
		{			
			if (_elevatorCallButtons[_currentFloor] != NULL)
			{
				_elevatorCallButtons[_currentFloor]->CloseOuterDoors();
			}

			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("_elevatorCallButtons at i is NULL"));
			}
			_targetFloor = targetFloor;
			CloseInnerDoors();
			_targetLocation = _elevatorCallButtons[targetFloor]->GetFloorMarkerLocation();			
			_moving = true;
			CheckIfDoorsClosed();

		}
	}
}

//---------------------------------------------------------------------------------------------------------------//

void AElevator::OpenInnerDoors()
{
	for (int i = 0; i < _innerElevatorDoors.Num(); i++)
	{
		if (_innerElevatorDoors[i] != NULL)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("opening inner doors"));
			_innerElevatorDoors[i]->OpenDoor();
		}

		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("_slidingDoors at i is NULL"));
		}
	}
}

//---------------------------------------------------------------------------------------------------------------//

void AElevator::CloseInnerDoors()
{
	for (int i = 0; i < _innerElevatorDoors.Num(); i++)
	{
		if (_innerElevatorDoors[i] != NULL)
		{
			_innerElevatorDoors[i]->CloseDoor();
		}

		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("_slidingDoors at i is NULL"));
		}
	}
}

//---------------------------------------------------------------------------------------------------------------//

void AElevator::CheckIfDoorsClosed()
{
	uint8 doorsClosed = 0;
	for (int i = 0; i < _innerElevatorDoors.Num(); i++)
	{
		if (!_innerElevatorDoors[i]->GetDoorOpen())
		{
			doorsClosed++;
		}
	}

	if (doorsClosed == _innerElevatorDoors.Num())
	{
		_innerDoorsClosed = true;
	}

	else
	{
		_innerDoorsClosed = false;
	}
}

//---------------------------------------------------------------------------------------------------------------//

void AElevator::DoorClosedMessage()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Inner door closed"));	
	_doorsClosed++;
	if (_doorsClosed == _innerElevatorDoors.Num())
	{
		_doorsClosed = 0;
		_innerDoorsClosed = true;
	}
}

//---------------------------------------------------------------------------------------------------------------//

void AElevator::OnTriggerEntered(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{		
		ACyberpunkProjectCharacter* player = Cast<ACyberpunkProjectCharacter>(OtherActor);
		//change to elevator camera
		_playerControllerReference->SetViewTarget(this);
	}
}

//---------------------------------------------------------------------------------------------------------------//

void AElevator::OnTriggerExit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player"))
	{		
		ACyberpunkProjectCharacter* player = Cast<ACyberpunkProjectCharacter>(OtherActor);
		//change to player camera
		_playerControllerReference->SetViewTarget(player);
	}
}

//---------------------------------------------------------------------------------------------------------------//

void AElevator::CalculateSpeed(float targetZ, float currentZ)
{
	int distance = FMath::Abs(targetZ - currentZ);
	_elevatorSpeed = distance/_elevatorSpeedModifier;
}

//---------------------------------------------------------------------------------------------------------------//
