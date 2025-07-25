// Fill out your copyright notice in the Description page of Project Settings.


#include "Core_Systems.h"

#include "Core_Components.h"

void UCore_Systems::Initialize(flecs::world& ECSWorld)
{
	/*
	 * Basic Systems
	 */
	// Velocity system
	// Runs on-update, should be before transform copies
	ECSWorld.system<Transform, const Velocity>("System Velocity")
	.kind(flecs::OnUpdate)
	.each([](flecs::iter& it, size_t index, Transform& cTransform, const Velocity& cVelocity)
	{
		cTransform.Value.SetLocation(cTransform.Value.GetLocation() + cVelocity.Value * it.delta_time());

		if(cVelocity.Value.Length() > 0)
		{
			// Todo: Do I want velocity to determine the direction the mesh is facing? If I get round to blendspaces, this is a good consideration
			FQuat correctionQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(-90.0f));
			cTransform.Value.SetRotation(FQuat::FastLerp(cTransform.Value.GetRotation(), cVelocity.Value.ToOrientationQuat() * correctionQuat, it.delta_time()));	
		}
	});

	/*
	 * State Machine Systems
	 */
	// Enter Wander state
	// For now it handles the movement logic and transition to another state
	// Not sure whether it makes more sense to split the transition logic and movement logic
	// This approach seems to sacrifice runtime speed for code readability
	// So what is the line here?

	// Uses OnValidate
	ECSWorld.system<Velocity>("System Enter Wander State")
	.kind(flecs::PostUpdate)
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
	.kind(flecs::PostUpdate)
	.with<FSM_State>(flecs::Wildcard)
	.with(FSM_State::Wander)
	.with<FSM_Status>(flecs::Wildcard)
	.with(FSM_Status::Running)
	.each([](flecs::iter& it, size_t index, WanderStateData& cWanderStateData)
	{
		cWanderStateData.CurrentWanderDuration -= it.delta_time();
		if(cWanderStateData.CurrentWanderDuration < 0)
		{
			// This is not used until it reaches bellow zero -- unused cache. Hence
			cWanderStateData.CurrentWanderDuration = FMath::RandRange(0.5f, 2.0f);
			it.entity(index).add(FSM_State::Wait);
			it.entity(index).add(FSM_Status::Enter);
		}
	});

	ECSWorld.system<Velocity>("System Enter Wait State")
	.kind(flecs::PostUpdate)
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
	.kind(flecs::PostUpdate)
	.with<FSM_State>(flecs::Wildcard)
	.with(FSM_State::Wait)
	.with<FSM_Status>(flecs::Wildcard)
	.with(FSM_Status::Running)
	.each([](flecs::iter& it, size_t index, WaitStateData& cWaitStateData)
	{
		cWaitStateData.CurrentWaitDuration -= it.delta_time();
		if(cWaitStateData.CurrentWaitDuration < 0)
		{
			cWaitStateData.CurrentWaitDuration = FMath::RandRange(1.0f, 5.0f);
			it.entity(index).add(FSM_State::Wander);
			it.entity(index).add(FSM_Status::Enter);
		}
	});
}
