// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/SM/SMUnitPawn.h"


// Sets default values
ASMUnitPawn::ASMUnitPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	// Might need to edit SM to keep performance comparisons fair
	StaticMeshComponent->SetEnableGravity(false);
	StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMeshComponent->CanCharacterStepUpOn = ECB_No;

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovement");
	// Some preset values for testing
	FloatingPawnMovement->Deceleration = 0.f;

	RootComponent = StaticMeshComponent;
}

// Called when the game starts or when spawned
void ASMUnitPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASMUnitPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASMUnitPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

