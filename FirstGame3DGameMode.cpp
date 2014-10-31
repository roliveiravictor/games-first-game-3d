// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "FirstGame3D.h"
#include "FirstGame3DGameMode.h"
#include "FirstGame3DCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "FirstGame3DHUD.h"

AFirstGame3DGameMode::AFirstGame3DGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FObjectFinder<UClass> PlayerPawnBPClass(TEXT("Class'/Game/Blueprints/MyCharacter.MyCharacter_C'"));
	if (PlayerPawnBPClass.Object != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Object;

	}

	/* set HUD default class */
	HUDClass = AFirstGame3DHUD::StaticClass();

	/* Base decay rate */
	DecayRate = 0.25f;
}

void AFirstGame3DGameMode::Tick(float DeltaSeconds)
{
	AFirstGame3DCharacter* MyCharacter = Cast<AFirstGame3DCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	
	/* if character still has power */
	if (MyCharacter->PowerLevel > 0.05)
	{
		/*decrease character's power */
		MyCharacter->PowerLevel = FMath::FInterpTo(MyCharacter->PowerLevel, 0.f, DeltaSeconds, DecayRate);
	}
	else
	{
		SetCurrentState(EFirstGame3DPlayState::EGameOver);
	}
}

void AFirstGame3DGameMode::SetCurrentState(EFirstGame3DPlayState NewState)
{
	CurrentState = NewState;

	HandleNewState(NewState);
}

void AFirstGame3DGameMode::HandleNewState(EFirstGame3DPlayState NewState)
{
	switch (NewState)
	{
	
		/* When we are playing spawnvolumes can spawn */
	case EFirstGame3DPlayState::EPlaying:
		for (ASpawnVolume* Volume : SpawnVolumeActors)
		{
			Volume->EnableSpawning();
		}
		break;
		
		/* deactivate spawn volume if game over */
	case EFirstGame3DPlayState::EGameOver:
	{
		for (ASpawnVolume* Volume : SpawnVolumeActors)
		{
			Volume->DisableSpawning();
		}
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		PlayerController->SetCinematicMode(true, true, true);
	}
		break;

	case EFirstGame3DPlayState::EUnknown:
	default:

		/* do nothing */
		break;

	}
}

void AFirstGame3DGameMode::BeginPlay()
{
	Super::BeginPlay();

	/* find all spawn volume actors */
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);

		if (SpawnVolumeActor)
		{
			SpawnVolumeActors.Add(SpawnVolumeActor);
		}
	}

	SetCurrentState(EFirstGame3DPlayState::EPlaying);
}