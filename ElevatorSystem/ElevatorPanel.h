#include "Interactable.h"
#include "ElevatorPanel.generated.h"

class AElevatorPanelButton;
class AElevator;
class ACyberpunkProjectPlayerController;
class ACyberpunkProjectCharacter;


/**
 * 
 */
UCLASS()
class CYBERPUNKPROJECT_API AElevatorPanel : public AInteractable
{
	GENERATED_BODY()

public:

	AElevatorPanel();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Interacting() override;

	void SelectNewButtonVertical(float input);
	void SelectNewButtonHorizontal(float input);

	UFUNCTION()
	void PanelInput();

	UFUNCTION()
	void SetCurrentFloor(uint8 newCurrentFloor);
	
private:

	//Used to calculate what button to select when pressing a or d
	UPROPERTY(EditAnywhere, Category = ElevatorPanel)
	uint8 _columnSize;

	//Used to toggle between player and panel view.
	bool _viewOnPanel;

	//Reference to the player controller for camera switching.
	ACyberpunkProjectPlayerController* _playerControllerReference;

	//Reference to the player controller for camera switching.
	ACyberpunkProjectCharacter* _playerReference;

	//When the focus is on the panel the first button in the array will automatically be set to the current button
	uint8 _currentButton;

	uint8 _currentFloor;

	UPROPERTY(EditAnywhere, Category = ElevatorPanel)
	UStaticMeshComponent* _panel;

	UPROPERTY(EditAnywhere, Category = ElevatorPanel)
	TArray<AElevatorPanelButton*> _panelButtons;

	UPROPERTY(EditAnywhere, Category = ElevatorPanel)
	AElevator* _elevatorReference;

	UFUNCTION()
	void LookAtPanel();

	UFUNCTION()
	void LookAwayFromPanel();	
};
