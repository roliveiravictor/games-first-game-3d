// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "SpawnVolume.h"
#include "FirstGame3DGameMode.generated.h"

/* Enum to store the current state of gameplay */
enum class EFirstGame3DPlayState : short
{
	EPlaying,
	EGameOver,
	EUnknown
};

UCLASS(minimalapi)
class AFirstGame3DGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()

	virtual void Tick(float DeltaSeconds) OVERRIDE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Power)
	float DecayRate;

	EFirstGame3DPlayState GetCurrentState() const;

	void SetCurrentState(EFirstGame3DPlayState NewState);

	virtual void BeginPlay() OVERRIDE;

private:

	TArray<ASpawnVolume*> SpawnVolumeActors;

	EFirstGame3DPlayState CurrentState;

	void HandleNewState(EFirstGame3DPlayState NewState);
};

FORCEINLINE EFirstGame3DPlayState AFirstGame3DGameMode::GetCurrentState() const
{
	return CurrentState;
}



