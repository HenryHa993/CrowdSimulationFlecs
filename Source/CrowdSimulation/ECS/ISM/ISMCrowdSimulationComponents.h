// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsModuleBase.h"
#include "TurboSequence_MinimalData_Lf.h"
#include "CrowdSimulation/ISM/ISMController.h"
#include "ISMCrowdSimulationComponents.generated.h"

// Basic Components
struct Transform
{
	FTransform Value;
};

struct Velocity
{
	FVector Value;
};

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

struct WorldRef
{
	UWorld* Value;
};

// TurboSequence Components

// Controller should update instances
struct TS_MeshUpdateContext
{
	FTurboSequence_UpdateContext_Lf Value;
};

struct TS_AddInstance
{
	UWorld* World;
	
	FTurboSequence_MeshSpawnData_Lf SpawnData;
	FTurboSequence_UpdateContext_Lf MeshUpdateContext;
	
	flecs::entity Prefab;
	FTransform Transform;
};

struct TS_Mesh
{
	FTurboSequence_MinimalMeshData_Lf Value;
	//FTurboSequence_AnimMinimalCollection_Lf AnimationID;
	//FTurboSequence_AnimPlaySettings_Lf AnimationPlaySettings;
};

struct TS_Animation
{
	FTurboSequence_AnimMinimalCollection_Lf AnimationID;
	FTurboSequence_AnimPlaySettings_Lf AnimationPlaySettings;
};

// Test State Machine
enum class FSM_State
{
	Wander,
	Wait
};

enum class FSM_Status
{
	Enter,
	Running,
	Exit
};

struct WanderStateData
{
	float WanderDuration;
	float CurrentWanderDuration;
};

struct WaitStateData
{
	float WaitDuration;
	float CurrentWaitDuration;
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
