// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsModuleBase.h"
#include "TurboSequence_MinimalData_Lf.h"
#include "CrowdSimulation/ISM/ISMController.h"
#include "ISMCrowdSimulationComponents.generated.h"

// ISM
struct ISM_Map
{
	TMap<uint32, AISMController*> ISMs;
};

struct ISM_ControllerRef
{
	AISMController* Value;
};

struct ISM_Hash
{
	int32 Value;
};

struct ISM_Index
{
	int Value;
};

// Event struct, destroyed once instance is spawned
struct ISM_AddInstance
{
	int32 Hash;
	flecs::entity Prefab;
	FTransform Transform;
};

/**
 * 
 */
UCLASS()
class CROWDSIMULATION_API UISMCrowdSimulationComponents : public UFlecsModuleBase
{
	GENERATED_BODY()

public:
	virtual void Initialize(flecs::world& ECSWorld) override;
};
