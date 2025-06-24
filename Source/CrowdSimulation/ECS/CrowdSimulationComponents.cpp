// Fill out your copyright notice in the Description page of Project Settings.


#include "CrowdSimulationComponents.h"

void UCrowdSimulationComponents::Initialize(flecs::world& ECSWorld)
{
	ECSWorld.component<test>();
}
