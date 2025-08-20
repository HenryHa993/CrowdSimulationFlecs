// Fill out your copyright notice in the Description page of Project Settings.


#include "ISMUnit.h"


// Sets default values
AISMUnit::AISMUnit()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AISMUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AISMUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ISMController)
	{
		ISMController->SetTransform(Index, GetActorTransform());
	}
}

// Called to bind functionality to input
void AISMUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

