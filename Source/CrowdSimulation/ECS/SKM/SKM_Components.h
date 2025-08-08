// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsModuleBase.h"
#include "CrowdSimulation/SKM/SKM_Pawn.h"
#include "SKM_Components.generated.h"

struct SKM_AddInstance
{
	flecs::entity Prefab;
	FTransform Transform;
};

struct SKM_ActorRef
{
	AActor* Value;
};

struct SKM_SkeletalMeshRef
{
	USkeletalMeshComponent* Value;
};

/**
 * 
 */
UCLASS()
class CROWDSIMULATION_API USKM_Components : public UFlecsModuleBase
{
	GENERATED_BODY()

	virtual void Initialize(flecs::world& ECSWorld) override;
};
