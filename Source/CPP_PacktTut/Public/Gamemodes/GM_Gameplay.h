// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_Gameplay.generated.h"

/**
 * 
 */
UCLASS()
class CPP_PACKTTUT_API AGM_Gameplay : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Game")
		void RespawnPlayer(AController* NewPlayer);

	UFUNCTION(BlueprintCallable, Category = "Game")
		void GameOver(int enemies);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|GameOver")
		FName LevelToGoTo;

	
};
