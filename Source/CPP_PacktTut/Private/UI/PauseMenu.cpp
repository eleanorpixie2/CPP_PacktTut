// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PauseMenu.h"
#include "Player_Character.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


bool UPauseMenu::Initialize()
{
	//check if we fail the parent version of super first
	if (!Super::Initialize())
	{
		return false;
	}

	//Bind the onresumeclicked function to the resume button delegate for onclicked
	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenu::OnResumeClicked);
	}

	//Bind the ondesktopclicked function to the desktop button delegate for onclicked
	if (DesktopButton)
	{
		DesktopButton->OnClicked.AddDynamic(this, &UPauseMenu::OnDesktopClicked);
	}

	//Bind the onmainmenuclicked function to the main menu button delegate for onclicked
	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenu::OnMainMenuClicked);
	}

	//finish the function
	return true;

}

void UPauseMenu::OnResumeClicked()
{
	//check for a valid player reference
	if (APlayer_Character* Player = Cast<APlayer_Character>(GetOwningPlayerPawn()))
	{
		//if it is valid then unpause the game, which handles removing the menu
		Player->UnPauseGame();
	}
}

void UPauseMenu::OnDesktopClicked()
{
	//Tell our owning player controller to quit the game
	GetOwningPlayer()->ConsoleCommand("quit");
}

void UPauseMenu::OnMainMenuClicked()
{
	//open the main menu level
	UGameplayStatics::OpenLevel(GetOwningPlayerPawn(), "Map_MainMenu");
}
