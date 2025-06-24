// Fill out your copyright notice in the Description page of Project Settings.


#include "CrowdSimulation/Public/ECS/CrowdSimulationSystems.h"

#include "CrowdSimulation/Public/ECS/CrowdSimulationComponents.h"

void UCrowdSimulationSystems::Initialize(flecs::world& ECSWorld)
{
	ECSWorld.system<test>("Test")
		.each([](test& t)
		{
			UE_LOG(LogTemp, Warning, TEXT("Entity with test component value"));
		});
}
