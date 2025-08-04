// Fill out your copyright notice in the Description page of Project Settings.


#include "TS_Init.h"

#include "TS_Components.h"
#include "TurboSequence_MinimalData_Lf.h"
#include "CrowdSimulation/ECS/Core/Core_Components.h"

void UTS_Init::Initialize(flecs::world& ECSWorld)
{
	// Spawn parameters
	UnitConfigRef unitConfigRef {UnitConfig};

	// Singleton Unit Config
	ECSWorld.set<UnitConfigRef>(unitConfigRef);
	
	// Animations map
	TS_Animations animationsMap {TMap<FSM_State, UAnimSequence*>()};
	animationsMap.Value.Add(FSM_State::Wait, UnitConfig->IdleAnim);
	animationsMap.Value.Add(FSM_State::Wander, UnitConfig->WalkingAnim);
	
	// Singleton animator
	ECSWorld.set<TS_Animations>({animationsMap});

	// Spawn data needs to be injected
	FTurboSequence_UpdateContext_Lf updateContext = FTurboSequence_UpdateContext_Lf();
	updateContext.GroupIndex = 0;

	ECSWorld.entity("Mesh Solver")
		.set<TS_MeshUpdateContext>({updateContext})
		.set<WorldRef>({GetWorld()});

	// Either store TS spawn parameters globally or within the prefab itself
	flecs::entity unitPrefab = ECSWorld.entity("TS Prefab")
	.set<Velocity>({ FVector{0,0,0} })
	.set<WanderStateData>({2.0f})
	.set<WaitStateData>({2.0f})
	.add(FSM_State::Wander)
	.add(FSM_Status::Enter);
	
	// Temporary mass spawning system -- TS
	/*{
		/*animationsMap.Value[FSM_State::Wait] = UnitConfig->IdleAnim;
		animationsMap.Value[FSM_State::Wander] = UnitConfig->WalkingAnim;#1#

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
					.set<TS_AddInstance>({GetWorld(), unitPrefab, unitTransform});
					// Trying to reduce it to this
					// .set<TS_AddInstance>({GetWorld(), unitPrefab, unitTransform});
					// Because I can access the unit prefab, generate a unit transform and getworld

					// But the issue is that I need to get the spawn data from somewhere.
			}
		}
	}*/

}
