// Fill out your copyright notice in the Description page of Project Settings.

#include "CyberpunkProject.h"
#include "PickUp.h"


// Sets default values
APickUp::APickUp()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	_mesh->SetupAttachment(RootComponent);
	_boxCollision->SetupAttachment(RootComponent);

	_boxCollision->bGenerateOverlapEvents = true;
	_boxCollision->OnComponentBeginOverlap.AddDynamic(this, &APickUp::OnTriggerEnter);
	
}

// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickUp::OnTriggerEnter(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		PickUpItem(OtherActor);	
	}
}

void APickUp::PickUpItem(AActor* Player = nullptr)
{

}