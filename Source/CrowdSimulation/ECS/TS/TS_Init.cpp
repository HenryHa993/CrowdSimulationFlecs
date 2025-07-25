// Fill out your copyright notice in the Description page of Project Settings.


#include "TS_Init.h"

#include "TS_Components.h"
#include "TurboSequence_MinimalData_Lf.h"
#include "CrowdSimulation/ECS/Core/Core_Components.h"

void UTS_Init::Initialize(flecs::world& ECSWorld)
{
	// Temporary mass spawning system -- TS
	{
		TS_Animations animationsMap {TMap<FSM_State, UAnimSequence*>()};
		animationsMap.Value.Add(FSM_State::Wait, UnitConfig->IdleAnim);
		animationsMap.Value.Add(FSM_State::Wander, UnitConfig->WalkingAnim);
		/*animationsMap.Value[FSM_State::Wait] = UnitConfig->IdleAnim;
		animationsMap.Value[FSM_State::Wander] = UnitConfig->WalkingAnim;*/
		
		// Spawn data needs to be injected
		FTurboSequence_UpdateContext_Lf updateContext = FTurboSequence_UpdateContext_Lf();
		updateContext.GroupIndex = 0;

		ECSWorld.entity("Mesh Solver")
			.set<TS_MeshUpdateContext>({updateContext})
			.set<WorldRef>({GetWorld()});
		
		flecs::entity unitPrefab = ECSWorld.entity("Unit")
		.set<Velocity>({ FVector{0,0,0} })
		.set<WanderStateData>({2.0f})
		.set<WaitStateData>({2.0f})
		.add(FSM_State::Wander)
		.add(FSM_Status::Enter);

		int x = 5;
		int y = 5;
		int gridX = 100;
		int gridY = 100;

		for(double i = 0; i < x * gridX; i += gridX)
		{
			for(double j = 0; j < y * gridY; j += gridY)
			{
				FTransform unitTransform{FVector{i, j, 0}};
				ECSWorld.entity()
					.set<TS_AddInstance>({GetWorld(), UnitConfig->SpawnData, updateContext, unitPrefab, unitTransform});
			}
		}

		// Singleton animator
		ECSWorld.set<TS_Animations>({animationsMap});
	}
}
