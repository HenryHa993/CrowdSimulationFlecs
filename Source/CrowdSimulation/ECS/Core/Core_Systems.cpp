// Fill out your copyright notice in the Description page of Project Settings.


#include "Core_Systems.h"

#include "Core_Components.h"

void UCore_Systems::Initialize(flecs::world& ECSWorld)
{
	/*
	 * Basic Systems
	 */
	ECSWorld.system<Transform, const Velocity>("System Velocity")
	.each([](flecs::iter& it, size_t index, Transform& cTransform, const Velocity& cVelocity)
	{
		cTransform.Value.SetLocation(cTransform.Value.GetLocation() + cVelocity.Value * it.delta_time());
	});

	/*
	 * State Machine Systems
	 */
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
