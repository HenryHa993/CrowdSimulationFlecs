// Fill out your copyright notice in the Description page of Project Settings.


#include "SKM_Systems.h"

#include "SKM_Components.h"
#include "CrowdSimulation/ECS/Core/Core_Components.h"

void USKM_Systems::Initialize(flecs::world& ECSWorld)
{
	ECSWorld.system<const WorldRef, const UnitConfigRef, const SKM_AddInstance>("System Add SKM Actor Instance")
	.term_at(0).singleton()
	.term_at(1).singleton()
	.each([](flecs::iter& it, size_t index, const WorldRef& cWorld, const UnitConfigRef& cUnitConfig, const SKM_AddInstance& cAdd)
	{
		ASKM_Pawn* unit = cWorld.Value->SpawnActor<ASKM_Pawn>(cUnitConfig.Value->Actor, cAdd.Transform);

		// todo: Might be worth putting some more initialisation stuff here
		// todo: Or might be better to separate them at a blueprint level
		// unit->FloatingPawnMovement->DestroyComponent();
		
		it.world().entity()
			.is_a(cAdd.Prefab)
			.set<SKM_ActorRef>({static_cast<AActor*>(unit)})
			.set<SKM_SkeletalMeshRef>({unit->SkeletalMeshComponent})
			.set<Transform>({cAdd.Transform});

		it.entity(index).destruct();
	});

	// Animation system

	// Copy transforms
	// This query does not seem to be matching
	ECSWorld.system<const SKM_ActorRef, const Transform>("System Copy SKM Transforms")
	.each([](const SKM_ActorRef& cActor, const Transform& cTransform)
	{
		UE_LOG(LogTemp, Warning, TEXT("Running?"));

		cActor.Value->SetActorTransform(cTransform.Value);
	});

	ECSWorld.system<SKM_ActorRef>("System Copy Transform to SKM Actor")
	.kind<OnDespawn>()
	.each([](flecs::iter& it, size_t index, SKM_ActorRef& cActor)
	{
		cActor.Value->Destroy();
		it.entity(index).destruct();
	});
}
