// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/ISM/BT_ISMPawn.h"


// Sets default values
ABT_ISMPawn::ABT_ISMPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovement");
}

// Called when the game starts or when spawned
void ABT_ISMPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABT_ISMPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABT_ISMPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

