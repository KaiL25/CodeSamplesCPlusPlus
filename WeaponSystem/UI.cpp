// Fill out your copyright notice in the Description page of Project Settings.

#include "CyberpunkProject.h"
#include "UI.h"


void UUI::SetCurrentAmmoInFirearm(uint8 NewAmmo)
{
	_currentAmmoInFirearm = NewAmmo;
	UpdateCurrentAmmoInFirearm();
}

void UUI::SetCurrentAmmoCarried(uint8 NewAmmo)
{
	_currentAmmoCarried = NewAmmo;
	UpdateCurrentAmmoCarried();
}
