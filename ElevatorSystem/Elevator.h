#pragma once

#include "GameFramework/Actor.h"
#include "Engine.h"
#include "Elevator.generated.h"

class AElevatorCallButton;
class ASlidingDoor;
class ACyberpunkProjectPlayerController;
UCLASS()
class CYBERPUNKPROJECT_API AElevator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElevator();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void CallElevator(uint8 targetFloor);

private:

	//Reference to the player controller for camera switching.
	ACyberpunkProjectPlayerController* _playerControllerReference;

	//Shows the number of doors closed in order to tell the elevator whether it can start moving.
	uint8 _doorsClosed;

	UPROPERTY(EditAnywhere, Category = Elevator)
	UStaticMeshComponent* _elevatorMesh;

	UPROPERTY(EditAnywhere, Category = Elevator)
	uint8 _currentFloor;	

	/*This value determines the elevators speed.
	  A ratio is calculated that will give the elevator an even speed 
	  regardless of the amount of stories it travels.*/
	UPROPERTY(EditAnywhere, Category = Elevator)
	float _elevatorSpeedModifier;

	UPROPERTY(VisibleAnywhere, Category = Elevator)
	float _elevatorSpeed;

	UPROPERTY(VisibleAnywhere, Category = Elevator)
	uint8 _targetFloor;

	UPROPERTY(VisibleAnywhere, Category = Elevator)
	FVector _targetLocation;

	UPROPERTY(VisibleAnywhere, Category = Elevator)
	bool _moving;

	UPROPERTY(VisibleAnywhere, Category = Elevator)
	bool _innerDoorsClosed;

	UPROPERTY(VisibleAnywhere, Category = Elevator)
	bool _outerDoorsClosed;	

	UPROPERTY(EditAnywhere, Category = Elevator)
	FString _movementAxis = "Yaw";			

	UPROPERTY(EditAnywhere, Category = Elevator)
	TArray<AElevatorCallButton*> _elevatorCallButtons;

	UPROPERTY(EditAnywhere, Category = Elevator)
	TArray<ASlidingDoor*> _innerElevatorDoors;

	UPROPERTY(EditAnywhere, Category = Elevator)
	UCameraComponent* _elevatorCamera;

	UPROPERTY(EditAnywhere, Category = Elevator)
	UBoxComponent* _boxCollision;
	
	UFUNCTION()
	void OnTriggerEntered(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnTriggerExit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void CalculateSpeed(float targetZ, float currentZ);

	UFUNCTION()
	void OpenInnerDoors();	

	UFUNCTION()
	void CloseInnerDoors();

	UFUNCTION()
	void DoorClosedMessage();

	UFUNCTION()
	void CheckIfDoorsClosed();
};
