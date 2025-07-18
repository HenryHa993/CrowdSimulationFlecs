// Fill out your copyright notice in the Description page of Project Settings.


#include "TSCrowdSimulationComponents.h"

void UTSCrowdSimulationComponents::Initialize(flecs::world& ECSWorld)
{
	ECSWorld.component<TS_MeshUpdateContext>();
	ECSWorld.component<TS_AddInstance>();
	ECSWorld.component<TS_Mesh>();
}
