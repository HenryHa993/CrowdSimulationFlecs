// Fill out your copyright notice in the Description page of Project Settings.


#include "ST_Systems.h"

#include "ST_Components.h"
#include "CrowdSimulation/ECS/Core/Core_Components.h"

void UST_Systems::Initialize(flecs::world& ECSWorld)
{
	ECSWorld.system<const WorldRef, const UnitConfigRef, const ST_AddInstance>("System Add ST Actor Instance")
	.term_at(0).singleton()
	.term_at(1).singleton()
	.each([](flecs::iter& it, size_t index, const WorldRef& cWorld, const UnitConfigRef& cUnitConfig, const ST_AddInstance& cAdd)
	{
		APawn* unit = cWorld.Value->SpawnActor<APawn>(cUnitConfig.Value->STActors[cAdd.Index], cAdd.Transform);
			
		it.world().entity()
			.set<ST_ActorRef>({static_cast<AActor*>(unit)});

		it.entity(index).destruct();
	});

	ECSWorld.system<ST_ActorRef>("System Despawn ST Actor")
	.kind<OnDespawn>()
	.each([](flecs::iter& it, size_t index, ST_ActorRef& cActor)
	{
		cActor.Value->Destroy();
		it.entity(index).destruct();
	});
}
