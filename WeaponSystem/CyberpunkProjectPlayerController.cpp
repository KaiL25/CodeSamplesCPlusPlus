// Fill out your copyright notice in the Description page of Project Settings.

#include "CyberpunkProject.h"
#include "CyberpunkProjectPlayerController.h"
#include "UI.h"

void ACyberpunkProjectPlayerController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);	
}

void ACyberpunkProjectPlayerController::CreateUIWidget()
{
	if (_uiBp)
	{
		_ui = CreateWidget<UUI>(this, _uiBp);
		_ui->AddToViewport();
	}
}

void ACyberpunkProjectPlayerController::UpdateCurrentAmmoInFirearm(uint8 NewAmmo)
{
	_ui->SetCurrentAmmoInFirearm(NewAmmo);
}

void ACyberpunkProjectPlayerController::UpdateCurrentAmmoCarried(uint8 NewAmmo)
{
	_ui->SetCurrentAmmoCarried(NewAmmo);
}

