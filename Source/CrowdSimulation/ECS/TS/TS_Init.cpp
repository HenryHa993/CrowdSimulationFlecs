// Fill out your copyright notice in the Description page of Project Settings.


#include "TS_Init.h"

#include "TS_Components.h"
#include "TurboSequence_MinimalData_Lf.h"
#include "CrowdSimulation/ECS/Core/Core_Components.h"

void UTS_Init::Initialize(flecs::world& ECSWorld)
{
	// Access unit config from singleton pointer
	UUnitConfig* UnitConfig = ECSWorld.get<UnitConfigRef>()->Value;
		
	// Set up animations map using unit config
	TS_Animations animationsMap {TMap<FSM_State, UAnimSequence*>()};
	animationsMap.Value.Add(FSM_State::Wait, UnitConfig->IdleAnim);
	animationsMap.Value.Add(FSM_State::Wander, UnitConfig->WalkingAnim);
	ECSWorld.set<TS_Animations>({animationsMap});

	// Set up mesh solver entity
	// This could potentially be a singleton also, but I want to leave room for multiple solvers
	FTurboSequence_UpdateContext_Lf updateContext = FTurboSequence_UpdateContext_Lf();
	updateContext.GroupIndex = 0;
	// Might be worth checking if solving 0 meshes has an impact on performance
	ECSWorld.entity("Mesh Solver")
		.set<TS_MeshUpdateContext>({updateContext});
}
