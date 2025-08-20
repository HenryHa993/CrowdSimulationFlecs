// Fill out your copyright notice in the Description page of Project Settings.


#include "TSUnit.h"

#include "TurboSequence_Manager_Lf.h"


// Sets default values
ATSUnit::ATSUnit()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATSUnit::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATSUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(MeshID.IsMeshDataValid())
	{
		ATurboSequence_Manager_Lf::SetMeshWorldSpaceLocationRotationScale_Concurrent(MeshID, GetActorLocation(), GetActorRotation().Quaternion(), GetActorScale3D());
	}
}

// Called to bind functionality to input
void ATSUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

