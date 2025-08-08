// Fill out your copyright notice in the Description page of Project Settings.


#include "SKM_Pawn.h"


// Sets default values
ASKM_Pawn::ASKM_Pawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal Mesh Component");

	/*FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("Floating Movement Component");
	FloatingPawnMovement->Deceleration = 0.f;*/

	RootComponent = SkeletalMeshComponent;
}

// Called when the game starts or when spawned
void ASKM_Pawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASKM_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASKM_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

