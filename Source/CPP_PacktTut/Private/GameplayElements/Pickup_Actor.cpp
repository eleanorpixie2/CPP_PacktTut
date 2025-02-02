// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayElements/Pickup_Actor.h"
#include "Components/SphereComponent.h"
#include "Player_Character.h"

// Sets default values
APickup_Actor::APickup_Actor()
{
	//create the collection box component and set it to the root
	RootComponent = CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionBox"));
	if (CollectionSphere)
	{
		//ignore everyother channel
		CollectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
		//only register pawns that overlap this actor and nothing else
		CollectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		//Bind the overlap delegate to the function
		CollectionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickup_Actor::OnSphereOverlap);
	}
}

void APickup_Actor::OnCollection_Implementation(APlayer_Character * Collector)
{
	//Destroy this actor
	Destroy();
}

void APickup_Actor::OnSphereOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//check if the pawn that entered is the player pawn
	APlayer_Character* Player = Cast<APlayer_Character>(OtherActor);
	if (Player)
	{
		//pass in the player reference to OnCollected
		OnCollection(Player);
	}
}



