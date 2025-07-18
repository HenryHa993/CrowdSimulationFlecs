// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsModuleBase.h"
#include "TurboSequence_MinimalData_Lf.h"
#include "TSCrowdSimulationComponents.generated.h"

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
};

struct TS_Animation
{
	FTurboSequence_AnimMinimalCollection_Lf AnimationID;
	FTurboSequence_AnimPlaySettings_Lf AnimationPlaySettings;
};

/**
 * 
 */
UCLASS()
class CROWDSIMULATION_API UTSCrowdSimulationComponents : public UFlecsModuleBase
{
	GENERATED_BODY()

	virtual void Initialize(flecs::world& ECSWorld) override;
};
