#include "CyberpunkProject.h"
#include "SlidingDoor.h"
#include "Engine.h"


// Sets default values
ASlidingDoor::ASlidingDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	_doorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	_targetLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Target Location"));
	_targetLocation->SetupAttachment(RootComponent);
	_doorMesh->SetupAttachment(RootComponent);
}

//---------------------------------------------------------------------------------------------------------------//

// Called when the game starts or when spawned
void ASlidingDoor::BeginPlay()
{
	Super::BeginPlay();	
	DetermineOpeningClosingAxis();
}

//---------------------------------------------------------------------------------------------------------------//

// Called every frame
void ASlidingDoor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (_opening)
	{
		if (!_open)
		{
			_doorMesh->SetWorldLocation(FMath::VInterpTo(_doorMesh->GetComponentLocation(), _targetLocation->GetComponentLocation(), DeltaTime, _slideSpeed));
			if (_openingAxis == EAxisEnum::VE_X)
			{
				if (_openingDirection == EOpeningDirectionEnum::VE_LEFT)
				{
					if (_doorMesh->GetComponentLocation().X >= _targetLocation->GetComponentLocation().X - 0.1)
					{
						_open = true;
						_countDownToClosing = true;
						_opening = false;
						_closing = false;
					}
				}

				else
				{
					if (_doorMesh->GetComponentLocation().X <= _targetLocation->GetComponentLocation().X + 0.1)
					{
						_open = true;
						_countDownToClosing = true;
						_opening = false;
						_closing = false;
					}					
				}				
			}

			else if (_openingAxis == EAxisEnum::VE_Z)
			{
				if (_openingDirection == EOpeningDirectionEnum::VE_DOWN)
				{
					if (_doorMesh->GetComponentLocation().Z <= _targetLocation->GetComponentLocation().Z + 0.1)
					{
						_open = true;
						_countDownToClosing = true;
						_opening = false;
						_closing = false;
					}
				}

				else
				{
					if (_doorMesh->GetComponentLocation().Z >= _targetLocation->GetComponentLocation().Z - 0.1)
					{
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Door opened up"));
						_open = true;
						_countDownToClosing = true;
						_opening = false;	
						_closing = false;
					}
				}				
			}
		}

		else
		{
			_opening = false;
		}
	}

	else if (_closing)
	{
		if (_open)
		{
			_doorMesh->SetWorldLocation(FMath::VInterpTo(_doorMesh->GetComponentLocation(), _initialLocation, DeltaTime, _slideSpeed));
			if (_openingAxis == EAxisEnum::VE_X)
			{
				if (_openingDirection == EOpeningDirectionEnum::VE_LEFT)
				{
					if (_doorMesh->GetComponentLocation().X <= _initialLocation.X + 0.1)
					{
						_open = false;
						_closing = false;
						_opening = false;
						if (OnDoorClosed.IsBound())
						{
							OnDoorClosed.Broadcast();
						}
					}
				}

				else
				{
					if (_doorMesh->GetComponentLocation().X >= _initialLocation.X - 0.1)
					{
						_open = false;
						_closing = false;
						_opening = false;
						if (OnDoorClosed.IsBound())
						{
							OnDoorClosed.Broadcast();
						}
					}
				}
			}	

			else if (_openingAxis == EAxisEnum::VE_Z)
			{
				if (_openingDirection == EOpeningDirectionEnum::VE_DOWN)
				{
					if (_doorMesh->GetComponentLocation().Z >= _initialLocation.Z - 0.1)
					{						
						_open = false;
						_closing = false;
						_opening = false;
						if (OnDoorClosed.IsBound())
						{
							OnDoorClosed.Broadcast();
						}
					}
				}

				else
				{
					if (_doorMesh->GetComponentLocation().Z <= _initialLocation.Z + 0.1)
					{
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Door closed down"));
						_open = false;
						_closing = false;
						_opening = false;
						if (OnDoorClosed.IsBound())
						{
							OnDoorClosed.Broadcast();
						}
					}					
				}				
			}			
		}

		else
		{
			_closing = false;
		}
	}

	if (_countDownToClosing && !_opening && !_closing)
	{
		_closingCountdown += DeltaTime;
		if (_closingCountdown >= _timeUntilDoorClosesAutomatically)
		{			
			_countDownToClosing = false;
			_closingCountdown = 0;
			_closing = true;
		}
	}
}

//---------------------------------------------------------------------------------------------------------------//

void ASlidingDoor::OpenDoor()
{	
	if (!GetDoorSliding() && !_open)
	{
		_initialLocation = _doorMesh->GetComponentLocation();
		_opening = true;
	}
}

//---------------------------------------------------------------------------------------------------------------//

void ASlidingDoor::CloseDoor()
{	
	if (!GetDoorSliding() && _open)
	{
		_closing = true;
	}
}

//---------------------------------------------------------------------------------------------------------------//

bool ASlidingDoor::GetDoorSliding()
{
	if (_closing || _opening)
	{
		return 1;
	}

	else
	{
		return 0;
	}
}

//---------------------------------------------------------------------------------------------------------------//

void ASlidingDoor::DetermineOpeningClosingAxis()
{
	//If the door opens in the x axis
	if (_openingAxis == EAxisEnum::VE_X)
	{
		if (_doorMesh->GetComponentLocation().X > _targetLocation->GetComponentLocation().X)
		{
			_openingDirection = EOpeningDirectionEnum::VE_RIGHT;
		}

		else
		{
			_openingDirection = EOpeningDirectionEnum::VE_LEFT;
		}
	}
	else if (_openingAxis == EAxisEnum::VE_Z)
	{
		if (_doorMesh->GetComponentLocation().Z > _targetLocation->GetComponentLocation().Z)
		{
			_openingDirection = EOpeningDirectionEnum::VE_DOWN;
		}

		else
		{
			_openingDirection = EOpeningDirectionEnum::VE_UP;			
		}
	}	
}

//---------------------------------------------------------------------------------------------------------------//

bool ASlidingDoor::GetDoorOpen()
{
	return _open;
}

//---------------------------------------------------------------------------------------------------------------//