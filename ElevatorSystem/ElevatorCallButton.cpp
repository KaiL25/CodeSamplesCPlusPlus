#include "CyberpunkProject.h"
#include "ElevatorCallButton.h"
#include "Elevator.h"
#include "Engine.h"
#include "SlidingDoor.h"



// Sets default values
AElevatorCallButton::AElevatorCallButton()
{
	_floorMarker = CreateDefaultSubobject<USceneComponent>(TEXT("Floor Marker"));	
}

//---------------------------------------------------------------------------------------------------------------//

void AElevatorCallButton::Interacting()
{		
	_elevatorToCall->CallElevator(_floor);	
}

//---------------------------------------------------------------------------------------------------------------//

void AElevatorCallButton::OpenOuterDoors()
{
	for (int i = 0; i < _slidingDoors.Num(); i++)
	{
		if (_slidingDoors[i] != NULL)
		{
			_slidingDoors[i]->OpenDoor();			
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("_slidingDoors at i is NULL"));
		}
	}
}

//---------------------------------------------------------------------------------------------------------------//

void AElevatorCallButton::CloseOuterDoors()
{
	for (int i = 0; i < _slidingDoors.Num(); i++)
	{
		if (_slidingDoors[i] != NULL)
		{
			_slidingDoors[i]->CloseDoor();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("_slidingDoors at i is NULL"));
		}
	}
}

//---------------------------------------------------------------------------------------------------------------//

FVector AElevatorCallButton::GetFloorMarkerLocation()
{
	return _floorMarker->GetComponentLocation();
}

//---------------------------------------------------------------------------------------------------------------//


