// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsModuleBase.h"
#include "TurboSequence_MinimalData_Lf.h"
#include "CrowdSimulation/ECS/Core/Core_Components.h"
#include "TS_Components.generated.h"

struct TS_MeshUpdateContext
{
	FTurboSequence_UpdateContext_Lf Value;
};

struct TS_AddInstance
{
	flecs::entity Prefab;
	FTransform Transform;
};

struct TS_Mesh
{
	FTurboSequence_MinimalMeshData_Lf Value;
};

struct TS_Animation
{
	//FTurboSequence_AnimMinimalCollection_Lf AnimationID;
	FTurboSequence_AnimPlaySettings_Lf AnimationPlaySettings;
};

struct TS_Animations
{
	TMap<FSM_State, UAnimSequence*> Value;
};

struct TS_RemoveInstance{};

/**
 * 
 */
UCLASS()
class CROWDSIMULATION_API UTS_Components : public UFlecsModuleBase
{
	GENERATED_BODY()

	virtual void Initialize(flecs::world& ECSWorld) override;
};
