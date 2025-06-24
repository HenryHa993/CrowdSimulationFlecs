// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/SM/BT_SMSpawner.h"


// Sets default values
ABT_SMSpawner::ABT_SMSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABT_SMSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABT_SMSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABT_SMSpawner::Spawn(int Rows, int Columns, float GridSize)
{
	for(int i = 0; i < Rows; i++)
	{
		for(int j = 0; j < Columns; j++)
		{
			GetWorld()->SpawnActor(Unit, new FVector(i * GridSize, j * GridSize, 0));
		}
	}
}

void ABT_SMSpawner::Despawn()
{
	
}

