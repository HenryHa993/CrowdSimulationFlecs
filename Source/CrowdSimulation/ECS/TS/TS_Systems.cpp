// Fill out your copyright notice in the Description page of Project Settings.


#include "TS_Systems.h"

#include "TS_Components.h"
#include "TurboSequence_Manager_Lf.h"
#include "CrowdSimulation/ECS/Core/Core_Components.h"

void UTS_Systems::Initialize(flecs::world& ECSWorld)
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

	// Animation component
	// todo: Should I have it play the animation whilst running or at the beginning
	ECSWorld.system<TS_Mesh, TS_Animation, const TS_Animations>("System Set Animation")
	.with<FSM_State>(flecs::Wildcard)
	.each([](flecs::iter& it, size_t index, TS_Mesh& cMesh, TS_Animation& cAnimation, const TS_Animations& cAnimations)
	{
		// Check if animation is already playing
		FSM_State state = it.entity(index).to_constant<FSM_State>();
		if(ATurboSequence_Manager_Lf::GetHighestPriorityPlayingAnimation_Concurrent(cMesh.Value) == cAnimations.Value[state])
		{
			return;
		}

		// If not, play animation
		ATurboSequence_Manager_Lf::PlayAnimation_Concurrent(cMesh.Value, cAnimations.Value[state], cAnimation.AnimationPlaySettings);
	});

	// Copy transforms
	ECSWorld.system<TS_Mesh, const Transform>("System Copy TS Instance Transform")
	.each([](TS_Mesh& cMesh, const Transform& cTransform)
	{
		//ATurboSequence_Manager_Lf::SetMeshWorldSpaceTransform_Concurrent(cMesh.Value, cTransform.Value);
		ATurboSequence_Manager_Lf::SetMeshWorldSpaceLocationRotationScale_Concurrent(cMesh.Value, cTransform.Value.GetLocation(), cTransform.Value.GetRotation(), cTransform.Value.GetScale3D());
	});

	ECSWorld.system<TS_MeshUpdateContext, WorldRef>("System Solve TS Instances")
	.each([](flecs::iter& it, size_t index, TS_MeshUpdateContext& cUpdateContext, WorldRef& cWorld)
	{
		ATurboSequence_Manager_Lf::SolveMeshes_GameThread(it.delta_time(), cWorld.Value, cUpdateContext.Value);
	});
}
