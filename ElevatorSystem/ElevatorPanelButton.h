#pragma once

#include "GameFramework/Actor.h"
#include "Engine.h"
#include "ElevatorPanelButton.generated.h"

//class UTextRenderComponent;

UCLASS()
class CYBERPUNKPROJECT_API AElevatorPanelButton : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AElevatorPanelButton();	

	void SetButtonSelected();
	void SetButtonDeselected();

private:	

	UPROPERTY(EditAnywhere, Category = PanelButton)
	uint8 _floorNumber;

	UPROPERTY(EditAnywhere, Category = PanelButton)
	UStaticMeshComponent* _panelButton;
	
	UPROPERTY(VisibleAnywhere, Category = ElevatorPanelButton)
	UTextRenderComponent* _floorText;
};
