// Fill out your copyright notice in the Description page of Project Settings.


#include "ISM_Init.h"

#include "CrowdSimulation/ECS/Core/Core_Components.h"

void UISM_Init::Initialize(flecs::world& ECSWorld)
{
	/*// Initialise map
	// todo: Remind me why we need a map?
	ISM_Map ismMap{TMap<uint32, AISMController*>()};

	// For each config, we initialise an ISMController
	int32 unitHash = CreateISMController(GetWorld(), UnitConfig->StaticMesh, UnitConfig->Material, ismMap);
	
	// Temporary mass spawning system
	{
		flecs::entity unitPrefab = ECSWorld.entity("Unit")
		.set<Velocity>({ FVector{0,0,0} })
		.set<WanderStateData>({2.0f})
		.set<WaitStateData>({2.0f})
		.add(FSM_State::Wander)
		.add(FSM_Status::Enter);

		int x = 5;
		int y = 5;
		int gridX = 100;
		int gridY = 100;

		for(double i = 0; i < x * gridX; i += gridX)
		{
			for(double j = 0; j < y * gridY; j += gridY)
			{
				FTransform unitTransform{FVector{i, j, 0}};
				// Each unit has it's hash to the ISM Controller
				ECSWorld.entity()
					.set<ISM_AddInstance>({unitHash, unitPrefab, unitTransform});
			}
		}
	}

	// ISM map is a singleton
	ECSWorld.set<ISM_Map>({ ismMap });*/

	// Set up ISM Manager entity with unit config
	UUnitConfig* unitConfig = ECSWorld.get<UnitConfigRef>()->Value;

	AISMController* controller = Cast<AISMController>(GetWorld()->SpawnActor(AISMController::StaticClass()));
	controller->Initialize(unitConfig->StaticMesh, unitConfig->Material);
	ISM_ControllerRef controllerRef {controller};

	// This entity can be looked up in order to spawn ISM instances
	ECSWorld.entity("ISM Manager")
		.set<ISM_ControllerRef>({controllerRef})
		.add<ISM_Manager>();

	
}

uint32 UISM_Init::CreateISMController(UWorld* World, UStaticMesh* StaticMesh,
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