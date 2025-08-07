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

	ECSWorld.system<ISM_AddInstance>("System Add ISM Instance")
	.each([](flecs::iter& it, size_t index, ISM_AddInstance& cAdd)
	{
		if(cAdd.ControllerRef != nullptr)
		{
			int32 ismIndex = cAdd.ControllerRef->AddInstance();
				
			it.world().entity()
				.is_a(cAdd.Prefab)
				.set<ISM_ControllerRef>({ cAdd.ControllerRef })
				.set<ISM_Index>( { ismIndex })
				.set<Transform>({ cAdd.Transform });
				
			cAdd.ControllerRef->CreateOrExpandTransformArray();
		}
		it.entity(index).destruct();
	});
	
	// Copy entity transform to ISM instance
	ECSWorld.system<const Transform, const ISM_Index, const ISM_ControllerRef>("System Copy Instance Transforms")
	.each([](const Transform& cTransform, const ISM_Index& cISMIndex, const ISM_ControllerRef& cISMControllerRef)
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
	.with<ISM_Manager>()
	.each([](const ISM_ControllerRef& cController)
	{
		cController.Value->BatchUpdateTransforms();
	});
}
