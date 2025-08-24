// Fill out your copyright notice in the Description page of Project Settings.


#include "ST_Systems.h"

#include "ST_Components.h"
#include "TurboSequence_Manager_Lf.h"
#include "CrowdSimulation/ECS/Core/Core_Components.h"
#include "CrowdSimulation/ECS/ISM/ISM_Components.h"
#include "CrowdSimulation/ECS/TS/TS_Components.h"
#include "CrowdSimulation/Pawns/ISMUnit.h"
#include "CrowdSimulation/Pawns/TSUnit.h"

void UST_Systems::Initialize(flecs::world& ECSWorld)
{
	ECSWorld.system<const WorldRef, const UnitConfigRef, const ST_AddInstance>("System Add ST Actor Instance")
	.term_at(0).singleton()
	.term_at(1).singleton()
	.each([](flecs::iter& it, size_t index, const WorldRef& cWorld, const UnitConfigRef& cUnitConfig, const ST_AddInstance& cAdd)
	{
		APawn* unit = cWorld.Value->SpawnActor<APawn>(cUnitConfig.Value->STActors[cAdd.Index], cAdd.Transform);

		// Initialise based off index
		// todo: Worth tidying
		switch (cAdd.Index)
		{
		default:
			break;
		case 1:
			{
				AISMController* controller = it.world().get<ISM_ControllerRef>()->Value;
				if(controller)
				{
					AISMUnit* ismUnit = Cast<AISMUnit>(unit);
					int32 instance = controller->AddInstance();
												
					ismUnit->ISMController = controller;
					ismUnit->Index = instance;
												
					controller->CreateOrExpandTransformArray();
				}

				break;
			}
		case 2:
			{
				ATSUnit* tsUnit = Cast<ATSUnit>(unit);
				tsUnit->MeshID = ATurboSequence_Manager_Lf::AddSkinnedMeshInstance_GameThread(cUnitConfig.Value->SpawnData, cAdd.Transform, cWorld.Value);
				tsUnit->AnimPlaySettings = FTurboSequence_AnimPlaySettings_Lf();

				if(tsUnit->MeshID.IsMeshDataValid())
				{
					ATurboSequence_Manager_Lf::AddInstanceToUpdateGroup_Concurrent(0, tsUnit->MeshID);
					// Entity helps to manage despawning the mesh to o avoid race-time conditions.
					// Will unfortunately affect memory usage, but will not affect performance
					it.world().entity()
					.set<TS_Mesh>({tsUnit->MeshID});
				}
			}
		}
			
		it.world().entity()
			.set<ST_ActorRef>({static_cast<AActor*>(unit)});

		it.entity(index).destruct();
	});

	ECSWorld.system<ST_ActorRef>("System Despawn ST Actor")
	.kind<OnDespawn>()
	.each([](flecs::iter& it, size_t index, ST_ActorRef& cActor)
	{
		cActor.Value->Destroy();
		it.entity(index).destruct();
	});
}
