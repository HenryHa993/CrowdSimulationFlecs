// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SpawnerBase.h"


// Sets default values
ASpawnerBase::ASpawnerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpawnerBase::BeginPlay()
{
	Super::BeginPlay();

	if(SpawnOnBeginPlay)
	{
		Spawn(InitRows, InitColumns, InitGridSize);
	}
}

// Called every frame
void ASpawnerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/* Spawn units based on grid size and amount*/
void ASpawnerBase::Spawn(int Columns, int Rows, float GridSize)
{
}

void ASpawnerBase::Despawn()
{
}

