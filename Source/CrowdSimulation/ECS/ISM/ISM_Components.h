// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsModuleBase.h"
#include "TurboSequence_MinimalData_Lf.h"
#include "CrowdSimulation/ECS/Core/Core_Components.h"
#include "CrowdSimulation/ISM/ISMController.h"
#include "ISM_Components.generated.h"

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
	flecs::entity Prefab;
	FTransform Transform;
};

struct ISM_AnimationMap
{
	TMap<FSM_State, std::pair<float, float>> Value;
};

// Tag to differentiate entities that own a reference to a controller vs. entities that are managing it
struct ISM_Manager{};

/**
 * 
 */
UCLASS()
class CROWDSIMULATION_API UISM_Components : public UFlecsModuleBase
{
	GENERATED_BODY()

public:
	virtual void Initialize(flecs::world& ECSWorld) override;
};
