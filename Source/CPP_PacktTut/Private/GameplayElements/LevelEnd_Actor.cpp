// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayElements/LevelEnd_Actor.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Player_Character.h"

// Sets default values
ALevelEnd_Actor::ALevelEnd_Actor()
{
	RootComponent = Root = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root"));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	if (CollisionBox)
	{
		CollisionBox->SetupAttachment(Root);
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelEnd_Actor::OnBoxOverlapBegin);
	}
	LevelToGoTo = "Map Name";
}

void ALevelEnd_Actor::OnBoxOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<APlayer_Character>(OtherActor))
	{
		//save string of map name
		FString MapName = LevelToGoTo.ToString();
		//Check if the map name is not valid
		if (!GEngine->MakeSureMapNameIsValid(MapName))
		{
			//if it is invalid that create an output message
			UE_LOG(LogTemp, Warning, TEXT("Warning:The map '%s' does not exist."), *MapName);
			//exit function
			return;
		}
		//if the map exists, open level
		UGameplayStatics::OpenLevel(this, LevelToGoTo);
	}
}


