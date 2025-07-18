// Fill out your copyright notice in the Description page of Project Settings.


#include "TSCrowdSimulationSystems.h"

#include "TSCrowdSimulationComponents.h"
#include "TurboSequence_Manager_Lf.h"
#include "CrowdSimulation/ECS/Core/Core_Components.h"

void UTSCrowdSimulationSystems::Initialize(flecs::world& ECSWorld)
{
	ECSWorld.system<TS_AddInstance>("System Add TS Instance")
	.each([](flecs::iter& it, size_t index, TS_AddInstance& cAdd)
	{
	// Add Instance to update group
	FTurboSequence_MinimalMeshData_Lf meshID = ATurboSequence_Manager_Lf::AddSkinnedMeshInstance_GameThread(cAdd.SpawnData, cAdd.Transform, cAdd.World);
		
	if(meshID.IsMeshDataValid())
	{
		// Add instance to update group
		ATurboSequence_Manager_Lf::AddInstanceToUpdateGroup_Concurrent(cAdd.MeshUpdateContext.GroupIndex, meshID);

		// Play default animation -- or maybe not?
			
		// Create entity with prefab
		it.world().entity()
			.is_a(cAdd.Prefab)
			.set<TS_Mesh>({meshID})
			.set<Transform>({cAdd.Transform});
	}
		
	// Destroy entity
	it.entity(index).destruct();
});

	ECSWorld.system<TS_MeshUpdateContext, WorldRef>("System Solve TS Instances")
	.each([](flecs::iter& it, size_t index, TS_MeshUpdateContext& cUpdateContext, WorldRef& cWorld)
	{
		ATurboSequence_Manager_Lf::SolveMeshes_GameThread(it.delta_time(), cWorld.Value, cUpdateContext.Value);
	});
}
