// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsModuleBase.h"
#include "CrowdSimulationComponents.generated.h"

struct test
{
	int a;
};

/**
 * 
 */
UCLASS()
class CROWDSIMULATION_API UCrowdSimulationComponents : public UFlecsModuleBase
{
	GENERATED_BODY()

public:
	virtual void Initialize(flecs::world& ECSWorld) override;
};
