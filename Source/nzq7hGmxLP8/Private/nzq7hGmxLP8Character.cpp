// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "nzq7hGmxLP8.h"
#include "nzq7hGmxLP8Character.h"

// dcr9 - added start -------------------------------------------------------
#include "BatteryPickup.h"
// dcr9 - added end ---------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// Anzq7hGmxLP8Character

Anzq7hGmxLP8Character::Anzq7hGmxLP8Character(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	// dcr9 - added start -------------------------------------------------------
	powerLevel(2000.0),
	baseSpeed(100.0),	// default is 10.0 but aggrovating to test
	speedModifyAmount(2.0)	// default == 0.74
	// dcr9 - added end ---------------------------------------------------------
{
	// dcr9 - added start -------------------------------------------------------
	batteryPowerCollectorSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("batteryPowerCollectorSphere"));
	batteryPowerCollectorSphere->AttachTo(RootComponent);
	batteryPowerCollectorSphere->SetSphereRadius(200.0);
	// dcr9 - added end ---------------------------------------------------------

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void Anzq7hGmxLP8Character::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);

	// dcr9 - added start -------------------------------------------------------
	InputComponent->BindAction("collectPickup", IE_Pressed, this, &Anzq7hGmxLP8Character::collectBatteries);
	// dcr9 - added end ---------------------------------------------------------

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &Anzq7hGmxLP8Character::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &Anzq7hGmxLP8Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &Anzq7hGmxLP8Character::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &Anzq7hGmxLP8Character::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &Anzq7hGmxLP8Character::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &Anzq7hGmxLP8Character::TouchStopped);
}


void Anzq7hGmxLP8Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void Anzq7hGmxLP8Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void Anzq7hGmxLP8Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void Anzq7hGmxLP8Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void Anzq7hGmxLP8Character::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void Anzq7hGmxLP8Character::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

// dcr9 - added start -------------------------------------------------------
void Anzq7hGmxLP8Character::collectBatteries()
{
	float collectedBatteryPower = 0.0;

	TArray<AActor*> batteriesFound;
	batteryPowerCollectorSphere->GetOverlappingActors(batteriesFound);

	for (int32 batteryCollectedIndex = 0; batteryCollectedIndex < batteriesFound.Num(); batteryCollectedIndex++)
	{
		ABatteryPickup* const currentBattery = Cast<ABatteryPickup, AActor>(batteriesFound[batteryCollectedIndex]);
		const bool isBeingDestroyed = false;

		if (currentBattery != nullptr &&
			currentBattery->IsPendingKill() == isBeingDestroyed &&
			currentBattery->isAbleToBePickedUp)
		{
			collectedBatteryPower = collectedBatteryPower + currentBattery->powerLevel;
			currentBattery->pickUp();
			currentBattery->isAbleToBePickedUp = false;
		}
	}

	if (collectedBatteryPower > 0)
	{
		powerUp(collectedBatteryPower);
	}
}

void Anzq7hGmxLP8Character::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);
	//CharacterMovement->MaxWalkSpeed = speedModifyAmount * powerLevel + baseSpeed;	// old api, put me back if stuff stops working
	GetCharacterMovement()->MaxWalkSpeed = speedModifyAmount * powerLevel + baseSpeed;	// does this just return a copy of character movement?
}
// dcr9 - added end ---------------------------------------------------------
