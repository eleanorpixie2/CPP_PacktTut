// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayElements/Pickup_Actor.h"
#include "PickUpSpawner.generated.h"

UCLASS()
class CPP_PACKTTUT_API APickUpSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpSpawner();

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
		TSubclassOf<APickup_Actor> BulletBP;

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
		float DelaySpawn = 5;

	UFUNCTION()
		void SpawnBullet();

	UFUNCTION(BlueprintCallable, Category="ActorSpawning")
		void Respawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool Spawned=false;

	TArray<AActor*> NumberOfBullet;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
