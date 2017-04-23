#include "CyberpunkProject.h"
#include "ElevatorPanelButton.h"


// Sets default values
AElevatorPanelButton::AElevatorPanelButton()
{
	_panelButton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Panel Button"));
	_floorText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Floor Number"));
	_floorText->SetText("" + _floorNumber);
}

//---------------------------------------------------------------------------------------------------------------//

void AElevatorPanelButton::SetButtonSelected()
{	
	_floorText->SetTextRenderColor(FColor::Red);
	
}

//---------------------------------------------------------------------------------------------------------------//

void AElevatorPanelButton::SetButtonDeselected()
{
	_floorText->SetTextRenderColor(FColor::Black);
}

//---------------------------------------------------------------------------------------------------------------//