// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlecsModuleBase.h"
#include "TSCrowdSimulationSystems.generated.h"

/**
 * 
 */
UCLASS()
class CROWDSIMULATION_API UTSCrowdSimulationSystems : public UFlecsModuleBase
{
	GENERATED_BODY()

	virtual void Initialize(flecs::world& ECSWorld) override;
};
