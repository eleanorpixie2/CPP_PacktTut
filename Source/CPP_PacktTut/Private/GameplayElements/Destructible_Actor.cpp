// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayElements/Destructible_Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/DamageType.h"
#include "EngineMinimal.h"

// Sets default values
ADestructible_Actor::ADestructible_Actor()
{
	RootComponent = Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (Mesh)
	{
		Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	}

	MaxHealth = 25.0f;
	CurrentHealth = MaxHealth;

	DestructionRadius = 400.0f;
	DestructionDamage = 25.0f;

}

// Called when the game starts or when spawned
void ADestructible_Actor::BeginPlay()
{
	Super::BeginPlay();
	if (CurrentHealth != MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
}

void ADestructible_Actor::OnDeath_Implementation()
{
	//sphere trace for pawns and world dynamics
	{
		TArray<FOverlapResult>Overlaps;

		//save start position
		const FVector StartPos = Mesh->GetComponentLocation();

		//save shape to use for trace
		FCollisionShape Sphere = FCollisionShape::MakeSphere(DestructionRadius);

		//check for overlapping
		if (GetWorld()->OverlapMultiByChannel(Overlaps, StartPos, FQuat::Identity, ECC_WorldDynamic, Sphere))
		{
			for (int i = 0; i < Overlaps.Num(); i++)
			{
				//make sure the damage type is valid
				TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());

				//setup damage event
				FDamageEvent DamageEvent(ValidDamageTypeClass);

				//apply damage
				Overlaps[i].GetActor()->TakeDamage(DestructionDamage, DamageEvent, nullptr, this);
			}
		}
	}

	//Destroy this actor
	Destroy();
}

float ADestructible_Actor::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHealth <= 0.0f)
	{
		return 0.0f;
	}

	const float SubtractedHealth = CurrentHealth - DamageAmount;

	//check if dead
	if (FMath::IsNearlyZero(SubtractedHealth) || SubtractedHealth < 0.0f)
	{
		CurrentHealth = 0.0f;
		OnDeath();
	}
	else
	{
		CurrentHealth = SubtractedHealth;
	}

	return CurrentHealth;

}


