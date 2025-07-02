// Fill out your copyright notice in the Description page of Project Settings.


#include "ISMCrowdSimulationComponents.h"

void UISMCrowdSimulationComponents::Initialize(flecs::world& ECSWorld)
{
	/*
	ECSWorld.component<test>();
*/
	ECSWorld.component<Transform>();
	
	ECSWorld.component<ISM_Map>();
	ECSWorld.component<ISM_ControllerRef>();
	ECSWorld.component<ISM_Hash>();
	ECSWorld.component<ISM_Index>();
	ECSWorld.component<ISM_AddInstance>();
}
