// Fill out your copyright notice in the Description page of Project Settings.


#include "ISMCrowdSimulationSystems.h"

#include "ISMCrowdSimulationComponents.h"

void UISMCrowdSimulationSystems::Initialize(flecs::world& ECSWorld)
{
	// This needs a bit of work and adaptation
	ECSWorld.system<ISM_AddInstance, ISM_Map>("System Add Instance")
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
