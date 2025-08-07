// Fill out your copyright notice in the Description page of Project Settings.


#include "TS_Systems.h"

#include "TS_Components.h"
#include "TurboSequence_Manager_Lf.h"
#include "CrowdSimulation/ECS/Core/Core_Components.h"

void UTS_Systems::Initialize(flecs::world& ECSWorld)
{
	// todo: testing
	ECSWorld.system<WorldRef, UnitConfigRef, TS_AddInstance>("System Add TS Instance")
	.kind(flecs::OnLoad)
	.term_at(0).singleton()
	.term_at(1).singleton()
	.each([](flecs::iter& it, size_t index, const WorldRef& cWorld, const UnitConfigRef& cUnitConfig, const TS_AddInstance& cAdd)
	{
		// Add Instance to update group
		FTurboSequence_MinimalMeshData_Lf meshID = ATurboSequence_Manager_Lf::AddSkinnedMeshInstance_GameThread(cUnitConfig.Value->SpawnData, cAdd.Transform, cWorld.Value);
				
		if(meshID.IsMeshDataValid())
		{
			// Add instance to update group
			// todo: Update mesh context
			ATurboSequence_Manager_Lf::AddInstanceToUpdateGroup_Concurrent(0, meshID);

			// Play default animation -- or maybe not?
					
			// Create entity with prefab
			it.world().entity()
				.is_a(cAdd.Prefab)
				.set<TS_Mesh>({meshID})
				.set<TS_Animation>({FTurboSequence_AnimPlaySettings_Lf()})
				.set<Transform>({cAdd.Transform});
		}
				
		// Destroy entity
		it.entity(index).destruct();
	});

	// Animation component
	// todo: Should I have it play the animation whilst running or at the beginning
	ECSWorld.system<TS_Animations, TS_Mesh, TS_Animation>("System Set Animation")
	.kind(flecs::OnUpdate)
	.term_at(0).singleton()
	.with<FSM_State>(flecs::Wildcard)
	.with<FSM_Status>(flecs::Wildcard)
	.with(FSM_Status::Enter)
	.each([](flecs::iter& it, size_t index, TS_Animations& cAnimations,const TS_Mesh& cMesh, const TS_Animation& cAnimation)
	{
		if(cMesh.Value.IsMeshDataValid())
		{
			const FSM_State* state = it.entity(index).get<FSM_State>();
			ATurboSequence_Manager_Lf::PlayAnimation_Concurrent(cMesh.Value, cAnimations.Value[*state], cAnimation.AnimationPlaySettings);
		}
	});

	// Copy transforms
	ECSWorld.system<TS_Mesh, const Transform>("System Copy TS Instance Transform")
	.kind(flecs::PreStore)
	.each([](const TS_Mesh& cMesh, const Transform& cTransform)
	{
		//ATurboSequence_Manager_Lf::SetMeshWorldSpaceTransform_Concurrent(cMesh.Value, cTransform.Value);
		if(cMesh.Value.IsMeshDataValid())
		{
			ATurboSequence_Manager_Lf::SetMeshWorldSpaceLocationRotationScale_Concurrent(cMesh.Value, cTransform.Value.GetLocation(), cTransform.Value.GetRotation(), cTransform.Value.GetScale3D());
		}
	});

	// Solve mesh instances
	ECSWorld.system<WorldRef, TS_MeshUpdateContext>("System Solve TS Instances")
	.kind(flecs::OnStore)
	.term_at(0).singleton()
	.each([](flecs::iter& it, size_t index, const WorldRef& cWorld, const TS_MeshUpdateContext& cUpdateContext)
	{
		ATurboSequence_Manager_Lf::SolveMeshes_GameThread(it.delta_time(), cWorld.Value, cUpdateContext.Value);
	});

	// Pipeline to despawn meshes
	ECSWorld.system<TS_Mesh>("System Initiate TS Instance Removal")
	.kind<OnDespawn>()
	.each([](flecs::iter& it, size_t index, const TS_Mesh& cMesh)
	{
		ATurboSequence_Manager_Lf::RemoveInstanceFromUpdateGroup_Concurrent(0, cMesh.Value);
		it.entity(index).add<TS_RemoveInstance>();
	});

	ECSWorld.system<WorldRef, TS_Mesh>("System Remove TS Instance")
	.kind(flecs::OnStore)
	.term_at(0).singleton()
	.with<TS_RemoveInstance>()
	.each([](flecs::iter& it, size_t index, const WorldRef& cWorld, const TS_Mesh& cMesh)
	{
		ATurboSequence_Manager_Lf::RemoveSkinnedMeshInstance_GameThread(cMesh.Value, cWorld.Value);
		it.entity(index).destruct();
	});

}
