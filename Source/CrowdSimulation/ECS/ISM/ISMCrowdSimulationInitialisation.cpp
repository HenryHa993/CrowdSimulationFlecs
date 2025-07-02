// Fill out your copyright notice in the Description page of Project Settings.


#include "ISMCrowdSimulationInitialisation.h"

void UISMCrowdSimulationInitialisation::Initialize(flecs::world& ECSWorld)
{
	/*
	ECSWorld.entity().set<test>({1});
*/
	// So now that I have the config, I can initialise entities depending on the bootstrap and go ham

	// How do I make an ECS Spawning system?

	// This should be where spawning occurs right?

	// Initialise map
	ISM_Map ismMap{TMap<uint32, AISMController*>()};

	// Create an ISMController for the unit we will spawn
	int32 unitHash = CreateISMController(GetWorld(), UnitConfig->StaticMesh, UnitConfig->Material, ismMap);

	flecs::entity unitPrefab = ECSWorld.entity("Unit");

	ECSWorld.entity()
		.set<ISM_AddInstance>({unitHash, unitPrefab, FTransform::Identity});

	// ISM map is a singleton
	ECSWorld.set<ISM_Map>({ ismMap });
}

uint32 UISMCrowdSimulationInitialisation::CreateISMController(UWorld* World, UStaticMesh* StaticMesh,
	UMaterialInterface* Material, ISM_Map& Map)
{
	uint32 hash = HashCombine(GetTypeHash(Material), GetTypeHash(StaticMesh));

	AISMController** find = Map.ISMs.Find(hash);

	// Cannot find in map -- spawn new ISM Controller
	if(find == nullptr)
	{
		AISMController* controller = Cast<AISMController>(World->SpawnActor(AISMController::StaticClass()));
		controller->Initialize(StaticMesh, Material);
		Map.ISMs.Add(hash, controller);
	}
	return hash;
}