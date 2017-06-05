// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "UI.generated.h"

/**
 * 
 */
UCLASS()
class CYBERPUNKPROJECT_API UUI : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetCurrentAmmoInFirearm(uint8 NewAmmo);
	void SetCurrentAmmoCarried(uint8 NewAmmo);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	uint8 _currentAmmoInFirearm = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	uint8 _currentAmmoCarried = 0;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCurrentAmmoInFirearm();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCurrentAmmoCarried();

private:

	
	
};
