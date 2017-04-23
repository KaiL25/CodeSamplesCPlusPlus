#pragma once

#include "GameFramework/Actor.h"
#include "Engine.h"
#include "SlidingDoor.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAxisEnum : uint8
{
	VE_X 	UMETA(DisplayName = "X"),
	VE_Z 	UMETA(DisplayName = "Z")	
};

enum class EOpeningDirectionEnum : uint8
{
	VE_RIGHT 	UMETA(DisplayName = "RIGHT"),
	VE_LEFT 	UMETA(DisplayName = "LEFT"),
	VE_UP 		UMETA(DisplayName = "UP"),
	VE_DOWN 	UMETA(DisplayName = "DOWN")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorClosedDelegate);

UCLASS()
class CYBERPUNKPROJECT_API ASlidingDoor : public AActor
{
	GENERATED_BODY()
	
public:		

	UPROPERTY()
	FDoorClosedDelegate OnDoorClosed;

	// Sets default values for this actor's properties
	ASlidingDoor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
	void OpenDoor();

	UFUNCTION()
	void CloseDoor();

	UFUNCTION()
	bool GetDoorSliding();

	UFUNCTION()
	bool GetDoorOpen();

private:
	
	//To indicate whether the next command should be close or open.
	bool _open;
	//To indicate whether the door is currently opening, or closing.
	//bool _sliding;

	bool _opening;

	bool _closing;	

	EOpeningDirectionEnum _openingDirection;

	bool _countDownToClosing;
	
	float _closingCountdown;

	UPROPERTY(EditAnywhere, Category = Door)
	float _timeUntilDoorClosesAutomatically;

	UPROPERTY(EditAnywhere, Category = Door)
	UStaticMeshComponent* _doorMesh;

	UPROPERTY(VisibleAnywhere, Category = Door)
	FVector _initialLocation;	

	UPROPERTY(EditAnywhere, Category = Door)
	EAxisEnum _openingAxis;

	UPROPERTY(EditAnywhere, Category = Door)
	USceneComponent* _targetLocation;

	UPROPERTY(EditAnywhere, Category = Door)
	float _slideSpeed;	

	void DetermineOpeningClosingAxis();
};
