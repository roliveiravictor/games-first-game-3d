

#pragma once

#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

/**
 * 
 */
UCLASS()
class ASpawnVolume : public AActor
{
	GENERATED_UCLASS_BODY()

	/* BoxComponent to specify the spawning area within the level */
	UPROPERTY(VisibleInstanceOnly, Category = Spawning)
	TSubobjectPtr<UBoxComponent> WhereToSpawn;

	/* The pickup to spawn */
	UPROPERTY(EditAnywhere, Category = Spawning)
	TSubclassOf<class APickup> WhatToSpawn;

	/* Minimum spawn delay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	float SpawnDelayRangeLow;

	/* Maximum spawn delay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	float SpawnDelayRangeHigh;

	/* Finds a random point within the BoxComponent */
	UFUNCTION(BlueprintPure, Category = Spawning)
	FVector GetRandomPointInVolume();

	virtual void Tick(float DeltaSeconds) OVERRIDE; 

	void EnableSpawning();

	void DisableSpawning();

private:

	/* wether or not spawning is enabled */
	bool bSpawningEnabled;

	/* Calculates random spawn delay */
	float GetRandomSpawnDelay();

	/* The current spawn delay */
	float SpawnDelay;

	/* Handles the spawning of a new pickup */
	void SpawnPickup();

	/* Timer to spawn pickup */
	float SpawnTime;
};
