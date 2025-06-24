// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/ISM/BT_ISMController.h"

#include "Components/InstancedStaticMeshComponent.h"


// Sets default values
ABT_ISMController::ABT_ISMController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>("InstancedStaticMeshComponent");
	SetRootComponent(InstancedStaticMeshComponent);
}

// Called when the game starts or when spawned
void ABT_ISMController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABT_ISMController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

