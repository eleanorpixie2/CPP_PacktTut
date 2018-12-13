 // Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayElements/PickUpSpawner.h"
#include "Engine/Classes/Engine/World.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
APickUpSpawner::APickUpSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APickUpSpawner::SpawnBullet()
{
	if (BulletBP)
	{
		FActorSpawnParameters Spawn;

		APickup_Actor* ref = GetWorld()->SpawnActor<APickup_Actor>(BulletBP, GetTransform(),Spawn);
	}
}

void APickUpSpawner::Respawn()
{
	FTimerHandle OutHandle;

	GetWorld()->GetTimerManager().SetTimer(OutHandle, this, &APickUpSpawner::SpawnBullet, DelaySpawn);

}

// Called when the game starts or when spawned
void APickUpSpawner::BeginPlay()
{
	Super::BeginPlay();

	APickUpSpawner::Respawn();

	/*FTimerHandle OutHandle;

	GetWorld()->GetTimerManager().SetTimer(OutHandle, this, &APickUpSpawner::SpawnBullet, DelaySpawn);*/
	
}

// Called every frame
void APickUpSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),BulletBP,NumberOfBullet);
	if (NumberOfBullet.Num()==0 && !Spawned)
	{
	    Spawned = true;
		APickUpSpawner::Respawn();
	}
	else if (NumberOfBullet.Num() != 0 && Spawned)
	{
		Spawned = false;
	}
}

