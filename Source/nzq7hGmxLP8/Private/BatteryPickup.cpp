// Fill out your copyright notice in the Description page of Project Settings.

#include "nzq7hGmxLP8.h"
#include "BatteryPickup.h"

ABatteryPickup::ABatteryPickup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	powerLevel(150.0)
{
}

void ABatteryPickup::pickUp_Implementation()
{
	Super::pickUp_Implementation();
	Destroy();
}