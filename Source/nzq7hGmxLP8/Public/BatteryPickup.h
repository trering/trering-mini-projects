// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 * 
 */
UCLASS()
class NZQ7HGMXLP8_API ABatteryPickup : public APickup
{
	GENERATED_UCLASS_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float powerLevel;
	
	void pickUp_Implementation() override;
};