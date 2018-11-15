// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class CPP_PACKTTUT_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
	
	
protected:

	//occurs on compile from blueprint editor
	virtual bool Initialize() override;

private:

	//resume game button
	UPROPERTY(meta = (BindWidget))
		class UButton* ResumeButton;

	//when the button is clicked
	UFUNCTION()
		void OnResumeClicked();

	//quit to desktop button
	UPROPERTY(meta = (BindWidget))
		class UButton* DesktopButton;

	//when the button is clicked
	UFUNCTION()
		void OnDesktopClicked();

	//quit to mainmenu button
	UPROPERTY(meta = (BindWidget))
		class UButton* MainMenuButton;

	//when the button is clicked
	UFUNCTION()
		void OnMainMenuClicked();
	
};
