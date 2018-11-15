// Fill out your copyright notice in the Description page of Project Settings.

#include "Player_Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/DamageType.h"
#include "UI/GenericHUD.h"


APlayer_Character::APlayer_Character()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	if (SpringArm)
	{
	    SpringArm->SetupAttachment(GetCapsuleComponent());
		SpringArm->RelativeLocation = FVector(-20.0f, 0.0f, 40.0f);
		SpringArm->TargetArmLength = 0.0f;
		SpringArm->bDoCollisionTest = false;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;

	}

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if (Camera)
	{
		Camera->FieldOfView = 90.0f;
		Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	}

	FP_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Mesh"));
	if (FP_Mesh)
	{
		FP_Mesh->SetupAttachment(Camera);
	}

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	if (GunMesh)
	{
		GunMesh->SetupAttachment(Camera);
	}

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	bCanShoot = true;
	bUnlimitedAmmo = false;
	MaxAmmo = 10;
	CurrentAmmo = MaxAmmo;

	BaseTurnRate = 100.0f;
	BaseLookUpRate = 100.0f;
	CameraPitchMin = -89.0f;
	CameraPitchMax = 89.0f;

	SprintSpeed = 1500.0f;

	DamageAmount = 10.0f;

	InteractionDistance = 250;

	FireAnimation = nullptr;

	TrailEffect = nullptr;
	HitEffect = nullptr;

	HudReference = nullptr;

}

void APlayer_Character::PauseGame()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		//check if hud is not valid
		if (!HudReference)
		{
			HudReference = Cast<AGenericHUD>(PC->GetHUD());
		}

		//checks if hud is valid
		if (HudReference)
		{
			HudReference->ShowSpecificMenu(HudReference->GetPauseMenuClas(), false, true);
		}

		//Pause the game
		PC->SetPause(true);
	}
}

void APlayer_Character::UnPauseGame()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{

		//Unpause the game
		PC->SetPause(false);

		//check if hud is not valid
		if (!HudReference)
		{
			HudReference = Cast<AGenericHUD>(PC->GetHUD());
		}

		//checks if hud is valid
		if (HudReference)
		{
			HudReference->ShowSpecificMenu(HudReference->GetGameplayHUDClass(), true, false);
		}

	}
}

void APlayer_Character::BeginPlay()
{
	Super::BeginPlay();

	//This will attach the gun to the hand's mash
	GunMesh->AttachToComponent(FP_Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripPoint"));

	//If the current ammo doesn't equal the max ammo at start of game
	//reset the current ammo to the max amount
	if (CurrentAmmo != MaxAmmo)
	{
		CurrentAmmo = MaxAmmo;
	}
}

void APlayer_Character::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//assertion check, causes the program to crash if not true
	check(PlayerInputComponent);

	//Interaction input
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayer_Character::Interact);

	// Shoot input
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayer_Character::OnFire);

	// Pause input
	PlayerInputComponent->BindAction("PauseGame", IE_Pressed, this, &APlayer_Character::PauseGame);

	//Movement input
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayer_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayer_Character::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayer_Character::OnSprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayer_Character::OnSprintEnd);

	//Camera Input
	PlayerInputComponent->BindAxis("Turn", this, &APlayer_Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayer_Character::LookUpAtRate);

}

void APlayer_Character::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	//Get and set HUD reference
	if (APlayerController* PC = Cast<APlayerController>(NewController))
	{
		//Try and get a reference to the generic hud
		HudReference = Cast<AGenericHUD>(PC->GetHUD());
		if (HudReference)
		{
			//If Hud is not null then show the gameplay hud
			HudReference->ShowSpecificMenu(HudReference->GetGameplayHUDClass(), true, false);
		}
	}

}

void APlayer_Character::OnDeath_Implementation()
{

	//dont allow shooting
	bCanShoot = false;

	//stop all movement so the player cannot move at all
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetJumpAllowed(false);

	//Detach the gun mesh
	GunMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

	//hide the player's hands and gun
	GunMesh->SetHiddenInGame(true);
	FP_Mesh->SetHiddenInGame(true);

	//Check if there is not a valid hud
	if (!HudReference)
	{
		//get and set hud refernce through player controller
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			HudReference = Cast<AGenericHUD>(PC->GetHUD());
		}

	}
	//checks if there is a valid hud
	if (HudReference)
	{
		//If Hud is not null then show the gameplay hud
		HudReference->ShowSpecificMenu(HudReference->GetDeadMenuClass(), false, true);
	}

}

float APlayer_Character::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//Don't continue if health is already below or at zero
	if (CurrentHealth <= 0.0f)
	{
		return 0.0f;
	}

	//Does the math for damaging the player
	const float SubtractedHealth = CurrentHealth - DamageAmount;

	if (FMath::IsNearlyZero(SubtractedHealth) || SubtractedHealth < 0.0f)
	{
		//if it is then just lock it to zero and then call our OnDeath function
		CurrentHealth = 0.0f;
		OnDeath();
	}
	else
	{
		//Otherwise set the current health to the subtracted amount
		CurrentHealth = SubtractedHealth;
	}
	//return the damaged amount of health
	return CurrentHealth;
}

void APlayer_Character::OnFire_Implementation()
{
	//first check if we're allowed to shoot
	if (bCanShoot)
	{
		//then check if we dont have ammo and we dont have unlimited ammo on
		if (!HasAmmo() && !bUnlimitedAmmo)
		{
			return;
		}
	}
	//If we're not allowed to shoot then don't continue
	else
	{
		return;
	}

	//Handles the actual firing and damaging of enemy actor along with spawning the correct particle effects
	{
		FVector ParticleLocation = FVector::ZeroVector;
		{
			//Prepare our invisible ray's values
			FHitResult Hit;
			const FVector StartTrace = Camera->GetComponentLocation();
			const FVector EndTrace = StartTrace + (Camera->GetForwardVector()*10000.0f);

			//Collision objects to ignore
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(GetOwner());
			QueryParams.AddIgnoredActor(this);

			//Fire an invisible ray
			GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace,
				ECollisionChannel::ECC_Visibility, QueryParams);

			//If we hit something then damage it and set our particle location
			if (Hit.bBlockingHit && Hit.GetActor())
			{
				//Set the location for the particles to end at
				ParticleLocation = Hit.ImpactPoint;
				// Create the object for a general object type
				TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(
					UDamageType::StaticClass());

				// Set out general damage type to a specific type: Point Damage
				FPointDamageEvent PointDamageEvent(DamageAmount, Hit,
					Hit.ImpactNormal, ValidDamageTypeClass);

				// Damage Actor
				Hit.GetActor()->TakeDamage(DamageAmount, PointDamageEvent, GetController(), GetOwner());
			}
			else
			{
				// Set the location for the particles to end at
				ParticleLocation = EndTrace;
			}
		}

		SpawnShootingParticles(ParticleLocation);
	}

	//Play the fire animation
	if (FireAnimation)
	{
		//Check if our skeletal mesh has a valid animation instance
		if (UAnimInstance* AnimInstance = FP_Mesh->GetAnimInstance())
		{
			//Tell the animation to play the fire animation
			AnimInstance->Montage_Play(FireAnimation);
		}
	}

	//If we don't have unlimited ammo on, then use one bullet
	if (!bUnlimitedAmmo)
	{
		//Subtract current ammo by one and then set it to that value
		CurrentAmmo--;
	}
}

void APlayer_Character::OnSprintStart_Implementation()
{
	//Stop us from being able to shoot and turn on the flag for sprinting
	bIsSprinting = true;
	bCanShoot = false;
	//save what our walk speed was initially
	PreviousWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	//Update our current walk speed to the sprint speed
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void APlayer_Character::OnSprintEnd_Implementation()
{
	//stop sprinting
	bIsSprinting = false;
	//Revert our walk speed from sprint to regular walking speed
	GetCharacterMovement()->MaxWalkSpeed = PreviousWalkSpeed;
	//Allow us to shoot again
	bCanShoot = true;

}

void APlayer_Character::MoveForward(float Scalar)
{
	if (Scalar != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Scalar);
	}
}

void APlayer_Character::MoveRight(float Scalar)
{
	if (Scalar != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Scalar);
	}
}

void APlayer_Character::LookUpAtRate(float Rate)
{
	//only continue if spring arm is not null
	if (SpringArm)
	{
		//Get current rotation
		FRotator CameraRelativeRot = SpringArm->RelativeRotation;
		//Get the intended pitch
		const float CameraNewPitch = FMath::ClampAngle(CameraRelativeRot.Pitch + Rate * BaseLookUpRate*GetWorld()->DeltaTimeSeconds, CameraPitchMin,CameraPitchMax);

		//set the new rotation
		CameraRelativeRot.Pitch = CameraNewPitch;

		//Confirm/Update the spring arm rotation
		SpringArm->SetRelativeRotation(CameraRelativeRot);
	}
}

void APlayer_Character::TurnAtRate(float Rate)
{
	//Adds to the Yaw rotation of the player at the specified speed
	AddControllerYawInput(Rate*BaseTurnRate*GetWorld()->DeltaTimeSeconds);
}

void APlayer_Character::SpawnShootingParticles(FVector ParticleLocation)
{
	if (TrailEffect)
	{
		//Spawn the particle
		UParticleSystemComponent* SpawnedParticle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), TrailEffect,GunMesh->GetSocketLocation(FName("Muzzle")));

		//Scale the particle up so its easily visible
		SpawnedParticle->SetWorldScale3D(FVector(5.0f));

		//Set the end of the particle beam
		SpawnedParticle->SetVectorParameter(FName("ShockBeamEnd"), ParticleLocation);
	}

	if (HitEffect)
	{
		//spawn the particle
		UParticleSystemComponent* SpawnedParticle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), HitEffect, ParticleLocation, FRotator::ZeroRotator, true);

		//Scale the particle up so its easily visible
		SpawnedParticle->SetWorldScale3D(FVector(0.25f));
	}
}

void APlayer_Character::Interact()
{
	//Prepare our invisible ray's values
	FHitResult Hit;
	const FVector StartTrace = Camera->GetComponentLocation();
	const FVector EndTrace = StartTrace + (Camera->GetForwardVector()*InteractionDistance);

	//Collision to ignore
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.AddIgnoredActor(this);

	//Fire an invisible ray
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECollisionChannel::ECC_Visibility, QueryParams);

	//Check if we hit anything
	if (Hit.bBlockingHit)
	{
		AActor* HitActor = Hit.GetActor();
		//check if it has the interface implemented
		if (HitActor->GetClass()->ImplementsInterface(UInteraction_Interface::StaticClass()))
		{
			//cast for c++ interface
			if (IInteraction_Interface* Interface = Cast<IInteraction_Interface>(HitActor))
			{
				//call C++ layer
				Interface->Execute_OnInteract(HitActor, this);
			}
			else
			{
				//Call BP Layer
				IInteraction_Interface::Execute_OnInteract(HitActor, this);
			}
		}
	}
}

