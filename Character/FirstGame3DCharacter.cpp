// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "FirstGame3D.h"
#include "FirstGame3DCharacter.h"
#include "BatteryPickup.h"
#include "FirstGame3DGameMode.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AFirstGame3DCharacter

AFirstGame3DCharacter::AFirstGame3DCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	/* Set a character's base power level */
	PowerLevel = 2000.f;

	/* Set PowerLevel's dependecy speed */
	SpeedFactor = 0.75f;
	BaseSpeed = 10.0f;

	/* Create collection volume of the battery */
	CollectionSphere = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("CollectionSpehere"));
	CollectionSphere->AttachTo(RootComponent);
	CollectionSphere->SetSphereRadius(200.f);


	// Set size for collision capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	CharacterMovement->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	CharacterMovement->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	CharacterMovement->JumpZVelocity = 600.f;
	CharacterMovement->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUseControllerViewRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUseControllerViewRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFirstGame3DCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("CollectPickups", IE_Pressed, this, &AFirstGame3DCharacter::CollectBatteries);

	InputComponent->BindAxis("MoveForward", this, &AFirstGame3DCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFirstGame3DCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AFirstGame3DCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AFirstGame3DCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFirstGame3DCharacter::TouchStarted);
}


void AFirstGame3DCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AFirstGame3DCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFirstGame3DCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFirstGame3DCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFirstGame3DCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AFirstGame3DCharacter::CollectBatteries()
{
	float BatteryPower = 0.f;

	/* Get game mode */
	AFirstGame3DGameMode* MyGameMode = Cast<AFirstGame3DGameMode>(UGameplayStatics::GetGameMode(this));

	/* if we are playing */
	if (MyGameMode->GetCurrentState() == EFirstGame3DPlayState::EPlaying)
	{
		/* Get all overllapping Actors and store them in a CollectedActorsArray */
		TArray<AActor*> CollectedActors;
		CollectionSphere->GetOverlappingActors(CollectedActors);

		/* For each Actor collected */
		for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected)
		{
			/* Cast the collected actor to ABatteryPickup */
			ABatteryPickup* const TestBattery = Cast<ABatteryPickup>(CollectedActors[iCollected]);

			/* if the cast is successful, and the battery is valid and active */
			if (TestBattery && !TestBattery->IsPendingKill() && TestBattery->bIsActive)
			{
				/* Store its battery power for adding to the characters power */
				BatteryPower = BatteryPower + TestBattery->PowerLevel;

				/* Call battery's OnPickedUp function */
				TestBattery->OnPickedUp();

				/* Deactivate battery */
				TestBattery->bIsActive = false;
			}
		}

		if (BatteryPower > 0.f)
		{
			/* Call blueprinted implementation of PowerUp with the total battery power as input  */
			PowerUp(BatteryPower);
		}
	}
}

void AFirstGame3DCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CharacterMovement->MaxWalkSpeed = SpeedFactor * PowerLevel + BaseSpeed;
}