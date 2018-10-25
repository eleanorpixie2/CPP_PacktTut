// Fill out your copyright notice in the Description page of Project Settings.

#include "Gamemodes/GM_MainMenu.h"
#include "Blueprint/UserWidget.h"

void AGM_MainMenu::BeginPlay()
{
	Super::BeginPlay();

	//Check if the class is valid to spawn
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		//set input mode to UI only
		FInputModeUIOnly InputMode;
		//allows the mouse to leave the viewport
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;

		//create and add our widget to the viewport
		UUserWidget* CreatedWidget = CreateWidget<UUserWidget>(PC, MainMenuClass);
		CreatedWidget->AddToPlayerScreen();
	}
}


