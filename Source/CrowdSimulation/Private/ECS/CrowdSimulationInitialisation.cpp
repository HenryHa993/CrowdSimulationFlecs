// Fill out your copyright notice in the Description page of Project Settings.


#include "CrowdSimulation/Public/ECS/CrowdSimulationInitialisation.h"

#include "CrowdSimulation/Public/ECS/CrowdSimulationComponents.h"

void UCrowdSimulationInitialisation::Initialize(flecs::world& ECSWorld)
{
	ECSWorld.entity().set<test>({1});
}
