// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayElements/EnemySpawner.h"
#include "Engine/Classes/Engine/World.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemySpawner::SpawnEnemy()
{
	if (EnemyBP)
	{
		FActorSpawnParameters Spawn;
		if (add)
		{
		position = GetActorLocation()+(FVector(rand()%RangeX,rand()%RangeY,0));
		add = false;
		}
		else
		{
			position = GetActorLocation() - (FVector(rand() % RangeX, rand() % RangeY, 0));
			add = true;
		}
		position.Z = 10;
		rotate = GetActorRotation();
		ADestructible_Actor* ref = GetWorld()->SpawnActor<ADestructible_Actor>(EnemyBP, position, rotate, Spawn);
	}
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < 10; i++)
	{
		FTimerHandle OutHandle;
		GetWorld()->GetTimerManager().SetTimer(OutHandle, this, &AEnemySpawner::SpawnEnemy, DelaySpawn);
	}
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

