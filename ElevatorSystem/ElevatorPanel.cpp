#include "CyberpunkProject.h"
#include "CyberpunkProjectPlayerController.h"
#include "CyberpunkProjectCharacter.h"
#include "ElevatorPanel.h"
#include "Elevator.h"
#include "ElevatorPanelButton.h"
#include "Engine.h"

AElevatorPanel::AElevatorPanel()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	_panel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Elevator Panel"));		
	_panel->SetupAttachment(RootComponent);
}

//---------------------------------------------------------------------------------------------------------------//

void AElevatorPanel::BeginPlay()
{
	_playerControllerReference = Cast<ACyberpunkProjectPlayerController>(GetWorld()->GetFirstPlayerController());
	_playerReference = Cast<ACyberpunkProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));		
}

//---------------------------------------------------------------------------------------------------------------//

void AElevatorPanel::Interacting()
{
	if (!_viewOnPanel)
	{			
		if (_playerControllerReference != NULL && _playerReference != NULL)
		{
			LookAtPanel();
		}

		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Player or player controller reference was NULL"));
		}
	}

	else
	{		
		if (_playerControllerReference != NULL && _playerReference != NULL)
		{
			LookAwayFromPanel();
		}

		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Player or player controller reference was NULL"));
		}		
	}
}

//---------------------------------------------------------------------------------------------------------------//

void AElevatorPanel::PanelInput()
{
	if (_viewOnPanel)
	{
		_elevatorReference->CallElevator(_currentButton);
		_panelButtons[_currentButton]->SetButtonDeselected();
		LookAwayFromPanel();
	}
}

//---------------------------------------------------------------------------------------------------------------//

void AElevatorPanel::SelectNewButtonVertical(float input)
{
	//If going up (w)
	if (input == 1)
	{
		if (_currentButton > 0)
		{
			_panelButtons[_currentButton]->SetButtonDeselected();
			_currentButton--;
			_panelButtons[_currentButton]->SetButtonSelected();
		}
	}

	//If going down (s)
	else if (input == -1)
	{
		if (_currentButton < _panelButtons.Num() - 1)
		{
			_panelButtons[_currentButton]->SetButtonDeselected();
			_currentButton++;
			_panelButtons[_currentButton]->SetButtonSelected();
		}
	}		
}

//---------------------------------------------------------------------------------------------------------------//

void AElevatorPanel::SelectNewButtonHorizontal(float input)
{
	if (input == 1)
	{
		if ((_currentButton + _columnSize) < _panelButtons.Num() - 1)
		{
			_panelButtons[_currentButton]->SetButtonDeselected();
			_currentButton += _columnSize;
			_panelButtons[_currentButton]->SetButtonSelected();
		}
	}

	else if (input == -1)
	{
		if ((_currentButton - _columnSize) >= 0)
		{
			_panelButtons[_currentButton]->SetButtonDeselected();
			_currentButton -= _columnSize;
			_panelButtons[_currentButton]->SetButtonSelected();
		}
	}	
}

//---------------------------------------------------------------------------------------------------------------//

void AElevatorPanel::LookAtPanel()
{
	_viewOnPanel = true;
	_playerReference->SetCameraFocusChanged(true);
	_playerControllerReference->SetViewTarget(this);
	_currentButton = 0;
	_panelButtons[0]->SetButtonSelected();
}

//---------------------------------------------------------------------------------------------------------------//

void AElevatorPanel::LookAwayFromPanel()
{
	_viewOnPanel = false;
	_playerReference->SetCameraFocusChanged(false);
	_playerControllerReference->SetViewTarget(_elevatorReference);
}

//---------------------------------------------------------------------------------------------------------------//

void AElevatorPanel::SetCurrentFloor(uint8 newCurrentFloor)
{
	_currentFloor = newCurrentFloor;
}

//---------------------------------------------------------------------------------------------------------------//
