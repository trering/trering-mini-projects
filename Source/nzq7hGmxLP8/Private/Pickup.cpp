// Fill out your copyright notice in the Description page of Project Settings.

#include "nzq7hGmxLP8.h"
#include "Pickup.h"

APickup::APickup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	isAbleToBePickedUp(true)
{
	baseCollisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("BaseSphereComponent"));

	RootComponent = baseCollisionComponent;

	mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("PickupMesh"));

	mesh->SetSimulatePhysics(true);
	mesh->AttachTo(RootComponent);
}

void APickup::pickUp_Implementation()
{

}