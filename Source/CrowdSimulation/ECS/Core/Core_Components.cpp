// Fill out your copyright notice in the Description page of Project Settings.


#include "Core_Components.h"

void UCore_Components::Initialize(flecs::world& ECSWorld)
{
	ECSWorld.component<Transform>();
	ECSWorld.component<Velocity>();
	
	ECSWorld.component<FSM_State>();
	ECSWorld.component<FSM_Status>();
	
	ECSWorld.component<WanderStateData>();
	ECSWorld.component<WaitStateData>();

	ECSWorld.component<UnitConfigRef>();
}
