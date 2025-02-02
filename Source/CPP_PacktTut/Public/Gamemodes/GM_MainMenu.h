// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class CPP_PACKTTUT_API AGM_MainMenu : public AGameModeBase
{
	GENERATED_BODY()
	

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Main Menu")
		class TSubclassOf<class UUserWidget> MainMenuClass;
		
};
