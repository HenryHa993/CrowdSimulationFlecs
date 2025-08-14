// Fill out your copyright notice in the Description page of Project Settings.


#include "ISM_Systems.h"

#include "ISM_Components.h"
#include "TurboSequence_Manager_Lf.h"
#include "CrowdSimulation/ECS/Core/Core_Components.h"

void UISM_Systems::Initialize(flecs::world& ECSWorld)
{
	// Event system which initialises an ISM instance and entity
	// todo: No longer in use
	/*ECSWorld.system<ISM_AddInstance, ISM_Map>("System Add ISM Instance")
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
				.set<ISM_Hash>({ cAdd.Hash }) // Store the hash to it's controller
				.set<Transform>({ cAdd.Transform });
			
			controller->CreateOrExpandTransformArray();
		}
		it.entity(index).destruct();
	});*/

	ECSWorld.system<const ISM_ControllerRef, ISM_AddInstance>("System Add ISM Instance")
	.term_at(0).singleton()
	.each([](flecs::iter& it, size_t index, const ISM_ControllerRef& cController, ISM_AddInstance& cAdd)
	{
		if(cController.Value != nullptr)
		{
			int32 ismIndex = cController.Value->AddInstance();
				
			it.world().entity()
				.is_a(cAdd.Prefab)
				.set<ISM_Index>( { ismIndex })
				.set<Transform>({ cAdd.Transform });
				
			cController.Value->CreateOrExpandTransformArray();
		}
		it.entity(index).destruct();
	});

	ECSWorld.system<const ISM_AnimationMap, const ISM_ControllerRef, const ISM_Index>("System Set ISM Animation")
	.kind(flecs::OnUpdate)
	.term_at(0).singleton()
	.term_at(1).singleton()
	.with<FSM_State>(flecs::Wildcard)
	.with<FSM_Status>(flecs::Wildcard)
	.with(FSM_Status::Enter)
	.each([](flecs::iter& it, size_t index, const ISM_AnimationMap& cAnimationMap,const ISM_ControllerRef& cController, const ISM_Index& cIndex)
	{
		const FSM_State* state = it.entity(index).get<FSM_State>();

		if(cAnimationMap.Value.Find(*state))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Switching animation!"));
			std::pair<float, float> animation = cAnimationMap.Value[*state];
			cController.Value->SetAnimation(cIndex.Value, animation.first, animation.second);	
		}
		//cController.Value->SetAnimation(cIndex.Value, 121.f, 145.f);	
	});
	
	// Copy entity transform to ISM instance
	// Opportunity to pack the cache line here
	ECSWorld.system<const ISM_ControllerRef, const Transform, const ISM_Index>("System Copy Instance Transforms")
	.term_at(0).singleton()
	.each([](const ISM_ControllerRef& cISMControllerRef, const Transform& cTransform, const ISM_Index& cISMIndex)
	{
		int index = cISMIndex.Value;
		cISMControllerRef.Value->SetTransform(index, cTransform.Value);
	});

	// Update transforms
	// todo: not in use
	/*ECSWorld.system<const ISM_Map>("System Batch Update Controllers")
	.term_at(1)
	.singleton()
	.each([](const ISM_Map& cMap)
	{
		for(auto& mapping : cMap.ISMs)
		{
			mapping.Value->BatchUpdateTransforms();
		}
	});*/

	// Controller managers batch update their own transforms
	ECSWorld.system<const ISM_ControllerRef>("System Batch Update Transforms")
	.term_at(0).singleton()
	.with<ISM_Manager>()
	.each([](const ISM_ControllerRef& cController)
	{
		cController.Value->BatchUpdateTransforms();
	});

	// Clear ISM Instances from the Manager side
	ECSWorld.system<const ISM_ControllerRef>("System Clear All ISM Instances")
	.kind<OnDespawn>()
	.term_at(0).singleton()
	.with<ISM_Manager>()
	.each([](const ISM_ControllerRef& cController)
	{
		cController.Value->ClearInstances();
	});

	// Clear entities with an ISM instance
	ECSWorld.system("System Destruct ISM Entities")
	.kind<OnDespawn>()
	.with<ISM_Index>()
	.each([](flecs::iter& it, size_t index)
	{
		it.entity(index).destruct();
	});
}
