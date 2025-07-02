// Fill out your copyright notice in the Description page of Project Settings.


#include "ISMCrowdSimulationSystems.h"

#include "ISMCrowdSimulationComponents.h"

void UISMCrowdSimulationSystems::Initialize(flecs::world& ECSWorld)
{
	/*ECSWorld.system<test>("Test")
		.each([](test& t)
		{
			UE_LOG(LogTemp, Warning, TEXT("Entity with test component value"));
		});*/

	// This needs a bit of work and adaptation
	//ECSWorld.system<ISM_AddInstance, ISM_Map>("Add Instance System")
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

	// Copy entity transform to ISM instance
	ECSWorld.system<const Transform, const ISM_Index, const ISM_ControllerRef>("System Copy Instance Transforms")
	.each([](const Transform& cTransform, const ISM_Index& cISMIndex, const ISM_ControllerRef& cISMControllerRef)
	{
		int index = cISMIndex.Value;
		cISMControllerRef.Value->SetTransform(index, cTransform.Value);
	});

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
}
