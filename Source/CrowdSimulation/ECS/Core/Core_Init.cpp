// Fill out your copyright notice in the Description page of Project Settings.


#include "Core_Init.h"

#include "Core_Components.h"

void UCore_Init::Initialize(flecs::world& ECSWorld)
{
	// Singleton world reference
	WorldRef worldRef {GetWorld()};
	ECSWorld.set<WorldRef>(worldRef);
	
	// Singleton unit configuration
	UnitConfigRef unitConfigRef {UnitConfig};
	ECSWorld.set<UnitConfigRef>(unitConfigRef);

	flecs::entity unitPrefab = ECSWorld.entity("Unit Prefab")
	.set<Velocity>({ FVector{0,0,0} })
	.set<WanderStateData>({2.0f})
	.set<WaitStateData>({2.0f})
	.add(FSM_State::Wander)
	.add(FSM_Status::Enter);
}
