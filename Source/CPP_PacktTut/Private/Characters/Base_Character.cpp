// Fill out your copyright notice in the Description page of Project Settings.

#include "Base_Character.h"


// Sets default values
ABase_Character::ABase_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 10.0f;
	CurrentHealth = MaxHealth;
	bIsSprinting = false;

}

// Called when the game starts or when spawned
void ABase_Character::BeginPlay()
{
	Super::BeginPlay();

	//If the current health doesn't equal max when game is started 
	//or restarted, reset current health to max health
	if (CurrentHealth != MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
	
}


