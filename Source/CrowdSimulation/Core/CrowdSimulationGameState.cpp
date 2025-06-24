// Fill out your copyright notice in the Description page of Project Settings.


#include "CrowdSimulationGameState.h"

void ACrowdSimulationGameState::BeginPlay()
{
	Super::BeginPlay();

	for(auto spawner : SpawnerArray)
	{
		GetWorld()->SpawnActor(spawner);
	}
}
