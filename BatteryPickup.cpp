

#include "FirstGame3D.h"
#include "BatteryPickup.h"


ABatteryPickup::ABatteryPickup(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	/*
	The base power level of the battery
	*/
	PowerLevel = 250.f;
}

void ABatteryPickup::OnPickedUp_Implementation()
{
	/*
	Call the parent implementation of OnPickedUp first
	*/
	Super::OnPickedUp_Implementation();

	/*
	Destroy the battery
	*/
	Destroy();
}


