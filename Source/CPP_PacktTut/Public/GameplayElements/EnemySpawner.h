// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayElements/Destructible_Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class CPP_PACKTTUT_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
		TSubclassOf<ADestructible_Actor> EnemyBP;

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
		float DelaySpawn = 0;

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
		int RangeY = 20;

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
		int RangeX = 200;

	UFUNCTION()
		void SpawnEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int Placed = 0;

	FVector position;

	bool add = false;

	FRotator rotate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
