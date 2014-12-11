

#pragma once

#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 * 
 */
UCLASS()
class ABatteryPickup : public APickup
{
	GENERATED_UCLASS_BODY()

	/*
	Set the amount of power the battery gives to the player
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Power)
	float PowerLevel;

	/*
	Override the OnPickedUp function (use implementation because this is a BlueprintNativeEvent)
	*/
	void OnPickedUp_Implementation() OVERRIDE;

};
