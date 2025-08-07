// Fill out your copyright notice in the Description page of Project Settings.


#include "TS_Init.h"

#include "TS_Components.h"
#include "TurboSequence_MinimalData_Lf.h"
#include "CrowdSimulation/ECS/Core/Core_Components.h"

void UTS_Init::Initialize(flecs::world& ECSWorld)
{
	WorldRef worldRef {GetWorld()};
	ECSWorld.set<WorldRef>(worldRef);
	
	// Spawn parameters
	UnitConfigRef unitConfigRef {UnitConfig};
	ECSWorld.set<UnitConfigRef>(unitConfigRef);
	
	// Animations map
	TS_Animations animationsMap {TMap<FSM_State, UAnimSequence*>()};
	animationsMap.Value.Add(FSM_State::Wait, UnitConfig->IdleAnim);
	animationsMap.Value.Add(FSM_State::Wander, UnitConfig->WalkingAnim);
	ECSWorld.set<TS_Animations>({animationsMap});

	// Spawn data needs to be injected
	FTurboSequence_UpdateContext_Lf updateContext = FTurboSequence_UpdateContext_Lf();
	updateContext.GroupIndex = 0;

	ECSWorld.entity("Mesh Solver")
		.set<TS_MeshUpdateContext>({updateContext});

	// Either store TS spawn parameters globally or within the prefab itself
	flecs::entity unitPrefab = ECSWorld.entity("TS Prefab")
	.set<Velocity>({ FVector{0,0,0} })
	.set<WanderStateData>({2.0f})
	.set<WaitStateData>({2.0f})
	.add(FSM_State::Wander)
	.add(FSM_Status::Enter);

}
