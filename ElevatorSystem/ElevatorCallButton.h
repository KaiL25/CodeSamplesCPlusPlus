#pragma once

#include "GameFramework/Actor.h"
#include "ButtonParentClass.h"
#include "ElevatorCallButton.generated.h"

class AElevator;
class ASlidingDoor;

UCLASS()
class CYBERPUNKPROJECT_API AElevatorCallButton : public AButtonParentClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElevatorCallButton();

	void OpenOuterDoors();
	void CloseOuterDoors();

	FVector GetFloorMarkerLocation();

private:
	
	UPROPERTY(EditAnywhere, Category = ElevatorCallButton)
	uint8 _floor;
	
	//Used to give the elevator a vector3 to stop at.
	UPROPERTY(EditAnywhere, Category = ElevatorCallButton)
	USceneComponent* _floorMarker;

	UPROPERTY(EditAnywhere, Category = ElevatorCallButton)
	AElevator* _elevatorToCall;

	UPROPERTY(EditAnywhere, Category = ElevatorCallButton)
	TArray<ASlidingDoor*> _slidingDoors;

	UFUNCTION()
	virtual void  Interacting() override;
};
