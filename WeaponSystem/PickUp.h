// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PickUp.generated.h"

UCLASS()
class CYBERPUNKPROJECT_API APickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUp();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	
	
protected:

	UPROPERTY(EditAnywhere, Category = PickUp)
	UStaticMeshComponent* _mesh;

	UPROPERTY(EditAnywhere, Category = PickUp)
	UBoxComponent* _boxCollision;	
		
	virtual void PickUpItem(AActor* Player);
	
private:
		
	UFUNCTION()
	void OnTriggerEnter(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};
