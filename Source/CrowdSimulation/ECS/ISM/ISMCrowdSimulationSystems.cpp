// Fill out your copyright notice in the Description page of Project Settings.


#include "ISMCrowdSimulationSystems.h"

#include "ISMCrowdSimulationComponents.h"
#include "TurboSequence_Manager_Lf.h"

void UISMCrowdSimulationSystems::Initialize(flecs::world& ECSWorld)
{
	// This needs a bit of work and adaptation
	ECSWorld.system<ISM_AddInstance, ISM_Map>("System Add ISM Instance")
	.term_at(1).singleton()
	.each([](flecs::iter& it, size_t index, ISM_AddInstance& cAdd, ISM_Map& cMap)
	{
		AISMController* controller = *(cMap.ISMs.Find(cAdd.Hash));
		if(controller != nullptr)
		{
			int32 ismIndex = controller->AddInstance();
			
			it.world().entity()
				.is_a(cAdd.Prefab)
				.set<ISM_ControllerRef>({ controller })
				.set<ISM_Index>( { ismIndex })
				.set<ISM_Hash>({ cAdd.Hash })
				.set<Transform>({ cAdd.Transform });
			
			controller->CreateOrExpandTransformArray();
		}
		it.entity(index).destruct();
	});

	/*
	 * TURBOSEQUENCE SPECIFIC COMPONENTS
	 */
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

	// Apply velocity to transform positions
	ECSWorld.system<Transform, const Velocity>("System Velocity")
	.each([](flecs::iter& it, size_t index, Transform& cTransform, const Velocity& cVelocity)
	{
		cTransform.Value.SetLocation(cTransform.Value.GetLocation() + cVelocity.Value * it.delta_time());
	});

	// Copy entity transform to ISM instance
	ECSWorld.system<const Transform, const ISM_Index, const ISM_ControllerRef>("System Copy Instance Transforms")
	.each([](const Transform& cTransform, const ISM_Index& cISMIndex, const ISM_ControllerRef& cISMControllerRef)
	{
		int index = cISMIndex.Value;
		cISMControllerRef.Value->SetTransform(index, cTransform.Value);
	});

	// Update transforms
	ECSWorld.system<const ISM_Map>("System Batch Update Transforms")
	.term_at(1)
	.singleton()
	.each([](const ISM_Map& cMap)
	{
		for(auto& mapping : cMap.ISMs)
		{
			mapping.Value->BatchUpdateTransforms();
		}
	});

	// Finite State Machine
	/*ECSWorld.system<Velocity>("System Wander")
	.with<FSM_State>(flecs::Wildcard)
	.with(FSM_State::Wander)
	.each([](Velocity& cVelocity)
	{
		cVelocity.Value = FVector(FMath::RandRange(-100, 100), FMath::RandRange(-100, 100), 0);
	});*/

	// Entering a wander
	ECSWorld.system<Velocity>("System Enter Wander State")
	.with<FSM_State>(flecs::Wildcard)
	.with(FSM_State::Wander)
	.with<FSM_Status>(flecs::Wildcard)
	.with(FSM_Status::Enter)
	.each([](flecs::iter& it, size_t index, Velocity& cVelocity)
	{
		cVelocity.Value = FVector(FMath::RandRange(-100, 100), FMath::RandRange(-100, 100), 0);
		it.entity(index).add(FSM_Status::Running);
	});

	ECSWorld.system<WanderStateData>("System Running Wander State")
	.with<FSM_State>(flecs::Wildcard)
	.with(FSM_State::Wander)
	.with<FSM_Status>(flecs::Wildcard)
	.with(FSM_Status::Running)
	.each([](flecs::iter& it, size_t index, WanderStateData& cWanderStateData)
	{
		cWanderStateData.CurrentWanderDuration -= it.delta_time();
		if(cWanderStateData.CurrentWanderDuration < 0)
		{
			cWanderStateData.CurrentWanderDuration = cWanderStateData.WanderDuration;
			it.entity(index).add(FSM_State::Wait);
			it.entity(index).add(FSM_Status::Enter);
		}
	});

	ECSWorld.system<Velocity>("System Enter Wait State")
	.with<FSM_State>(flecs::Wildcard)
	.with(FSM_State::Wait)
	.with<FSM_Status>(flecs::Wildcard)
	.with(FSM_Status::Enter)
	.each([](flecs::iter& it, size_t index, Velocity& cVelocity)
	{
		cVelocity.Value = FVector::ZeroVector;
		it.entity(index).add(FSM_Status::Running);
	});

	ECSWorld.system<WaitStateData>("System Running Wait State")
	.with<FSM_State>(flecs::Wildcard)
	.with(FSM_State::Wait)
	.with<FSM_Status>(flecs::Wildcard)
	.with(FSM_Status::Running)
	.each([](flecs::iter& it, size_t index, WaitStateData& cWaitStateData)
	{
		cWaitStateData.CurrentWaitDuration -= it.delta_time();
		if(cWaitStateData.CurrentWaitDuration < 0)
		{
			cWaitStateData.CurrentWaitDuration = cWaitStateData.WaitDuration;
			it.entity(index).add(FSM_State::Wander);
			it.entity(index).add(FSM_Status::Enter);
		}
	});
}
