// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsModuleBase.h"
#include "ST_Components.generated.h"

struct ST_AddInstance
{
	int Index;
	FTransform Transform;
};

struct ST_ActorRef
{
	AActor* Value;
};

/**
 * 
 */
UCLASS()
class CROWDSIMULATION_API UST_Components : public UFlecsModuleBase
{
	GENERATED_BODY()

	virtual void Initialize(flecs::world& ECSWorld) override;
};
