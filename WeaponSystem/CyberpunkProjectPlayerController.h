// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "CyberpunkProjectPlayerController.generated.h"

class UUI;

/**
 * 
 */
UCLASS()
class CYBERPUNKPROJECT_API ACyberpunkProjectPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:

	UUI* _ui;

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUI> _uiBp;

public:

	virtual void Possess(APawn* InPawn) override;
	void UpdateCurrentAmmoInFirearm(uint8 NewAmmo);
	void UpdateCurrentAmmoCarried(uint8 NewAmmo);

	UFUNCTION(BlueprintCallable, Category = UI)
	void CreateUIWidget();

	
	
};
