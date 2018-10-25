// Fill out your copyright notice in the Description page of Project Settings.

#include "Gamemodes/GM_Gameplay.h"

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


