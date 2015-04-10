// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "nzq7hGmxLP8.h"
#include "nzq7hGmxLP8GameMode.h"
#include "nzq7hGmxLP8Character.h"

Anzq7hGmxLP8GameMode::Anzq7hGmxLP8GameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/MyCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
