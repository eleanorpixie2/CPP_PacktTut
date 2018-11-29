// Fill out your copyright notice in the Description page of Project Settings.

#include "Gamemodes/GM_Gameplay.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

void AGM_Gameplay::RespawnPlayer(AController* NewPlayer)
{
	//make sure new player is valid
	if (NewPlayer)
	{
		//destroy the current pawn, then creates a new one at a random spawn point
		NewPlayer->GetPawn()->Destroy();
		RestartPlayer(NewPlayer);
	}
}

void AGM_Gameplay::GameOver(int enemies)
{
	if (enemies==0)
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





