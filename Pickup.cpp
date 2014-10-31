

#include "FirstGame3D.h"
#include "Pickup.h"


APickup::APickup(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	/*
	The pickup is valid when it is created
	*/
	bIsActive = true;

	/*
	Create the root SphereComponent to handle the pickup's collision
	*/
	BaseCollisionComponent = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("BaseSphereComponent"));

	/*
	Set SphereComponent as root component
	*/
	RootComponent = BaseCollisionComponent;

	/*
	Create the static mesh component
	*/
	PickupMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("PickupMesh"));

	/*
	Turn on Physics for static mesh
	*/
	PickupMesh->SetSimulatePhysics(true);

	/*
	Attach the StaticMeshComponent to root component
	*/
	PickupMesh->AttachTo(RootComponent);
}

void APickup::OnPickedUp_Implementation()
{
	/*
	There is no default behavior for a pickup when its is picked up.
	*/
}


