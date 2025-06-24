// Fill out your copyright notice in the Description page of Project Settings.


#include "CrowdSimulationInitialisation.h"

#include "CrowdSimulationComponents.h"

void UCrowdSimulationInitialisation::Initialize(flecs::world& ECSWorld)
{
	ECSWorld.entity().set<test>({1});
}
